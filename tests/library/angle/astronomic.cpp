// Copyright (c) 2025 CNES
//
// All rights reserved. Use of this source code is governed by a
// BSD-style license that can be found in the LICENSE file.
#include "fes/angle/astronomic.hpp"

#include <gtest/gtest.h>

TEST(Angle, Meeus) {
  // 946684800 is 2000-01-01T00:00:00Z
  auto a =
      fes::angle::Astronomic(fes::angle::Formulae::kMeeus, 946684800.0, 32);
  EXPECT_NEAR(a.h(), 4.886470984554807, 1e-6);
  EXPECT_NEAR(a.n(), 2.1829004947295840, 1e-6);
  EXPECT_NEAR(a.nu(), 0.20721813091600161, 1e-6);
  EXPECT_NEAR(a.nuprim(), 0.13805659123725886, 1e-6);
  EXPECT_NEAR(a.nusec(), 0.132258440531486, 1e-6);
  EXPECT_NEAR(a.p(), 1.4538178589080255, 1e-6);
  EXPECT_NEAR(a.p1(), 4.938184370650754, 1e-6);
  EXPECT_NEAR(a.r(), 0.10104533494633117, 1e-6);
  EXPECT_NEAR(a.s(), 3.6955259116230854, 1e-6);
  EXPECT_NEAR(a.t(), 3.1415926535897931, 1e-6);
  EXPECT_NEAR(a.x1ra(), 1.1723204500596927, 1e-6);
  EXPECT_NEAR(a.xi(), 0.19203231321420278, 1e-6);

  // -2208988800 is 1900-01-01T00:00:00Z
  a = fes::angle::Astronomic(fes::angle::Formulae::kMeeus, -2208988800.0, 0);
  EXPECT_NEAR(a.h(), 4.890236551186639, 1e-6);
  EXPECT_NEAR(a.n(), 4.5231326946195560, 1e-6);
  EXPECT_NEAR(a.nu(), -0.22723534369606191, 1e-6);
  EXPECT_NEAR(a.nuprim(), -0.15525635947276978, 1e-6);
  EXPECT_NEAR(a.nusec(), -0.15460551183194723, 1e-6);
  EXPECT_NEAR(a.p(), 5.83611841445075, 1e-6);
  EXPECT_NEAR(a.p1(), 4.908181469091293, 1e-6);
  EXPECT_NEAR(a.r(), -0.14529700758557412, 1e-6);
  EXPECT_NEAR(a.s(), 4.8350287204635256, 1e-6);
  EXPECT_NEAR(a.t(), 3.1415926535897931, 1e-6);
  EXPECT_NEAR(a.x1ra(), 0.7873131023129901, 1e-6);
  EXPECT_NEAR(a.xi(), -0.20894666114487137, 1e-6);
}

TEST(Angle, SchuremanOrder1) {
  // 946684800 is 2000-01-01T00:00:00Z
  auto a = fes::angle::Astronomic(fes::angle::Formulae::kSchuremanOrder1,
                                  946684800.0, 32);
  EXPECT_NEAR(a.h(), 4.886446725117425, 1e-6);
  EXPECT_NEAR(a.n(), 2.1828241817408123, 1e-6);
  EXPECT_NEAR(a.p(), 1.4539382076250487, 1e-6);
  EXPECT_NEAR(a.p1(), 4.938234319108096, 1e-6);
  EXPECT_NEAR(a.s(), 3.695581518047498, 1e-6);
  EXPECT_NEAR(a.t(), 3.141592653589793, 1e-6);

  // -2208988800 is 1900-01-01T00:00:00Z
  a = fes::angle::Astronomic(fes::angle::Formulae::kSchuremanOrder1,
                             -2208988800.0, 0);
  EXPECT_NEAR(a.h(), 4.890229330696591, 1e-6);
  EXPECT_NEAR(a.n(), 4.523126458918793, 1e-6);
  EXPECT_NEAR(a.p(), 5.836096903501423, 1e-6);
  EXPECT_NEAR(a.p1(), 4.908229876439484, 1e-6);
  EXPECT_NEAR(a.s(), 4.83500919185063, 1e-6);
  EXPECT_NEAR(a.t(), 3.141592653589793, 1e-6);
}

TEST(Angle, SchuremanOrder3) {
  // 946684800 is 2000-01-01T00:00:00Z
  auto a = fes::angle::Astronomic(fes::angle::Formulae::kSchuremanOrder3,
                                  946684800.0, 32);
  EXPECT_NEAR(a.h(), 4.886452090906138, 1e-6);
  EXPECT_NEAR(a.n(), 2.1828609691751804, 1e-6);
  EXPECT_NEAR(a.p(), 1.4537574500601673, 1e-6);
  EXPECT_NEAR(a.p1(), 4.938241458772235, 1e-6);
  EXPECT_NEAR(a.s(), 3.6956256194908494, 1e-6);
  EXPECT_NEAR(a.t(), 3.141592653589793, 1e-6);

  // -2208988800 is 1900-01-01T00:00:00Z
  a = fes::angle::Astronomic(fes::angle::Formulae::kSchuremanOrder3,
                             -2208988800.0, 0);
  EXPECT_NEAR(a.h(), 4.8902293297448836, 1e-6);
  EXPECT_NEAR(a.n(), 4.5231264600866012, 1e-6);
  EXPECT_NEAR(a.p(), 5.8360969045412325, 1e-6);
  EXPECT_NEAR(a.p1(), 4.908229056119207, 1e-6);
  EXPECT_NEAR(a.s(), 4.8350091919708946, 1e-6);
  EXPECT_NEAR(a.t(), 3.141592653589793, 1e-6);
}
