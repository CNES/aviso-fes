// Copyright (c) 2025 CNES
//
// All rights reserved. Use of this source code is governed by a
// BSD-style license that can be found in the LICENSE file.
#include <pybind11/pybind11.h>

namespace py = pybind11;

extern void init_abstract_tide_model(py::module& m, py::module& perth);
extern void init_angle(py::module& m);
extern void init_settings(py::module& m, py::module& perth);
extern void init_axis(py::module& m);
extern void init_cartesian_model(py::module& m, py::module& perth);
extern void init_constituent(py::module& m, py::module& perth);
extern void init_datemanip(py::module& m);
extern void init_lgp_model(py::module& m, py::module& perth);
extern void init_mesh_index(py::module& m);
extern void init_tide(py::module& m);
extern void init_perth_tide(py::module& m);
extern void init_wave_order2(py::module& m);
extern void init_wave_table(py::module& m);
extern void init_wave(py::module& m);

// Third-party library initialization
// NOLINTNEXTLINE(cppcoreguidelines-*,*use-trailing-return-type)
PYBIND11_MODULE(core, m) {
  auto datemanip = m.def_submodule("datemanip", "Date manipulation");
  auto tidal_model = m.def_submodule("tidal_model", "Tidal model");
  auto mesh = m.def_submodule("mesh", "Mesh");
  auto perth = m.def_submodule("perth", "PERTH-specific models and settings.");
  auto perth_tidal_model =
      tidal_model.def_submodule("perth", "PERTH-specific tidal models.");

  // Date manipulation (convert the date to UTC seconds, always ignoring the
  // local time zone)
  init_datemanip(datemanip);

  // Define the tidal constituents
  init_constituent(m, perth);

  // Define the objects handling astronomical angles, axis and tidal waves.
  init_angle(m);
  init_axis(m);
  init_wave(m);

  // Define wave handling objects
  init_wave_table(m);
  init_wave_order2(m);

  // Define the mesh indexer.
  init_mesh_index(mesh);

  // Define the calculation settings.
  init_settings(m, perth);

  // Define the tidal models.
  init_abstract_tide_model(m, perth);
  init_cartesian_model(tidal_model, perth_tidal_model);
  init_lgp_model(tidal_model, perth_tidal_model);

  // Define the tide estimator.
  init_tide(m);

  // Define perth tide estimator.
  init_perth_tide(perth);
}
