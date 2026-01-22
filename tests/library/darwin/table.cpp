// Copyright (c) 2025 CNES
//
// All rights reserved. Use of this source code is governed by a
// BSD-style license that can be found in the LICENSE file.
#include "fes/darwin/table.hpp"

#include <gtest/gtest.h>

namespace detail = fes::detail;

// NOLINTBEGIN(readability-function-cognitive-complexity,*-function-size)
// Used to test all wave components in a single function
inline auto check_wave_table_constructor(const fes::darwin::wave::Table& table)
    -> void {
  for (auto&& item : table) {
    switch (item->ident()) {
      case fes::darwin::kO1:
        EXPECT_EQ(item->ident(), fes::darwin::kO1);
        EXPECT_NEAR(item->freq(), detail::math::radians(13.9430356), 1e-8);
        EXPECT_EQ(item->type(), fes::darwin::Wave::kShortPeriod);
        break;
      case fes::darwin::kP1:
        EXPECT_EQ(item->ident(), fes::darwin::kP1);
        EXPECT_NEAR(item->freq(), detail::math::radians(14.9589314), 1e-8);
        EXPECT_EQ(item->type(), fes::darwin::Wave::kShortPeriod);
        break;
      case fes::darwin::kK1:
        EXPECT_EQ(item->ident(), fes::darwin::kK1);
        EXPECT_NEAR(item->freq(), detail::math::radians(15.0410686), 1e-8);
        EXPECT_EQ(item->type(), fes::darwin::Wave::kShortPeriod);
        break;
      case fes::darwin::k2N2:
        EXPECT_EQ(item->ident(), fes::darwin::k2N2);
        EXPECT_NEAR(item->freq(), detail::math::radians(27.8953548), 1e-8);
        EXPECT_EQ(item->type(), fes::darwin::Wave::kShortPeriod);
        break;
      case fes::darwin::kMu2:
        EXPECT_EQ(item->ident(), fes::darwin::kMu2);
        EXPECT_NEAR(item->freq(), detail::math::radians(27.9682084), 1e-8);
        EXPECT_EQ(item->type(), fes::darwin::Wave::kShortPeriod);
        break;
      case fes::darwin::kN2:
        EXPECT_EQ(item->ident(), fes::darwin::kN2);
        EXPECT_NEAR(item->freq(), detail::math::radians(28.4397295), 1e-8);
        EXPECT_EQ(item->type(), fes::darwin::Wave::kShortPeriod);
        break;
      case fes::darwin::kNu2:
        EXPECT_EQ(item->ident(), fes::darwin::kNu2);
        EXPECT_NEAR(item->freq(), detail::math::radians(28.5125831), 1e-8);
        EXPECT_EQ(item->type(), fes::darwin::Wave::kShortPeriod);
        break;
      case fes::darwin::kM2:
        EXPECT_EQ(item->ident(), fes::darwin::kM2);
        EXPECT_NEAR(item->freq(), detail::math::radians(28.9841042), 1e-8);
        EXPECT_EQ(item->type(), fes::darwin::Wave::kShortPeriod);
        break;
      case fes::darwin::kL2:
        EXPECT_EQ(item->ident(), fes::darwin::kL2);
        EXPECT_NEAR(item->freq(), detail::math::radians(29.5284789), 1e-8);
        EXPECT_EQ(item->type(), fes::darwin::Wave::kShortPeriod);
        break;
      case fes::darwin::kT2:
        EXPECT_EQ(item->ident(), fes::darwin::kT2);
        EXPECT_NEAR(item->freq(), detail::math::radians(29.9589333), 1e-8);
        EXPECT_EQ(item->type(), fes::darwin::Wave::kShortPeriod);
        break;
      case fes::darwin::kS2:
        EXPECT_EQ(item->ident(), fes::darwin::kS2);
        EXPECT_NEAR(item->freq(), detail::math::radians(30.0), 1e-8);
        EXPECT_EQ(item->type(), fes::darwin::Wave::kShortPeriod);
        break;
      case fes::darwin::kK2:
        EXPECT_EQ(item->ident(), fes::darwin::kK2);
        EXPECT_NEAR(item->freq(), detail::math::radians(30.0821373), 1e-8);
        EXPECT_EQ(item->type(), fes::darwin::Wave::kShortPeriod);
        break;
      case fes::darwin::kM4:
        EXPECT_EQ(item->ident(), fes::darwin::kM4);
        EXPECT_NEAR(item->freq(), detail::math::radians(57.9682084), 1e-8);
        EXPECT_EQ(item->type(), fes::darwin::Wave::kShortPeriod);
        break;
      case fes::darwin::kS1:
        EXPECT_EQ(item->ident(), fes::darwin::kS1);
        EXPECT_NEAR(item->freq(), detail::math::radians(15.0), 1e-8);
        EXPECT_EQ(item->type(), fes::darwin::Wave::kShortPeriod);
        break;
      case fes::darwin::kQ1:
        EXPECT_EQ(item->ident(), fes::darwin::kQ1);
        EXPECT_NEAR(item->freq(), detail::math::radians(13.3986609), 1e-8);
        EXPECT_EQ(item->type(), fes::darwin::Wave::kShortPeriod);
        break;
      case fes::darwin::kMm:
        EXPECT_EQ(item->ident(), fes::darwin::kMm);
        EXPECT_NEAR(item->freq(), detail::math::radians(0.5443747), 1e-8);
        EXPECT_EQ(item->type(), fes::darwin::Wave::kLongPeriod);
        break;
      case fes::darwin::kMf:
        EXPECT_EQ(item->ident(), fes::darwin::kMf);
        EXPECT_NEAR(item->freq(), detail::math::radians(1.0980331), 1e-8);
        EXPECT_EQ(item->type(), fes::darwin::Wave::kLongPeriod);
        break;
      case fes::darwin::kMtm:
        EXPECT_EQ(item->ident(), fes::darwin::kMtm);
        EXPECT_NEAR(item->freq(), detail::math::radians(1.642407762), 1e-8);
        EXPECT_EQ(item->type(), fes::darwin::Wave::kLongPeriod);
        break;
      case fes::darwin::kMSqm:
        EXPECT_EQ(item->ident(), fes::darwin::kMSqm);
        EXPECT_NEAR(item->freq(), detail::math::radians(2.113928850), 1e-8);
        EXPECT_EQ(item->type(), fes::darwin::Wave::kLongPeriod);
        break;
      case fes::darwin::kEps2:
        EXPECT_EQ(item->ident(), fes::darwin::kEps2);
        EXPECT_NEAR(item->freq(), detail::math::radians(27.423833730), 1e-8);
        EXPECT_EQ(item->type(), fes::darwin::Wave::kShortPeriod);
        break;
      case fes::darwin::kLambda2:
        EXPECT_EQ(item->ident(), fes::darwin::kLambda2);
        EXPECT_NEAR(item->freq(), detail::math::radians(29.4556253), 1e-8);
        EXPECT_EQ(item->type(), fes::darwin::Wave::kShortPeriod);
        break;
      case fes::darwin::kEta2:
        EXPECT_EQ(item->ident(), fes::darwin::kEta2);
        EXPECT_NEAR(item->freq(), detail::math::radians(30.6265120), 1e-8);
        EXPECT_EQ(item->type(), fes::darwin::Wave::kShortPeriod);
        break;
      case fes::darwin::k2Q1:
        EXPECT_EQ(item->ident(), fes::darwin::k2Q1);
        EXPECT_NEAR(item->freq(), detail::math::radians(12.8542862), 1e-8);
        EXPECT_EQ(item->type(), fes::darwin::Wave::kShortPeriod);
        break;
      case fes::darwin::kSigma1:
        EXPECT_EQ(item->ident(), fes::darwin::kSigma1);
        EXPECT_NEAR(item->freq(), detail::math::radians(12.9271398), 1e-8);
        EXPECT_EQ(item->type(), fes::darwin::Wave::kShortPeriod);
        break;
      case fes::darwin::kRho1:
        EXPECT_EQ(item->ident(), fes::darwin::kRho1);
        EXPECT_NEAR(item->freq(), detail::math::radians(13.4715145), 1e-8);
        EXPECT_EQ(item->type(), fes::darwin::Wave::kShortPeriod);
        break;
      case fes::darwin::kM1:
        EXPECT_EQ(item->ident(), fes::darwin::kM1);
        EXPECT_NEAR(item->freq(), detail::math::radians(14.49669394), 1e-8);
        EXPECT_EQ(item->type(), fes::darwin::Wave::kShortPeriod);
        break;
      case fes::darwin::kM11:
        EXPECT_EQ(item->ident(), fes::darwin::kM11);
        EXPECT_NEAR(item->freq(), detail::math::radians(14.48741027), 1e-8);
        EXPECT_EQ(item->type(), fes::darwin::Wave::kShortPeriod);
        break;
      case fes::darwin::kM12:
        EXPECT_EQ(item->ident(), fes::darwin::kM12);
        EXPECT_NEAR(item->freq(), detail::math::radians(14.4966939), 1e-8);
        EXPECT_EQ(item->type(), fes::darwin::Wave::kShortPeriod);
        break;
      case fes::darwin::kM13:
        EXPECT_EQ(item->ident(), fes::darwin::kM13);
        EXPECT_NEAR(item->freq(), detail::math::radians(14.49205210), 1e-8);
        EXPECT_EQ(item->type(), fes::darwin::Wave::kShortPeriod);
        break;
      case fes::darwin::kChi1:
        EXPECT_EQ(item->ident(), fes::darwin::kChi1);
        EXPECT_NEAR(item->freq(), detail::math::radians(14.5695476), 1e-8);
        EXPECT_EQ(item->type(), fes::darwin::Wave::kShortPeriod);
        break;
      case fes::darwin::kPi1:
        EXPECT_EQ(item->ident(), fes::darwin::kPi1);
        EXPECT_NEAR(item->freq(), detail::math::radians(14.9178647), 1e-8);
        EXPECT_EQ(item->type(), fes::darwin::Wave::kShortPeriod);
        break;
      case fes::darwin::kPhi1:
        EXPECT_EQ(item->ident(), fes::darwin::kPhi1);
        EXPECT_NEAR(item->freq(), detail::math::radians(15.1232059), 1e-8);
        EXPECT_EQ(item->type(), fes::darwin::Wave::kShortPeriod);
        break;
      case fes::darwin::kTheta1:
        EXPECT_EQ(item->ident(), fes::darwin::kTheta1);
        EXPECT_NEAR(item->freq(), detail::math::radians(15.5125897), 1e-8);
        EXPECT_EQ(item->type(), fes::darwin::Wave::kShortPeriod);
        break;
      case fes::darwin::kJ1:
        EXPECT_EQ(item->ident(), fes::darwin::kJ1);
        EXPECT_NEAR(item->freq(), detail::math::radians(15.5854433), 1e-8);
        EXPECT_EQ(item->type(), fes::darwin::Wave::kShortPeriod);
        break;
      case fes::darwin::kOO1:
        EXPECT_EQ(item->ident(), fes::darwin::kOO1);
        EXPECT_NEAR(item->freq(), detail::math::radians(16.1391017), 1e-8);
        EXPECT_EQ(item->type(), fes::darwin::Wave::kShortPeriod);
        break;
      case fes::darwin::kM3:
        EXPECT_EQ(item->ident(), fes::darwin::kM3);
        EXPECT_NEAR(item->freq(), detail::math::radians(43.4761563), 1e-8);
        EXPECT_EQ(item->type(), fes::darwin::Wave::kShortPeriod);
        break;
      case fes::darwin::kM6:
        EXPECT_EQ(item->ident(), fes::darwin::kM6);
        EXPECT_NEAR(item->freq(), detail::math::radians(86.9523126), 1e-8);
        EXPECT_EQ(item->type(), fes::darwin::Wave::kShortPeriod);
        break;
      case fes::darwin::kMN4:
        EXPECT_EQ(item->ident(), fes::darwin::kMN4);
        EXPECT_NEAR(item->freq(), detail::math::radians(57.4238337), 1e-8);
        EXPECT_EQ(item->type(), fes::darwin::Wave::kShortPeriod);
        break;
      case fes::darwin::kMS4:
        EXPECT_EQ(item->ident(), fes::darwin::kMS4);
        EXPECT_NEAR(item->freq(), detail::math::radians(58.9841042), 1e-8);
        EXPECT_EQ(item->type(), fes::darwin::Wave::kShortPeriod);
        break;
      case fes::darwin::kN4:
        EXPECT_EQ(item->ident(), fes::darwin::kN4);
        EXPECT_NEAR(item->freq(), detail::math::radians(56.8794590), 1e-8);
        EXPECT_EQ(item->type(), fes::darwin::Wave::kShortPeriod);
        break;
      case fes::darwin::kR2:
        EXPECT_EQ(item->ident(), fes::darwin::kR2);
        EXPECT_NEAR(item->freq(), detail::math::radians(30.0410667), 1e-8);
        EXPECT_EQ(item->type(), fes::darwin::Wave::kShortPeriod);
        break;
      case fes::darwin::kR4:
        EXPECT_EQ(item->ident(), fes::darwin::kR4);
        EXPECT_NEAR(item->freq(), detail::math::radians(30.0410667 * 2), 1e-8);
        EXPECT_EQ(item->type(), fes::darwin::Wave::kShortPeriod);
        break;
      case fes::darwin::kS4:
        EXPECT_EQ(item->ident(), fes::darwin::kS4);
        EXPECT_NEAR(item->freq(), detail::math::radians(60.0), 1e-8);
        EXPECT_EQ(item->type(), fes::darwin::Wave::kShortPeriod);
        break;
      case fes::darwin::kMNS2:
        EXPECT_EQ(item->ident(), fes::darwin::kMNS2);
        EXPECT_NEAR(item->freq(), detail::math::radians(27.4238337), 1e-8);
        EXPECT_EQ(item->type(), fes::darwin::Wave::kShortPeriod);
        break;
      case fes::darwin::kMK4:
        EXPECT_EQ(item->ident(), fes::darwin::kMK4);
        EXPECT_NEAR(item->freq(), detail::math::radians(59.0662415), 1e-8);
        EXPECT_EQ(item->type(), fes::darwin::Wave::kShortPeriod);
        break;
      case fes::darwin::kSN4:
        EXPECT_EQ(item->ident(), fes::darwin::kSN4);
        EXPECT_NEAR(item->freq(), detail::math::radians(58.4397295), 1e-8);
        EXPECT_EQ(item->type(), fes::darwin::Wave::kShortPeriod);
        break;
      case fes::darwin::kSK4:
        EXPECT_EQ(item->ident(), fes::darwin::kSK4);
        EXPECT_NEAR(item->freq(), detail::math::radians(60.0821373), 1e-8);
        EXPECT_EQ(item->type(), fes::darwin::Wave::kShortPeriod);
        break;
      case fes::darwin::k2MN6:
        EXPECT_EQ(item->ident(), fes::darwin::k2MN6);
        EXPECT_NEAR(item->freq(), detail::math::radians(86.4079380), 1e-8);
        EXPECT_EQ(item->type(), fes::darwin::Wave::kShortPeriod);
        break;
      case fes::darwin::k2MS6:
        EXPECT_EQ(item->ident(), fes::darwin::k2MS6);
        EXPECT_NEAR(item->freq(), detail::math::radians(87.9682084), 1e-8);
        EXPECT_EQ(item->type(), fes::darwin::Wave::kShortPeriod);
        break;
      case fes::darwin::k2MK6:
        EXPECT_EQ(item->ident(), fes::darwin::k2MK6);
        EXPECT_NEAR(item->freq(), detail::math::radians(88.0503457), 1e-8);
        EXPECT_EQ(item->type(), fes::darwin::Wave::kShortPeriod);
        break;
      case fes::darwin::kMSN6:
        EXPECT_EQ(item->ident(), fes::darwin::kMSN6);
        EXPECT_NEAR(item->freq(), detail::math::radians(87.4238337), 1e-8);
        EXPECT_EQ(item->type(), fes::darwin::Wave::kShortPeriod);
        break;
      case fes::darwin::k2SM6:
        EXPECT_EQ(item->ident(), fes::darwin::k2SM6);
        EXPECT_NEAR(item->freq(), detail::math::radians(88.9841042), 1e-8);
        EXPECT_EQ(item->type(), fes::darwin::Wave::kShortPeriod);
        break;
      case fes::darwin::kMSK6:
        EXPECT_EQ(item->ident(), fes::darwin::kMSK6);
        EXPECT_NEAR(item->freq(), detail::math::radians(89.0662415), 1e-8);
        EXPECT_EQ(item->type(), fes::darwin::Wave::kShortPeriod);
        break;
      case fes::darwin::kMP1:
        EXPECT_EQ(item->ident(), fes::darwin::kMP1);
        EXPECT_NEAR(item->freq(), detail::math::radians(14.0251729), 1e-8);
        EXPECT_EQ(item->type(), fes::darwin::Wave::kShortPeriod);
        break;
      case fes::darwin::k2SM2:
        EXPECT_EQ(item->ident(), fes::darwin::k2SM2);
        EXPECT_NEAR(item->freq(), detail::math::radians(31.0158958), 1e-8);
        EXPECT_EQ(item->type(), fes::darwin::Wave::kShortPeriod);
        break;
      case fes::darwin::kPsi1:
        EXPECT_EQ(item->ident(), fes::darwin::kPsi1);
        EXPECT_NEAR(item->freq(), detail::math::radians(15.0821353), 1e-8);
        EXPECT_EQ(item->type(), fes::darwin::Wave::kShortPeriod);
        break;
      case fes::darwin::k2MS2:
        EXPECT_EQ(item->ident(), fes::darwin::k2MS2);
        EXPECT_NEAR(item->freq(), detail::math::radians(27.9682084), 1e-8);
        EXPECT_EQ(item->type(), fes::darwin::Wave::kShortPeriod);
        break;
      case fes::darwin::kMKS2:
        EXPECT_EQ(item->ident(), fes::darwin::kMKS2);
        EXPECT_NEAR(item->freq(), detail::math::radians(29.0662415), 1e-8);
        EXPECT_EQ(item->type(), fes::darwin::Wave::kShortPeriod);
        break;
      case fes::darwin::k2MN2:
        EXPECT_EQ(item->ident(), fes::darwin::k2MN2);
        EXPECT_NEAR(item->freq(), detail::math::radians(29.5284789), 1e-8);
        EXPECT_EQ(item->type(), fes::darwin::Wave::kShortPeriod);
        break;
      case fes::darwin::kMSN2:
        EXPECT_EQ(item->ident(), fes::darwin::kMSN2);
        EXPECT_NEAR(item->freq(), detail::math::radians(30.5443747), 1e-8);
        EXPECT_EQ(item->type(), fes::darwin::Wave::kShortPeriod);
        break;
      case fes::darwin::kMO3:
        EXPECT_EQ(item->ident(), fes::darwin::kMO3);
        EXPECT_NEAR(item->freq(), detail::math::radians(42.9271398), 1e-8);
        EXPECT_EQ(item->type(), fes::darwin::Wave::kShortPeriod);
        break;
      case fes::darwin::k2MK3:
        EXPECT_EQ(item->ident(), fes::darwin::k2MK3);
        EXPECT_NEAR(item->freq(), detail::math::radians(42.9271398), 1e-8);
        EXPECT_EQ(item->type(), fes::darwin::Wave::kShortPeriod);
        break;
      case fes::darwin::kMK3:
        EXPECT_EQ(item->ident(), fes::darwin::kMK3);
        EXPECT_NEAR(item->freq(), detail::math::radians(44.0251729), 1e-8);
        EXPECT_EQ(item->type(), fes::darwin::Wave::kShortPeriod);
        break;
      case fes::darwin::kS6:
        EXPECT_EQ(item->ident(), fes::darwin::kS6);
        EXPECT_NEAR(item->freq(), detail::math::radians(90.0), 1e-8);
        EXPECT_EQ(item->type(), fes::darwin::Wave::kShortPeriod);
        break;
      case fes::darwin::kM8:
        EXPECT_EQ(item->ident(), fes::darwin::kM8);
        EXPECT_NEAR(item->freq(), detail::math::radians(115.9364169), 1e-8);
        EXPECT_EQ(item->type(), fes::darwin::Wave::kShortPeriod);
        break;
      case fes::darwin::kMSf:
        EXPECT_EQ(item->ident(), fes::darwin::kMSf);
        EXPECT_NEAR(item->freq(), detail::math::radians(1.01589578), 1e-8);
        EXPECT_EQ(item->type(), fes::darwin::Wave::kLongPeriod);
        break;
      case fes::darwin::kSsa:
        EXPECT_EQ(item->ident(), fes::darwin::kSsa);
        EXPECT_NEAR(item->freq(), detail::math::radians(0.08213728), 1e-8);
        EXPECT_EQ(item->type(), fes::darwin::Wave::kLongPeriod);
        break;
      case fes::darwin::kSa:
        EXPECT_EQ(item->ident(), fes::darwin::kSa);
        EXPECT_NEAR(item->freq(), detail::math::radians(0.04106864), 1e-8);
        EXPECT_EQ(item->type(), fes::darwin::Wave::kLongPeriod);
        break;
      case fes::darwin::kA5:
        EXPECT_EQ(item->ident(), fes::darwin::kA5);
        EXPECT_NEAR(item->freq(), detail::math::radians(1.01589579), 1e-8);
        EXPECT_EQ(item->type(), fes::darwin::Wave::kLongPeriod);
        break;
      case fes::darwin::kSa1:
        EXPECT_EQ(item->ident(), fes::darwin::kSa1);
        EXPECT_NEAR(item->freq(), detail::math::radians(0.041066678), 1e-8);
        EXPECT_EQ(item->type(), fes::darwin::Wave::kLongPeriod);
        break;
      case fes::darwin::kSta:
        EXPECT_EQ(item->ident(), fes::darwin::kSta);
        EXPECT_NEAR(item->freq(), detail::math::radians(0.123203956), 1e-8);
        EXPECT_EQ(item->type(), fes::darwin::Wave::kLongPeriod);
        break;
      case fes::darwin::kMm1:
        EXPECT_EQ(item->ident(), fes::darwin::kMm1);
        EXPECT_NEAR(item->freq(), detail::math::radians(0.553658366), 1e-8);
        EXPECT_EQ(item->type(), fes::darwin::Wave::kLongPeriod);
        break;
      case fes::darwin::kMm2:
        EXPECT_EQ(item->ident(), fes::darwin::kMm2);
        EXPECT_NEAR(item->freq(), detail::math::radians(0.549016532), 1e-8);
        EXPECT_EQ(item->type(), fes::darwin::Wave::kLongPeriod);
        break;
      case fes::darwin::kMf1:
        EXPECT_EQ(item->ident(), fes::darwin::kMf1);
        EXPECT_NEAR(item->freq(), detail::math::radians(1.088749395), 1e-8);
        EXPECT_EQ(item->type(), fes::darwin::Wave::kLongPeriod);
        break;
      case fes::darwin::kMf2:
        EXPECT_EQ(item->ident(), fes::darwin::kMf2);
        EXPECT_NEAR(item->freq(), detail::math::radians(1.093391230), 1e-8);
        EXPECT_EQ(item->type(), fes::darwin::Wave::kLongPeriod);
        break;
      case fes::darwin::kM0:
        EXPECT_EQ(item->ident(), fes::darwin::kM0);
        EXPECT_NEAR(item->freq(), 0, 1e-8);
        EXPECT_EQ(item->type(), fes::darwin::Wave::kLongPeriod);
        break;
      case fes::darwin::kL2P:
        EXPECT_EQ(item->ident(), fes::darwin::kL2P);
        EXPECT_NEAR(item->freq(), detail::math::radians(29.5331207461), 1e-8);
        EXPECT_EQ(item->type(), fes::darwin::Wave::kShortPeriod);
        break;
      case fes::darwin::kN2P:
        EXPECT_EQ(item->ident(), fes::darwin::kN2P);
        EXPECT_NEAR(item->freq(), detail::math::radians(28.435087682), 1e-8);
        EXPECT_EQ(item->type(), fes::darwin::Wave::kShortPeriod);
        break;
      case fes::darwin::kMSK2:
        EXPECT_EQ(item->ident(), fes::darwin::kMSK2);
        EXPECT_NEAR(item->freq(), detail::math::radians(28.9019669359), 1e-8);
        EXPECT_EQ(item->type(), fes::darwin::Wave::kShortPeriod);
        break;
      case fes::darwin::kSKM2:
        EXPECT_EQ(item->ident(), fes::darwin::kSKM2);
        EXPECT_NEAR(item->freq(), detail::math::radians(31.0980330641), 1e-8);
        EXPECT_EQ(item->type(), fes::darwin::Wave::kShortPeriod);
        break;
      case fes::darwin::kOQ2:
        EXPECT_EQ(item->ident(), fes::darwin::kOQ2);
        EXPECT_NEAR(item->freq(), detail::math::radians(27.3416964522), 1e-8);
        EXPECT_EQ(item->type(), fes::darwin::Wave::kShortPeriod);
        break;
      case fes::darwin::k3MS4:
        EXPECT_EQ(item->ident(), fes::darwin::k3MS4);
        EXPECT_NEAR(item->freq(), detail::math::radians(56.952312642), 1e-8);
        EXPECT_EQ(item->type(), fes::darwin::Wave::kShortPeriod);
        break;
      case fes::darwin::kMNu4:
        EXPECT_EQ(item->ident(), fes::darwin::kMNu4);
        EXPECT_NEAR(item->freq(), detail::math::radians(57.4966873397), 1e-8);
        EXPECT_EQ(item->type(), fes::darwin::Wave::kShortPeriod);
        break;
      case fes::darwin::k2MSN4:
        EXPECT_EQ(item->ident(), fes::darwin::k2MSN4);
        EXPECT_NEAR(item->freq(), detail::math::radians(59.5284789117), 1e-8);
        EXPECT_EQ(item->type(), fes::darwin::Wave::kShortPeriod);
        break;
      case fes::darwin::k2NS2:
        EXPECT_EQ(item->ident(), fes::darwin::k2NS2);
        EXPECT_NEAR(item->freq(), detail::math::radians(26.8794590326), 1e-8);
        EXPECT_EQ(item->type(), fes::darwin::Wave::kShortPeriod);
        break;
      case fes::darwin::kMNuS2:
        EXPECT_EQ(item->ident(), fes::darwin::kMNuS2);
        EXPECT_NEAR(item->freq(), detail::math::radians(27.4966873397), 1e-8);
        EXPECT_EQ(item->type(), fes::darwin::Wave::kShortPeriod);
        break;
      case fes::darwin::k2MK2:
        EXPECT_EQ(item->ident(), fes::darwin::k2MK2);
        EXPECT_NEAR(item->freq(), detail::math::radians(27.8860711499), 1e-8);
        EXPECT_EQ(item->type(), fes::darwin::Wave::kShortPeriod);
        break;
      case fes::darwin::kNKM2:
        EXPECT_EQ(item->ident(), fes::darwin::kNKM2);
        EXPECT_NEAR(item->freq(), detail::math::radians(29.5377625804), 1e-8);
        EXPECT_EQ(item->type(), fes::darwin::Wave::kShortPeriod);
        break;
      case fes::darwin::kML4:
        EXPECT_EQ(item->ident(), fes::darwin::kML4);
        EXPECT_NEAR(item->freq(), detail::math::radians(58.5125831257), 1e-8);
        EXPECT_EQ(item->type(), fes::darwin::Wave::kShortPeriod);
        break;
      case fes::darwin::kSO1:
        EXPECT_EQ(item->ident(), fes::darwin::kSO1);
        EXPECT_NEAR(item->freq(), detail::math::radians(16.0569644251), 1e-8);
        EXPECT_EQ(item->type(), fes::darwin::Wave::kShortPeriod);
        break;
      case fes::darwin::kSO3:
        EXPECT_EQ(item->ident(), fes::darwin::kSO3);
        EXPECT_NEAR(item->freq(), detail::math::radians(43.9430355749), 1e-8);
        EXPECT_EQ(item->type(), fes::darwin::Wave::kShortPeriod);
        break;
      case fes::darwin::kNK4:
        EXPECT_EQ(item->ident(), fes::darwin::kNK4);
        EXPECT_NEAR(item->freq(), detail::math::radians(58.5218667944), 1e-8);
        EXPECT_EQ(item->type(), fes::darwin::Wave::kShortPeriod);
        break;
      case fes::darwin::kMNK6:
        EXPECT_EQ(item->ident(), fes::darwin::kMNK6);
        EXPECT_NEAR(item->freq(), detail::math::radians(87.5059710084), 1e-8);
        EXPECT_EQ(item->type(), fes::darwin::Wave::kShortPeriod);
        break;
      case fes::darwin::k2NM6:
        EXPECT_EQ(item->ident(), fes::darwin::k2NM6);
        EXPECT_NEAR(item->freq(), detail::math::radians(85.8635632466), 1e-8);
        EXPECT_EQ(item->type(), fes::darwin::Wave::kShortPeriod);
        break;
      case fes::darwin::k3MS8:
        EXPECT_EQ(item->ident(), fes::darwin::k3MS8);
        EXPECT_NEAR(item->freq(), detail::math::radians(116.952312642), 1e-8);
        EXPECT_EQ(item->type(), fes::darwin::Wave::kShortPeriod);
        break;
      case fes::darwin::kSK3:
        EXPECT_EQ(item->ident(), fes::darwin::kSK3);
        EXPECT_NEAR(item->freq(), detail::math::radians(45.0410686391), 1e-8);
        EXPECT_EQ(item->type(), fes::darwin::Wave::kShortPeriod);
        break;
      case fes::darwin::k2MNS4:
        EXPECT_EQ(item->ident(), fes::darwin::k2MNS4);
        EXPECT_NEAR(item->freq(), detail::math::radians(56.4079379443), 1e-8);
        EXPECT_EQ(item->type(), fes::darwin::Wave::kShortPeriod);
        break;
      case fes::darwin::k2SMu2:
        EXPECT_EQ(item->ident(), fes::darwin::k2SMu2);
        EXPECT_NEAR(item->freq(), detail::math::radians(32.031791572), 1e-8);
        EXPECT_EQ(item->type(), fes::darwin::Wave::kShortPeriod);
        break;
      case fes::darwin::k2MP5:
        EXPECT_EQ(item->ident(), fes::darwin::k2MP5);
        EXPECT_NEAR(item->freq(), detail::math::radians(72.927139789), 1e-8);
        EXPECT_EQ(item->type(), fes::darwin::Wave::kShortPeriod);
        break;
      default:
        throw std::runtime_error(std::to_string(item->ident()));
    }
  }
}
// NOLINTEND(readability-function-cognitive-complexity,*-function-size)

