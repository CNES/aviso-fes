// Copyright (c) 2026 CNES
//
// All rights reserved. Use of this source code is governed by a
// BSD-style license that can be found in the LICENSE file.
#include <pybind11/complex.h>
#include <pybind11/eigen.h>
#include <pybind11/pybind11.h>
#include <pybind11/stl.h>

#include "fes/interface/wave.hpp"

namespace py = pybind11;

namespace fes {

inline auto init_frequency_unit(py::module& m) -> void {
  py::enum_<FrequencyUnit>(m, "FrequencyUnit", "Unit for tidal wave frequency.")
      .value("RADIAN_PER_HOUR", FrequencyUnit::kRadianPerHour,
             "Frequency in radians per hour.")
      .value("DEGREE_PER_HOUR", FrequencyUnit::kDegreePerHour,
             "Frequency in degrees per hour.")
      .export_values();
}

inline auto init_angle_unit(py::module& m) -> void {
  py::enum_<AngleUnit>(m, "AngleUnit", "Angular unit enumeration.")
      .value("RADIAN", AngleUnit::kRadian, "Radian.")
      .value("DEGREE", AngleUnit::kDegree, "Degree.")
      .export_values();
}

inline auto init_wave_type(py::module& m) -> void {
  py::enum_<WaveType>(m, "WaveType", "Possible type of tidal wave.")
      .value("LONG_PERIOD", WaveType::kLongPeriod, "Long period tidal waves.")
      .value("SHORT_PERIOD", WaveType::kShortPeriod,
             "Short period tidal waves.")
      .export_values();
}

// Trampoline class for WaveInterface
class PyWaveInterface : public WaveInterface {
 public:
  using WaveInterface::WaveInterface;

  auto clone() const -> std::unique_ptr<WaveInterface> override {
    PYBIND11_OVERLOAD_PURE(std::unique_ptr<WaveInterface>, WaveInterface,
                           clone);
  }

  auto frequency() const noexcept -> double override {
    PYBIND11_OVERLOAD_PURE(double, WaveInterface, frequency);
  }

  auto compute_nodal_corrections(const angle::Astronomic& angles,
                                 const bool group_modulations)
      -> void override {
    PYBIND11_OVERLOAD_PURE(void, WaveInterface, compute_nodal_corrections,
                           angles, group_modulations);
  }

  auto xdo_numerical() const -> std::string override {
    PYBIND11_OVERLOAD_PURE(std::string, WaveInterface, xdo_numerical);
  }

  auto xdo_alphabetical() const -> std::string override {
    PYBIND11_OVERLOAD_PURE(std::string, WaveInterface, xdo_alphabetical);
  }

  auto doodson_numbers() const -> Vector7b override {
    PYBIND11_OVERLOAD_PURE(Vector7b, WaveInterface, doodson_numbers);
  }
};

inline auto init_wave_interface(py::module& m) -> void {
  py::class_<WaveInterface, PyWaveInterface, std::unique_ptr<WaveInterface>>(
      m, "WaveInterface", "Tidal wave interface.")
      .def_property_readonly("name", &WaveInterface::name,
                             "The name of the tidal wave.")
      .def_property_readonly("latex_name", &WaveInterface::latex_name,
                             "The LaTeX name of the tidal wave.")
      .def_property_readonly("type", &WaveInterface::type,
                             "The type of tidal wave.")
      .def_property("is_modeled", &WaveInterface::is_modeled,
                    &WaveInterface::set_is_modeled,
                    "Whether the tide is provided by the model.")
      .def_property("tide", &WaveInterface::tide, &WaveInterface::set_tide,
                    "The tide value.")
      .def_property_readonly("corrected_tide", &WaveInterface::corrected_tide,
                             "The tide value after applying the nodal "
                             "corrections.")
      .def_property_readonly("v", &WaveInterface::v,
                             "The Greenwich argument, in radians.")
      .def_property_readonly("u", &WaveInterface::u,
                             "The nodal correction for phase, in radians.")
      .def_property_readonly("f", &WaveInterface::f,
                             "The nodal correction for amplitude.")
      .def_property_readonly("vu", &WaveInterface::vu,
                             "The sum of the Greenwich argument and the nodal "
                             "phase correction, in radians.")
      .def(
          "frequency",
          [](const WaveInterface& self, const FrequencyUnit unit) -> double {
            return unit == FrequencyUnit::kRadianPerHour
                       ? self.frequency()
                       : self.frequency<FrequencyUnit::kDegreePerHour>();
          },
          py::arg("unit") = FrequencyUnit::kRadianPerHour,
          R"__doc__(
Get the frequency of the tidal wave.

Args:
  unit: The frequency unit. Default is radians per hour.

Returns:
  The frequency of the tidal wave.
)__doc__")
      .def_property_readonly("period", &WaveInterface::period,
                             "The period of the wave in hours.")
      .def("set_nodal_corrections", &WaveInterface::set_nodal_corrections,
           py::arg("f"), py::arg("u"), py::arg("v"), py::arg("unit"),
           R"__doc__(
Set the nodal corrections directly.

Args:
  f: Nodal correction for amplitude.
  u: Nodal correction for phase.
  v: Greenwich argument.
  unit: The angular unit of the nodal corrections.
)__doc__")
      .def("compute_nodal_corrections",
           &WaveInterface::compute_nodal_corrections, py::arg("angles"),
           py::arg("group_modulations") = false,
           R"__doc__(
Compute the nodal corrections for the wave.

Args:
  angles: Astronomical angles used to compute nodal corrections.
  group_modulations: If true, applies group modulations to nodal
    corrections.
)__doc__")
      .def("xdo_numerical", &WaveInterface::xdo_numerical,
           "Get the XDO numerical representation of the wave.")
      .def("xdo_alphabetical", &WaveInterface::xdo_alphabetical,
           "Get the XDO alphabetical representation of the wave.")
      .def("doodson_numbers", &WaveInterface::doodson_numbers,
           "Get the Doodson number of the wave.")
      .def("__repr__", [](const WaveInterface& self) -> std::string {
        return std::string("<WaveInterface '") + self.name() + "'>";
      });
}

}  // namespace fes

void init_wave_interface(py::module& m) {
  fes::init_frequency_unit(m);
  fes::init_angle_unit(m);
  fes::init_wave_type(m);
  fes::init_wave_interface(m);
}
