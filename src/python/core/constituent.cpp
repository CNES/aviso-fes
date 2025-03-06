// Copyright (c) 2025 CNES
//
// All rights reserved. Use of this source code is governed by a
// BSD-style license that can be found in the LICENSE file.
#include "fes/constituent.hpp"

#include <pybind11/pybind11.h>
#include <pybind11/stl.h>

namespace py = pybind11;

void init_constituent(py::module& m) {
  py::enum_<fes::Constituent>(m, "Constituent")
      .value("kMm", fes::kMm, ":math:`Mm`")
      .value("kMf", fes::kMf, ":math:`Mf`")
      .value("kMtm", fes::kMtm, ":math:`Mtm`")
      .value("kMsqm", fes::kMsqm, ":math:`Msqm`")
      .value("k2Q1", fes::k2Q1, ":math:`2Q_{1}`")
      .value("kSigma1", fes::kSigma1, ":math:`\\sigma_{1}`")
      .value("kQ1", fes::kQ1, ":math:`Q_{1}`")
      .value("kRho1", fes::kRho1, ":math:`\\rho_{1}`")
      .value("kO1", fes::kO1, ":math:`O_{1}`")
      .value("kMP1", fes::kMP1, ":math:`MP_{1}`")
      .value("kM11", fes::kM11, ":math:`M_{11}`")
      .value("kM12", fes::kM12, ":math:`M_{12}`")
      .value("kM13", fes::kM13, ":math:`M_{13}`")
      .value("kChi1", fes::kChi1, ":math:`\\chi_{1}`")
      .value("kPi1", fes::kPi1, ":math:`\\pi_{1}`")
      .value("kP1", fes::kP1, ":math:`P_{1}`")
      .value("kS1", fes::kS1, ":math:`S_{1}`")
      .value("kK1", fes::kK1, ":math:`K_{1}`")
      .value("kPsi1", fes::kPsi1, ":math:`\\psi_{1}`")
      .value("kPhi1", fes::kPhi1, ":math:`\\varphi_{1}`")
      .value("kTheta1", fes::kTheta1, ":math:`\\theta_{1}`")
      .value("kJ1", fes::kJ1, ":math:`J_{1}`")
      .value("kOO1", fes::kOO1, ":math:`OO_{1}`")
      .value("kMNS2", fes::kMNS2, ":math:`MNS_{2}`")
      .value("kEps2", fes::kEps2, ":math:`\\varepsilon_{2}`")
      .value("k2N2", fes::k2N2, ":math:`2N_{2}`")
      .value("kMu2", fes::kMu2, ":math:`\\upsilon_{2}`")
      .value("k2MS2", fes::k2MS2, ":math:`2MS_{2}`")
      .value("kN2", fes::kN2, ":math:`N_{2}`")
      .value("kNu2", fes::kNu2, ":math:`\\nu_{2}`")
      .value("kM2", fes::kM2, ":math:`M_{2}`")
      .value("kMKS2", fes::kMKS2, ":math:`MKS_{2}`")
      .value("kLambda2", fes::kLambda2, ":math:`\\lambda_{2}`")
      .value("kL2", fes::kL2, ":math:`L_{2}`")
      .value("k2MN2", fes::k2MN2, ":math:`2MN_{2}`")
      .value("kT2", fes::kT2, ":math:`T_{2}`")
      .value("kS2", fes::kS2, ":math:`S_{2}`")
      .value("kR2", fes::kR2, ":math:`R_{2}`")
      .value("kK2", fes::kK2, ":math:`K_{2}`")
      .value("kMSN2", fes::kMSN2, ":math:`MSN_{2}`")
      .value("kEta2", fes::kEta2, ":math:`\\eta_{2}`")
      .value("k2SM2", fes::k2SM2, ":math:`2SM_{2}`")
      .value("kMO3", fes::kMO3, ":math:`MO_{3}`")
      .value("k2MK3", fes::k2MK3, ":math:`2MK_{3}`")
      .value("kM3", fes::kM3, ":math:`M_{3}`")
      .value("kMK3", fes::kMK3, ":math:`MK_{3}`")
      .value("kN4", fes::kN4, ":math:`N_{4}`")
      .value("kMN4", fes::kMN4, ":math:`MN_{4}`")
      .value("kM4", fes::kM4, ":math:`M_{4}`")
      .value("kSN4", fes::kSN4, ":math:`SN_{4}`")
      .value("kMS4", fes::kMS4, ":math:`MS_{4}`")
      .value("kMK4", fes::kMK4, ":math:`MK_{4}`")
      .value("kS4", fes::kS4, ":math:`S_{4}`")
      .value("kSK4", fes::kSK4, ":math:`SK_{4}`")
      .value("kR4", fes::kR4, ":math:`R_{4}`")
      .value("k2MN6", fes::k2MN6, ":math:`2MN_{6}`")
      .value("kM6", fes::kM6, ":math:`M_{6}`")
      .value("kMSN6", fes::kMSN6, ":math:`MSN_{6}`")
      .value("k2MS6", fes::k2MS6, ":math:`2MS_{6}`")
      .value("k2MK6", fes::k2MK6, ":math:`2MK_{6}`")
      .value("k2SM6", fes::k2SM6, ":math:`2SM_{6}`")
      .value("kMSK6", fes::kMSK6, ":math:`MSK_{6}`")
      .value("kS6", fes::kS6, ":math:`S_{6}`")
      .value("kM8", fes::kM8, ":math:`M_{8}`")
      .value("kMSf", fes::kMSf, ":math:`MSf`")
      .value("kSsa", fes::kSsa, ":math:`Ssa`")
      .value("kSa", fes::kSa, ":math:`Sa`")
      .value("kA5", fes::kA5, ":math:`A_{5}`")
      .value("kSa1", fes::kSa1, ":math:`Sa_{1}`")
      .value("kSta", fes::kSta, ":math:`Sta`")
      .value("kMm2", fes::kMm2, ":math:`Mm_{2}`")
      .value("kMm1", fes::kMm1, ":math:`Mm_{1}`")
      .value("kMf1", fes::kMf1, ":math:`Mf_{1}`")
      .value("kMf2", fes::kMf2, ":math:`Mf_{2}`")
      .value("kM0", fes::kM0, ":math:`M_{0}`")
      .export_values();

  m.def_submodule("constituents", "Constituent names.")
      .def("parse", &fes::constituents::parse, py::arg("name"),
           R"__doc__(
Parse a constituent name.

Args:
  name: Constituent name. Search is not case sensitive. So "M1" and "m1" are
    equivalent.

Returns:
  Constituent enum value.
)__doc__")
      .def("name", &fes::constituents::name, py::arg("constituent"),
           R"__doc__(
Return the name of a constituent.

Args:
  constituent: Constituent enum value.

Returns:
  Constituent name.
)__doc__")
      .def("known", &fes::constituents::known,
           R"__doc__(
Return the list of known constituents by this library.

Returns:
  List of constituent names.
)__doc__");
}
