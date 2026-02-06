// Copyright (c) 2026 CNES
//
// All rights reserved. Use of this source code is governed by a
// BSD-style license that can be found in the LICENSE file.
#include <gtest/gtest.h>

#include "fes/darwin/wave_table.hpp"

namespace fes {
namespace darwin {

// NOLINTBEGIN(readability-function-cognitive-complexity,*-function-size)
// Used to test all wave components in a single function
inline auto check_wave_table_constructor(const WaveTable& table) -> void {
  for (auto&& item : table) {
    const auto& wave = item.value();
    switch (wave->ident()) {
      case kO1:
        EXPECT_EQ(wave->ident(), kO1);
        EXPECT_NEAR(wave->frequency<kDegreePerHour>(), 13.9430356, 1e-7);
        EXPECT_EQ(wave->type(), kShortPeriod);
        break;
      case kP1:
        EXPECT_EQ(wave->ident(), kP1);
        EXPECT_NEAR(wave->frequency<kDegreePerHour>(), 14.9589314, 1e-7);
        EXPECT_EQ(wave->type(), kShortPeriod);
        break;
      case kK1:
        EXPECT_EQ(wave->ident(), kK1);
        EXPECT_NEAR(wave->frequency<kDegreePerHour>(), 15.0410686, 1e-7);
        EXPECT_EQ(wave->type(), kShortPeriod);
        break;
      case k2N2:
        EXPECT_EQ(wave->ident(), k2N2);
        EXPECT_NEAR(wave->frequency<kDegreePerHour>(), 27.8953548, 1e-7);
        EXPECT_EQ(wave->type(), kShortPeriod);
        break;
      case kMu2:
        EXPECT_EQ(wave->ident(), kMu2);
        EXPECT_NEAR(wave->frequency<kDegreePerHour>(), 27.9682084, 1e-7);
        EXPECT_EQ(wave->type(), kShortPeriod);
        break;
      case kN2:
        EXPECT_EQ(wave->ident(), kN2);
        EXPECT_NEAR(wave->frequency<kDegreePerHour>(), 28.4397295, 1e-7);
        EXPECT_EQ(wave->type(), kShortPeriod);
        break;
      case kNu2:
        EXPECT_EQ(wave->ident(), kNu2);
        EXPECT_NEAR(wave->frequency<kDegreePerHour>(), 28.5125831, 1e-7);
        EXPECT_EQ(wave->type(), kShortPeriod);
        break;
      case kM2:
        EXPECT_EQ(wave->ident(), kM2);
        EXPECT_NEAR(wave->frequency<kDegreePerHour>(), 28.9841042, 1e-7);
        EXPECT_EQ(wave->type(), kShortPeriod);
        break;
      case kL2:
        EXPECT_EQ(wave->ident(), kL2);
        EXPECT_NEAR(wave->frequency<kDegreePerHour>(), 29.5284789, 1e-7);
        EXPECT_EQ(wave->type(), kShortPeriod);
        break;
      case kT2:
        EXPECT_EQ(wave->ident(), kT2);
        EXPECT_NEAR(wave->frequency<kDegreePerHour>(), 29.9589333, 1e-7);
        EXPECT_EQ(wave->type(), kShortPeriod);
        break;
      case kS2:
        EXPECT_EQ(wave->ident(), kS2);
        EXPECT_NEAR(wave->frequency<kDegreePerHour>(), 30.0, 1e-7);
        EXPECT_EQ(wave->type(), kShortPeriod);
        break;
      case kK2:
        EXPECT_EQ(wave->ident(), kK2);
        EXPECT_NEAR(wave->frequency<kDegreePerHour>(), 30.0821373, 1e-7);
        EXPECT_EQ(wave->type(), kShortPeriod);
        break;
      case kM4:
        EXPECT_EQ(wave->ident(), kM4);
        EXPECT_NEAR(wave->frequency<kDegreePerHour>(), 57.9682084, 1e-7);
        EXPECT_EQ(wave->type(), kShortPeriod);
        break;
      case kS1:
        EXPECT_EQ(wave->ident(), kS1);
        EXPECT_NEAR(wave->frequency<kDegreePerHour>(), 15.0, 1e-7);
        EXPECT_EQ(wave->type(), kShortPeriod);
        break;
      case kQ1:
        EXPECT_EQ(wave->ident(), kQ1);
        EXPECT_NEAR(wave->frequency<kDegreePerHour>(), 13.3986609, 1e-7);
        EXPECT_EQ(wave->type(), kShortPeriod);
        break;
      case kMm:
        EXPECT_EQ(wave->ident(), kMm);
        EXPECT_NEAR(wave->frequency<kDegreePerHour>(), 0.5443747, 1e-7);
        EXPECT_EQ(wave->type(), kLongPeriod);
        break;
      case kMf:
        EXPECT_EQ(wave->ident(), kMf);
        EXPECT_NEAR(wave->frequency<kDegreePerHour>(), 1.0980331, 1e-7);
        EXPECT_EQ(wave->type(), kLongPeriod);
        break;
      case kMtm:
        EXPECT_EQ(wave->ident(), kMtm);
        EXPECT_NEAR(wave->frequency<kDegreePerHour>(), 1.642407762, 1e-7);
        EXPECT_EQ(wave->type(), kLongPeriod);
        break;
      case kMSqm:
        EXPECT_EQ(wave->ident(), kMSqm);
        EXPECT_NEAR(wave->frequency<kDegreePerHour>(), 2.113928850, 1e-7);
        EXPECT_EQ(wave->type(), kLongPeriod);
        break;
      case kEps2:
        EXPECT_EQ(wave->ident(), kEps2);
        EXPECT_NEAR(wave->frequency<kDegreePerHour>(), 27.423833730, 1e-7);
        EXPECT_EQ(wave->type(), kShortPeriod);
        break;
      case kLambda2:
        EXPECT_EQ(wave->ident(), kLambda2);
        EXPECT_NEAR(wave->frequency<kDegreePerHour>(), 29.4556253, 1e-7);
        EXPECT_EQ(wave->type(), kShortPeriod);
        break;
      case kEta2:
        EXPECT_EQ(wave->ident(), kEta2);
        EXPECT_NEAR(wave->frequency<kDegreePerHour>(), 30.6265120, 1e-7);
        EXPECT_EQ(wave->type(), kShortPeriod);
        break;
      case k2Q1:
        EXPECT_EQ(wave->ident(), k2Q1);
        EXPECT_NEAR(wave->frequency<kDegreePerHour>(), 12.8542862, 1e-7);
        EXPECT_EQ(wave->type(), kShortPeriod);
        break;
      case kSigma1:
        EXPECT_EQ(wave->ident(), kSigma1);
        EXPECT_NEAR(wave->frequency<kDegreePerHour>(), 12.9271398, 1e-7);
        EXPECT_EQ(wave->type(), kShortPeriod);
        break;
      case kRho1:
        EXPECT_EQ(wave->ident(), kRho1);
        EXPECT_NEAR(wave->frequency<kDegreePerHour>(), 13.4715145, 1e-7);
        EXPECT_EQ(wave->type(), kShortPeriod);
        break;
      case kM1:
        EXPECT_EQ(wave->ident(), kM1);
        EXPECT_NEAR(wave->frequency<kDegreePerHour>(), 14.49669394, 1e-7);
        EXPECT_EQ(wave->type(), kShortPeriod);
        break;
      case kM11:
        EXPECT_EQ(wave->ident(), kM11);
        EXPECT_NEAR(wave->frequency<kDegreePerHour>(), 14.48741027, 1e-7);
        EXPECT_EQ(wave->type(), kShortPeriod);
        break;
      case kM12:
        EXPECT_EQ(wave->ident(), kM12);
        EXPECT_NEAR(wave->frequency<kDegreePerHour>(), 14.4966939, 1e-7);
        EXPECT_EQ(wave->type(), kShortPeriod);
        break;
      case kM13:
        EXPECT_EQ(wave->ident(), kM13);
        EXPECT_NEAR(wave->frequency<kDegreePerHour>(), 14.49205210, 1e-7);
        EXPECT_EQ(wave->type(), kShortPeriod);
        break;
      case kChi1:
        EXPECT_EQ(wave->ident(), kChi1);
        EXPECT_NEAR(wave->frequency<kDegreePerHour>(), 14.5695476, 1e-7);
        EXPECT_EQ(wave->type(), kShortPeriod);
        break;
      case kPi1:
        EXPECT_EQ(wave->ident(), kPi1);
        EXPECT_NEAR(wave->frequency<kDegreePerHour>(), 14.9178647, 1e-7);
        EXPECT_EQ(wave->type(), kShortPeriod);
        break;
      case kPhi1:
        EXPECT_EQ(wave->ident(), kPhi1);
        EXPECT_NEAR(wave->frequency<kDegreePerHour>(), 15.1232059, 1e-7);
        EXPECT_EQ(wave->type(), kShortPeriod);
        break;
      case kTheta1:
        EXPECT_EQ(wave->ident(), kTheta1);
        EXPECT_NEAR(wave->frequency<kDegreePerHour>(), 15.5125897, 1e-7);
        EXPECT_EQ(wave->type(), kShortPeriod);
        break;
      case kJ1:
        EXPECT_EQ(wave->ident(), kJ1);
        EXPECT_NEAR(wave->frequency<kDegreePerHour>(), 15.5854433, 1e-7);
        EXPECT_EQ(wave->type(), kShortPeriod);
        break;
      case kOO1:
        EXPECT_EQ(wave->ident(), kOO1);
        EXPECT_NEAR(wave->frequency<kDegreePerHour>(), 16.1391017, 1e-7);
        EXPECT_EQ(wave->type(), kShortPeriod);
        break;
      case kM3:
        EXPECT_EQ(wave->ident(), kM3);
        EXPECT_NEAR(wave->frequency<kDegreePerHour>(), 43.4761563, 1e-7);
        EXPECT_EQ(wave->type(), kShortPeriod);
        break;
      case kM6:
        EXPECT_EQ(wave->ident(), kM6);
        EXPECT_NEAR(wave->frequency<kDegreePerHour>(), 86.9523127, 1e-7);
        EXPECT_EQ(wave->type(), kShortPeriod);
        break;
      case kMN4:
        EXPECT_EQ(wave->ident(), kMN4);
        EXPECT_NEAR(wave->frequency<kDegreePerHour>(), 57.4238337, 1e-7);
        EXPECT_EQ(wave->type(), kShortPeriod);
        break;
      case kMS4:
        EXPECT_EQ(wave->ident(), kMS4);
        EXPECT_NEAR(wave->frequency<kDegreePerHour>(), 58.9841042, 1e-7);
        EXPECT_EQ(wave->type(), kShortPeriod);
        break;
      case kN4:
        EXPECT_EQ(wave->ident(), kN4);
        EXPECT_NEAR(wave->frequency<kDegreePerHour>(), 56.8794590, 1e-7);
        EXPECT_EQ(wave->type(), kShortPeriod);
        break;
      case kR2:
        EXPECT_EQ(wave->ident(), kR2);
        EXPECT_NEAR(wave->frequency<kDegreePerHour>(), 30.0410667, 1e-7);
        EXPECT_EQ(wave->type(), kShortPeriod);
        break;
      case kR4:
        EXPECT_EQ(wave->ident(), kR4);
        EXPECT_NEAR(wave->frequency<kDegreePerHour>(), 30.0410667 * 2, 1e-7);
        EXPECT_EQ(wave->type(), kShortPeriod);
        break;
      case kS4:
        EXPECT_EQ(wave->ident(), kS4);
        EXPECT_NEAR(wave->frequency<kDegreePerHour>(), 60.0, 1e-7);
        EXPECT_EQ(wave->type(), kShortPeriod);
        break;
      case kMNS2:
        EXPECT_EQ(wave->ident(), kMNS2);
        EXPECT_NEAR(wave->frequency<kDegreePerHour>(), 27.4238337, 1e-7);
        EXPECT_EQ(wave->type(), kShortPeriod);
        break;
      case kMK4:
        EXPECT_EQ(wave->ident(), kMK4);
        EXPECT_NEAR(wave->frequency<kDegreePerHour>(), 59.0662415, 1e-7);
        EXPECT_EQ(wave->type(), kShortPeriod);
        break;
      case kSN4:
        EXPECT_EQ(wave->ident(), kSN4);
        EXPECT_NEAR(wave->frequency<kDegreePerHour>(), 58.4397295, 1e-7);
        EXPECT_EQ(wave->type(), kShortPeriod);
        break;
      case kSK4:
        EXPECT_EQ(wave->ident(), kSK4);
        EXPECT_NEAR(wave->frequency<kDegreePerHour>(), 60.0821373, 1e-7);
        EXPECT_EQ(wave->type(), kShortPeriod);
        break;
      case k2MN6:
        EXPECT_EQ(wave->ident(), k2MN6);
        EXPECT_NEAR(wave->frequency<kDegreePerHour>(), 86.4079380, 1e-7);
        EXPECT_EQ(wave->type(), kShortPeriod);
        break;
      case k2MS6:
        EXPECT_EQ(wave->ident(), k2MS6);
        EXPECT_NEAR(wave->frequency<kDegreePerHour>(), 87.9682084, 1e-7);
        EXPECT_EQ(wave->type(), kShortPeriod);
        break;
      case k2MK6:
        EXPECT_EQ(wave->ident(), k2MK6);
        EXPECT_NEAR(wave->frequency<kDegreePerHour>(), 88.0503457, 1e-7);
        EXPECT_EQ(wave->type(), kShortPeriod);
        break;
      case kMSN6:
        EXPECT_EQ(wave->ident(), kMSN6);
        EXPECT_NEAR(wave->frequency<kDegreePerHour>(), 87.4238337, 1e-7);
        EXPECT_EQ(wave->type(), kShortPeriod);
        break;
      case k2SM6:
        EXPECT_EQ(wave->ident(), k2SM6);
        EXPECT_NEAR(wave->frequency<kDegreePerHour>(), 88.9841042, 1e-7);
        EXPECT_EQ(wave->type(), kShortPeriod);
        break;
      case kMSK6:
        EXPECT_EQ(wave->ident(), kMSK6);
        EXPECT_NEAR(wave->frequency<kDegreePerHour>(), 89.0662415, 1e-7);
        EXPECT_EQ(wave->type(), kShortPeriod);
        break;
      case kMP1:
        EXPECT_EQ(wave->ident(), kMP1);
        EXPECT_NEAR(wave->frequency<kDegreePerHour>(), 14.0251729, 1e-7);
        EXPECT_EQ(wave->type(), kShortPeriod);
        break;
      case k2SM2:
        EXPECT_EQ(wave->ident(), k2SM2);
        EXPECT_NEAR(wave->frequency<kDegreePerHour>(), 31.0158958, 1e-7);
        EXPECT_EQ(wave->type(), kShortPeriod);
        break;
      case kPsi1:
        EXPECT_EQ(wave->ident(), kPsi1);
        EXPECT_NEAR(wave->frequency<kDegreePerHour>(), 15.0821353, 1e-7);
        EXPECT_EQ(wave->type(), kShortPeriod);
        break;
      case k2MS2:
        EXPECT_EQ(wave->ident(), k2MS2);
        EXPECT_NEAR(wave->frequency<kDegreePerHour>(), 27.9682084, 1e-7);
        EXPECT_EQ(wave->type(), kShortPeriod);
        break;
      case kMKS2:
        EXPECT_EQ(wave->ident(), kMKS2);
        EXPECT_NEAR(wave->frequency<kDegreePerHour>(), 29.0662415, 1e-7);
        EXPECT_EQ(wave->type(), kShortPeriod);
        break;
      case k2MN2:
        EXPECT_EQ(wave->ident(), k2MN2);
        EXPECT_NEAR(wave->frequency<kDegreePerHour>(), 29.5284789, 1e-7);
        EXPECT_EQ(wave->type(), kShortPeriod);
        break;
      case kMSN2:
        EXPECT_EQ(wave->ident(), kMSN2);
        EXPECT_NEAR(wave->frequency<kDegreePerHour>(), 30.5443747, 1e-7);
        EXPECT_EQ(wave->type(), kShortPeriod);
        break;
      case kMO3:
        EXPECT_EQ(wave->ident(), kMO3);
        EXPECT_NEAR(wave->frequency<kDegreePerHour>(), 42.9271398, 1e-7);
        EXPECT_EQ(wave->type(), kShortPeriod);
        break;
      case k2MK3:
        EXPECT_EQ(wave->ident(), k2MK3);
        EXPECT_NEAR(wave->frequency<kDegreePerHour>(), 42.9271398, 1e-7);
        EXPECT_EQ(wave->type(), kShortPeriod);
        break;
      case kMK3:
        EXPECT_EQ(wave->ident(), kMK3);
        EXPECT_NEAR(wave->frequency<kDegreePerHour>(), 44.0251729, 1e-7);
        EXPECT_EQ(wave->type(), kShortPeriod);
        break;
      case kS6:
        EXPECT_EQ(wave->ident(), kS6);
        EXPECT_NEAR(wave->frequency<kDegreePerHour>(), 90.0, 1e-7);
        EXPECT_EQ(wave->type(), kShortPeriod);
        break;
      case kM8:
        EXPECT_EQ(wave->ident(), kM8);
        EXPECT_NEAR(wave->frequency<kDegreePerHour>(), 115.9364169, 1e-7);
        EXPECT_EQ(wave->type(), kShortPeriod);
        break;
      case kMSf:
        EXPECT_EQ(wave->ident(), kMSf);
        EXPECT_NEAR(wave->frequency<kDegreePerHour>(), 1.01589578, 1e-7);
        EXPECT_EQ(wave->type(), kLongPeriod);
        break;
      case kSsa:
        EXPECT_EQ(wave->ident(), kSsa);
        EXPECT_NEAR(wave->frequency<kDegreePerHour>(), 0.08213728, 1e-7);
        EXPECT_EQ(wave->type(), kLongPeriod);
        break;
      case kSa:
        EXPECT_EQ(wave->ident(), kSa);
        EXPECT_NEAR(wave->frequency<kDegreePerHour>(), 0.04106864, 1e-7);
        EXPECT_EQ(wave->type(), kLongPeriod);
        break;
      case kA5:
        EXPECT_EQ(wave->ident(), kA5);
        EXPECT_NEAR(wave->frequency<kDegreePerHour>(), 1.01589579, 1e-7);
        EXPECT_EQ(wave->type(), kLongPeriod);
        break;
      case kSa1:
        EXPECT_EQ(wave->ident(), kSa1);
        EXPECT_NEAR(wave->frequency<kDegreePerHour>(), 0.041066678, 1e-7);
        EXPECT_EQ(wave->type(), kLongPeriod);
        break;
      case kSta:
        EXPECT_EQ(wave->ident(), kSta);
        EXPECT_NEAR(wave->frequency<kDegreePerHour>(), 0.123203956, 1e-7);
        EXPECT_EQ(wave->type(), kLongPeriod);
        break;
      case kMm1:
        EXPECT_EQ(wave->ident(), kMm1);
        EXPECT_NEAR(wave->frequency<kDegreePerHour>(), 0.553658366, 1e-7);
        EXPECT_EQ(wave->type(), kLongPeriod);
        break;
      case kMm2:
        EXPECT_EQ(wave->ident(), kMm2);
        EXPECT_NEAR(wave->frequency<kDegreePerHour>(), 0.549016532, 1e-7);
        EXPECT_EQ(wave->type(), kLongPeriod);
        break;
      case kMf1:
        EXPECT_EQ(wave->ident(), kMf1);
        EXPECT_NEAR(wave->frequency<kDegreePerHour>(), 1.088749395, 1e-7);
        EXPECT_EQ(wave->type(), kLongPeriod);
        break;
      case kMf2:
        EXPECT_EQ(wave->ident(), kMf2);
        EXPECT_NEAR(wave->frequency<kDegreePerHour>(), 1.093391230, 1e-7);
        EXPECT_EQ(wave->type(), kLongPeriod);
        break;
      case kM0:
        EXPECT_EQ(wave->ident(), kM0);
        EXPECT_NEAR(wave->frequency<kDegreePerHour>(), 0.0, 1e-7);
        EXPECT_EQ(wave->type(), kLongPeriod);
        break;
      case kL2P:
        EXPECT_EQ(wave->ident(), kL2P);
        EXPECT_NEAR(wave->frequency<kDegreePerHour>(), 29.5331207461, 1e-7);
        EXPECT_EQ(wave->type(), kShortPeriod);
        break;
      case kN2P:
        EXPECT_EQ(wave->ident(), kN2P);
        EXPECT_NEAR(wave->frequency<kDegreePerHour>(), 28.435087682, 1e-7);
        EXPECT_EQ(wave->type(), kShortPeriod);
        break;
      case kMSK2:
        EXPECT_EQ(wave->ident(), kMSK2);
        EXPECT_NEAR(wave->frequency<kDegreePerHour>(), 28.9019669359, 1e-7);
        EXPECT_EQ(wave->type(), kShortPeriod);
        break;
      case kSKM2:
        EXPECT_EQ(wave->ident(), kSKM2);
        EXPECT_NEAR(wave->frequency<kDegreePerHour>(), 31.0980330641, 1e-7);
        EXPECT_EQ(wave->type(), kShortPeriod);
        break;
      case kOQ2:
        EXPECT_EQ(wave->ident(), kOQ2);
        EXPECT_NEAR(wave->frequency<kDegreePerHour>(), 27.3416964522, 1e-7);
        EXPECT_EQ(wave->type(), kShortPeriod);
        break;
      case k3MS4:
        EXPECT_EQ(wave->ident(), k3MS4);
        EXPECT_NEAR(wave->frequency<kDegreePerHour>(), 56.952312642, 1e-7);
        EXPECT_EQ(wave->type(), kShortPeriod);
        break;
      case kMNu4:
        EXPECT_EQ(wave->ident(), kMNu4);
        EXPECT_NEAR(wave->frequency<kDegreePerHour>(), 57.4966873397, 1e-7);
        EXPECT_EQ(wave->type(), kShortPeriod);
        break;
      case k2MSN4:
        EXPECT_EQ(wave->ident(), k2MSN4);
        EXPECT_NEAR(wave->frequency<kDegreePerHour>(), 59.5284789117, 1e-7);
        EXPECT_EQ(wave->type(), kShortPeriod);
        break;
      case k2NS2:
        EXPECT_EQ(wave->ident(), k2NS2);
        EXPECT_NEAR(wave->frequency<kDegreePerHour>(), 26.8794590326, 1e-7);
        EXPECT_EQ(wave->type(), kShortPeriod);
        break;
      case kMNuS2:
        EXPECT_EQ(wave->ident(), kMNuS2);
        EXPECT_NEAR(wave->frequency<kDegreePerHour>(), 27.4966873397, 1e-7);
        EXPECT_EQ(wave->type(), kShortPeriod);
        break;
      case k2MK2:
        EXPECT_EQ(wave->ident(), k2MK2);
        EXPECT_NEAR(wave->frequency<kDegreePerHour>(), 27.8860711499, 1e-7);
        EXPECT_EQ(wave->type(), kShortPeriod);
        break;
      case kNKM2:
        EXPECT_EQ(wave->ident(), kNKM2);
        EXPECT_NEAR(wave->frequency<kDegreePerHour>(), 29.5377625804, 1e-7);
        EXPECT_EQ(wave->type(), kShortPeriod);
        break;
      case kML4:
        EXPECT_EQ(wave->ident(), kML4);
        EXPECT_NEAR(wave->frequency<kDegreePerHour>(), 58.5125831257, 1e-7);
        EXPECT_EQ(wave->type(), kShortPeriod);
        break;
      case kSO1:
        EXPECT_EQ(wave->ident(), kSO1);
        EXPECT_NEAR(wave->frequency<kDegreePerHour>(), 16.0569644251, 1e-7);
        EXPECT_EQ(wave->type(), kShortPeriod);
        break;
      case kSO3:
        EXPECT_EQ(wave->ident(), kSO3);
        EXPECT_NEAR(wave->frequency<kDegreePerHour>(), 43.9430355749, 1e-7);
        EXPECT_EQ(wave->type(), kShortPeriod);
        break;
      case kNK4:
        EXPECT_EQ(wave->ident(), kNK4);
        EXPECT_NEAR(wave->frequency<kDegreePerHour>(), 58.5218667944, 1e-7);
        EXPECT_EQ(wave->type(), kShortPeriod);
        break;
      case kMNK6:
        EXPECT_EQ(wave->ident(), kMNK6);
        EXPECT_NEAR(wave->frequency<kDegreePerHour>(), 87.5059710084, 1e-7);
        EXPECT_EQ(wave->type(), kShortPeriod);
        break;
      case k2NM6:
        EXPECT_EQ(wave->ident(), k2NM6);
        EXPECT_NEAR(wave->frequency<kDegreePerHour>(), 85.8635632466, 1e-7);
        EXPECT_EQ(wave->type(), kShortPeriod);
        break;
      case k3MS8:
        EXPECT_EQ(wave->ident(), k3MS8);
        EXPECT_NEAR(wave->frequency<kDegreePerHour>(), 116.952312642, 1e-7);
        EXPECT_EQ(wave->type(), kShortPeriod);
        break;
      case kSK3:
        EXPECT_EQ(wave->ident(), kSK3);
        EXPECT_NEAR(wave->frequency<kDegreePerHour>(), 45.0410686391, 1e-7);
        EXPECT_EQ(wave->type(), kShortPeriod);
        break;
      case k2MNS4:
        EXPECT_EQ(wave->ident(), k2MNS4);
        EXPECT_NEAR(wave->frequency<kDegreePerHour>(), 56.4079379443, 1e-7);
        EXPECT_EQ(wave->type(), kShortPeriod);
        break;
      case k2SMu2:
        EXPECT_EQ(wave->ident(), k2SMu2);
        EXPECT_NEAR(wave->frequency<kDegreePerHour>(), 32.031791572, 1e-7);
        EXPECT_EQ(wave->type(), kShortPeriod);
        break;
      case k2MP5:
        EXPECT_EQ(wave->ident(), k2MP5);
        EXPECT_NEAR(wave->frequency<kDegreePerHour>(), 72.927139789, 1e-7);
        EXPECT_EQ(wave->type(), kShortPeriod);
        break;
      default:
        break;
    }
  }
}
// NOLINTEND(readability-function-cognitive-complexity,*-function-size)

TEST(WaveTable, Constructor) { check_wave_table_constructor(WaveTable()); }

TEST(WaveTable, IdentName) {
  for (auto&& item : WaveTable()) {
    const auto& wave = item.value();
    const auto* name = wave->name();
    auto ident = constituents::parse(name);
    EXPECT_EQ(wave->ident(), ident);
  }
}

TEST(WaveTable, Sparse) {
  auto table =
      WaveTable({"O1", "K1", "M2", "S2", "N2", "K2", "M4", "M6", "Mf2"});
  EXPECT_EQ(table.size(), 9);
  EXPECT_EQ(table[kO1]->ident(), kO1);
  EXPECT_EQ(table[kK1]->ident(), kK1);
  EXPECT_EQ(table[kN2]->ident(), kN2);
  EXPECT_EQ(table[kM2]->ident(), kM2);
  EXPECT_EQ(table[kS2]->ident(), kS2);
  EXPECT_EQ(table[kK2]->ident(), kK2);
  EXPECT_EQ(table[kM4]->ident(), kM4);
  EXPECT_EQ(table[kM6]->ident(), kM6);
  EXPECT_EQ(table[kMf2]->ident(), kMf2);
  EXPECT_THROW(table[kP1], std::out_of_range);
}

}  // namespace darwin
}  // namespace fes
