// Copyright (c) 2026 CNES
//
// All rights reserved. Use of this source code is governed by a
// BSD-style license that can be found in the LICENSE file.

#include "fes/perth/wave.hpp"

#include "fes/angle/astronomic.hpp"
#include "fes/interface/wave.hpp"
#include "fes/perth/doodson.hpp"
#include "fes/perth/nodal_corrections.hpp"

namespace fes {
namespace perth {

auto Wave::compute_nodal_corrections(const angle::Astronomic& angles,
                                     const bool group_modulations) -> void {
  auto nc = NodalCorrectionProcessor(
      NodalCorrectionsArgs{angles, group_modulations})(ident());
  set_nodal_corrections(
      nc.f, nc.u,
      calculate_doodson_argument(angles, doodson_numbers_.cast<double>()),
      AngleUnit::kDegree);
}

namespace wave {

_2MK3::_2MK3()
    : Wave(ConstituentId::k2MK3, kShortPeriod,
           Vector7b{{3, -1, 0, 0, 0, 0, 3}}) {}

_2MK6::_2MK6()
    : Wave(ConstituentId::k2MK6, kShortPeriod,
           Vector7b{{6, 2, 0, 0, 0, 0, 0}}) {}

_2MN2::_2MN2()
    : Wave(ConstituentId::k2MN2, kShortPeriod,
           Vector7b{{2, 1, 0, -1, 0, 0, 0}}) {}

_2MN6::_2MN6()
    : Wave(ConstituentId::k2MN6, kShortPeriod,
           Vector7b{{6, -1, 0, 1, 0, 0, 0}}) {}

_2MS2::_2MS2()
    : Wave(ConstituentId::k2MS2, kShortPeriod,
           Vector7b{{2, -2, 2, 0, 0, 0, 0}}) {}

_2MS6::_2MS6()
    : Wave(ConstituentId::k2MS6, kShortPeriod,
           Vector7b{{6, 2, -2, 0, 0, 0, 0}}) {}

_2N2::_2N2()
    : Wave(ConstituentId::k2N2, kShortPeriod,
           Vector7b{{2, -2, 0, 2, 0, 0, 0}}) {}

_2Q1::_2Q1()
    : Wave(ConstituentId::k2Q1, kShortPeriod,
           Vector7b{{1, -3, 0, 2, 0, 0, 3}}) {}

_2SM2::_2SM2()
    : Wave(ConstituentId::k2SM2, kShortPeriod,
           Vector7b{{2, 4, -4, 0, 0, 0, 0}}) {}

_2SM6::_2SM6()
    : Wave(ConstituentId::k2SM6, kShortPeriod,
           Vector7b{{6, 4, -4, 0, 0, 0, 0}}) {}

Alpha2::Alpha2()
    : Wave(ConstituentId::kAlpha2, kShortPeriod,
           Vector7b{{2, 0, -1, 0, 0, 1, 2}}) {}

Beta2::Beta2()
    : Wave(ConstituentId::kBeta2, kShortPeriod,
           Vector7b{{2, 0, 1, 0, 0, -1, 0}}) {}

Beta1::Beta1()
    : Wave(ConstituentId::kBeta1, kShortPeriod,
           Vector7b{{1, 0, -2, 1, 0, 0, 1}}) {}

Chi1::Chi1()
    : Wave(ConstituentId::kChi1, kShortPeriod,
           Vector7b{{1, 0, 2, -1, 0, 0, 1}}) {}

Delta2::Delta2()
    : Wave(ConstituentId::kDelta2, kShortPeriod,
           Vector7b{{2, 0, 2, 0, 0, 0, 0}}) {}

Eps2::Eps2()
    : Wave(ConstituentId::kEps2, kShortPeriod,
           Vector7b{{2, -3, 2, 1, 0, 0, 0}}) {}

Eta2::Eta2()
    : Wave(ConstituentId::kEta2, kShortPeriod,
           Vector7b{{2, 3, 0, -1, 0, 0, 0}}) {}

Gamma2::Gamma2()
    : Wave(ConstituentId::kGamma2, kShortPeriod,
           Vector7b{{2, 0, -2, 2, 0, 0, 2}}) {}

J1::J1()
    : Wave(ConstituentId::kJ1, kShortPeriod, Vector7b{{1, 2, 0, -1, 0, 0, 1}}) {
}

K1::K1()
    : Wave(ConstituentId::kK1, kShortPeriod, Vector7b{{1, 1, 0, 0, 0, 0, 1}}) {}

K2::K2()
    : Wave(ConstituentId::kK2, kShortPeriod, Vector7b{{2, 2, 0, 0, 0, 0, 0}}) {}

L2::L2()
    : Wave(ConstituentId::kL2, kShortPeriod, Vector7b{{2, 1, 0, -1, 0, 0, 2}}) {
}

L2P::L2P()
    : Wave(ConstituentId::kL2P, kShortPeriod, Vector7b{{2, 1, 0, 0, 0, 0, 3}}) {
}

Lambda2::Lambda2()
    : Wave(ConstituentId::kLambda2, kShortPeriod,
           Vector7b{{2, 1, -2, 1, 0, 0, 2}}) {}

M1::M1()
    : Wave(ConstituentId::kM1, kShortPeriod, Vector7b{{1, 0, 0, 1, 0, 0, 1}}) {}

M13::M13()
    : Wave(ConstituentId::kM13, kShortPeriod, Vector7b{{1, 0, 0, 0, 0, 0, 2}}) {
}

M2::M2()
    : Wave(ConstituentId::kM2, kShortPeriod, Vector7b{{2, 0, 0, 0, 0, 0, 0}}) {}

M3::M3()
    : Wave(ConstituentId::kM3, kShortPeriod, Vector7b{{3, 0, 0, 0, 0, 0, 2}}) {}

M4::M4()
    : Wave(ConstituentId::kM4, kShortPeriod, Vector7b{{4, 0, 0, 0, 0, 0, 0}}) {}

M6::M6()
    : Wave(ConstituentId::kM6, kShortPeriod, Vector7b{{6, 0, 0, 0, 0, 0, 0}}) {}

M8::M8()
    : Wave(ConstituentId::kM8, kShortPeriod, Vector7b{{8, 0, 0, 0, 0, 0, 0}}) {}

MK3::MK3()
    : Wave(ConstituentId::kMK3, kShortPeriod, Vector7b{{3, 1, 0, 0, 0, 0, 1}}) {
}

MK4::MK4()
    : Wave(ConstituentId::kMK4, kShortPeriod, Vector7b{{4, 2, 0, 0, 0, 0, 0}}) {
}

MKS2::MKS2()
    : Wave(ConstituentId::kMKS2, kShortPeriod,
           Vector7b{{2, 0, 2, 0, 0, 0, 0}}) {}

MN4::MN4()
    : Wave(ConstituentId::kMN4, kShortPeriod,
           Vector7b{{4, -1, 0, 1, 0, 0, 0}}) {}

MO3::MO3()
    : Wave(ConstituentId::kMO3, kShortPeriod,
           Vector7b{{3, -1, 0, 0, 0, 0, 3}}) {}

MS4::MS4()
    : Wave(ConstituentId::kMS4, kShortPeriod,
           Vector7b{{4, 2, -2, 0, 0, 0, 0}}) {}

MSK6::MSK6()
    : Wave(ConstituentId::kMSK6, kShortPeriod,
           Vector7b{{6, 4, -2, 0, 0, 0, 0}}) {}

MSN2::MSN2()
    : Wave(ConstituentId::kMSN2, kShortPeriod,
           Vector7b{{2, 3, -2, -1, 0, 0, 0}}) {}

MSN6::MSN6()
    : Wave(ConstituentId::kMSN6, kShortPeriod,
           Vector7b{{6, 1, -2, 1, 0, 0, 0}}) {}

Mu2::Mu2()
    : Wave(ConstituentId::kMu2, kShortPeriod,
           Vector7b{{2, -2, 2, 0, 0, 0, 0}}) {}

N2::N2()
    : Wave(ConstituentId::kN2, kShortPeriod, Vector7b{{2, -1, 0, 1, 0, 0, 0}}) {
}

N2P::N2P()
    : Wave(ConstituentId::kN2P, kShortPeriod,
           Vector7b{{2, -1, 0, 0, 0, 0, 1}}) {}

N4::N4()
    : Wave(ConstituentId::kN4, kShortPeriod, Vector7b{{4, -2, 0, 2, 0, 0, 0}}) {
}

Nu2::Nu2()
    : Wave(ConstituentId::kNu2, kShortPeriod,
           Vector7b{{2, -1, 2, -1, 0, 0, 0}}) {}

O1::O1()
    : Wave(ConstituentId::kO1, kShortPeriod, Vector7b{{1, -1, 0, 0, 0, 0, 3}}) {
}

OO1::OO1()
    : Wave(ConstituentId::kOO1, kShortPeriod, Vector7b{{1, 3, 0, 0, 0, 0, 1}}) {
}

P1::P1()
    : Wave(ConstituentId::kP1, kShortPeriod, Vector7b{{1, 1, -2, 0, 0, 0, 3}}) {
}

Phi1::Phi1()
    : Wave(ConstituentId::kPhi1, kShortPeriod,
           Vector7b{{1, 1, 2, 0, 0, 0, 1}}) {}

Pi1::Pi1()
    : Wave(ConstituentId::kPi1, kShortPeriod,
           Vector7b{{1, 1, -3, 0, 0, 1, 3}}) {}

Psi1::Psi1()
    : Wave(ConstituentId::kPsi1, kShortPeriod,
           Vector7b{{1, 1, 1, 0, 0, -1, 1}}) {}

Q1::Q1()
    : Wave(ConstituentId::kQ1, kShortPeriod, Vector7b{{1, -2, 0, 1, 0, 0, 3}}) {
}

R2::R2()
    : Wave(ConstituentId::kR2, kShortPeriod,
           Vector7b{{2, 2, -1, 0, 0, -1, 2}}) {}

R4::R4()
    : Wave(ConstituentId::kR4, kShortPeriod, Vector7b{{4, 4, -3, 0, 0, 0, 0}}) {
}

Rho1::Rho1()
    : Wave(ConstituentId::kRho1, kShortPeriod,
           Vector7b{{1, -2, 2, -1, 0, 0, 3}}) {}

S1::S1()
    : Wave(ConstituentId::kS1, kShortPeriod, Vector7b{{1, 1, -1, 0, 0, 0, 2}}) {
}

S2::S2()
    : Wave(ConstituentId::kS2, kShortPeriod, Vector7b{{2, 2, -2, 0, 0, 0, 0}}) {
}

S4::S4()
    : Wave(ConstituentId::kS4, kShortPeriod, Vector7b{{4, 4, -4, 0, 0, 0, 0}}) {
}

S6::S6()
    : Wave(ConstituentId::kS6, kShortPeriod, Vector7b{{6, 6, -6, 0, 0, 0, 0}}) {
}

Sigma1::Sigma1()
    : Wave(ConstituentId::kSigma1, kShortPeriod,
           Vector7b{{1, -3, 2, 0, 0, 0, 3}}) {}

SK4::SK4()
    : Wave(ConstituentId::kSK4, kShortPeriod,
           Vector7b{{4, 4, -2, 0, 0, 0, 0}}) {}

SN4::SN4()
    : Wave(ConstituentId::kSN4, kShortPeriod,
           Vector7b{{4, 1, -2, 1, 0, 0, 0}}) {}

SO1::SO1()
    : Wave(ConstituentId::kSO1, kShortPeriod,
           Vector7b{{1, 3, -2, 0, 0, 0, 1}}) {}

T2::T2()
    : Wave(ConstituentId::kT2, kShortPeriod, Vector7b{{2, 2, -3, 0, 0, 1, 0}}) {
}

Tau1::Tau1()
    : Wave(ConstituentId::kTau1, kShortPeriod,
           Vector7b{{1, -1, 2, 0, 0, 0, 1}}) {}

Theta1::Theta1()
    : Wave(ConstituentId::kTheta1, kShortPeriod,
           Vector7b{{1, 2, -2, 1, 0, 0, 1}}) {}

Ups1::Ups1()
    : Wave(ConstituentId::kUps1, kShortPeriod,
           Vector7b{{1, 4, 0, -1, 0, 0, 1}}) {}

Mf::Mf()
    : Wave(ConstituentId::kMf, kLongPeriod, Vector7b{{0, 2, 0, 0, 0, 0, 0}}) {}

Mm::Mm()
    : Wave(ConstituentId::kMm, kLongPeriod, Vector7b{{0, 1, 0, -1, 0, 0, 0}}) {}

Mqm::Mqm()
    : Wave(ConstituentId::kMqm, kLongPeriod, Vector7b{{0, 4, 0, -2, 0, 0, 0}}) {
}

MSf::MSf()
    : Wave(ConstituentId::kMSf, kLongPeriod, Vector7b{{0, 2, -2, 0, 0, 0, 0}}) {
}

MSm::MSm()
    : Wave(ConstituentId::kMSm, kLongPeriod, Vector7b{{0, 1, -2, 1, 0, 0, 0}}) {
}

MSqm::MSqm()
    : Wave(ConstituentId::kMSqm, kLongPeriod,
           Vector7b{{0, 4, -2, 0, 0, 0, 0}}) {}

MStm::MStm()
    : Wave(ConstituentId::kMStm, kLongPeriod,
           Vector7b{{0, 3, -2, 1, 0, 0, 0}}) {}

Mtm::Mtm()
    : Wave(ConstituentId::kMtm, kLongPeriod, Vector7b{{0, 3, 0, -1, 0, 0, 0}}) {
}

Node::Node()
    : Wave(ConstituentId::kNode, kLongPeriod, Vector7b{{0, 0, 0, 0, 1, 0, 2}}) {
}

Sa::Sa()
    : Wave(ConstituentId::kSa, kLongPeriod, Vector7b{{0, 0, 1, 0, 0, 0, 0}}) {}

Sa1::Sa1()
    : Wave(ConstituentId::kSa1, kLongPeriod, Vector7b{{0, 0, 1, 0, 0, -1, 0}}) {
}

Ssa::Ssa()
    : Wave(ConstituentId::kSsa, kLongPeriod, Vector7b{{0, 0, 2, 0, 0, 0, 0}}) {}

Sta::Sta()
    : Wave(ConstituentId::kSta, kLongPeriod, Vector7b{{0, 0, 3, 0, 0, 0, 0}}) {}

}  // namespace wave
}  // namespace perth
}  // namespace fes
