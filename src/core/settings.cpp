// Copyright (c) 2026 CNES
//
// All rights reserved. Use of this source code is governed by a
// BSD-style license that can be found in the LICENSE file.
#include "fes/settings.hpp"

#include <pybind11/pybind11.h>
#include <pybind11/stl.h>

namespace py = pybind11;

namespace fes {

inline auto init_inference_type(py::module& m) -> void {
  py::enum_<InferenceType>(
      m, "InferenceType",
      "This Enum class represents the different inference types available in "
      "PyFES for handling minor tidal constituents not explicitly provided in "
      "the tidal atlas. Each inference type corresponds to a specific method "
      "of estimating the contributions of these minor constituents based on "
      "the provided data and settings.")
      .value("SPLINE", InferenceType::kSpline,
             "Spline interpolation of admittances.")
      .value("ZERO", InferenceType::kZero,
             "No inference performed; use only explicitly provided "
             "constituents.")
      .value("LINEAR", InferenceType::kLinear,
             "Piecewise linear interpolation of admittances.")
      .value("FOURIER", InferenceType::kFourier,
             "Munk-Cartwright Fourier series interpolation of admittances.")
      .export_values();
}

inline auto init_settings(py::module& m) -> void {
  py::class_<Settings>(m, "Settings",
                       R"__doc__(
Settings for the tide computation.

This class provides a builder-style interface for configuring all parameters
controlling tide computation, including the harmonic constituent notation
system, astronomic angle formulae, inference method, parallelism, and
various physical corrections.
)__doc__")
      .def(py::init<>(), "Default constructor.")
      .def_property(
          "engine_type", &Settings::engine_type,
          [](Settings& self, EngineType engine_type) -> void {
            self.with_engine_type(engine_type);
          },
          "Engine type for the tidal constituent notation system.")
      .def_property(
          "astronomic_formulae", &Settings::astronomic_formulae,
          [](Settings& self, angle::Formulae formulae) -> void {
            self.with_astronomic_formulae(formulae);
          },
          "Astronomic formulae used to calculate the astronomic angles.")
      .def_property(
          "time_tolerance", &Settings::time_tolerance,
          [](Settings& self, double time_tolerance) -> void {
            self.with_time_tolerance(time_tolerance);
          },
          "Time in seconds for which astronomical angles are considered "
          "constant. A value of 0 means angles are recomputed for every "
          "time step.")
      .def_property(
          "group_modulations", &Settings::group_modulations,
          [](Settings& self, bool group_modulations) -> void {
            self.with_group_modulations(group_modulations);
          },
          "Whether to use group modulation nodal corrections. Only "
          "effective with the Perth WaveTable.")
      .def_property(
          "compute_long_period_equilibrium",
          &Settings::compute_long_period_equilibrium,
          [](Settings& self, bool compute_lpe) -> void {
            self.with_compute_long_period_equilibrium(compute_lpe);
          },
          "Whether to compute the long period equilibrium tide.")
      .def_property(
          "inference_type", &Settings::inference_type,
          [](Settings& self, InferenceType inference_type) -> void {
            self.with_inference_type(inference_type);
          },
          "Inference (admittance) interpolation type.")
      .def_property(
          "num_threads", &Settings::num_threads,
          [](Settings& self, size_t num_threads) -> void {
            self.with_num_threads(num_threads);
          },
          "Number of threads to use for computation. If 0, the number "
          "of threads is automatically determined.")
      // Builder-style methods returning self for method chaining
      .def("with_engine_type", &Settings::with_engine_type,
           py::arg("engine_type"),
           R"__doc__(
Set the engine type for the tidal constituent notation system.

Args:
  engine_type: The engine type.

Returns:
  self
)__doc__")
      .def("with_astronomic_formulae", &Settings::with_astronomic_formulae,
           py::arg("formulae"),
           R"__doc__(
Set the astronomic formulae.

Args:
  formulae: Astronomic formulae.

Returns:
  self
)__doc__")
      .def("with_time_tolerance", &Settings::with_time_tolerance,
           py::arg("time_tolerance"),
           R"__doc__(
Set the time tolerance.

Args:
  time_tolerance: Time tolerance in seconds during which astronomical
    angles are considered constant.

Returns:
  self
)__doc__")
      .def("with_group_modulations", &Settings::with_group_modulations,
           py::arg("group_modulations"),
           R"__doc__(
Set whether to use group modulation nodal corrections.

Args:
  group_modulations: If true, uses group modulation nodal corrections.
    Only effective with the Perth WaveTable.

Returns:
  self
)__doc__")
      .def("with_compute_long_period_equilibrium",
           &Settings::with_compute_long_period_equilibrium,
           py::arg("compute_long_period_equilibrium"),
           R"__doc__(
Set whether to compute the long period equilibrium tide.

Args:
  compute_long_period_equilibrium: If true, computes the long period
    equilibrium tide.

Returns:
  self
)__doc__")
      .def("with_inference_type", &Settings::with_inference_type,
           py::arg("inference_type"),
           R"__doc__(
Set the inference interpolation type.

Args:
  inference_type: The inference interpolation type.

Returns:
  self
)__doc__")
      .def("with_num_threads", &Settings::with_num_threads,
           py::arg("num_threads"),
           R"__doc__(
Set the number of threads to use for computation.

Args:
  num_threads: Number of threads. If 0, the number of threads is
    automatically determined.

Returns:
  self
)__doc__")
      .def("__repr__", [](const Settings& self) -> std::string {
        return "<Settings engine_type=" +
                   std::string(self.engine_type() == EngineType::kDarwin
                                   ? "DARWIN"
                                   : "DOODSON") +
                   " inference_type=" + [&]() -> std::string {
          switch (self.inference_type()) {
            case InferenceType::kSpline:
              return "SPLINE";
            case InferenceType::kZero:
              return "ZERO";
            case InferenceType::kLinear:
              return "LINEAR";
            case InferenceType::kFourier:
              return "FOURIER";
            default:
              return "UNKNOWN";
          }
        }() + " num_threads=" + std::to_string(self.num_threads()) +
                                                     ">";
      });
}

inline auto init_fes_settings(py::module& m) -> void {
  py::class_<FESSettings, Settings>(m, "FESSettings",
                                    R"__doc__(
Default settings for FES models.

This class provides pre-configured settings optimized for FES tidal models:
  - Engine type: Darwin
  - Astronomic formulae: Schureman order 1
  - Inference type: Spline
  - Long period equilibrium: enabled
  - Group modulations: N/A (not effective with the Darwin WaveTable)
)__doc__")
      .def(py::init<>(), "Default constructor with FES model settings.")
      .def("__repr__", [](const FESSettings& self) -> std::string {
        return "<FESSettings num_threads=" +
               std::to_string(self.num_threads()) + ">";
      });
}

inline auto init_perth_settings(py::module& m) -> void {
  py::class_<PerthSettings, Settings>(m, "PerthSettings",
                                      R"__doc__(
Default settings for GOT/Perth models.

This class provides pre-configured settings optimized for GOT tidal models:
  - Engine type: Doodson
  - Astronomic formulae: IERS
  - Inference type: Linear
  - Long period equilibrium: disabled
  - Group modulations: disabled
)__doc__")
      .def(py::init<>(), "Default constructor with Perth/GOT model settings.")
      .def("__repr__", [](const PerthSettings& self) -> std::string {
        return "<PerthSettings num_threads=" +
               std::to_string(self.num_threads()) + ">";
      });
}

inline auto init_generate_markdown_table(py::module& m) -> void {
  m.def(
      "generate_markdown_table",
      [](const Settings& settings, bool ascending,
         const std::vector<std::string>& modeled_constituents) -> std::string {
        auto ids = std::vector<ConstituentId>();
        ids.reserve(modeled_constituents.size());
        for (const auto& name : modeled_constituents) {
          ids.push_back(constituents::parse(name));
        }
        return generate_markdown_table(settings, ascending, ids);
      },
      py::arg("settings"), py::arg("ascending") = true,
      py::arg("modeled_constituents") = std::vector<std::string>{},
      R"__doc__(
Generate a Markdown table describing the settings, the constituents
provided by the model and inferred.

Args:
  settings: The settings for which to generate the table.
  ascending: If true (default), sort the table by ascending frequency;
    otherwise, sort by descending frequency.
  modeled_constituents: The list of constituent names provided by
    the model. Default is empty, in which case the table will only
    contain the inferred constituents.

Returns:
  A string containing the Markdown table.
)__doc__");
}

}  // namespace fes

void init_settings(py::module& m) {
  fes::init_inference_type(m);
  fes::init_settings(m);
  fes::init_fes_settings(m);
  fes::init_perth_settings(m);
  fes::init_generate_markdown_table(m);
}
