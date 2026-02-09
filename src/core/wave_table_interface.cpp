// Copyright (c) 2026 CNES
//
// All rights reserved. Use of this source code is governed by a
// BSD-style license that can be found in the LICENSE file.
#include <pybind11/cast.h>
#include <pybind11/complex.h>
#include <pybind11/eigen.h>
#include <pybind11/numpy.h>
#include <pybind11/pybind11.h>
#include <pybind11/stl.h>

#include <boost/optional.hpp>

#include "fes/constituent.hpp"
#include "fes/harmonic_analysis.hpp"
#include "fes/interface/wave_table.hpp"
#include "fes/python/datemanip.hpp"
#include "fes/python/datetime64.hpp"
#include "fes/python/optional.hpp"  // IWYU pragma: keep

namespace py = pybind11;

namespace fes {

inline auto init_engine_type(py::module& m) -> void {
  py::enum_<EngineType>(m, "EngineType",
                        "Available harmonic constituent notation systems.")
      .value("DARWIN", EngineType::kDarwin,
             "Darwin's notation system, used by FES models.")
      .value("DOODSON", EngineType::kDoodson,
             "Doodson's notation system, used by the GOT models.")
      .export_values();
}

// Trampoline class for WaveTableInterface
class PyWaveTableInterface : public WaveTableInterface {
 public:
  using WaveTableInterface::WaveTableInterface;

  auto clone() const -> std::unique_ptr<WaveTableInterface> override {
    PYBIND11_OVERLOAD_PURE(std::unique_ptr<WaveTableInterface>,
                           WaveTableInterface, clone);
  }

