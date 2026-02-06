// Copyright (c) 2026 CNES
//
// All rights reserved. Use of this source code is governed by a
// BSD-style license that can be found in the LICENSE file.
#include <pybind11/pybind11.h>

namespace py = pybind11;

extern void init_angle(py::module& m);
extern void init_axis(py::module& m);
extern void init_cartesian_model(py::module& m);
extern void init_constituents(py::module& m);
extern void init_darwin(py::module& m);
extern void init_datemanip(py::module& m);
extern void init_harmonic_analysis(py::module& m);
extern void init_lgp_model(py::module& m);
extern void init_lpe_tide(py::module& m);
extern void init_mesh_index(py::module& m);
extern void init_perth(py::module& m);
extern void init_settings(py::module& m);
extern void init_tidal_model_interface(py::module& m);
extern void init_tide(py::module& m);
extern void init_wave_interface(py::module& m);
extern void init_wave_table_interface(py::module& m);

PYBIND11_MODULE(core, m) {
  m.doc() = "FES Tidal Prediction Library";
  auto tidal_model = m.def_submodule("tidal_model", "Tidal model handlers");

  // Common types and utilities
  init_angle(m);
  init_axis(m);
  init_datemanip(m);

  // Constituent query and parsing
  init_constituents(m);

  // Abstract interfaces (must be registered before concrete implementations)
  init_wave_interface(m);
  init_wave_table_interface(m);
  init_tidal_model_interface(m);

  // Settings (depends on EngineType, Formulae, and InferenceType enums)
  init_settings(m);

  // Mesh index
  init_mesh_index(m);

  // Concrete implementations of WaveInterface and WaveTableInterface
  init_darwin(m);
  init_perth(m);

  // Concrete implementations of tidal model interface
  init_cartesian_model(tidal_model);
  init_lgp_model(tidal_model);

  // Tide evaluation function (depends on all the above)
  init_tide(m);
  init_lpe_tide(m);
}
