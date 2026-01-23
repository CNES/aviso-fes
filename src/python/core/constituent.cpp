// Copyright (c) 2025 CNES
//
// All rights reserved. Use of this source code is governed by a
// BSD-style license that can be found in the LICENSE file.
#include "fes/darwin/constituent.hpp"

#include <pybind11/pybind11.h>
#include <pybind11/stl.h>

#include "fes/perth/constituent.hpp"

namespace py = pybind11;

static void init_darwin_constituent(py::module& m) {
  py::enum_<fes::darwin::Constituent>(m, "Constituent")
      .value("k2MK2", fes::darwin::k2MK2, ":math:`2MK_{2}`")
      .value("k2MK3", fes::darwin::k2MK3, ":math:`2MK_{3}`")
      .value("k2MK6", fes::darwin::k2MK6, ":math:`2MK_{6}`")
      .value("k2MN2", fes::darwin::k2MN2, ":math:`2MN_{2}`")
      .value("k2MN6", fes::darwin::k2MN6, ":math:`2MN_{6}`")
      .value("k2MNS4", fes::darwin::k2MNS4, ":math:`2MNS_{4}`")
      .value("k2MP5", fes::darwin::k2MP5, ":math:`2MP_{5}`")
      .value("k2MS2", fes::darwin::k2MS2, ":math:`2MS_{2}`")
      .value("k2MS6", fes::darwin::k2MS6, ":math:`2MS_{6}`")
      .value("k2MSN4", fes::darwin::k2MSN4, ":math:`2MSN_{4}`")
      .value("k2N2", fes::darwin::k2N2, ":math:`2N_{2}`")
      .value("k2NM6", fes::darwin::k2NM6, ":math:`2NM_{6}`")
      .value("k2NS2", fes::darwin::k2NS2, ":math:`2NS_{2}`")
      .value("k2Q1", fes::darwin::k2Q1, ":math:`2Q_{1}`")
      .value("k2SM2", fes::darwin::k2SM2, ":math:`2SM_{2}`")
      .value("k2SM6", fes::darwin::k2SM6, ":math:`2SM_{6}`")
      .value("k2SMu2", fes::darwin::k2SMu2, ":math:`2SMu_{2}`")
      .value("k3MS4", fes::darwin::k3MS4, ":math:`3MS_{4}`")
      .value("k3MS8", fes::darwin::k3MS8, ":math:`3MS_{8}`")
      .value("kA5", fes::darwin::kA5, ":math:`A_{5}`")
      .value("kChi1", fes::darwin::kChi1, ":math:`\\chi_{1}`")
      .value("kEps2", fes::darwin::kEps2, ":math:`\\varepsilon_{2}`")
      .value("kEta2", fes::darwin::kEta2, ":math:`\\eta_{2}`")
      .value("kJ1", fes::darwin::kJ1, ":math:`J_{1}`")
      .value("kK1", fes::darwin::kK1, ":math:`K_{1}`")
      .value("kK2", fes::darwin::kK2, ":math:`K_{2}`")
      .value("kL2", fes::darwin::kL2, ":math:`L_{2}`")
      .value("kL2P", fes::darwin::kL2P, ":math:`L2_{P}`")
      .value("kLambda2", fes::darwin::kLambda2, ":math:`\\lambda_{2}`")
      .value("kM0", fes::darwin::kM0, ":math:`M_{0}`")
      .value("kM1", fes::darwin::kM1, ":math:`M1`")
      .value("kM11", fes::darwin::kM11, ":math:`M1_{1}`")
      .value("kM12", fes::darwin::kM12, ":math:`M1_{2}`")
      .value("kM13", fes::darwin::kM13, ":math:`M1_{3}`")
      .value("kM2", fes::darwin::kM2, ":math:`M_{2}`")
      .value("kM3", fes::darwin::kM3, ":math:`M_{3}`")
      .value("kM4", fes::darwin::kM4, ":math:`M_{4}`")
      .value("kM6", fes::darwin::kM6, ":math:`M_{6}`")
      .value("kM8", fes::darwin::kM8, ":math:`M_{8}`")
      .value("kMf", fes::darwin::kMf, ":math:`Mf`")
      .value("kMf1", fes::darwin::kMf1, ":math:`Mf_{1}`")
      .value("kMf2", fes::darwin::kMf2, ":math:`Mf_{2}`")
      .value("kMK3", fes::darwin::kMK3, ":math:`MK_{3}`")
      .value("kMK4", fes::darwin::kMK4, ":math:`MK_{4}`")
      .value("kMKS2", fes::darwin::kMKS2, ":math:`MKS_{2}`")
      .value("kML4", fes::darwin::kML4, ":math:`ML_{4}`")
      .value("kMm", fes::darwin::kMm, ":math:`Mm`")
      .value("kMm1", fes::darwin::kMm1, ":math:`Mm_{1}`")
      .value("kMm2", fes::darwin::kMm2, ":math:`Mm_{2}`")
      .value("kMN4", fes::darwin::kMN4, ":math:`MN_{4}`")
      .value("kMNK6", fes::darwin::kMNK6, ":math:`MNK_{6}`")
      .value("kMNS2", fes::darwin::kMNS2, ":math:`MNS_{2}`")
      .value("kMNu4", fes::darwin::kMNu4, ":math:`MNu_{4}`")
      .value("kMNuS2", fes::darwin::kMNuS2, ":math:`MNuS_{2}`")
      .value("kMO3", fes::darwin::kMO3, ":math:`MO_{3}`")
      .value("kMP1", fes::darwin::kMP1, ":math:`MP_{1}`")
      .value("kMS4", fes::darwin::kMS4, ":math:`MS_{4}`")
      .value("kMSf", fes::darwin::kMSf, ":math:`MSf`")
      .value("kMSK2", fes::darwin::kMSK2, ":math:`MSK_{2}`")
      .value("kMSK6", fes::darwin::kMSK6, ":math:`MSK_{6}`")
      .value("kMSN2", fes::darwin::kMSN2, ":math:`MSN_{2}`")
      .value("kMSN6", fes::darwin::kMSN6, ":math:`MSN_{6}`")
      .value("kMSqm", fes::darwin::kMSqm, ":math:`MSqm`")
      .value("kMtm", fes::darwin::kMtm, ":math:`Mtm`")
      .value("kMu2", fes::darwin::kMu2, ":math:`\\upsilon_{2}`")
      .value("kN2", fes::darwin::kN2, ":math:`N_{2}`")
      .value("kN2P", fes::darwin::kN2P, ":math:`N2_{P}`")
      .value("kN4", fes::darwin::kN4, ":math:`N_{4}`")
      .value("kNK4", fes::darwin::kNK4, ":math:`NK_{4}`")
      .value("kNKM2", fes::darwin::kNKM2, ":math:`NKM_{2}`")
      .value("kNu2", fes::darwin::kNu2, ":math:`\\nu_{2}`")
      .value("kO1", fes::darwin::kO1, ":math:`O_{1}`")
      .value("kOO1", fes::darwin::kOO1, ":math:`OO_{1}`")
      .value("kOQ2", fes::darwin::kOQ2, ":math:`OQ_{2}`")
      .value("kP1", fes::darwin::kP1, ":math:`P_{1}`")
      .value("kPhi1", fes::darwin::kPhi1, ":math:`\\varphi_{1}`")
      .value("kPi1", fes::darwin::kPi1, ":math:`\\pi_{1}`")
      .value("kPsi1", fes::darwin::kPsi1, ":math:`\\psi_{1}`")
      .value("kQ1", fes::darwin::kQ1, ":math:`Q_{1}`")
      .value("kR2", fes::darwin::kR2, ":math:`R_{2}`")
      .value("kR4", fes::darwin::kR4, ":math:`R_{4}`")
      .value("kRho1", fes::darwin::kRho1, ":math:`\\rho_{1}`")
      .value("kS1", fes::darwin::kS1, ":math:`S_{1}`")
      .value("kS2", fes::darwin::kS2, ":math:`S_{2}`")
      .value("kS4", fes::darwin::kS4, ":math:`S_{4}`")
      .value("kS6", fes::darwin::kS6, ":math:`S_{6}`")
      .value("kSa", fes::darwin::kSa, ":math:`Sa`")
      .value("kSa1", fes::darwin::kSa1, ":math:`Sa_{1}`")
      .value("kSigma1", fes::darwin::kSigma1, ":math:`\\sigma_{1}`")
      .value("kSK3", fes::darwin::kSK3, ":math:`SK_{3}`")
      .value("kSK4", fes::darwin::kSK4, ":math:`SK_{4}`")
      .value("kSKM2", fes::darwin::kSKM2, ":math:`SKM_{2}`")
      .value("kSN4", fes::darwin::kSN4, ":math:`SN_{4}`")
      .value("kSO1", fes::darwin::kSO1, ":math:`SO_{1}`")
      .value("kSO3", fes::darwin::kSO3, ":math:`SO_{3}`")
      .value("kSsa", fes::darwin::kSsa, ":math:`Ssa`")
      .value("kSta", fes::darwin::kSta, ":math:`Sta`")
      .value("kT2", fes::darwin::kT2, ":math:`T_{2}`")
      .value("kTheta1", fes::darwin::kTheta1, ":math:`\\theta_{1}`")
      .export_values();

  m.def_submodule("constituents", "Constituent names.")
      .def("parse", &fes::darwin::constituents::parse, py::arg("name"),
           R"__doc__(
  Parse a constituent name.

  Args:
    name: Constituent name. Search is not case sensitive. So "M1" and "m1" are
      equivalent.

  Returns:
    Constituent enum value.
  )__doc__")
      .def("name", &fes::darwin::constituents::name, py::arg("constituent"),
           R"__doc__(
  Return the name of a constituent.

  Args:
    constituent: Constituent enum value.

  Returns:
    Constituent name.
  )__doc__")
      .def("known", &fes::darwin::constituents::known,
           R"__doc__(
  Return the list of known constituents by this library.

  Returns:
    List of constituent names.
  )__doc__");
}