TEST(WaveTable, Constructor) {
  check_wave_table_constructor(fes::darwin::wave::Table());
}

static void admittance(const double r, const fes::darwin::Constituent ident,
                       fes::darwin::wave::Table& table) {
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
  auto table = fes::darwin::wave::Table();
  for (auto&& item : table) {
    item->tide({1, 1});
  }

  admittance(0.2378, fes::darwin::k2Q1, table);
  admittance(0.2706, fes::darwin::kSigma1, table);
  admittance(0.1688, fes::darwin::kRho1, table);
  admittance(0.0241, fes::darwin::kM11, table);
  admittance(0.0671, fes::darwin::kM12, table);
  admittance(0.0124, fes::darwin::kChi1, table);
  admittance(0.0201, fes::darwin::kPi1, table);
  admittance(0.0137, fes::darwin::kPhi1, table);
  admittance(0.009, fes::darwin::kTheta1, table);
  admittance(0.0447, fes::darwin::kJ1, table);
  admittance(0.0182, fes::darwin::kOO1, table);
  admittance(0.0796782, fes::darwin::kEta2, table);
  admittance(0.374697218357, fes::darwin::kMu2, table);
  admittance(0.157529811402, fes::darwin::kNu2, table);
  admittance(0.010949128375, fes::darwin::kLambda2, table);
  admittance(0.053354227357, fes::darwin::kL2, table);
  admittance(0.16871051505, fes::darwin::kT2, table);
  admittance(0.2387, fes::darwin::k2N2, table);
  admittance(0.094151295, fes::darwin::kEps2, table);
  // force 2N2 to be calculated in dynamically
  table[fes::darwin::k2N2]->admittance(false);
  table[fes::darwin::k2N2]->tide({1, 1});
  admittance(0.499810, fes::darwin::kEps2, table);
}

