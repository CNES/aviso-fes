// Copyright (c) 2025 CNES
//
// All rights reserved. Use of this source code is governed by a
// BSD-style license that can be found in the LICENSE file.
#include "fes/wave.hpp"

#include <pybind11/pybind11.h>
#include <pybind11/stl.h>

#include "fes/python/datemanip.hpp"
#include "fes/python/datetime64.hpp"

namespace py = pybind11;

void init_wave(py::module& m) {
  py::class_<fes::Wave, std::shared_ptr<fes::Wave>> wave(m, "Wave",
                                                         "Wave definition");
  py::enum_<fes::Wave::TidalType>(wave, "TidalType",
                                  "Possible type of tidal wave")
      .value("kLongPeriod", fes::Wave::kLongPeriod, "Long period tidal waves")
      .value("kShortPeriod", fes::Wave::kShortPeriod,
             "Short period tidal waves");

  wave.def_property_readonly("ident", &fes::Wave::ident, R"__doc__(
Gets the wave ident
)__doc__")
      .def_property_readonly("freq", &fes::Wave::freq, R"__doc__(
Gets the wave frequency (radians per seconds)
)__doc__")
      .def_property_readonly("type", &fes::Wave::type, R"__doc__(
Gets the wave type
)__doc__")
      .def_property_readonly("f", &fes::Wave::f, R"__doc__(
Gets the nodal correction for amplitude
)__doc__")
      .def_property_readonly("u", &fes::Wave::u, R"__doc__(
Gets the nodal correction for phase
)__doc__")
      .def("nodal_a", &fes::Wave::nodal_a, py::arg("a"), R"__doc__(
Compute nodal corrections from SCHUREMAN (1958).

Args:
  a: Astronomic angle
)__doc__")
      .def("nodal_g", &fes::Wave::nodal_g, py::arg("a"), R"__doc__(
Compute nodal corrections from SCHUREMAN (1958).

Args:
  a: Astronomic angle
)__doc__")
      .def("vu", &fes::Wave::vu, R"__doc__(
Gets :math:`v` (greenwich argument) + :math:`u` (nodal correction for phase)
)__doc__")
      .def("v", &fes::Wave::v, R"__doc__(
Gets :math:`v` (greenwich argument)
)__doc__")
      .def("name", &fes::Wave::name, R"__doc__(
Gets the wave name
)__doc__")
      .def_property(
          "tide",
          [](const fes::Wave& self) -> std::complex<double> {
            return self.tide();
          },
          [](fes::Wave& self, std::complex<double>& tide) { self.tide(tide); },
          R"__doc__(
Gets the tide associated to the wave.
)__doc__")
      .def_property(
          "admittance",
          [](const fes::Wave& self) -> bool { return self.admittance(); },
          [](fes::Wave& self, bool& admittance) {
            self.admittance(admittance);
          },
          R"__doc__(
Returns True if the wave is computed by admittance.
)__doc__")
      .def_property(
          "dynamic",
          [](const fes::Wave& self) -> bool { return self.dynamic(); },
          [](fes::Wave& self, bool& dynamic) { self.dynamic(dynamic); },
          R"__doc__(
Returns True if the wave is computed by dynamically.
)__doc__")
      .def("period", &fes::Wave::period, R"__doc__(
Gets the wave period in hours.
)__doc__")
      .def("doodson_number", &fes::Wave::doodson_number, R"__doc__(
Gets the doodson number.

Returns:
    Doodson number.
)__doc__");
}
