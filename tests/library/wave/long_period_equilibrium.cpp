// Copyright (c) 2024 CNES
//
// All rights reserved. Use of this source code is governed by a
// BSD-style license that can be found in the LICENSE file.
#include "fes/wave/long_period_equilibrium.hpp"

#include <gtest/gtest.h>

namespace detail = fes::detail;

class AstronomicAngle : public fes::angle::Astronomic {
 public:
  AstronomicAngle(const bool overload_angle = false)
      : fes::angle::Astronomic(fes::angle::Formulae::kMeeus, 1.0, 0.0) {
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
  auto table = fes::wave::Table();
  auto lpe = fes::wave::LongPeriodEquilibrium(table);
  EXPECT_NEAR(lpe.lpe_minus_n_waves(AstronomicAngle(), 1), 0.37049504058579608,
              1e-6);

  table[fes::kMm]->dynamic(true);
  table[fes::kMf]->dynamic(true);
  table[fes::kMtm]->dynamic(true);
  table[fes::kMsqm]->dynamic(true);
  table[fes::kSsa]->dynamic(true);

  lpe = fes::wave::LongPeriodEquilibrium(table);
  EXPECT_NEAR(lpe.lpe_minus_n_waves(AstronomicAngle(), 1), -0.52432975585203723,
              1e-6);

  table[fes::kSa1]->dynamic(true);
  table[fes::kSta]->dynamic(true);
  table[fes::kMm1]->dynamic(true);
  table[fes::kMf1]->dynamic(true);
  table[fes::kA5]->dynamic(true);

  lpe = fes::wave::LongPeriodEquilibrium(table);
  EXPECT_NEAR(lpe.lpe_minus_n_waves(AstronomicAngle(), 1), -0.47509232232339443,
              1e-6);

  table[fes::kMm2]->dynamic(true);
  table[fes::kMf2]->dynamic(true);

  lpe = fes::wave::LongPeriodEquilibrium(table);
  EXPECT_NEAR(lpe.lpe_minus_n_waves(AstronomicAngle(), 1), -0.4732370091873217,
              1e-6);
}

TEST(WaveOrder2, LpeMinus5WavesAvisoFES) {
  auto table = fes::wave::Table();
  auto lpe = fes::wave::LongPeriodEquilibrium(table);
  EXPECT_NEAR(lpe.lpe_minus_n_waves(AstronomicAngle(true), 1),
              -2.8996939083027535, 1e-6);

  table[fes::kMm]->dynamic(true);
  table[fes::kMf]->dynamic(true);
  table[fes::kMtm]->dynamic(true);
  table[fes::kMsqm]->dynamic(true);
  lpe = fes::wave::LongPeriodEquilibrium(table);
  EXPECT_NEAR(lpe.lpe_minus_n_waves(AstronomicAngle(true), 1), -1.069196011706,
              1e-6);

  table[fes::kSsa]->dynamic(true);
  table[fes::kSa1]->dynamic(true);
  table[fes::kSta]->dynamic(true);
  table[fes::kMm1]->dynamic(true);
  table[fes::kMf1]->dynamic(true);
  table[fes::kA5]->dynamic(true);
  table[fes::kMm2]->dynamic(true);
  table[fes::kMf2]->dynamic(true);
  lpe = fes::wave::LongPeriodEquilibrium(table);
  EXPECT_NEAR(lpe.lpe_minus_n_waves(AstronomicAngle(true), 1),
              -0.63411272420708709, 1e-6);
}
