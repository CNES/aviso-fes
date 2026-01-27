// Copyright (c) 2026 CNES
//
// All rights reserved. Use of this source code is governed by a
// BSD-style license that can be found in the LICENSE file.
#include <pybind11/pybind11.h>

namespace py = pybind11;

extern void init_abstract_tidal_model(py::module& m);
extern void init_angle(py::module& m);
extern void init_axis(py::module& m);
extern void init_cartesian_model(py::module& m);
extern void init_constituent(py::module& m, py::module& darwin,
                             py::module& perth);
extern void init_darwin_long_period_equilibrium_tide(py::module& m);
extern void init_darwin_tide(py::module& m);
extern void init_darwin_table(py::module& m);
extern void init_darwin_wave(py::module& m);
extern void init_datemanip(py::module& m);
extern void init_lgp_model(py::module& m);
extern void init_mesh_index(py::module& m);
extern void init_perth_table(py::module& m);
extern void init_settings(py::module& m);
extern void init_tide(py::module& m);

// Third-party library initialization
// NOLINTNEXTLINE(cppcoreguidelines-*,*use-trailing-return-type)
PYBIND11_MODULE(core, m) {
  auto datemanip = m.def_submodule("datemanip", "Date manipulation");
  auto tidal_model = m.def_submodule("tidal_model", "Tidal model");
  auto mesh = m.def_submodule("mesh", "Mesh");
  auto darwin = m.def_submodule("darwin", "FES tidal model");
  auto perth = m.def_submodule("perth", "PERTH tidal model");

  // Date manipulation (convert the date to UTC seconds, always ignoring the
  // local time zone)
  init_datemanip(datemanip);

  // Settings for the tide computation
  init_angle(m);
  init_settings(m);

  // Mesh handling
  init_mesh_index(mesh);

  // Tidal model abstractions
  init_axis(m);
  init_abstract_tidal_model(tidal_model);
  init_cartesian_model(tidal_model);
  init_lgp_model(tidal_model);

  // Tidal constituents for FES and Perth5
  init_constituent(m, darwin, perth);

  // FES Model
  init_darwin_wave(darwin);
  init_darwin_table(darwin);
  init_darwin_long_period_equilibrium_tide(darwin);
  init_darwin_tide(darwin);

  // Perth5 Model
  init_perth_table(perth);

  // Unified tide evaluation (dispatches based on settings)
  init_tide(m);
}
