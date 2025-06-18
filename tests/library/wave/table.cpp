// Copyright (c) 2025 CNES
//
// All rights reserved. Use of this source code is governed by a
// BSD-style license that can be found in the LICENSE file.
#include "fes/wave/table.hpp"

#include <gtest/gtest.h>

namespace detail = fes::detail;

// NOLINTBEGIN(readability-function-cognitive-complexity,*-function-size)
// Used to test all wave components in a single function
inline auto check_wave_table_constructor(const fes::wave::Table& table)
    -> void {
  for (auto&& item : table) {
    switch (item->ident()) {
      case fes::kO1:
        EXPECT_EQ(item->ident(), fes::kO1);
        EXPECT_NEAR(item->freq(), detail::math::radians(13.9430356), 1e-8);
        EXPECT_EQ(item->type(), fes::Wave::kShortPeriod);
        break;
      case fes::kP1:
        EXPECT_EQ(item->ident(), fes::kP1);
        EXPECT_NEAR(item->freq(), detail::math::radians(14.9589314), 1e-8);
        EXPECT_EQ(item->type(), fes::Wave::kShortPeriod);
        break;
      case fes::kK1:
        EXPECT_EQ(item->ident(), fes::kK1);
        EXPECT_NEAR(item->freq(), detail::math::radians(15.0410686), 1e-8);
        EXPECT_EQ(item->type(), fes::Wave::kShortPeriod);
        break;
      case fes::k2N2:
        EXPECT_EQ(item->ident(), fes::k2N2);
        EXPECT_NEAR(item->freq(), detail::math::radians(27.8953548), 1e-8);
        EXPECT_EQ(item->type(), fes::Wave::kShortPeriod);
        break;
      case fes::kMu2:
        EXPECT_EQ(item->ident(), fes::kMu2);
        EXPECT_NEAR(item->freq(), detail::math::radians(27.9682084), 1e-8);
        EXPECT_EQ(item->type(), fes::Wave::kShortPeriod);
        break;
      case fes::kN2:
        EXPECT_EQ(item->ident(), fes::kN2);
        EXPECT_NEAR(item->freq(), detail::math::radians(28.4397295), 1e-8);
        EXPECT_EQ(item->type(), fes::Wave::kShortPeriod);
        break;
      case fes::kNu2:
        EXPECT_EQ(item->ident(), fes::kNu2);
        EXPECT_NEAR(item->freq(), detail::math::radians(28.5125831), 1e-8);
        EXPECT_EQ(item->type(), fes::Wave::kShortPeriod);
        break;
      case fes::kM2:
        EXPECT_EQ(item->ident(), fes::kM2);
        EXPECT_NEAR(item->freq(), detail::math::radians(28.9841042), 1e-8);
        EXPECT_EQ(item->type(), fes::Wave::kShortPeriod);
        break;
      case fes::kL2:
        EXPECT_EQ(item->ident(), fes::kL2);
        EXPECT_NEAR(item->freq(), detail::math::radians(29.5284789), 1e-8);
        EXPECT_EQ(item->type(), fes::Wave::kShortPeriod);
        break;
      case fes::kT2:
        EXPECT_EQ(item->ident(), fes::kT2);
        EXPECT_NEAR(item->freq(), detail::math::radians(29.9589333), 1e-8);
        EXPECT_EQ(item->type(), fes::Wave::kShortPeriod);
        break;
      case fes::kS2:
        EXPECT_EQ(item->ident(), fes::kS2);
        EXPECT_NEAR(item->freq(), detail::math::radians(30.0), 1e-8);
        EXPECT_EQ(item->type(), fes::Wave::kShortPeriod);
        break;
      case fes::kK2:
        EXPECT_EQ(item->ident(), fes::kK2);
        EXPECT_NEAR(item->freq(), detail::math::radians(30.0821373), 1e-8);
        EXPECT_EQ(item->type(), fes::Wave::kShortPeriod);
        break;
      case fes::kM4:
        EXPECT_EQ(item->ident(), fes::kM4);
        EXPECT_NEAR(item->freq(), detail::math::radians(57.9682084), 1e-8);
        EXPECT_EQ(item->type(), fes::Wave::kShortPeriod);
        break;
      case fes::kS1:
        EXPECT_EQ(item->ident(), fes::kS1);
        EXPECT_NEAR(item->freq(), detail::math::radians(15.0), 1e-8);
        EXPECT_EQ(item->type(), fes::Wave::kShortPeriod);
        break;
      case fes::kQ1:
        EXPECT_EQ(item->ident(), fes::kQ1);
        EXPECT_NEAR(item->freq(), detail::math::radians(13.3986609), 1e-8);
        EXPECT_EQ(item->type(), fes::Wave::kShortPeriod);
        break;
      case fes::kMm:
        EXPECT_EQ(item->ident(), fes::kMm);
        EXPECT_NEAR(item->freq(), detail::math::radians(0.5443747), 1e-8);
        EXPECT_EQ(item->type(), fes::Wave::kLongPeriod);
        break;
      case fes::kMf:
        EXPECT_EQ(item->ident(), fes::kMf);
        EXPECT_NEAR(item->freq(), detail::math::radians(1.0980331), 1e-8);
        EXPECT_EQ(item->type(), fes::Wave::kLongPeriod);
        break;
      case fes::kMtm:
        EXPECT_EQ(item->ident(), fes::kMtm);
        EXPECT_NEAR(item->freq(), detail::math::radians(1.642407762), 1e-8);
        EXPECT_EQ(item->type(), fes::Wave::kLongPeriod);
        break;
      case fes::kMsqm:
        EXPECT_EQ(item->ident(), fes::kMsqm);
        EXPECT_NEAR(item->freq(), detail::math::radians(2.113928850), 1e-8);
        EXPECT_EQ(item->type(), fes::Wave::kLongPeriod);
        break;
      case fes::kEps2:
        EXPECT_EQ(item->ident(), fes::kEps2);
        EXPECT_NEAR(item->freq(), detail::math::radians(27.423833730), 1e-8);
        EXPECT_EQ(item->type(), fes::Wave::kShortPeriod);
        break;
      case fes::kLambda2:
        EXPECT_EQ(item->ident(), fes::kLambda2);
        EXPECT_NEAR(item->freq(), detail::math::radians(29.4556253), 1e-8);
        EXPECT_EQ(item->type(), fes::Wave::kShortPeriod);
        break;
      case fes::kEta2:
        EXPECT_EQ(item->ident(), fes::kEta2);
        EXPECT_NEAR(item->freq(), detail::math::radians(30.6265120), 1e-8);
        EXPECT_EQ(item->type(), fes::Wave::kShortPeriod);
        break;
      case fes::k2Q1:
        EXPECT_EQ(item->ident(), fes::k2Q1);
        EXPECT_NEAR(item->freq(), detail::math::radians(12.8542862), 1e-8);
        EXPECT_EQ(item->type(), fes::Wave::kShortPeriod);
        break;
      case fes::kSigma1:
        EXPECT_EQ(item->ident(), fes::kSigma1);
        EXPECT_NEAR(item->freq(), detail::math::radians(12.9271398), 1e-8);
        EXPECT_EQ(item->type(), fes::Wave::kShortPeriod);
        break;
      case fes::kRho1:
        EXPECT_EQ(item->ident(), fes::kRho1);
        EXPECT_NEAR(item->freq(), detail::math::radians(13.4715145), 1e-8);
        EXPECT_EQ(item->type(), fes::Wave::kShortPeriod);
        break;
      case fes::kM1:
        EXPECT_EQ(item->ident(), fes::kM1);
        EXPECT_NEAR(item->freq(), detail::math::radians(14.49669394), 1e-8);
        EXPECT_EQ(item->type(), fes::Wave::kShortPeriod);
        break;
      case fes::kM11:
        EXPECT_EQ(item->ident(), fes::kM11);
        EXPECT_NEAR(item->freq(), detail::math::radians(14.48741027), 1e-8);
        EXPECT_EQ(item->type(), fes::Wave::kShortPeriod);
        break;
      case fes::kM12:
        EXPECT_EQ(item->ident(), fes::kM12);
        EXPECT_NEAR(item->freq(), detail::math::radians(14.4966939), 1e-8);
        EXPECT_EQ(item->type(), fes::Wave::kShortPeriod);
        break;
      case fes::kM13:
        EXPECT_EQ(item->ident(), fes::kM13);
        EXPECT_NEAR(item->freq(), detail::math::radians(14.49205210), 1e-8);
        EXPECT_EQ(item->type(), fes::Wave::kShortPeriod);
        break;
      case fes::kChi1:
        EXPECT_EQ(item->ident(), fes::kChi1);
        EXPECT_NEAR(item->freq(), detail::math::radians(14.5695476), 1e-8);
        EXPECT_EQ(item->type(), fes::Wave::kShortPeriod);
        break;
      case fes::kPi1:
        EXPECT_EQ(item->ident(), fes::kPi1);
        EXPECT_NEAR(item->freq(), detail::math::radians(14.9178647), 1e-8);
        EXPECT_EQ(item->type(), fes::Wave::kShortPeriod);
        break;
      case fes::kPhi1:
        EXPECT_EQ(item->ident(), fes::kPhi1);
        EXPECT_NEAR(item->freq(), detail::math::radians(15.1232059), 1e-8);
        EXPECT_EQ(item->type(), fes::Wave::kShortPeriod);
        break;
      case fes::kTheta1:
        EXPECT_EQ(item->ident(), fes::kTheta1);
        EXPECT_NEAR(item->freq(), detail::math::radians(15.5125897), 1e-8);
        EXPECT_EQ(item->type(), fes::Wave::kShortPeriod);
        break;
      case fes::kJ1:
        EXPECT_EQ(item->ident(), fes::kJ1);
        EXPECT_NEAR(item->freq(), detail::math::radians(15.5854433), 1e-8);
        EXPECT_EQ(item->type(), fes::Wave::kShortPeriod);
        break;
      case fes::kOO1:
        EXPECT_EQ(item->ident(), fes::kOO1);
        EXPECT_NEAR(item->freq(), detail::math::radians(16.1391017), 1e-8);
        EXPECT_EQ(item->type(), fes::Wave::kShortPeriod);
        break;
      case fes::kM3:
        EXPECT_EQ(item->ident(), fes::kM3);
        EXPECT_NEAR(item->freq(), detail::math::radians(43.4761563), 1e-8);
        EXPECT_EQ(item->type(), fes::Wave::kShortPeriod);
        break;
      case fes::kM6:
        EXPECT_EQ(item->ident(), fes::kM6);
        EXPECT_NEAR(item->freq(), detail::math::radians(86.9523126), 1e-8);
        EXPECT_EQ(item->type(), fes::Wave::kShortPeriod);
        break;
      case fes::kMN4:
        EXPECT_EQ(item->ident(), fes::kMN4);
        EXPECT_NEAR(item->freq(), detail::math::radians(57.4238337), 1e-8);
        EXPECT_EQ(item->type(), fes::Wave::kShortPeriod);
        break;
      case fes::kMS4:
        EXPECT_EQ(item->ident(), fes::kMS4);
        EXPECT_NEAR(item->freq(), detail::math::radians(58.9841042), 1e-8);
        EXPECT_EQ(item->type(), fes::Wave::kShortPeriod);
        break;
      case fes::kN4:
        EXPECT_EQ(item->ident(), fes::kN4);
        EXPECT_NEAR(item->freq(), detail::math::radians(56.8794590), 1e-8);
        EXPECT_EQ(item->type(), fes::Wave::kShortPeriod);
        break;
      case fes::kR2:
        EXPECT_EQ(item->ident(), fes::kR2);
        EXPECT_NEAR(item->freq(), detail::math::radians(30.0410667), 1e-8);
        EXPECT_EQ(item->type(), fes::Wave::kShortPeriod);
        break;
      case fes::kR4:
        EXPECT_EQ(item->ident(), fes::kR4);
        EXPECT_NEAR(item->freq(), detail::math::radians(30.0410667 * 2), 1e-8);
        EXPECT_EQ(item->type(), fes::Wave::kShortPeriod);
        break;
      case fes::kS4:
        EXPECT_EQ(item->ident(), fes::kS4);
        EXPECT_NEAR(item->freq(), detail::math::radians(60.0), 1e-8);
        EXPECT_EQ(item->type(), fes::Wave::kShortPeriod);
        break;
      case fes::kMNS2:
        EXPECT_EQ(item->ident(), fes::kMNS2);
        EXPECT_NEAR(item->freq(), detail::math::radians(27.4238337), 1e-8);
        EXPECT_EQ(item->type(), fes::Wave::kShortPeriod);
        break;
      case fes::kMK4:
        EXPECT_EQ(item->ident(), fes::kMK4);
        EXPECT_NEAR(item->freq(), detail::math::radians(59.0662415), 1e-8);
        EXPECT_EQ(item->type(), fes::Wave::kShortPeriod);
        break;
      case fes::kSN4:
        EXPECT_EQ(item->ident(), fes::kSN4);
        EXPECT_NEAR(item->freq(), detail::math::radians(58.4397295), 1e-8);
        EXPECT_EQ(item->type(), fes::Wave::kShortPeriod);
        break;
      case fes::kSK4:
        EXPECT_EQ(item->ident(), fes::kSK4);
        EXPECT_NEAR(item->freq(), detail::math::radians(60.0821373), 1e-8);
        EXPECT_EQ(item->type(), fes::Wave::kShortPeriod);
        break;
      case fes::k2MN6:
        EXPECT_EQ(item->ident(), fes::k2MN6);
        EXPECT_NEAR(item->freq(), detail::math::radians(86.4079380), 1e-8);
        EXPECT_EQ(item->type(), fes::Wave::kShortPeriod);
        break;
      case fes::k2MS6:
        EXPECT_EQ(item->ident(), fes::k2MS6);
        EXPECT_NEAR(item->freq(), detail::math::radians(87.9682084), 1e-8);
        EXPECT_EQ(item->type(), fes::Wave::kShortPeriod);
        break;
      case fes::k2MK6:
        EXPECT_EQ(item->ident(), fes::k2MK6);
        EXPECT_NEAR(item->freq(), detail::math::radians(88.0503457), 1e-8);
        EXPECT_EQ(item->type(), fes::Wave::kShortPeriod);
        break;
      case fes::kMSN6:
        EXPECT_EQ(item->ident(), fes::kMSN6);
        EXPECT_NEAR(item->freq(), detail::math::radians(87.4238337), 1e-8);
        EXPECT_EQ(item->type(), fes::Wave::kShortPeriod);
        break;
      case fes::k2SM6:
        EXPECT_EQ(item->ident(), fes::k2SM6);
        EXPECT_NEAR(item->freq(), detail::math::radians(88.9841042), 1e-8);
        EXPECT_EQ(item->type(), fes::Wave::kShortPeriod);
        break;
      case fes::kMSK6:
        EXPECT_EQ(item->ident(), fes::kMSK6);
        EXPECT_NEAR(item->freq(), detail::math::radians(89.0662415), 1e-8);
        EXPECT_EQ(item->type(), fes::Wave::kShortPeriod);
        break;
      case fes::kMP1:
        EXPECT_EQ(item->ident(), fes::kMP1);
        EXPECT_NEAR(item->freq(), detail::math::radians(14.0251729), 1e-8);
        EXPECT_EQ(item->type(), fes::Wave::kShortPeriod);
        break;
      case fes::k2SM2:
        EXPECT_EQ(item->ident(), fes::k2SM2);
        EXPECT_NEAR(item->freq(), detail::math::radians(31.0158958), 1e-8);
        EXPECT_EQ(item->type(), fes::Wave::kShortPeriod);
        break;
      case fes::kPsi1:
        EXPECT_EQ(item->ident(), fes::kPsi1);
        EXPECT_NEAR(item->freq(), detail::math::radians(15.0821353), 1e-8);
        EXPECT_EQ(item->type(), fes::Wave::kShortPeriod);
        break;
      case fes::k2MS2:
        EXPECT_EQ(item->ident(), fes::k2MS2);
        EXPECT_NEAR(item->freq(), detail::math::radians(27.9682084), 1e-8);
        EXPECT_EQ(item->type(), fes::Wave::kShortPeriod);
        break;
      case fes::kMKS2:
        EXPECT_EQ(item->ident(), fes::kMKS2);
        EXPECT_NEAR(item->freq(), detail::math::radians(29.0662415), 1e-8);
        EXPECT_EQ(item->type(), fes::Wave::kShortPeriod);
        break;
      case fes::k2MN2:
        EXPECT_EQ(item->ident(), fes::k2MN2);
        EXPECT_NEAR(item->freq(), detail::math::radians(29.5284789), 1e-8);
        EXPECT_EQ(item->type(), fes::Wave::kShortPeriod);
        break;
      case fes::kMSN2:
        EXPECT_EQ(item->ident(), fes::kMSN2);
        EXPECT_NEAR(item->freq(), detail::math::radians(30.5443747), 1e-8);
        EXPECT_EQ(item->type(), fes::Wave::kShortPeriod);
        break;
      case fes::kMO3:
        EXPECT_EQ(item->ident(), fes::kMO3);
        EXPECT_NEAR(item->freq(), detail::math::radians(42.9271398), 1e-8);
        EXPECT_EQ(item->type(), fes::Wave::kShortPeriod);
        break;
      case fes::k2MK3:
        EXPECT_EQ(item->ident(), fes::k2MK3);
        EXPECT_NEAR(item->freq(), detail::math::radians(42.9271398), 1e-8);
        EXPECT_EQ(item->type(), fes::Wave::kShortPeriod);
        break;
      case fes::kMK3:
        EXPECT_EQ(item->ident(), fes::kMK3);
        EXPECT_NEAR(item->freq(), detail::math::radians(44.0251729), 1e-8);
        EXPECT_EQ(item->type(), fes::Wave::kShortPeriod);
        break;
      case fes::kS6:
        EXPECT_EQ(item->ident(), fes::kS6);
        EXPECT_NEAR(item->freq(), detail::math::radians(90.0), 1e-8);
        EXPECT_EQ(item->type(), fes::Wave::kShortPeriod);
        break;
      case fes::kM8:
        EXPECT_EQ(item->ident(), fes::kM8);
        EXPECT_NEAR(item->freq(), detail::math::radians(115.9364169), 1e-8);
        EXPECT_EQ(item->type(), fes::Wave::kShortPeriod);
        break;
      case fes::kMSf:
        EXPECT_EQ(item->ident(), fes::kMSf);
        EXPECT_NEAR(item->freq(), detail::math::radians(1.01589578), 1e-8);
        EXPECT_EQ(item->type(), fes::Wave::kLongPeriod);
        break;
      case fes::kSsa:
        EXPECT_EQ(item->ident(), fes::kSsa);
        EXPECT_NEAR(item->freq(), detail::math::radians(0.08213728), 1e-8);
        EXPECT_EQ(item->type(), fes::Wave::kLongPeriod);
        break;
      case fes::kSa:
        EXPECT_EQ(item->ident(), fes::kSa);
        EXPECT_NEAR(item->freq(), detail::math::radians(0.04106864), 1e-8);
        EXPECT_EQ(item->type(), fes::Wave::kLongPeriod);
        break;
      case fes::kA5:
        EXPECT_EQ(item->ident(), fes::kA5);
        EXPECT_NEAR(item->freq(), detail::math::radians(1.01589579), 1e-8);
        EXPECT_EQ(item->type(), fes::Wave::kLongPeriod);
        break;
      case fes::kSa1:
        EXPECT_EQ(item->ident(), fes::kSa1);
        EXPECT_NEAR(item->freq(), detail::math::radians(0.041066678), 1e-8);
        EXPECT_EQ(item->type(), fes::Wave::kLongPeriod);
        break;
      case fes::kSta:
        EXPECT_EQ(item->ident(), fes::kSta);
        EXPECT_NEAR(item->freq(), detail::math::radians(0.123203956), 1e-8);
        EXPECT_EQ(item->type(), fes::Wave::kLongPeriod);
        break;
      case fes::kMm1:
        EXPECT_EQ(item->ident(), fes::kMm1);
        EXPECT_NEAR(item->freq(), detail::math::radians(0.553658366), 1e-8);
        EXPECT_EQ(item->type(), fes::Wave::kLongPeriod);
        break;
      case fes::kMm2:
        EXPECT_EQ(item->ident(), fes::kMm2);
        EXPECT_NEAR(item->freq(), detail::math::radians(0.549016532), 1e-8);
        EXPECT_EQ(item->type(), fes::Wave::kLongPeriod);
        break;
      case fes::kMf1:
        EXPECT_EQ(item->ident(), fes::kMf1);
        EXPECT_NEAR(item->freq(), detail::math::radians(1.088749395), 1e-8);
        EXPECT_EQ(item->type(), fes::Wave::kLongPeriod);
        break;
      case fes::kMf2:
        EXPECT_EQ(item->ident(), fes::kMf2);
        EXPECT_NEAR(item->freq(), detail::math::radians(1.093391230), 1e-8);
        EXPECT_EQ(item->type(), fes::Wave::kLongPeriod);
        break;
      case fes::kM0:
        EXPECT_EQ(item->ident(), fes::kM0);
        EXPECT_NEAR(item->freq(), 0, 1e-8);
        EXPECT_EQ(item->type(), fes::Wave::kLongPeriod);
        break;

      default:
        throw std::runtime_error(std::to_string(item->ident()));
    }
  }
}
// NOLINTEND(readability-function-cognitive-complexity,*-function-size)

