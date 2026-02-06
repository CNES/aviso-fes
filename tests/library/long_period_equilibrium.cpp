// Copyright (c) 2026 CNES
//
// All rights reserved. Use of this source code is governed by a
// BSD-style license that can be found in the LICENSE file.
#include "fes/long_period_equilibrium.hpp"

#include <gtest/gtest.h>

#include "fes/darwin/wave_table.hpp"

namespace fes {

class AstronomicAngle : public angle::Astronomic {
 public:
  explicit AstronomicAngle(const bool overload_angle = false) {
    s_ = 3.4550013579944832;
    h_ = 4.8910358580921542;
    p_ = 5.2822083020245900;
    n_ = 6.0263705975251547;
    p1_ = 4.9291820072528578;

    if (!overload_angle) {
      return;
    }
    // overload the angle to be compatible with AVISO/FES
    auto td = ((1 + 33282.0) * 86400.0 - 4043174400.0) / 86400.0;
    s_ = detail::math::radians(std::fmod(290.210 + (td * 13.17639650), 360.0));
    h_ = detail::math::radians(std::fmod(280.120 + (td * 0.98564730), 360.0));
    p_ = detail::math::radians(std::fmod(274.350 + (td * 0.11140410), 360.0));
    n_ = -detail::math::radians(std::fmod(343.510 + (td * 0.05295390), 360.0));
    p1_ = detail::math::radians(std::fmod(283.000 + (td * 0.00000000), 360.0));
  }
};

TEST(WaveOrder2, LpeMinus5WavesNonRegression) {
  auto table = darwin::WaveTable();
  auto lpe = LongPeriodEquilibrium(table);
  EXPECT_NEAR(lpe.lpe_minus_n_waves(AstronomicAngle(), 1), 0.41377275320048151,
              1e-6);

  table[kMm]->set_is_modeled(true);
  table[kMf]->set_is_modeled(true);
  table[kMtm]->set_is_modeled(true);
  table[kMSqm]->set_is_modeled(true);
  table[kSsa]->set_is_modeled(true);

  lpe = LongPeriodEquilibrium(table);
  EXPECT_NEAR(lpe.lpe_minus_n_waves(AstronomicAngle(), 1), -0.58598307964904195,
              1e-6);

  table[kSa1]->set_is_modeled(true);
  table[kSta]->set_is_modeled(true);
  table[kMm1]->set_is_modeled(true);
  table[kMf1]->set_is_modeled(true);
  table[kA5]->set_is_modeled(true);

  lpe = LongPeriodEquilibrium(table);
  EXPECT_NEAR(lpe.lpe_minus_n_waves(AstronomicAngle(), 1), -0.53097185112613943,
              1e-6);

  table[kMm2]->set_is_modeled(true);
  table[kMf2]->set_is_modeled(true);

  lpe = LongPeriodEquilibrium(table);
  EXPECT_NEAR(lpe.lpe_minus_n_waves(AstronomicAngle(), 1), -0.52879623974557166,
              1e-6);
}

TEST(WaveOrder2, LpeMinus5WavesAvisoFES) {
  auto table = darwin::WaveTable();
  auto lpe = LongPeriodEquilibrium(table);
  EXPECT_NEAR(lpe.lpe_minus_n_waves(AstronomicAngle(true), 1),
              -3.2392765310488252, 1e-6);

  table[kMm]->set_is_modeled(true);
  table[kMf]->set_is_modeled(true);
  table[kMtm]->set_is_modeled(true);
  table[kMSqm]->set_is_modeled(true);
  lpe = LongPeriodEquilibrium(table);
  EXPECT_NEAR(lpe.lpe_minus_n_waves(AstronomicAngle(true), 1),
              -1.1941264878178992, 1e-6);

  table[kSsa]->set_is_modeled(true);
  table[kSa1]->set_is_modeled(true);
  table[kSta]->set_is_modeled(true);
  table[kMm1]->set_is_modeled(true);
  table[kMf1]->set_is_modeled(true);
  table[kA5]->set_is_modeled(true);
  table[kMm2]->set_is_modeled(true);
  table[kMf2]->set_is_modeled(true);
  lpe = LongPeriodEquilibrium(table);
  EXPECT_NEAR(lpe.lpe_minus_n_waves(AstronomicAngle(true), 1),
              -0.70850451575143991, 1e-6);
}

}  // namespace fes
