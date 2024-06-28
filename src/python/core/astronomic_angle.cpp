// Copyright (c) 2023 CNES
//
// All rights reserved. Use of this source code is governed by a
// BSD-style license that can be found in the LICENSE file.
#include <pybind11/pybind11.h>
#include <pybind11/stl.h>

#include "fes/angle/astronomic.hpp"
#include "fes/python/datemanip.hpp"

namespace py = pybind11;

void init_angle(py::module& m) {
  py::enum_<fes::angle::Formulae>(m, "Formulae")
      .value("kSchuremanOrder1", fes::angle::Formulae::kSchuremanOrder1)
      .value("kSchuremanOrder3", fes::angle::Formulae::kSchuremanOrder3)
      .value("kMeeus", fes::angle::Formulae::kMeeus)
      .export_values();

  py::class_<fes::angle::Astronomic>(m, "AstronomicAngle")
      .def(py::init<fes::angle::Formulae>(),
           py::arg("formulae") = fes::angle::Formulae::kSchuremanOrder1,
           R"__doc__(
Build an astronomic angle object.

Args:
    formulae: Formulae used to compute the astronomic angles. Default is
        Schureman order 1.
)__doc__")
      .def(
          "update",
          [](fes::angle::Astronomic& self, const py::handle& date,
             const int16_t leap_seconds) {
            self.update(fes::python::datemanip::as_float64(date), leap_seconds);
          },
          py::arg("date"), py::arg("leap_seconds"),
          R"__doc__(
Update the astronomic angles.

Args:
  date: Desired UTC time
  leap_seconds: Number of leap seconds to add to UTC time to get TAI time
)__doc__")
      .def_property_readonly("t", &fes::angle::Astronomic::t,
                             "Hour angle of mean sun")
      .def_property_readonly("n", &fes::angle::Astronomic::n,
                             "Longitude of the moon's node")
      .def_property_readonly("h", &fes::angle::Astronomic::h,
                             "Mean longitude of the sun")
      .def_property_readonly("s", &fes::angle::Astronomic::s,
                             "Mean longitude of the moon")
      .def_property_readonly("p1", &fes::angle::Astronomic::p1,
                             "Mean longitude of solar perigee")
      .def_property_readonly("p", &fes::angle::Astronomic::p,
                             "Mean longitude of lunar perigee")
      .def_property_readonly(
          "i", &fes::angle::Astronomic::i,
          "Obliquity of lunar orbit with respect to earth's equator")
      .def_property_readonly(
          "xi", &fes::angle::Astronomic::xi,
          "Longitude in the moon's orbit of lunar intersection")
      .def_property_readonly("nu", &fes::angle::Astronomic::nu,
                             "Right ascension of lunar intersection")
      .def_property_readonly("x1ra", &fes::angle::Astronomic::x1ra,
                             "Factor in amplitude of constituent :math:`L_{2}`")
      .def_property_readonly("r", &fes::angle::Astronomic::r,
                             "Term in argument of constituent :math:`L_{2}`")
      .def_property_readonly(
          "nuprim", &fes::angle::Astronomic::nuprim,
          "Term in argument of lunisolar constituent :math:`K_{1}`")
      .def_property_readonly(
          "nusec", &fes::angle::Astronomic::nusec,
          "Term in argument of lunisolar constituent :math:`K_{2}`");
}
