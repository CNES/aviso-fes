// Copyright (c) 2025 CNES
//
// All rights reserved. Use of this source code is governed by a
// BSD-style license that can be found in the LICENSE file.
#include "fes/darwin/wave.hpp"

#include <pybind11/pybind11.h>
#include <pybind11/stl.h>

namespace py = pybind11;

namespace fes {
namespace darwin {

static void init_wave(py::module& m) {
  py::class_<Wave, std::shared_ptr<Wave>>(m, "Wave", "Wave definition")
      .def_property_readonly("freq", &Wave::freq, R"__doc__(
Gets the wave frequency (radians per seconds)
)__doc__")
      .def_property_readonly("type", &Wave::type, R"__doc__(
Gets the wave type
)__doc__")
      .def_property_readonly("f", &Wave::f, R"__doc__(
Gets the nodal correction for amplitude
)__doc__")
      .def_property_readonly("u", &Wave::u, R"__doc__(
Gets the nodal correction for phase
)__doc__")
      .def("nodal_a", &Wave::nodal_a, py::arg("a"), R"__doc__(
Compute nodal corrections from SCHUREMAN (1958).

Args:
  a: Astronomic angle
)__doc__")
      .def("nodal_g", &Wave::nodal_g, py::arg("a"), R"__doc__(
Compute nodal corrections from SCHUREMAN (1958).

Args:
  a: Astronomic angle
)__doc__")
      .def("vu", &Wave::vu, R"__doc__(
Gets :math:`v` (greenwich argument) + :math:`u` (nodal correction for phase)
)__doc__")
      .def("v", &Wave::v, R"__doc__(
Gets :math:`v` (greenwich argument)
)__doc__")
      .def("name", &Wave::name, R"__doc__(
Gets the wave name
)__doc__")
      .def_property(
          "tide",
          [](const Wave& self) -> std::complex<double> { return self.tide(); },
          [](Wave& self, std::complex<double>& tide) { self.tide(tide); },
          R"__doc__(
Gets the tide associated to the wave.
)__doc__")
      .def_property(
          "admittance",
          [](const Wave& self) -> bool { return self.admittance(); },
          [](Wave& self, bool& admittance) { self.admittance(admittance); },
          R"__doc__(
Returns True if the wave is computed by admittance.
)__doc__")
      .def_property(
          "dynamic", [](const Wave& self) -> bool { return self.dynamic(); },
          [](Wave& self, bool& dynamic) { self.dynamic(dynamic); },
          R"__doc__(
Returns True if the wave is computed by dynamically.
)__doc__")
      .def("period", &Wave::period, R"__doc__(
Gets the wave period in hours.
)__doc__")
      .def("xdo_numerical", &Wave::xdo_numerical, R"__doc__(
Gets the XDO numerical representation of the wave.

Returns:
    XDO numerical representation.
)__doc__")
      .def("xdo_alphabetical", &Wave::xdo_alphabetical,
           R"__doc__(
Gets the XDO alphabetical representation of the wave.

Returns:
    XDO alphabetical representation.
)__doc__")
      .def("doodson_numbers", &Wave::doodson_numbers, R"__doc__(
Gets the Doodson representation of the wave.

.. note::

      The 7th number follows the convention established in Doodson & Warburg's 1941 book.
      This number can be 0, 1, 2, or -1, representing multiples of 90 degrees
      added to the tidal argument when using cosine functions.

Returns:
    Doodson numbers.
)__doc__");
}

}  // namespace darwin
}  // namespace fes

auto init_darwin_wave(py::module& m) -> void { fes::darwin::init_wave(m); }