  auto compute_nodal_corrections(const angle::Astronomic& angles,
                                 const bool group_modulations)
      -> void override {
    PYBIND11_OVERLOAD_PURE(void, WaveTableInterface, compute_nodal_corrections,
                           angles, group_modulations);
  }
};

inline auto init_wave_table_interface(py::module& m) -> void {
  py::class_<WaveTableInterface, PyWaveTableInterface,
             std::unique_ptr<WaveTableInterface>>(m, "WaveTableInterface",
                                                  "Tidal wave table interface.")
      .def(py::init<>(), "Default constructor.")
      .def(
          "__getitem__",
          [](const WaveTableInterface& self,
             const std::string& name) -> const WaveInterface& {
            auto ident = constituents::parse(name);
            return *self[ident];
          },
          py::arg("name"), py::return_value_policy::reference_internal,
          R"__doc__(
Get the wave corresponding to the given constituent name.

Args:
  name: The constituent name. Search is not case sensitive.

Returns:
  The wave interface.
)__doc__")
      .def(
          "__getitem__",
          [](const WaveTableInterface& self, const size_t index)
              -> const WaveInterface& { return *self[index]; },
          py::arg("index"), py::return_value_policy::reference_internal,
          R"__doc__(
Get the wave at the given index.

Args:
  index: The index of the constituent.

Returns:
  The wave interface.
)__doc__")
      .def("__len__", &WaveTableInterface::size,
           "Return the number of constituents in the table.")
      .def(
          "waves",
          [](const WaveTableInterface& self) -> py::list {
            auto result = py::list();
            for (const auto& item : self) {
              result.append(py::cast(item.value().get(),
                                     py::return_value_policy::reference));
            }
            return result;
          },
          "Return a list of waves in the table.")
      .def(
          "__contains__",
          [](const WaveTableInterface& self, const std::string& name) -> bool {
            try {
              auto ident = constituents::parse(name);
              return self.contains(ident);
            } catch (const ConstituentValidationError&) {
              return false;
            }
          },
          py::arg("name"),
          R"__doc__(
Check if a constituent is in the table.

Args:
  name: The constituent name.

Returns:
  True if the constituent is in the table.
)__doc__")
      .def_property_readonly("constituents", &WaveTableInterface::constituents,
                             "The list of constituent names in the table.")
      .def("compute_nodal_corrections",
           &WaveTableInterface::compute_nodal_corrections, py::arg("angles"),
           py::arg("group_modulations") = false,
           R"__doc__(
Compute the nodal corrections for all constituents in the table.

Args:
  angles: Astronomical angles used to compute nodal corrections.
  group_modulations: If true, applies group modulations to nodal
    corrections.
)__doc__")
      .def(
          "set_tide",
          [](WaveTableInterface& self, const std::string& name,
             const Complex& value) -> void {
            auto ident = constituents::parse(name);
            self.set_tide(ident, value);
          },
          py::arg("name"), py::arg("value"),
          R"__doc__(
Set the tide of a constituent.

Args:
  name: The constituent name.
  value: The tide value.
)__doc__")
      .def("select_waves_for_analysis",
           &WaveTableInterface::select_waves_for_analysis, py::arg("duration"),
           py::arg("rayleigh_criterion") = 1.0,
           R"__doc__(
Select tidal waves that can be resolved from a record of a given duration
based on the Rayleigh criterion.

Converts the record duration to hours, computes the minimum angular separation
:math:`2\pi C_R / T` then iterates over the wave table, rejecting waves too
close to :math:`DC` or closer than the Rayleigh limit to any already - selected
wave.

Args:
  duration: Duration of the time series analyzed in seconds.
  rayleigh_criterion: The Rayleigh criterion factor (C_R) to determine the
    minimum frequency. Default is 1.0, which corresponds to the classical
    Rayleigh criterion.

Returns:
  List of selected tidal wave names.
)__doc__")
      .def(
          "sort_by_frequency",
          [](const WaveTableInterface& self, const bool ascending) {
            auto sorted = self.sort_by_frequency(ascending);
            auto result = std::vector<std::string>();
            result.reserve(sorted.size());
            for (const auto& ident : sorted) {
              result.push_back(constituents::name(ident));
            }
            return result;
          },
          py::arg("ascending") = true,
          R"__doc__(
Returns the constituents sorted by frequency.

Args:
  ascending: If True (default), sort by ascending frequency;
    otherwise, sort by descending frequency.

Returns:
  List of constituent names sorted by frequency.
)__doc__")
      .def_static(
          "harmonic_analysis",
          [](const Eigen::Ref<const Eigen::VectorXd>& h,
             const DynamicRef<const Eigen::MatrixXd>& f,
             const DynamicRef<const Eigen::MatrixXd>& vu) -> Eigen::VectorXcd {
            py::gil_scoped_release release;
            return harmonic_analysis(h, f, vu);
          },
          py::arg("h"), py::arg("f"), py::arg("vu"),
          R"__doc__(
Harmonic analysis of the tide.

The harmonic analysis method consists in expressing the ocean tidal
variations as a sum of independent constituents accordingly to the tidal
potential spectrum. Then the sea surface elevation at a point
:math:`(x, y)` and time :math:`(t)` can be expressed as a linear sum as
follow:

.. math::

  S_{ap} = S_{0}(x, y) + \sum_{k=0}^n f_{k}(t)S_{k}(x, y)
    	imes \cos [\omega_{k}t + v_{k}(t) + u_{k}(t) - G_{k}(x,y)]

where:
  - :math:`(n)` is the number of constituents,
  - :math:`S_{0}(x, y)` is the mean sea level,
  - :math:`S_{k}(x, y)` is the amplitude of the constituent of index
    :math:`(k)`,
  - :math:`G_{k}(x, y)` is the phase lag relative to Greenwich time,
  - :math:`\omega_{k}` is the angular frequency of the constituent of index
    :math:`(k)`,
  - :math:`v_{k}` is the astronomical argument at time :math:`(t)`,
  - :math:`f_{k}(t)` is the nodal correction coefficient applied to
    the amplitude of the constituent of index :math:`(k)`,
  - :math:`u_{k}(t)` is the nodal correction coefficient applied to
    the phase of the constituent of index :math:`(k)`.

The a priori analysis spectrum includes the most important astronomical
constituents in the Darwin development, completed by Schureman in 1958,
and many non-linear waves. The definition of tidal constants and
astronomical arguments is taken from FES2014 tidal prediction software
and a complete definition of waves is also available in Schureman (1958).
This spectrum is the most commonly used for harmonic analysis due the
simplification given by the nodal correction concept (:math:`(f)` and
:math:`(u)` coefficients above) which allows dealing with slow motions of
the lunar ascending node and reducing the number of constituents in the
tidal spectrum. More details about this harmonic analysis method can be
found in Ponchaut et al. 1999.

Args:
  h: Sea level.
  f: Nodal correction coefficient applied to the amplitude of the
    constituents analyzed.
  vu: Astronomical argument at time :math:`(t)` + the nodal correction
    coefficient applied to the phase of the constituents analyzed.

Returns:
  The complex number representing the different reconstructed waves.
)__doc__")
      .def(
          "tide_from_tide_series",
          [](const WaveTableInterface& self, const py::array& epoch,
             const Eigen::Ref<const Eigen::VectorXcd>& wave,
             const angle::Formulae formulae,
             const bool group_modulations) -> Eigen::VectorXd {
            auto epoch_sec = python::npdatetime64_to_epoch(epoch);
            auto args = NodalCorrectionsArgs(angle::Astronomic(formulae),
                                             group_modulations);
            return self.tide_from_tide_series(epoch_sec, wave, args);
          },
          py::arg("epoch"), py::arg("wave"),
          py::arg("formulae") = angle::Formulae::kSchuremanOrder1,
          py::arg("group_modulations") = false,
          R"__doc__(
Calculate the tide of a given time series.

Args:
  epoch: Desired UTC time as a numpy array of datetime64.
  wave: Tidal wave properties computed by an harmonic analysis.
  formulae: Formulae used to compute the astronomic angles.
  group_modulations: If true, applies group modulations to nodal
    corrections.

Returns:
  The tide at the given times.
)__doc__")
      .def(
          "tide_from_mapping",
          [](const WaveTableInterface& self, const py::handle& epoch,
             const DynamicRef<const Eigen::MatrixXcd>& wave,
             const angle::Formulae formulae, const bool group_modulations,
             const size_t num_threads) -> Eigen::MatrixXd {
            auto args = NodalCorrectionsArgs(angle::Astronomic(formulae),
                                             group_modulations);
            auto epoch_sec = python::datemanip::as_float64(epoch);
            py::gil_scoped_release release;
            return self.tide_from_mapping(epoch_sec, wave, args, num_threads);
          },
          py::arg("epoch"), py::arg("wave"),
          py::arg("formulae") = angle::Formulae::kSchuremanOrder1,
          py::arg("group_modulations") = false, py::arg("num_threads") = 0,
          R"__doc__(
Calculate the tide for a given date from a grid describing the
properties of tidal waves over an area.

Args:
  epoch: Desired UTC time as a datetime.datetime.
  wave: Tidal wave properties computed by an harmonic analysis.
  formulae: Formulae used to compute the astronomic angles.
  group_modulations: If true, applies group modulations to nodal
    corrections.
  num_threads: Number of threads to use for the computation. If
    0, the number of threads is automatically determined.

Returns:
  The tide at the given time for each grid point.
)__doc__")
      .def(
          "compute_nodal_modulations",
          [](const WaveTableInterface& self, const py::array& epoch,
             const angle::Formulae formulae, const bool group_modulations)
              -> std::tuple<Eigen::MatrixXd, Eigen::MatrixXd> {
            auto epoch_sec = python::npdatetime64_to_epoch(epoch);
            auto args = NodalCorrectionsArgs(angle::Astronomic(formulae),
                                             group_modulations);
            return self.compute_nodal_modulations(epoch_sec, args);
          },
          py::arg("epoch"),
          py::arg("formulae") = angle::Formulae::kSchuremanOrder1,
          py::arg("group_modulations") = false,
          R"__doc__(
Compute nodal modulations for amplitude and phase.

Args:
  epoch: Desired UTC time as a numpy array of datetime64.
  formulae: Formulae used to compute the astronomic angles.
  group_modulations: If true, applies group modulations to nodal
    corrections.

Returns:
  A tuple containing the nodal correction for amplitude and the sum of
  the Greenwich argument and the nodal phase correction.
)__doc__")
      .def("generate_markdown_table",
           &WaveTableInterface::generate_markdown_table,
           py::arg("ascending") = true,
           R"__doc__(
Generate a markdown table summarizing the constituents handled by the wave
table.

Args:
  ascending: If true (default), sort the table by ascending frequency;
    otherwise, sort by descending frequency.

Returns:
  A string containing the markdown table.
)__doc__")
      .def(
          "set_modeled_constituents",
          [](WaveTableInterface& self, const std::vector<std::string>& names) {
            std::vector<ConstituentId> ids;
            ids.reserve(names.size());
            for (const auto& name : names) {
              ids.push_back(constituents::parse(name));
            }
            self.set_modeled_constituents(ids);
          },
          py::arg("names"),
          R"__doc__(
Set the list of constituents that are modeled by the wave table.

Args:
  names: List of constituent names.
)__doc__")
      .def("__repr__", [](const WaveTableInterface& self) -> std::string {
        return "<WaveTableInterface with " + std::to_string(self.size()) +
               " constituents>";
      });
}

inline auto init_wave_table_factory(py::module& m) -> void {
  m.def(
      "wave_table_factory",
      [](const EngineType engine_type,
         const boost::optional<std::vector<std::string>>& constituents) {
        if (constituents) {
          return wave_table_factory(engine_type, *constituents);
        } else {
          return wave_table_factory(engine_type);
        }
      },
      py::arg("engine_type") = EngineType::kDarwin,
      py::arg("constituents") = boost::none,
      R"__doc__(
Factory function to create a wave table based on the specified engine type.

Args:
  engine_type: The engine type for the tidal constituent notation system.
  constituents: Optional list of constituent names to include in the wave table.
    If not provided, all known constituents for the engine type are included.

Returns:
  A wave table instance.
)__doc__");
}

}  // namespace fes

void init_wave_table_interface(py::module& m) {
  fes::init_engine_type(m);
  fes::init_wave_table_interface(m);
  fes::init_wave_table_factory(m);
}
