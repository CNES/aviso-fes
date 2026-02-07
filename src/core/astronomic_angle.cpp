// Copyright (c) 2026 CNES
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
  py::enum_<angle::Formulae>(
      m, "Formulae",
      "This Enum class encapsulates the various astronomical formulae required "
      "for computing astronomical angles.")
      .value("SCHUREMAN_ORDER_1", angle::Formulae::kSchuremanOrder1,
             "First-order polynomial expressions from Schureman (1940), Table "
             "1, p. 162. Uses the J1900.0 epoch and UTC time scale. This is "
             "the simplest and most traditional formula set, suitable for "
             "predictions within a few decades of the reference epoch.")
      .value("SCHUREMAN_ORDER_3", angle::Formulae::kSchuremanOrder3,
             "Third-order polynomial expressions extending Schureman's "
             "formulae with quadratic and cubic correction terms. Same J1900.0 "
             "epoch and UTC time scale, but with improved accuracy for dates "
             "further from the reference epoch.")
      .value("MEEUS", angle::Formulae::kMeeus,
             "Expressions from Jean Meeus, *Astronomical Algorithms* (2nd ed., "
             "1998). Uses the J2000.0 epoch and Terrestrial Dynamical Time "
             "(TDT), with fourth-order polynomials derived from modern "
             "ephemerides. PyFES automatically applies the UTC-to-TDT "
             "correction when this formula set is selected.")
      .value("IERS", angle::Formulae::kIERS,
             "Expressions from the IERS Conventions (2010), based on Simon et "
             "al.(1994). Uses the J2000.0 epoch and TDT, computing the five "
             "lunisolarfundamental arguments (:math:`l, l', F, D, \\Omega`) "
             "and converting them to the traditional Doodson variables.")
      .export_values();

  py::class_<angle::Astronomic>(
      m, "AstronomicAngle",
      "The :class:`AstronomicAngle` class computes and stores the astronomical "
      "angles needed for tidal prediction and harmonic analysis. This include "
      "the six :term:`fundamental variables` of the harmonic development as "
      "well as auxiliary angles derived from the lunar node longitude "
      ":math:`N`, which are required for :term:`nodal corrections <nodal "
      "correction>`.")
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
          [](angle::Astronomic& self, const py::handle& date) -> void {
            self.update(python::datemanip::as_float64(date));
          },
          py::arg("date"),
          R"__doc__(
Update the astronomic angles.

Args:
  date: Desired UTC time
)__doc__")
      .def_property_readonly("t", &angle::Astronomic::t,
                             "Mean solar angle relative to Greenwich (hour "
                             "angle of the mean Sun), in radians")
      .def_property_readonly("n", &angle::Astronomic::n,
                             "Longitude of the Moon's ascending node, in "
                             "radians (~0.053°/day, 18.61-year cycle)")
      .def_property_readonly(
          "h", &angle::Astronomic::h,
          "Mean longitude of the Sun, in radians (~0.99°/day)")
      .def_property_readonly(
          "s", &angle::Astronomic::s,
          "Mean longitude of the Moon, in radians (~13.18°/day).")
      .def_property_readonly("p1", &angle::Astronomic::p1,
                             "Longitude of the solar perihelion, in radians "
                             "(~0.00005°/day, ~20,940-year cycle)")
      .def_property_readonly("p", &angle::Astronomic::p,
                             "Longitude of the lunar perigee, in radians "
                             "(~0.11°/day, 8.85-year cycle)")
      .def_property_readonly(
          "i", &angle::Astronomic::i,
          "Obliquity of the lunar orbit to the celestial equator (:math:`I`), "
          "in radians. Varies between ~18.3° and ~28.6° over the nodal cycle.")
      .def_property_readonly("xi", &angle::Astronomic::xi,
                             "Longitude in the Moon's orbit of the lunar "
                             "intersection (:math:`\\xi`), in radians.")
      .def_property_readonly("nu", &angle::Astronomic::nu,
                             "Right ascension of the lunar intersection "
                             "(:math:`\\nu`), in radians.")
      .def_property_readonly(
          "x1ra", &angle::Astronomic::x1ra,
          "Amplitude factor for constituent :math:`L_2` (:math:`1/P_a`), "
          "dimensionless. Schureman formula 213.")
      .def_property_readonly("r", &angle::Astronomic::r,
                             "Phase factor for constituent :math:`L_2` "
                             "(:math:`R`), in radians. Schureman formula 196.")
      .def_property_readonly(
          "nuprim", &angle::Astronomic::nuprim,
          "Correction angle for :math:`K_1` (:math:`\\nu'`), in radians. "
          "Combines lunar and solar diurnal contributions.")
      .def_property_readonly(
          "nusec", &angle::Astronomic::nusec,
          "Correction angle for :math:`K_2` (:math:`\\nu''`), in radians. "
          "Combines lunar and solar semidiurnal contributions.");
}

}  // namespace fes

void init_angle(py::module& m) { fes::init_angle(m); }