TEST(WaveTable, Constructor) {
  check_wave_table_constructor(fes::wave::Table());
}

static void admittance(const double r, const fes::Constituent ident,
                       fes::wave::Table& table) {
  table.admittance();
  EXPECT_NEAR(table[ident]->tide().real(), r, 1e-6);
  EXPECT_NEAR(table[ident]->tide().imag(), r, 1e-6);

  table[ident]->admittance(false);
  table[ident]->tide({1, 1});
  table.admittance();
  EXPECT_EQ(table[ident]->tide(), std::complex<double>(1, 1));

  table[ident]->admittance(true);
  table[ident]->tide({1, 1});
}

TEST(WaveTable, Admittance) {
  auto table = fes::wave::Table();
  for (auto&& item : table) {
    item->tide({1, 1});
  }

  admittance(0.2378, fes::k2Q1, table);
  admittance(0.2706, fes::kSigma1, table);
  admittance(0.1688, fes::kRho1, table);
  admittance(0.0241, fes::kM11, table);
  admittance(0.0671, fes::kM12, table);
  admittance(0.0124, fes::kChi1, table);
  admittance(0.0201, fes::kPi1, table);
  admittance(0.0137, fes::kPhi1, table);
  admittance(0.009, fes::kTheta1, table);
  admittance(0.0447, fes::kJ1, table);
  admittance(0.0182, fes::kOO1, table);
  admittance(0.0796782, fes::kEta2, table);
  admittance(0.374697218357, fes::kMu2, table);
  admittance(0.157529811402, fes::kNu2, table);
  admittance(0.010949128375, fes::kLambda2, table);
  admittance(0.053354227357, fes::kL2, table);
  admittance(0.16871051505, fes::kT2, table);
  admittance(0.2387, fes::k2N2, table);
  admittance(0.094151295, fes::kEps2, table);
  // force 2N2 to be calculated in dynamically
  table[fes::k2N2]->admittance(false);
  table[fes::k2N2]->tide({1, 1});
  admittance(0.499810, fes::kEps2, table);
}

