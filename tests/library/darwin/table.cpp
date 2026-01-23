// Copyright (c) 2025 CNES
//
// All rights reserved. Use of this source code is governed by a
// BSD-style license that can be found in the LICENSE file.
#include "fes/darwin/table.hpp"

#include <gtest/gtest.h>

namespace fes {
namespace darwin {

// NOLINTBEGIN(readability-function-cognitive-complexity,*-function-size)
// Used to test all wave components in a single function
inline auto check_wave_table_constructor(const WaveTable& table) -> void {
  for (auto&& item : table) {
    switch (item->ident()) {
      case kO1:
        EXPECT_EQ(item->ident(), kO1);
        EXPECT_NEAR(item->freq(), detail::math::radians(13.9430356), 1e-8);
        EXPECT_EQ(item->type(), Wave::kShortPeriod);
        break;
      case kP1:
        EXPECT_EQ(item->ident(), kP1);
        EXPECT_NEAR(item->freq(), detail::math::radians(14.9589314), 1e-8);
        EXPECT_EQ(item->type(), Wave::kShortPeriod);
        break;
      case kK1:
        EXPECT_EQ(item->ident(), kK1);
        EXPECT_NEAR(item->freq(), detail::math::radians(15.0410686), 1e-8);
        EXPECT_EQ(item->type(), Wave::kShortPeriod);
        break;
      case k2N2:
        EXPECT_EQ(item->ident(), k2N2);
        EXPECT_NEAR(item->freq(), detail::math::radians(27.8953548), 1e-8);
        EXPECT_EQ(item->type(), Wave::kShortPeriod);
        break;
      case kMu2:
        EXPECT_EQ(item->ident(), kMu2);
        EXPECT_NEAR(item->freq(), detail::math::radians(27.9682084), 1e-8);
        EXPECT_EQ(item->type(), Wave::kShortPeriod);
        break;
      case kN2:
        EXPECT_EQ(item->ident(), kN2);
        EXPECT_NEAR(item->freq(), detail::math::radians(28.4397295), 1e-8);
        EXPECT_EQ(item->type(), Wave::kShortPeriod);
        break;
      case kNu2:
        EXPECT_EQ(item->ident(), kNu2);
        EXPECT_NEAR(item->freq(), detail::math::radians(28.5125831), 1e-8);
        EXPECT_EQ(item->type(), Wave::kShortPeriod);
        break;
      case kM2:
        EXPECT_EQ(item->ident(), kM2);
        EXPECT_NEAR(item->freq(), detail::math::radians(28.9841042), 1e-8);
        EXPECT_EQ(item->type(), Wave::kShortPeriod);
        break;
      case kL2:
        EXPECT_EQ(item->ident(), kL2);
        EXPECT_NEAR(item->freq(), detail::math::radians(29.5284789), 1e-8);
        EXPECT_EQ(item->type(), Wave::kShortPeriod);
        break;
      case kT2:
        EXPECT_EQ(item->ident(), kT2);
        EXPECT_NEAR(item->freq(), detail::math::radians(29.9589333), 1e-8);
        EXPECT_EQ(item->type(), Wave::kShortPeriod);
        break;
      case kS2:
        EXPECT_EQ(item->ident(), kS2);
        EXPECT_NEAR(item->freq(), detail::math::radians(30.0), 1e-8);
        EXPECT_EQ(item->type(), Wave::kShortPeriod);
        break;
      case kK2:
        EXPECT_EQ(item->ident(), kK2);
        EXPECT_NEAR(item->freq(), detail::math::radians(30.0821373), 1e-8);
        EXPECT_EQ(item->type(), Wave::kShortPeriod);
        break;
      case kM4:
        EXPECT_EQ(item->ident(), kM4);
        EXPECT_NEAR(item->freq(), detail::math::radians(57.9682084), 1e-8);
        EXPECT_EQ(item->type(), Wave::kShortPeriod);
        break;
      case kS1:
        EXPECT_EQ(item->ident(), kS1);
        EXPECT_NEAR(item->freq(), detail::math::radians(15.0), 1e-8);
        EXPECT_EQ(item->type(), Wave::kShortPeriod);
        break;
      case kQ1:
        EXPECT_EQ(item->ident(), kQ1);
        EXPECT_NEAR(item->freq(), detail::math::radians(13.3986609), 1e-8);
        EXPECT_EQ(item->type(), Wave::kShortPeriod);
        break;
      case kMm:
        EXPECT_EQ(item->ident(), kMm);
        EXPECT_NEAR(item->freq(), detail::math::radians(0.5443747), 1e-8);
        EXPECT_EQ(item->type(), Wave::kLongPeriod);
        break;
      case kMf:
        EXPECT_EQ(item->ident(), kMf);
        EXPECT_NEAR(item->freq(), detail::math::radians(1.0980331), 1e-8);
        EXPECT_EQ(item->type(), Wave::kLongPeriod);
        break;
      case kMtm:
        EXPECT_EQ(item->ident(), kMtm);
        EXPECT_NEAR(item->freq(), detail::math::radians(1.642407762), 1e-8);
        EXPECT_EQ(item->type(), Wave::kLongPeriod);
        break;
      case kMSqm:
        EXPECT_EQ(item->ident(), kMSqm);
        EXPECT_NEAR(item->freq(), detail::math::radians(2.113928850), 1e-8);
        EXPECT_EQ(item->type(), Wave::kLongPeriod);
        break;
      case kEps2:
        EXPECT_EQ(item->ident(), kEps2);
        EXPECT_NEAR(item->freq(), detail::math::radians(27.423833730), 1e-8);
        EXPECT_EQ(item->type(), Wave::kShortPeriod);
        break;
      case kLambda2:
        EXPECT_EQ(item->ident(), kLambda2);
        EXPECT_NEAR(item->freq(), detail::math::radians(29.4556253), 1e-8);
        EXPECT_EQ(item->type(), Wave::kShortPeriod);
        break;
      case kEta2:
        EXPECT_EQ(item->ident(), kEta2);
        EXPECT_NEAR(item->freq(), detail::math::radians(30.6265120), 1e-8);
        EXPECT_EQ(item->type(), Wave::kShortPeriod);
        break;
      case k2Q1:
        EXPECT_EQ(item->ident(), k2Q1);
        EXPECT_NEAR(item->freq(), detail::math::radians(12.8542862), 1e-8);
        EXPECT_EQ(item->type(), Wave::kShortPeriod);
        break;
      case kSigma1:
        EXPECT_EQ(item->ident(), kSigma1);
        EXPECT_NEAR(item->freq(), detail::math::radians(12.9271398), 1e-8);
        EXPECT_EQ(item->type(), Wave::kShortPeriod);
        break;
      case kRho1:
        EXPECT_EQ(item->ident(), kRho1);
        EXPECT_NEAR(item->freq(), detail::math::radians(13.4715145), 1e-8);
        EXPECT_EQ(item->type(), Wave::kShortPeriod);
        break;
      case kM1:
        EXPECT_EQ(item->ident(), kM1);
        EXPECT_NEAR(item->freq(), detail::math::radians(14.49669394), 1e-8);
        EXPECT_EQ(item->type(), Wave::kShortPeriod);
        break;
      case kM11:
        EXPECT_EQ(item->ident(), kM11);
        EXPECT_NEAR(item->freq(), detail::math::radians(14.48741027), 1e-8);
        EXPECT_EQ(item->type(), Wave::kShortPeriod);
        break;
      case kM12:
        EXPECT_EQ(item->ident(), kM12);
        EXPECT_NEAR(item->freq(), detail::math::radians(14.4966939), 1e-8);
        EXPECT_EQ(item->type(), Wave::kShortPeriod);
        break;
      case kM13:
        EXPECT_EQ(item->ident(), kM13);
        EXPECT_NEAR(item->freq(), detail::math::radians(14.49205210), 1e-8);
        EXPECT_EQ(item->type(), Wave::kShortPeriod);
        break;
      case kChi1:
        EXPECT_EQ(item->ident(), kChi1);
        EXPECT_NEAR(item->freq(), detail::math::radians(14.5695476), 1e-8);
        EXPECT_EQ(item->type(), Wave::kShortPeriod);
        break;
      case kPi1:
        EXPECT_EQ(item->ident(), kPi1);
        EXPECT_NEAR(item->freq(), detail::math::radians(14.9178647), 1e-8);
        EXPECT_EQ(item->type(), Wave::kShortPeriod);
        break;
      case kPhi1:
        EXPECT_EQ(item->ident(), kPhi1);
        EXPECT_NEAR(item->freq(), detail::math::radians(15.1232059), 1e-8);
        EXPECT_EQ(item->type(), Wave::kShortPeriod);
        break;
      case kTheta1:
        EXPECT_EQ(item->ident(), kTheta1);
        EXPECT_NEAR(item->freq(), detail::math::radians(15.5125897), 1e-8);
        EXPECT_EQ(item->type(), Wave::kShortPeriod);
        break;
      case kJ1:
        EXPECT_EQ(item->ident(), kJ1);
        EXPECT_NEAR(item->freq(), detail::math::radians(15.5854433), 1e-8);
        EXPECT_EQ(item->type(), Wave::kShortPeriod);
        break;
      case kOO1:
        EXPECT_EQ(item->ident(), kOO1);
        EXPECT_NEAR(item->freq(), detail::math::radians(16.1391017), 1e-8);
        EXPECT_EQ(item->type(), Wave::kShortPeriod);
        break;
      case kM3:
        EXPECT_EQ(item->ident(), kM3);
        EXPECT_NEAR(item->freq(), detail::math::radians(43.4761563), 1e-8);
        EXPECT_EQ(item->type(), Wave::kShortPeriod);
        break;
      case kM6:
        EXPECT_EQ(item->ident(), kM6);
        EXPECT_NEAR(item->freq(), detail::math::radians(86.9523126), 1e-8);
        EXPECT_EQ(item->type(), Wave::kShortPeriod);
        break;
      case kMN4:
        EXPECT_EQ(item->ident(), kMN4);
        EXPECT_NEAR(item->freq(), detail::math::radians(57.4238337), 1e-8);
        EXPECT_EQ(item->type(), Wave::kShortPeriod);
        break;
      case kMS4:
        EXPECT_EQ(item->ident(), kMS4);
        EXPECT_NEAR(item->freq(), detail::math::radians(58.9841042), 1e-8);
        EXPECT_EQ(item->type(), Wave::kShortPeriod);
        break;
      case kN4:
        EXPECT_EQ(item->ident(), kN4);
        EXPECT_NEAR(item->freq(), detail::math::radians(56.8794590), 1e-8);
        EXPECT_EQ(item->type(), Wave::kShortPeriod);
        break;
      case kR2:
        EXPECT_EQ(item->ident(), kR2);
        EXPECT_NEAR(item->freq(), detail::math::radians(30.0410667), 1e-8);
        EXPECT_EQ(item->type(), Wave::kShortPeriod);
        break;
      case kR4:
        EXPECT_EQ(item->ident(), kR4);
        EXPECT_NEAR(item->freq(), detail::math::radians(30.0410667 * 2), 1e-8);
        EXPECT_EQ(item->type(), Wave::kShortPeriod);
        break;
      case kS4:
        EXPECT_EQ(item->ident(), kS4);
        EXPECT_NEAR(item->freq(), detail::math::radians(60.0), 1e-8);
        EXPECT_EQ(item->type(), Wave::kShortPeriod);
        break;
      case kMNS2:
        EXPECT_EQ(item->ident(), kMNS2);
        EXPECT_NEAR(item->freq(), detail::math::radians(27.4238337), 1e-8);
        EXPECT_EQ(item->type(), Wave::kShortPeriod);
        break;
      case kMK4:
        EXPECT_EQ(item->ident(), kMK4);
        EXPECT_NEAR(item->freq(), detail::math::radians(59.0662415), 1e-8);
        EXPECT_EQ(item->type(), Wave::kShortPeriod);
        break;
      case kSN4:
        EXPECT_EQ(item->ident(), kSN4);
        EXPECT_NEAR(item->freq(), detail::math::radians(58.4397295), 1e-8);
        EXPECT_EQ(item->type(), Wave::kShortPeriod);
        break;
      case kSK4:
        EXPECT_EQ(item->ident(), kSK4);
        EXPECT_NEAR(item->freq(), detail::math::radians(60.0821373), 1e-8);
        EXPECT_EQ(item->type(), Wave::kShortPeriod);
        break;
      case k2MN6:
        EXPECT_EQ(item->ident(), k2MN6);
        EXPECT_NEAR(item->freq(), detail::math::radians(86.4079380), 1e-8);
        EXPECT_EQ(item->type(), Wave::kShortPeriod);
        break;
      case k2MS6:
        EXPECT_EQ(item->ident(), k2MS6);
        EXPECT_NEAR(item->freq(), detail::math::radians(87.9682084), 1e-8);
        EXPECT_EQ(item->type(), Wave::kShortPeriod);
        break;
      case k2MK6:
        EXPECT_EQ(item->ident(), k2MK6);
        EXPECT_NEAR(item->freq(), detail::math::radians(88.0503457), 1e-8);
        EXPECT_EQ(item->type(), Wave::kShortPeriod);
        break;
      case kMSN6:
        EXPECT_EQ(item->ident(), kMSN6);
        EXPECT_NEAR(item->freq(), detail::math::radians(87.4238337), 1e-8);
        EXPECT_EQ(item->type(), Wave::kShortPeriod);
        break;
      case k2SM6:
        EXPECT_EQ(item->ident(), k2SM6);
        EXPECT_NEAR(item->freq(), detail::math::radians(88.9841042), 1e-8);
        EXPECT_EQ(item->type(), Wave::kShortPeriod);
        break;
      case kMSK6:
        EXPECT_EQ(item->ident(), kMSK6);
        EXPECT_NEAR(item->freq(), detail::math::radians(89.0662415), 1e-8);
        EXPECT_EQ(item->type(), Wave::kShortPeriod);
        break;
      case kMP1:
        EXPECT_EQ(item->ident(), kMP1);
        EXPECT_NEAR(item->freq(), detail::math::radians(14.0251729), 1e-8);
        EXPECT_EQ(item->type(), Wave::kShortPeriod);
        break;
      case k2SM2:
        EXPECT_EQ(item->ident(), k2SM2);
        EXPECT_NEAR(item->freq(), detail::math::radians(31.0158958), 1e-8);
        EXPECT_EQ(item->type(), Wave::kShortPeriod);
        break;
      case kPsi1:
        EXPECT_EQ(item->ident(), kPsi1);
        EXPECT_NEAR(item->freq(), detail::math::radians(15.0821353), 1e-8);
        EXPECT_EQ(item->type(), Wave::kShortPeriod);
        break;
      case k2MS2:
        EXPECT_EQ(item->ident(), k2MS2);
        EXPECT_NEAR(item->freq(), detail::math::radians(27.9682084), 1e-8);
        EXPECT_EQ(item->type(), Wave::kShortPeriod);
        break;
      case kMKS2:
        EXPECT_EQ(item->ident(), kMKS2);
        EXPECT_NEAR(item->freq(), detail::math::radians(29.0662415), 1e-8);
        EXPECT_EQ(item->type(), Wave::kShortPeriod);
        break;
      case k2MN2:
        EXPECT_EQ(item->ident(), k2MN2);
        EXPECT_NEAR(item->freq(), detail::math::radians(29.5284789), 1e-8);
        EXPECT_EQ(item->type(), Wave::kShortPeriod);
        break;
      case kMSN2:
        EXPECT_EQ(item->ident(), kMSN2);
        EXPECT_NEAR(item->freq(), detail::math::radians(30.5443747), 1e-8);
        EXPECT_EQ(item->type(), Wave::kShortPeriod);
        break;
      case kMO3:
        EXPECT_EQ(item->ident(), kMO3);
        EXPECT_NEAR(item->freq(), detail::math::radians(42.9271398), 1e-8);
        EXPECT_EQ(item->type(), Wave::kShortPeriod);
        break;
      case k2MK3:
        EXPECT_EQ(item->ident(), k2MK3);
        EXPECT_NEAR(item->freq(), detail::math::radians(42.9271398), 1e-8);
        EXPECT_EQ(item->type(), Wave::kShortPeriod);
        break;
      case kMK3:
        EXPECT_EQ(item->ident(), kMK3);
        EXPECT_NEAR(item->freq(), detail::math::radians(44.0251729), 1e-8);
        EXPECT_EQ(item->type(), Wave::kShortPeriod);
        break;
      case kS6:
        EXPECT_EQ(item->ident(), kS6);
        EXPECT_NEAR(item->freq(), detail::math::radians(90.0), 1e-8);
        EXPECT_EQ(item->type(), Wave::kShortPeriod);
        break;
      case kM8:
        EXPECT_EQ(item->ident(), kM8);
        EXPECT_NEAR(item->freq(), detail::math::radians(115.9364169), 1e-8);
        EXPECT_EQ(item->type(), Wave::kShortPeriod);
        break;
      case kMSf:
        EXPECT_EQ(item->ident(), kMSf);
        EXPECT_NEAR(item->freq(), detail::math::radians(1.01589578), 1e-8);
        EXPECT_EQ(item->type(), Wave::kLongPeriod);
        break;
      case kSsa:
        EXPECT_EQ(item->ident(), kSsa);
        EXPECT_NEAR(item->freq(), detail::math::radians(0.08213728), 1e-8);
        EXPECT_EQ(item->type(), Wave::kLongPeriod);
        break;
      case kSa:
        EXPECT_EQ(item->ident(), kSa);
        EXPECT_NEAR(item->freq(), detail::math::radians(0.04106864), 1e-8);
        EXPECT_EQ(item->type(), Wave::kLongPeriod);
        break;
      case kA5:
        EXPECT_EQ(item->ident(), kA5);
        EXPECT_NEAR(item->freq(), detail::math::radians(1.01589579), 1e-8);
        EXPECT_EQ(item->type(), Wave::kLongPeriod);
        break;
      case kSa1:
        EXPECT_EQ(item->ident(), kSa1);
        EXPECT_NEAR(item->freq(), detail::math::radians(0.041066678), 1e-8);
        EXPECT_EQ(item->type(), Wave::kLongPeriod);
        break;
      case kSta:
        EXPECT_EQ(item->ident(), kSta);
        EXPECT_NEAR(item->freq(), detail::math::radians(0.123203956), 1e-8);
        EXPECT_EQ(item->type(), Wave::kLongPeriod);
        break;
      case kMm1:
        EXPECT_EQ(item->ident(), kMm1);
        EXPECT_NEAR(item->freq(), detail::math::radians(0.553658366), 1e-8);
        EXPECT_EQ(item->type(), Wave::kLongPeriod);
        break;
      case kMm2:
        EXPECT_EQ(item->ident(), kMm2);
        EXPECT_NEAR(item->freq(), detail::math::radians(0.549016532), 1e-8);
        EXPECT_EQ(item->type(), Wave::kLongPeriod);
        break;
      case kMf1:
        EXPECT_EQ(item->ident(), kMf1);
        EXPECT_NEAR(item->freq(), detail::math::radians(1.088749395), 1e-8);
        EXPECT_EQ(item->type(), Wave::kLongPeriod);
        break;
      case kMf2:
        EXPECT_EQ(item->ident(), kMf2);
        EXPECT_NEAR(item->freq(), detail::math::radians(1.093391230), 1e-8);
        EXPECT_EQ(item->type(), Wave::kLongPeriod);
        break;
      case kM0:
        EXPECT_EQ(item->ident(), kM0);
        EXPECT_NEAR(item->freq(), 0, 1e-8);
        EXPECT_EQ(item->type(), Wave::kLongPeriod);
        break;
      case kL2P:
        EXPECT_EQ(item->ident(), kL2P);
        EXPECT_NEAR(item->freq(), detail::math::radians(29.5331207461), 1e-8);
        EXPECT_EQ(item->type(), Wave::kShortPeriod);
        break;
      case kN2P:
        EXPECT_EQ(item->ident(), kN2P);
        EXPECT_NEAR(item->freq(), detail::math::radians(28.435087682), 1e-8);
        EXPECT_EQ(item->type(), Wave::kShortPeriod);
        break;
      case kMSK2:
        EXPECT_EQ(item->ident(), kMSK2);
        EXPECT_NEAR(item->freq(), detail::math::radians(28.9019669359), 1e-8);
        EXPECT_EQ(item->type(), Wave::kShortPeriod);
        break;
      case kSKM2:
        EXPECT_EQ(item->ident(), kSKM2);
        EXPECT_NEAR(item->freq(), detail::math::radians(31.0980330641), 1e-8);
        EXPECT_EQ(item->type(), Wave::kShortPeriod);
        break;
      case kOQ2:
        EXPECT_EQ(item->ident(), kOQ2);
        EXPECT_NEAR(item->freq(), detail::math::radians(27.3416964522), 1e-8);
        EXPECT_EQ(item->type(), Wave::kShortPeriod);
        break;
      case k3MS4:
        EXPECT_EQ(item->ident(), k3MS4);
        EXPECT_NEAR(item->freq(), detail::math::radians(56.952312642), 1e-8);
        EXPECT_EQ(item->type(), Wave::kShortPeriod);
        break;
      case kMNu4:
        EXPECT_EQ(item->ident(), kMNu4);
        EXPECT_NEAR(item->freq(), detail::math::radians(57.4966873397), 1e-8);
        EXPECT_EQ(item->type(), Wave::kShortPeriod);
        break;
      case k2MSN4:
        EXPECT_EQ(item->ident(), k2MSN4);
        EXPECT_NEAR(item->freq(), detail::math::radians(59.5284789117), 1e-8);
        EXPECT_EQ(item->type(), Wave::kShortPeriod);
        break;
      case k2NS2:
        EXPECT_EQ(item->ident(), k2NS2);
        EXPECT_NEAR(item->freq(), detail::math::radians(26.8794590326), 1e-8);
        EXPECT_EQ(item->type(), Wave::kShortPeriod);
        break;
      case kMNuS2:
        EXPECT_EQ(item->ident(), kMNuS2);
        EXPECT_NEAR(item->freq(), detail::math::radians(27.4966873397), 1e-8);
        EXPECT_EQ(item->type(), Wave::kShortPeriod);
        break;
      case k2MK2:
        EXPECT_EQ(item->ident(), k2MK2);
        EXPECT_NEAR(item->freq(), detail::math::radians(27.8860711499), 1e-8);
        EXPECT_EQ(item->type(), Wave::kShortPeriod);
        break;
      case kNKM2:
        EXPECT_EQ(item->ident(), kNKM2);
        EXPECT_NEAR(item->freq(), detail::math::radians(29.5377625804), 1e-8);
        EXPECT_EQ(item->type(), Wave::kShortPeriod);
        break;
      case kML4:
        EXPECT_EQ(item->ident(), kML4);
        EXPECT_NEAR(item->freq(), detail::math::radians(58.5125831257), 1e-8);
        EXPECT_EQ(item->type(), Wave::kShortPeriod);
        break;
      case kSO1:
        EXPECT_EQ(item->ident(), kSO1);
        EXPECT_NEAR(item->freq(), detail::math::radians(16.0569644251), 1e-8);
        EXPECT_EQ(item->type(), Wave::kShortPeriod);
        break;
      case kSO3:
        EXPECT_EQ(item->ident(), kSO3);
        EXPECT_NEAR(item->freq(), detail::math::radians(43.9430355749), 1e-8);
        EXPECT_EQ(item->type(), Wave::kShortPeriod);
        break;
      case kNK4:
        EXPECT_EQ(item->ident(), kNK4);
        EXPECT_NEAR(item->freq(), detail::math::radians(58.5218667944), 1e-8);
        EXPECT_EQ(item->type(), Wave::kShortPeriod);
        break;
      case kMNK6:
        EXPECT_EQ(item->ident(), kMNK6);
        EXPECT_NEAR(item->freq(), detail::math::radians(87.5059710084), 1e-8);
        EXPECT_EQ(item->type(), Wave::kShortPeriod);
        break;
      case k2NM6:
        EXPECT_EQ(item->ident(), k2NM6);
        EXPECT_NEAR(item->freq(), detail::math::radians(85.8635632466), 1e-8);
        EXPECT_EQ(item->type(), Wave::kShortPeriod);
        break;
      case k3MS8:
        EXPECT_EQ(item->ident(), k3MS8);
        EXPECT_NEAR(item->freq(), detail::math::radians(116.952312642), 1e-8);
        EXPECT_EQ(item->type(), Wave::kShortPeriod);
        break;
      case kSK3:
        EXPECT_EQ(item->ident(), kSK3);
        EXPECT_NEAR(item->freq(), detail::math::radians(45.0410686391), 1e-8);
        EXPECT_EQ(item->type(), Wave::kShortPeriod);
        break;
      case k2MNS4:
        EXPECT_EQ(item->ident(), k2MNS4);
        EXPECT_NEAR(item->freq(), detail::math::radians(56.4079379443), 1e-8);
        EXPECT_EQ(item->type(), Wave::kShortPeriod);
        break;
      case k2SMu2:
        EXPECT_EQ(item->ident(), k2SMu2);
        EXPECT_NEAR(item->freq(), detail::math::radians(32.031791572), 1e-8);
        EXPECT_EQ(item->type(), Wave::kShortPeriod);
        break;
      case k2MP5:
        EXPECT_EQ(item->ident(), k2MP5);
        EXPECT_NEAR(item->freq(), detail::math::radians(72.927139789), 1e-8);
        EXPECT_EQ(item->type(), Wave::kShortPeriod);
        break;
      default:
        throw std::runtime_error(std::to_string(item->ident()));
    }
  }
}
// NOLINTEND(readability-function-cognitive-complexity,*-function-size)

TEST(WaveTable, Constructor) { check_wave_table_constructor(WaveTable()); }

static void admittance(const double r, const Constituent ident,
                       WaveTable& table) {
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
  auto table = WaveTable();
  for (auto&& item : table) {
    item->tide({1, 1});
  }

  admittance(0.2378, k2Q1, table);
  admittance(0.2706, kSigma1, table);
  admittance(0.1688, kRho1, table);
  admittance(0.0241, kM11, table);
  admittance(0.0671, kM12, table);
  admittance(0.0124, kChi1, table);
  admittance(0.0201, kPi1, table);
  admittance(0.0137, kPhi1, table);
  admittance(0.009, kTheta1, table);
  admittance(0.0447, kJ1, table);
  admittance(0.0182, kOO1, table);
  admittance(0.0796782, kEta2, table);
  admittance(0.374697218357, kMu2, table);
  admittance(0.157529811402, kNu2, table);
  admittance(0.010949128375, kLambda2, table);
  admittance(0.053354227357, kL2, table);
  admittance(0.16871051505, kT2, table);
  admittance(0.2387, k2N2, table);
  admittance(0.094151295, kEps2, table);
  // force 2N2 to be calculated in dynamically
  table[k2N2]->admittance(false);
  table[k2N2]->tide({1, 1});
  admittance(0.499810, kEps2, table);
}

TEST(WaveTable, IdentName) {
  for (auto&& item : WaveTable()) {
    const auto* name = item->name();
    auto ident = constituents::parse(name);
    EXPECT_EQ(item->ident(), ident);
  }
}

TEST(WaveTable, Sparse) {
  auto table =
      WaveTable({"O1", "K1", "M2", "S2", "N2", "K2", "M4", "M6", "Mf2"});
  EXPECT_EQ(table.size(), 9);
  EXPECT_EQ(table[kO1]->ident(), kO1);
  EXPECT_EQ(table[0]->ident(), kO1);
  EXPECT_EQ(table[1]->ident(), kK1);
  EXPECT_EQ(table[2]->ident(), kN2);
  EXPECT_EQ(table[3]->ident(), kM2);
  EXPECT_EQ(table[4]->ident(), kS2);
  EXPECT_EQ(table[5]->ident(), kK2);
  EXPECT_EQ(table[6]->ident(), kM4);
  EXPECT_EQ(table[7]->ident(), kM6);
  EXPECT_EQ(table[8]->ident(), kMf2);
  EXPECT_THROW(table[kP1], std::out_of_range);
  EXPECT_THROW(table.admittance(), std::out_of_range);
  EXPECT_THROW(
      {
        auto wave = table.find("_o1");
        wave->ident();
      },
      std::invalid_argument);
  EXPECT_EQ(table.find("Mf"), nullptr);
  EXPECT_EQ(table.find("O1")->ident(), kO1);
}

}  // namespace darwin
}  // namespace fes