TEST(WaveTable, IdentName) {
  for (auto&& item : fes::darwin::wave::Table()) {
    const auto* name = item->name();
    auto ident = fes::darwin::constituents::parse(name);
    EXPECT_EQ(item->ident(), ident);
  }
}

TEST(WaveTable, Sparse) {
  auto table = fes::darwin::wave::Table(
      {"O1", "K1", "M2", "S2", "N2", "K2", "M4", "M6", "Mf2"});
  EXPECT_EQ(table.size(), 9);
  EXPECT_EQ(table[fes::darwin::kO1]->ident(), fes::darwin::kO1);
  EXPECT_EQ(table[0]->ident(), fes::darwin::kO1);
  EXPECT_EQ(table[1]->ident(), fes::darwin::kK1);
  EXPECT_EQ(table[2]->ident(), fes::darwin::kN2);
  EXPECT_EQ(table[3]->ident(), fes::darwin::kM2);
  EXPECT_EQ(table[4]->ident(), fes::darwin::kS2);
  EXPECT_EQ(table[5]->ident(), fes::darwin::kK2);
  EXPECT_EQ(table[6]->ident(), fes::darwin::kM4);
  EXPECT_EQ(table[7]->ident(), fes::darwin::kM6);
  EXPECT_EQ(table[8]->ident(), fes::darwin::kMf2);
  EXPECT_THROW(table[fes::darwin::kP1], std::out_of_range);
  EXPECT_THROW(table.admittance(), std::out_of_range);
  EXPECT_THROW(
      {
        auto wave = table.find("_o1");
        wave->ident();
      },
      std::invalid_argument);
  EXPECT_EQ(table.find("Mf"), nullptr);
  EXPECT_EQ(table.find("O1")->ident(), fes::darwin::kO1);
}