static void init_perth_constituents(py::module& m) {
  py::enum_<fes::perth::Constituent>(m, "Constituent")
      .value("k2MK3", fes::perth::k2MK3, ":math:`2MK_{3}`")
      .value("k2MK6", fes::perth::k2MK6, ":math:`2MK_{6}`")
      .value("k2MN2", fes::perth::k2MN2, ":math:`2MN_{2}`")
      .value("k2MN6", fes::perth::k2MN6, ":math:`2MN_{6}`")
      .value("k2MS2", fes::perth::k2MS2, ":math:`2MS_{2}`")
      .value("k2MS6", fes::perth::k2MS6, ":math:`2MS_{6}`")
      .value("k2N2", fes::perth::k2N2, ":math:`2N_{2}`")
      .value("k2Q1", fes::perth::k2Q1, ":math:`2Q_{1}`")
      .value("k2SM2", fes::perth::k2SM2, ":math:`2SM_{2}`")
      .value("k2SM6", fes::perth::k2SM6, ":math:`2SM_{6}`")
      .value("kAlpa2", fes::perth::kAlpa2, ":math:`Alpa_{2}`")
      .value("kBeta1", fes::perth::kBeta1, ":math:`Beta_{1}`")
      .value("kBeta2", fes::perth::kBeta2, ":math:`Beta_{2}`")
      .value("kChi1", fes::perth::kChi1, ":math:`Chi_{1}`")
      .value("kDelta2", fes::perth::kDelta2, ":math:`Delta_{2}`")
      .value("kEps2", fes::perth::kEps2, ":math:`Eps_{2}`")
      .value("kEta2", fes::perth::kEta2, ":math:`Eta_{2}`")
      .value("kGamma2", fes::perth::kGamma2, ":math:`Gamma_{2}`")
      .value("kJ1", fes::perth::kJ1, ":math:`J_{1}`")
      .value("kK1", fes::perth::kK1, ":math:`K_{1}`")
      .value("kK2", fes::perth::kK2, ":math:`K_{2}`")
      .value("kL2", fes::perth::kL2, ":math:`L_{2}`")
      .value("kL2P", fes::perth::kL2P, ":math:`L2_{P}`")
      .value("kLambda2", fes::perth::kLambda2, ":math:`Lambda_{2}`")
      .value("kM1", fes::perth::kM1, ":math:`M_{1}`")
      .value("kM13", fes::perth::kM13, ":math:`M13`")
      .value("kM2", fes::perth::kM2, ":math:`M_{2}`")
      .value("kM3", fes::perth::kM3, ":math:`M_{3}`")
      .value("kM4", fes::perth::kM4, ":math:`M_{4}`")
      .value("kM6", fes::perth::kM6, ":math:`M_{6}`")
      .value("kM8", fes::perth::kM8, ":math:`M_{8}`")
      .value("kMf", fes::perth::kMf, ":math:`Mf`")
      .value("kMK3", fes::perth::kMK3, ":math:`MK_{3}`")
      .value("kMK4", fes::perth::kMK4, ":math:`MK_{4}`")
      .value("kMKS2", fes::perth::kMKS2, ":math:`MKS_{2}`")
      .value("kMm", fes::perth::kMm, ":math:`Mm`")
      .value("kMN4", fes::perth::kMN4, ":math:`MN_{4}`")
      .value("kMO3", fes::perth::kMO3, ":math:`MO_{3}`")
      .value("kMqm", fes::perth::kMqm, ":math:`Mqm`")
      .value("kMS4", fes::perth::kMS4, ":math:`MS_{4}`")
      .value("kMSf", fes::perth::kMSf, ":math:`MSf`")
      .value("kMSK6", fes::perth::kMSK6, ":math:`MSK_{6}`")
      .value("kMSm", fes::perth::kMSm, ":math:`MSm`")
      .value("kMSN2", fes::perth::kMSN2, ":math:`MSN_{2}`")
      .value("kMSN6", fes::perth::kMSN6, ":math:`MSN_{6}`")
      .value("kMSqm", fes::perth::kMSqm, ":math:`MSqm`")
      .value("kMStm", fes::perth::kMStm, ":math:`MStm`")
      .value("kMtm", fes::perth::kMtm, ":math:`Mtm`")
      .value("kMu2", fes::perth::kMu2, ":math:`Mu_{2}`")
      .value("kN2", fes::perth::kN2, ":math:`N_{2}`")
      .value("kN2P", fes::perth::kN2P, ":math:`N2_{P}`")
      .value("kN4", fes::perth::kN4, ":math:`N_{4}`")
      .value("kNode", fes::perth::kNode, ":math:`Node`")
      .value("kNu2", fes::perth::kNu2, ":math:`Nu_{2}`")
      .value("kO1", fes::perth::kO1, ":math:`O_{1}`")
      .value("kOO1", fes::perth::kOO1, ":math:`OO_{1}`")
      .value("kP1", fes::perth::kP1, ":math:`P_{1}`")
      .value("kPhi1", fes::perth::kPhi1, ":math:`Phi_{1}`")
      .value("kPi1", fes::perth::kPi1, ":math:`Pi_{1}`")
      .value("kPsi1", fes::perth::kPsi1, ":math:`Psi_{1}`")
      .value("kQ1", fes::perth::kQ1, ":math:`Q_{1}`")
      .value("kR2", fes::perth::kR2, ":math:`R_{2}`")
      .value("kR4", fes::perth::kR4, ":math:`R_{4}`")
      .value("kRho1", fes::perth::kRho1, ":math:`Rho_{1}`")
      .value("kS1", fes::perth::kS1, ":math:`S_{1}`")
      .value("kS2", fes::perth::kS2, ":math:`S_{2}`")
      .value("kS4", fes::perth::kS4, ":math:`S_{4}`")
      .value("kS6", fes::perth::kS6, ":math:`S_{6}`")
      .value("kSa", fes::perth::kSa, ":math:`Sa`")
      .value("kSa1", fes::perth::kSa1, ":math:`Sa_{1}`")
      .value("kSigma1", fes::perth::kSigma1, ":math:`Sigma_{1}`")
      .value("kSK4", fes::perth::kSK4, ":math:`SK_{4}`")
      .value("kSN4", fes::perth::kSN4, ":math:`SN_{4}`")
      .value("kSO1", fes::perth::kSO1, ":math:`SO_{1}`")
      .value("kSsa", fes::perth::kSsa, ":math:`Ssa`")
      .value("kSta", fes::perth::kSta, ":math:`Sta`")
      .value("kT2", fes::perth::kT2, ":math:`T_{2}`")
      .value("kTau1", fes::perth::kTau1, ":math:`Tau_{1}`")
      .value("kTheta1", fes::perth::kTheta1, ":math:`Theta_{1}`")
      .value("kUps1", fes::perth::kUps1, ":math:`Ups_{1}`")
      .export_values();

  m.def_submodule("constituents", "Constituent names.")
      .def("parse", &fes::perth::constituents::parse, py::arg("name"),
           R"__doc__(
Parse a constituent name.

Args:
  name: Constituent name. Search is not case sensitive. So "M2" and "m2" are
    equivalent.
Returns:
  Constituent enum value.
)__doc__")
      .def("name", &fes::perth::constituents::name, py::arg("constituent"),
           R"__doc__(
Return the name of a constituent.
)__doc__")
      .def("known", &fes::perth::constituents::known,
           R"__doc__(
Return the list of known constituents by this library.
Returns:
  List of constituent names.
)__doc__");
}

void init_constituent(py::module& m, py::module& perth) {
  init_darwin_constituent(m);
  init_perth_constituents(perth);
}
