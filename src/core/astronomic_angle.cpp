// Copyright (c) 2025 CNES
//
// All rights reserved. Use of this source code is governed by a
// BSD-style license that can be found in the LICENSE file.
#include <pybind11/pybind11.h>
#include <pybind11/stl.h>

#include "fes/angle/astronomic.hpp"
#include "fes/python/datemanip.hpp"

namespace py = pybind11;

namespace fes {

inline void init_angle(py::module& m) {
  py::enum_<angle::Formulae>(m, "Formulae")
      .value("SCHUREMAN_ORDER_1", angle::Formulae::kSchuremanOrder1)
      .value("SCHUREMAN_ORDER_3", angle::Formulae::kSchuremanOrder3)
      .value("MEEUS", angle::Formulae::kMeeus)
      .value("IERS", angle::Formulae::kIERS)
      .export_values();

  py::class_<angle::Astronomic>(m, "AstronomicAngle")
      .def(py::init<angle::Formulae>(),
           py::arg("formulae") = angle::Formulae::kSchuremanOrder1,
           R"__doc__(
Build an astronomic angle object.

Args:
    formulae: Formulae used to compute the astronomic angles. Default is
        Schureman order 1.
)__doc__")
      .def(
          "update",
          [](angle::Astronomic& self, const py::handle& date) {
            self.update(python::datemanip::as_float64(date));
          },
          py::arg("date"),
          R"__doc__(
Update the astronomic angles.

Args:
  date: Desired UTC time
)__doc__")
      .def_property_readonly("t", &angle::Astronomic::t,
                             "Hour angle of mean sun")
      .def_property_readonly("n", &angle::Astronomic::n,
                             "Longitude of the moon's node")
      .def_property_readonly("h", &angle::Astronomic::h,
                             "Mean longitude of the sun")
      .def_property_readonly("s", &angle::Astronomic::s,
                             "Mean longitude of the moon")
      .def_property_readonly("p1", &angle::Astronomic::p1,
                             "Mean longitude of solar perigee")
      .def_property_readonly("p", &angle::Astronomic::p,
                             "Mean longitude of lunar perigee")
      .def_property_readonly(
          "i", &angle::Astronomic::i,
          "Obliquity of lunar orbit with respect to earth's equator")
      .def_property_readonly(
          "xi", &angle::Astronomic::xi,
          "Longitude in the moon's orbit of lunar intersection")
      .def_property_readonly("nu", &angle::Astronomic::nu,
                             "Right ascension of lunar intersection")
      .def_property_readonly("x1ra", &angle::Astronomic::x1ra,
                             "Factor in amplitude of constituent :math:`L_{2}`")
      .def_property_readonly("r", &angle::Astronomic::r,
                             "Term in argument of constituent :math:`L_{2}`")
      .def_property_readonly(
          "nuprim", &angle::Astronomic::nuprim,
          "Term in argument of lunisolar constituent :math:`K_{1}`")
      .def_property_readonly(
          "nusec", &angle::Astronomic::nusec,
          "Term in argument of lunisolar constituent :math:`K_{2}`");
}

}  // namespace fes

void init_angle(py::module& m) { fes::init_angle(m); }
