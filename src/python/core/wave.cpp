// Copyright (c) 2025 CNES
//
// All rights reserved. Use of this source code is governed by a
// BSD-style license that can be found in the LICENSE file.
#include "fes/darwin/wave.hpp"

#include <pybind11/pybind11.h>
#include <pybind11/stl.h>

#include "fes/python/datemanip.hpp"
#include "fes/python/datetime64.hpp"

namespace py = pybind11;

void init_wave(py::module& m) {
  py::class_<fes::darwin::Wave, std::shared_ptr<fes::darwin::Wave>> wave(
      m, "Wave", "Wave definition");
  py::enum_<fes::darwin::Wave::TidalType>(wave, "TidalType",
                                          "Possible type of tidal wave")
      .value("kLongPeriod", fes::darwin::Wave::kLongPeriod,
             "Long period tidal waves")
      .value("kShortPeriod", fes::darwin::Wave::kShortPeriod,
             "Short period tidal waves");

  wave.def_property_readonly("ident", &fes::darwin::Wave::ident, R"__doc__(
Gets the wave ident
)__doc__")
      .def_property_readonly("freq", &fes::darwin::Wave::freq, R"__doc__(
Gets the wave frequency (radians per seconds)
)__doc__")
      .def_property_readonly("type", &fes::darwin::Wave::type, R"__doc__(
Gets the wave type
)__doc__")
      .def_property_readonly("f", &fes::darwin::Wave::f, R"__doc__(
Gets the nodal correction for amplitude
)__doc__")
      .def_property_readonly("u", &fes::darwin::Wave::u, R"__doc__(
Gets the nodal correction for phase
)__doc__")
      .def("nodal_a", &fes::darwin::Wave::nodal_a, py::arg("a"), R"__doc__(
Compute nodal corrections from SCHUREMAN (1958).

Args:
  a: Astronomic angle
)__doc__")
      .def("nodal_g", &fes::darwin::Wave::nodal_g, py::arg("a"), R"__doc__(
Compute nodal corrections from SCHUREMAN (1958).

Args:
  a: Astronomic angle
)__doc__")
      .def("vu", &fes::darwin::Wave::vu, R"__doc__(
Gets :math:`v` (greenwich argument) + :math:`u` (nodal correction for phase)
)__doc__")
      .def("v", &fes::darwin::Wave::v, R"__doc__(
Gets :math:`v` (greenwich argument)
)__doc__")
      .def("name", &fes::darwin::Wave::name, R"__doc__(
Gets the wave name
)__doc__")
      .def_property(
          "tide",
          [](const fes::darwin::Wave& self) -> std::complex<double> {
            return self.tide();
          },
          [](fes::darwin::Wave& self, std::complex<double>& tide) {
            self.tide(tide);
          },
          R"__doc__(
Gets the tide associated to the wave.
)__doc__")
      .def_property(
          "admittance",
          [](const fes::darwin::Wave& self) -> bool {
            return self.admittance();
          },
          [](fes::darwin::Wave& self, bool& admittance) {
            self.admittance(admittance);
          },
          R"__doc__(
Returns True if the wave is computed by admittance.
)__doc__")
      .def_property(
          "dynamic",
          [](const fes::darwin::Wave& self) -> bool { return self.dynamic(); },
          [](fes::darwin::Wave& self, bool& dynamic) { self.dynamic(dynamic); },
          R"__doc__(
Returns True if the wave is computed by dynamically.
)__doc__")
      .def("period", &fes::darwin::Wave::period, R"__doc__(
Gets the wave period in hours.
)__doc__")
      .def("xdo_numerical", &fes::darwin::Wave::xdo_numerical, R"__doc__(
Gets the XDO numerical representation of the wave.

Returns:
    XDO numerical representation.
)__doc__")
      .def("xdo_alphabetical", &fes::darwin::Wave::xdo_alphabetical,
           R"__doc__(
Gets the XDO alphabetical representation of the wave.

Returns:
    XDO alphabetical representation.
)__doc__")
      .def("doodson_numbers", &fes::darwin::Wave::doodson_numbers, R"__doc__(
Gets the Doodson representation of the wave.

.. note::

      The 7th number follows the convention established in Doodson & Warburg's 1941 book.
      This number can be 0, 1, 2, or -1, representing multiples of 90 degrees
      added to the tidal argument when using cosine functions.

Returns:
    Doodson numbers.
)__doc__");
}
