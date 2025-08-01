

// Copyright (c) 2025 CNES
//
// All rights reserved. Use of this source code is governed by a
// BSD-style license that can be found in the LICENSE file.
#include "fes/wave.hpp"

#include <stdexcept>

#include "fes/detail/wave/name.hpp"

namespace fes {

constexpr auto darwin_to_doodson(const std::array<int8_t, 11>& darwin)
    -> std::array<int8_t, 7> {
  // Extract Darwin coefficients: t, s, h, p, n, p1, shift
  const auto t = darwin[0];
  const auto s = darwin[1];
  const auto h = darwin[2];
  const auto p = darwin[3];
  const auto n = darwin[4];
  const auto p1 = darwin[5];

  // Normalize shift: convert 1 or -1 to their negatives, leave others unchanged
  const int8_t normalized_shift =
      (darwin[6] == 1 || darwin[6] == -1) ? -darwin[6] : darwin[6];

  return {t,                           // tau
          static_cast<int8_t>(s + t),  // s + tau
          static_cast<int8_t>(h - t),  // h - tau
          p,                           // p
          n,                           // n
          p1,                          // p1
          normalized_shift};           // normalized shift
}

// Get the doodson number
inline auto code(const int number) -> char {
  switch (number) {
    case -1:
      return '*';
    case 10:
      return 'X';
    case 11:
      return 'E';
    case 12:
      return 'T';
    default:
      return static_cast<char>(number + 48);
  }
}

auto Wave::xdo_numerical() const -> std::string {
  const auto doodson = darwin_to_doodson(argument_);
  std::string result;
  result.reserve(7);

  // First element uses raw value, others add offset of 5
  result.push_back(code(doodson[0]));
  for (size_t i = 1; i < doodson.size(); ++i) {
    result.push_back(code(doodson[i] + 5));
  }

  return result;
}

auto Wave::xdo_alphabetical() const -> std::string {
  constexpr auto xdo = std::array<char, 25>{
      'R', 'S', 'T', 'U', 'V', 'W', 'X', 'Y', 'Z', 'A', 'B', 'C', 'D',
      'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L', 'M', 'N', 'O', 'P'};
  auto result = std::string();
  result.reserve(7);
  for (const auto& value : darwin_to_doodson(argument_)) {
    result.push_back(xdo[value + 8]);
  }
  return result;
}

auto Wave::doodson_numbers() const -> std::array<int8_t, 7> {
  return darwin_to_doodson(argument_);
}

namespace wave {

Mm::Mm()
    : Wave(kMm, kLongPeriod, false, Darwin::Builder().s(1).p(-1).build(),
           &angle::Astronomic::f_mm) {}

Mf::Mf()
    : Wave(kMf, kLongPeriod, false, Darwin::Builder().s(2).xi(-2).build(),
           &angle::Astronomic::f_mf) {}

Mtm::Mtm()
    : Wave(kMtm, kLongPeriod, false,
           Darwin::Builder().s(3).p(-1).xi(-2).build(),
           &angle::Astronomic::f_mf) {}

MSqm::MSqm()
    : Wave(kMSqm, kLongPeriod, false,
           Darwin::Builder().s(4).h(-2).xi(-2).build(),
           &angle::Astronomic::f_mf) {}

Ssa::Ssa()
    : Wave(kSsa, kLongPeriod, false, Darwin::Builder().h(2).build(),
           &angle::Astronomic::f_1) {}

Sa::Sa()
    : Wave(kSa, kLongPeriod, false, Darwin::Builder().h(1).build(),
           &angle::Astronomic::f_1) {}

_2Q1::_2Q1()
    : Wave(k2Q1, kShortPeriod, true,
           Darwin::Builder().T(1).s(-4).h(1).p(2).shift(1).xi(2).nu(-1).build(),
           &angle::Astronomic::f_o1) {}

Sigma1::Sigma1()
    : Wave(kSigma1, kShortPeriod, true,
           Darwin::Builder().T(1).s(-4).h(3).shift(1).xi(2).nu(-1).build(),
           &angle::Astronomic::f_o1) {}

Q1::Q1()
    : Wave(kQ1, kShortPeriod, false,
           Darwin::Builder().T(1).s(-3).h(1).p(1).shift(1).xi(2).nu(-1).build(),
           &angle::Astronomic::f_o1) {}

Rho1::Rho1()
    : Wave(
          kRho1, kShortPeriod, true,
          Darwin::Builder().T(1).s(-3).h(3).p(-1).shift(1).xi(2).nu(-1).build(),
          &angle::Astronomic::f_o1) {}

O1::O1()
    : Wave(kO1, kShortPeriod, false,
           Darwin::Builder().T(1).s(-2).h(1).p(0).shift(1).xi(2).nu(-1).build(),
           &angle::Astronomic::f_o1) {}

MP1::MP1()
    : Wave(kMP1, kShortPeriod, false,
           Darwin::Builder().T(1).s(-2).h(3).shift(-1).nu(-1).build(),
           &angle::Astronomic::f_j1) {}

M1::M1()
    : Wave(kM1, kShortPeriod, false,
           Darwin::Builder().T(1).s(-1).h(1).p(1).shift(-1).nu(-1).build(),
           &angle::Astronomic::f_m1) {}

M11::M11()
    : Wave(kM11, kShortPeriod, true,
           Darwin::Builder()
               .T(1)
               .s(-1)
               .h(1)
               .p(-1)
               .shift(-1)
               .xi(2)
               .nu(-1)
               .build(),
           &angle::Astronomic::f_o1) {}

M12::M12()
    : Wave(kM12, kShortPeriod, true,
           Darwin::Builder().T(1).s(-1).h(1).p(1).shift(-1).nu(-1).build(),
           &angle::Astronomic::f_j1) {}

M13::M13()
    : Wave(kM13, kShortPeriod, true,
           Darwin::Builder().T(1).s(-1).h(1).xi(1).nu(-1).build(),
           &angle::Astronomic::f_144) {}

Chi1::Chi1()
    : Wave(kChi1, kShortPeriod, true,
           Darwin::Builder().T(1).s(-1).h(3).p(-1).shift(-1).nu(-1).build(),
           &angle::Astronomic::f_j1) {}

Pi1::Pi1()
    : Wave(kPi1, kShortPeriod, true,
           Darwin::Builder().T(1).h(-2).p1(1).shift(1).build(),
           &angle::Astronomic::f_1) {}

P1::P1()
    : Wave(kP1, kShortPeriod, false,
           Darwin::Builder().T(1).h(-1).shift(1).build(),
           &angle::Astronomic::f_1) {}

S1::S1()
    : Wave(kS1, kShortPeriod, false, Darwin::Builder().T(1).build(),
           &angle::Astronomic::f_1) {}

K1::K1()
    : Wave(kK1, kShortPeriod, false,
           Darwin::Builder().T(1).h(1).shift(-1).nuprim(-1).build(),
           &angle::Astronomic::f_k1) {}

Psi1::Psi1()
    : Wave(kPsi1, kShortPeriod, false,
           Darwin::Builder().T(1).h(2).p1(-1).shift(-1).build(),
           &angle::Astronomic::f_1) {}

Phi1::Phi1()
    : Wave(kPhi1, kShortPeriod, true,
           Darwin::Builder().T(1).h(3).shift(-1).build(),
           &angle::Astronomic::f_1) {}

Theta1::Theta1()
    : Wave(kTheta1, kShortPeriod, true,
           Darwin::Builder().T(1).s(1).h(-1).p(1).shift(-1).nu(-1).build(),
           &angle::Astronomic::f_j1) {}

J1::J1()
    : Wave(kJ1, kShortPeriod, true,
           Darwin::Builder().T(1).s(1).h(1).p(-1).shift(-1).nu(-1).build(),
           &angle::Astronomic::f_j1) {}

OO1::OO1()
    : Wave(kOO1, kShortPeriod, true,
           Darwin::Builder().T(1).s(2).h(1).shift(-1).xi(-2).nu(-1).build(),
           &angle::Astronomic::f_oo1) {}

MNS2::MNS2()
    : Wave(kMNS2, kShortPeriod, false,
           Darwin::Builder().T(2).s(-5).h(4).p(1).xi(4).nu(-4).build(),
           &angle::Astronomic::f_m22) {}

Eps2::Eps2()
    : Wave(kEps2, kShortPeriod, true,
           Darwin::Builder().T(2).s(-5).h(4).p(1).xi(2).nu(-2).build(),
           &angle::Astronomic::f_m2) {}

_2N2::_2N2()
    : Wave(k2N2, kShortPeriod, true,
           Darwin::Builder().T(2).s(-4).h(2).p(2).xi(2).nu(-2).build(),
           &angle::Astronomic::f_m2) {}

Mu2::Mu2()
    : Wave(kMu2, kShortPeriod, true,
           Darwin::Builder().T(2).s(-4).h(4).xi(2).nu(-2).build(),
           &angle::Astronomic::f_m2) {}

_2MS2::_2MS2()
    : Wave(k2MS2, kShortPeriod, false,
           Darwin::Builder().T(2).s(-4).h(4).xi(4).nu(-4).build(),
           &angle::Astronomic::f_m22) {}

N2::N2()
    : Wave(kN2, kShortPeriod, false,
           Darwin::Builder().T(2).s(-3).h(2).p(1).xi(2).nu(-2).build(),
           &angle::Astronomic::f_m2) {}

Nu2::Nu2()
    : Wave(kNu2, kShortPeriod, true,
           Darwin::Builder().T(2).s(-3).h(4).p(-1).xi(2).nu(-2).build(),
           &angle::Astronomic::f_m2) {}

M2::M2()
    : Wave(kM2, kShortPeriod, false,
           Darwin::Builder().T(2).s(-2).h(2).xi(2).nu(-2).build(),
           &angle::Astronomic::f_m2) {}

MKS2::MKS2()
    : Wave(kMKS2, kShortPeriod, false,
           Darwin::Builder().T(2).s(-2).h(4).xi(2).nu(-2).nusec(-2).build(),
           &angle::Astronomic::f_m2_k2) {}

Lambda2::Lambda2()
    : Wave(kLambda2, kShortPeriod, true,
           Darwin::Builder().T(2).s(-1).p(1).shift(2).xi(2).nu(-2).build(),
           &angle::Astronomic::f_m2) {}

L2::L2()
    : Wave(
          kL2, kShortPeriod, true,
          Darwin::Builder().T(2).s(-1).h(2).p(-1).shift(2).xi(2).nu(-2).build(),
          &angle::Astronomic::f_l2) {}

_2MN2::_2MN2()
    : Wave(
          k2MN2, kShortPeriod, false,
          Darwin::Builder().T(2).s(-1).h(2).p(-1).shift(2).xi(2).nu(-2).build(),
          &angle::Astronomic::f_m23) {}

T2::T2()
    : Wave(kT2, kShortPeriod, true, Darwin::Builder().T(2).h(-1).p1(1).build(),
           &angle::Astronomic::f_1) {}

S2::S2()
    : Wave(kS2, kShortPeriod, false, Darwin::Builder().T(2).build(),
           &angle::Astronomic::f_1) {}

R2::R2()
    : Wave(kR2, kShortPeriod, false,
           Darwin::Builder().T(2).h(1).p1(-1).shift(2).build(),
           &angle::Astronomic::f_1) {}

K2::K2()
    : Wave(kK2, kShortPeriod, false,
           Darwin::Builder().T(2).h(2).nusec(-2).build(),
           &angle::Astronomic::f_k2) {}

MSN2::MSN2()
    : Wave(kMSN2, kShortPeriod, false,
           Darwin::Builder().T(2).s(1).p(-1).build(),
           &angle::Astronomic::f_m22) {}

Eta2::Eta2()
    : Wave(kEta2, kShortPeriod, true,
           Darwin::Builder().T(2).s(1).h(2).p(-1).nu(-2).build(),
           &angle::Astronomic::f_79) {}

_2SM2::_2SM2()
    : Wave(k2SM2, kShortPeriod, false,
           Darwin::Builder().T(2).s(2).h(-2).xi(-2).nu(2).build(),
           &angle::Astronomic::f_m2) {}

MO3::MO3()
    : Wave(kMO3, kShortPeriod, false,
           Darwin::Builder().T(3).s(-4).h(3).shift(1).xi(4).nu(-3).build(),
           &angle::Astronomic::f_m2_o1) {}

_2MK3::_2MK3()
    : Wave(k2MK3, kShortPeriod, false,
           Darwin::Builder()
               .T(3)
               .s(-4)
               .h(3)
               .shift(1)
               .xi(4)
               .nu(-4)
               .nuprim(1)
               .build(),
           &angle::Astronomic::f_m22_k1) {}

M3::M3()
    : Wave(kM3, kShortPeriod, false,
           Darwin::Builder().T(3).s(-3).h(3).xi(3).nu(-3).build(),
           &angle::Astronomic::f_m3) {}

MK3::MK3()
    : Wave(kMK3, kShortPeriod, false,
           Darwin::Builder()
               .T(3)
               .s(-2)
               .h(3)
               .shift(-1)
               .xi(2)
               .nu(-2)
               .nuprim(-1)
               .build(),
           &angle::Astronomic::f_m2_k1) {}

N4::N4()
    : Wave(kN4, kShortPeriod, false,
           Darwin::Builder().T(4).s(-6).h(4).p(2).xi(4).nu(-4).build(),
           &angle::Astronomic::f_m22) {}

MN4::MN4()
    : Wave(kMN4, kShortPeriod, false,
           Darwin::Builder().T(4).s(-5).h(4).p(1).xi(4).nu(-4).build(),
           &angle::Astronomic::f_m22) {}

M4::M4()
    : Wave(kM4, kShortPeriod, false,
           Darwin::Builder().T(4).s(-4).h(4).xi(4).nu(-4).build(),
           &angle::Astronomic::f_m22) {}

SN4::SN4()
    : Wave(kSN4, kShortPeriod, false,
           Darwin::Builder().T(4).s(-3).h(2).p(1).xi(2).nu(-2).build(),
           &angle::Astronomic::f_m2) {}

MS4::MS4()
    : Wave(kMS4, kShortPeriod, false,
           Darwin::Builder().T(4).s(-2).h(2).xi(2).nu(-2).build(),
           &angle::Astronomic::f_m2) {}

MK4::MK4()
    : Wave(kMK4, kShortPeriod, false,
           Darwin::Builder().T(4).s(-2).h(4).xi(2).nu(-2).nusec(-2).build(),
           &angle::Astronomic::f_m2_k2) {}

S4::S4()
    : Wave(kS4, kShortPeriod, false, Darwin::Builder().T(4).build(),
           &angle::Astronomic::f_1) {}

SK4::SK4()
    : Wave(kSK4, kShortPeriod, false,
           Darwin::Builder().T(4).h(2).nusec(-2).build(),
           &angle::Astronomic::f_k2) {}

R4::R4()
    : Wave(kR4, kShortPeriod, false, Darwin::Builder().T(4).h(2).p1(-2).build(),
           &angle::Astronomic::f_1) {}

_2MN6::_2MN6()
    : Wave(k2MN6, kShortPeriod, false,
           Darwin::Builder().T(6).s(-7).h(6).p(1).xi(6).nu(-6).build(),
           &angle::Astronomic::f_m23) {}

M6::M6()
    : Wave(kM6, kShortPeriod, false,
           Darwin::Builder().T(6).s(-6).h(6).xi(6).nu(-6).build(),
           &angle::Astronomic::f_m23) {}

MSN6::MSN6()
    : Wave(kMSN6, kShortPeriod, false,
           Darwin::Builder().T(6).s(-5).h(4).p(1).xi(4).nu(-4).build(),
           &angle::Astronomic::f_m22) {}

_2MS6::_2MS6()
    : Wave(k2MS6, kShortPeriod, false,
           Darwin::Builder().T(6).s(-4).h(4).xi(4).nu(-4).build(),
           &angle::Astronomic::f_m22) {}

_2MK6::_2MK6()
    : Wave(k2MK6, kShortPeriod, false,
           Darwin::Builder().T(6).s(-4).h(6).xi(4).nu(-4).nusec(-2).build(),
           &angle::Astronomic::f_m23_k2) {}

_2SM6::_2SM6()
    : Wave(k2SM6, kShortPeriod, false,
           Darwin::Builder().T(6).s(-2).h(2).xi(2).nu(-2).build(),
           &angle::Astronomic::f_m2) {}

MSK6::MSK6()
    : Wave(kMSK6, kShortPeriod, false,
           Darwin::Builder().T(6).s(-2).h(4).xi(2).nu(-2).nuprim(-2).build(),
           &angle::Astronomic::f_m2_k2) {}

S6::S6()
    : Wave(kS6, kShortPeriod, false, Darwin::Builder().T(6).build(),
           &angle::Astronomic::f_1) {}

M8::M8()
    : Wave(kM8, kShortPeriod, false,
           Darwin::Builder().T(8).s(-8).h(8).xi(8).nu(-8).build(),
           &angle::Astronomic::f_m24) {}

MSf::MSf()
    : Wave(kMSf, kLongPeriod, false,
           Darwin::Builder().s(2).h(-2).xi(2).nu(-2).build(),
           &angle::Astronomic::f_m2) {}

A5::A5()
    : Wave(kA5, kLongPeriod, false, Darwin::Builder().s(2).h(-2).build(),
           &angle::Astronomic::f_mm) {}

Sa1::Sa1()
    : Wave(kSa1, kLongPeriod, false, Darwin::Builder().h(1).p1(-1).build(),
           &angle::Astronomic::f_1) {}

Sta::Sta()
    : Wave(kSta, kLongPeriod, false, Darwin::Builder().h(3).p1(-1).build(),
           &angle::Astronomic::f_1) {}

Mm2::Mm2()
    : Wave(kMm2, kLongPeriod, false,
           Darwin::Builder().s(1).shift(-1).xi(-1).build(),
           &angle::Astronomic::f_141) {}

Mm1::Mm1()
    : Wave(kMm1, kLongPeriod, false,
           Darwin::Builder().s(1).p(1).shift(2).xi(-2).build(),
           &angle::Astronomic::f_mf) {}

Mf1::Mf1()
    : Wave(kMf1, kLongPeriod, false, Darwin::Builder().s(2).p(-2).build(),
           &angle::Astronomic::f_mm) {}

Mf2::Mf2()
    : Wave(kMf2, kLongPeriod, false,
           Darwin::Builder().s(2).p(-1).shift(-1).xi(-1).build(),
           &angle::Astronomic::f_141) {}

M0::M0()
    : Wave(kM0, kLongPeriod, false, Darwin::Builder().build(),
           &angle::Astronomic::f_mm) {}

N2P::N2P()
    : Wave(kN2P, kShortPeriod, false,
           Darwin::Builder().T(2).s(-3).h(2).shift(1).xi(3).nu(-2).build(),
           &angle::Astronomic::f_146) {}

L2P::L2P()
    : Wave(kL2P, kShortPeriod, false,
           Darwin::Builder().T(2).s(-1).h(2).shift(-1).xi(1).nu(-2).build(),
           &angle::Astronomic::f_147) {}

MSK2::MSK2()
    : Wave(kMSK2, kShortPeriod, false,
           Darwin::Builder().T(2).s(-2).xi(2).nu(-2).nusec(2).build(),
           &angle::Astronomic::f_m2_k2) {}

SKM2::SKM2()
    : Wave(kSKM2, kShortPeriod, false,
           Darwin::Builder().T(2).s(2).xi(-2).nu(2).nusec(-2).build(),
           &angle::Astronomic::f_m2_k2) {}

OQ2::OQ2()
    : Wave(kOQ2, kShortPeriod, false,
           Darwin::Builder().T(2).s(-5).h(2).p(1).shift(2).build(),
           &angle::Astronomic::f_o12) {}  // f(O1) * f(Q1) = f(O1)^2

_3MS4::_3MS4()
    : Wave(k3MS4, kShortPeriod, false,
           Darwin::Builder().T(4).s(-6).h(6).xi(6).nu(-6).build(),
           &angle::Astronomic::f_m23) {}

MNu4::MNu4()
    : Wave(kMNu4, kShortPeriod, false,
           Darwin::Builder().T(4).s(-5).h(6).p(-1).xi(4).nu(-4).build(),
           &angle::Astronomic::f_m22) {}

_2MSN4::_2MSN4()
    : Wave(k2MSN4, kShortPeriod, false,
           Darwin::Builder().T(4).s(-1).h(2).p(-1).xi(2).nu(-2).build(),
           &angle::Astronomic::f_m23) {}

_2NS2::_2NS2()
    : Wave(k2NS2, kShortPeriod, false,
           Darwin::Builder().T(2).s(-6).h(4).p(2).xi(4).nu(-4).build(),
           &angle::Astronomic::f_m22) {}

MNuS2::MNuS2()
    : Wave(kMNuS2, kShortPeriod, false,
           Darwin::Builder().T(2).s(-5).h(6).p(-1).xi(4).nu(-4).build(),
           &angle::Astronomic::f_m22) {}

_2MK2::_2MK2()
    : Wave(k2MK2, kShortPeriod, false,
           Darwin::Builder().T(2).s(-4).h(2).xi(4).nu(-4).nusec(2).build(),
           &angle::Astronomic::f_m22_k2) {}

NKM2::NKM2()
    : Wave(kNKM2, kShortPeriod, false,
           Darwin::Builder().T(2).s(-1).h(2).p(1).nusec(-2).build(),
           &angle::Astronomic::f_m22_k2) {}

ML4::ML4()
    : Wave(kML4, kShortPeriod, false,
           Darwin::Builder().T(4).s(-3).h(4).p(-1).xi(4).nu(-4).build(),
           &angle::Astronomic::f_m2_l2) {}

SO1::SO1()
    : Wave(kSO1, kShortPeriod, false,
           Darwin::Builder().T(1).s(2).h(-1).shift(-1).nu(-1).build(),
           &angle::Astronomic::f_o1) {}

SO3::SO3()
    : Wave(kSO3, kShortPeriod, false,
           Darwin::Builder().T(3).s(-2).h(1).shift(1).xi(2).nu(-1).build(),
           &angle::Astronomic::f_o1) {}

NK4::NK4()
    : Wave(
          kNK4, kShortPeriod, false,
          Darwin::Builder().T(4).s(-3).h(4).p(1).xi(2).nu(-2).nusec(-2).build(),
          &angle::Astronomic::f_m2_k2) {}

MNK6::MNK6()
    : Wave(
          kMNK6, kShortPeriod, false,
          Darwin::Builder().T(6).s(-5).h(6).p(1).xi(4).nu(-4).nusec(-2).build(),
          &angle::Astronomic::f_m22_k2) {}

_2NM6::_2NM6()
    : Wave(k2NM6, kShortPeriod, false,
           Darwin::Builder().T(6).s(-8).h(6).p(2).xi(6).nu(-6).build(),
           &angle::Astronomic::f_m24_l2) {}

_3MS8::_3MS8()
    : Wave(k3MS8, kShortPeriod, false,
           Darwin::Builder().T(8).s(-6).h(6).xi(6).nu(-6).build(),
           &angle::Astronomic::f_m23) {}

SK3::SK3()
    : Wave(kSK3, kShortPeriod, false,
           Darwin::Builder().T(3).h(1).shift(-1).nuprim(-1).build(),
           &angle::Astronomic::f_k1) {}

_2MNS4::_2MNS4()
    : Wave(k2MNS4, kShortPeriod, false,
           Darwin::Builder().T(4).s(-7).h(6).p(1).xi(6).nu(-6).build(),
           &angle::Astronomic::f_m23) {}

_2SMu2::_2SMu2()
    : Wave(k2SMu2, kShortPeriod, false,
           Darwin::Builder().T(2).s(4).h(-4).xi(-2).nu(2).build(),
           &angle::Astronomic::f_m2) {}

_2MP5::_2MP5()
    : Wave(k2MP5, kShortPeriod, false,
           Darwin::Builder().T(5).s(-4).h(3).shift(1).xi(4).nu(-4).build(),
           &angle::Astronomic::f_m22) {}

}  // namespace wave
}  // namespace fes
