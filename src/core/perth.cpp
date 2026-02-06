// Copyright (c) 2026 CNES
//
// All rights reserved. Use of this source code is governed by a
// BSD-style license that can be found in the LICENSE file.
#include <pybind11/complex.h>
#include <pybind11/eigen.h>
#include <pybind11/pybind11.h>
#include <pybind11/stl.h>

#include "fes/perth/constituent.hpp"
#include "fes/perth/wave.hpp"
#include "fes/perth/wave_table.hpp"

namespace py = pybind11;

namespace fes {
namespace perth {

inline auto init_wave(py::module& m) -> void {
  py::class_<Wave, WaveInterface, std::unique_ptr<Wave>>(m, "Wave",
                                                         R"__doc__(
A tidal wave using Doodson's notation system.

Perth waves are described by a 7-element Doodson number vector that directly
encodes the tidal argument in terms of astronomical variables.
)__doc__")
      .def_property_readonly("name", &Wave::name, "The name of the tidal wave.")
      .def_property_readonly("type", &Wave::type, "The type of tidal wave.")
      .def(
          "frequency",
          [](const Wave& self, const FrequencyUnit unit) -> double {
            return unit == FrequencyUnit::kRadianPerHour
                       ? self.frequency()
                       : detail::math::degrees(self.frequency());
          },
          py::arg("unit") = FrequencyUnit::kRadianPerHour,
          R"__doc__(
Get the frequency of the tidal wave.

Args:
  unit: The frequency unit. Default is radians per hour.

Returns:
  The frequency of the tidal wave.
)__doc__")
      .def_property_readonly("period", &Wave::period,
                             "The period of the wave in hours.")
      .def("doodson_numbers", &Wave::doodson_numbers,
           "Get the Doodson number of the wave.")
      .def("xdo_numerical", &Wave::xdo_numerical,
           "Get the XDO numerical representation of the wave.")
      .def("xdo_alphabetical", &Wave::xdo_alphabetical,
           "Get the XDO alphabetical representation of the wave.")
      .def("__repr__", [](const Wave& self) -> std::string {
        return std::string("<perth.Wave '") + self.name() + "'>";
      });
}

inline auto init_wave_table(py::module& m) -> void {
  py::class_<WaveTable, WaveTableInterface, std::unique_ptr<WaveTable>>(
      m, "WaveTable",
      R"__doc__(
Table of tidal constituents using Doodson's notation system.

This table manages 80 tidal constituents used by GOT/Perth models. Each
constituent is represented as a Perth wave with its Doodson number encoding.
)__doc__")
      .def(py::init<>(),
           "Default constructor with all known Perth constituents.")
      .def(py::init<const std::vector<std::string>&>(), py::arg("names"),
           R"__doc__(
Constructor with a list of constituent names.

Args:
  names: List of constituent names to include in the table.
)__doc__")
      .def("__repr__", [](const WaveTable& self) -> std::string {
        return "<perth.WaveTable with " + std::to_string(self.size()) +
               " constituents>";
      });
}

}  // namespace perth
}  // namespace fes

void init_perth(py::module& m) {
  auto perth_mod = m.def_submodule("perth", "Perth tidal model components.");
  fes::perth::init_wave(perth_mod);
  fes::perth::init_wave_table(perth_mod);
}