TEST(WaveTable, IdentName) {
  for (auto&& item : fes::wave::Table()) {
    const auto* name = item->name();
    auto ident = fes::constituents::parse(name);
    EXPECT_EQ(item->ident(), ident);
  }
}

TEST(WaveTable, Sparse) {
  auto table =
      fes::wave::Table({"O1", "K1", "M2", "S2", "N2", "K2", "M4", "M6", "Mf2"});
  EXPECT_EQ(table.size(), 9);
  EXPECT_EQ(table[fes::kO1]->ident(), fes::kO1);
  EXPECT_EQ(table[0]->ident(), fes::kO1);
  EXPECT_EQ(table[1]->ident(), fes::kK1);
  EXPECT_EQ(table[2]->ident(), fes::kN2);
  EXPECT_EQ(table[3]->ident(), fes::kM2);
  EXPECT_EQ(table[4]->ident(), fes::kS2);
  EXPECT_EQ(table[5]->ident(), fes::kK2);
  EXPECT_EQ(table[6]->ident(), fes::kM4);
  EXPECT_EQ(table[7]->ident(), fes::kM6);
  EXPECT_EQ(table[8]->ident(), fes::kMf2);
  EXPECT_THROW(table[fes::kP1], std::out_of_range);
  EXPECT_THROW(table.admittance(), std::out_of_range);
  EXPECT_THROW(
      {
        auto wave = table.find("_o1");
        wave->ident();
      },
      std::invalid_argument);
  EXPECT_EQ(table.find("Mf"), nullptr);
  EXPECT_EQ(table.find("O1")->ident(), fes::kO1);
}
