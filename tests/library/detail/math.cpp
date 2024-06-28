// Copyright (c) 2023 CNES
//
// All rights reserved. Use of this source code is governed by a
// BSD-style license that can be found in the LICENSE file.
#include "fes/detail/math.hpp"

#include <gtest/gtest.h>

#include <cmath>

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

namespace math = fes::detail::math;

TEST(Math, Pi) {
  // pi<double> == π
  EXPECT_DOUBLE_EQ(math::pi<double>(), M_PI);
}

TEST(Math, PiDiv2) {
  // pi_2<double> == π / 2
  EXPECT_DOUBLE_EQ(math::pi_2<double>(), M_PI * 0.5);
}

TEST(Math, TwoPi) {
  // two_pi<double> == 2π
  EXPECT_DOUBLE_EQ(math::two_pi<double>(), 2 * M_PI);
}

TEST(Math, Square) {
  // sqr(2) == 4
  EXPECT_DOUBLE_EQ(math::sqr(2), 4);
}

TEST(Math, Pow3) {
  // pow3(2) == 8
  EXPECT_DOUBLE_EQ(math::pow3(2), 8);
}

TEST(Math, Pow4) {
  // pow4(2) == 16
  EXPECT_DOUBLE_EQ(math::pow4(2), 16);
}

TEST(Math, Radians) {
  // radians(180) == π
  EXPECT_DOUBLE_EQ(math::radians(180.0), M_PI);
}

TEST(Math, Degrees) {
  // degrees(π) == 180
  EXPECT_DOUBLE_EQ(math::degrees(M_PI), 180.0);
}

TEST(Math, Sind) {
  // sind(x) == sin(x * π / 180)
  auto x = -720.0;
  while (x <= 720) {
    EXPECT_NEAR(math::sind(x), std::sin(math::radians(x)), 1e-12);
    x += 0.1;
  }
}

TEST(Math, Cosd) {
  // cosd(x) == cos(x * π / 180)
  auto x = -720.0;
  while (x <= 720) {
    EXPECT_NEAR(math::cosd(x), std::cos(math::radians(x)), 1e-12);
    x += 0.1;
  }
}

TEST(Math, SinCosd) {
  // sincosd(x) == sin(x * π / 180), cos(x * π / 180)
  auto x = -720.0;
  while (x <= 720) {
    double sinx = NAN;
    double cosx = NAN;
    std::tie(sinx, cosx) = math::sincosd(x);
    EXPECT_NEAR(sinx, std::sin(math::radians(x)), 1e-12);
    EXPECT_NEAR(cosx, std::cos(math::radians(x)), 1e-12);
    x += 0.1;
  }
}

TEST(Math, IsAlmostZero) {
  // is_almost_zero(x) == (x == 0)
  EXPECT_TRUE(math::is_almost_zero(0.0));
  EXPECT_FALSE(math::is_almost_zero(1.0));
  EXPECT_TRUE(math::is_almost_zero(1e-12, 1e-11));
}

TEST(Math, NormalizeAngle) {
  // normalize_angle(x + kπ) == x
  EXPECT_NEAR(math::normalize_angle(720.001, -180.0, 360.0), 0.001, 1e-12);
  EXPECT_DOUBLE_EQ(math::normalize_angle(180.0, -180.0, 360.0), -180.0);
  EXPECT_DOUBLE_EQ(math::normalize_angle(2.5 * M_PI, -M_PI, 2 * M_PI),
                   M_PI * 0.5);
}

TEST(Math, Remainder) {
  // x % y like Python
  EXPECT_EQ(math::remainder(360.0, 181.0), 179.0);
  EXPECT_EQ(math::remainder(360.0, -181.0), -2.0);

  EXPECT_EQ(math::remainder(360, 181), 179);
  EXPECT_EQ(math::remainder(360, -181), -2);
}

TEST(Math, IsSame) {
  // is_same(x, y) == (x == y)
  EXPECT_TRUE(math::is_same(1.0, 1.0));
  EXPECT_FALSE(math::is_same(1.0, 2.0));
  EXPECT_TRUE(math::is_same(1.0, 1.0 - 1e-14, 1e-12));
  EXPECT_FALSE(math::is_same(1.0, 1.0 - 1e-12, 1e-14));
}

TEST(Math, BilinearInterpolation) {
  // bilinear_weights(x, y, x0, y0, x1, y1) == (w0, w1, w2, w3)
  double w0 = NAN;
  double w1 = NAN;
  double w2 = NAN;
  double w3 = NAN;
  std::tie(w0, w1, w2, w3) =
      math::bilinear_weights(0.5, 0.5, 0.0, 0.0, 1.0, 1.0);
  EXPECT_DOUBLE_EQ(w0, 0.5);
  EXPECT_DOUBLE_EQ(w1, 0.5);
  EXPECT_DOUBLE_EQ(w2, 0.5);
  EXPECT_DOUBLE_EQ(w3, 0.5);
  auto n = static_cast<int64_t>(0);
  auto z = math::bilinear_interpolation(w0, w1, w2, w3, 0.0, 1.0, 2.0, 3.0, n);
  EXPECT_DOUBLE_EQ(z, 1.5);
  EXPECT_EQ(n, 4);

  z = math::bilinear_interpolation(w0, w1, w2, w3,
                                   std::numeric_limits<double>::quiet_NaN(),
                                   1.0, 2.0, 3.0, n);
  EXPECT_DOUBLE_EQ(z, 2);
  EXPECT_EQ(n, 3);
}

TEST(Math, BilinearInterpolationComplex) {
  double w0 = NAN;
  double w1 = NAN;
  double w2 = NAN;
  double w3 = NAN;
  std::tie(w0, w1, w2, w3) =
      math::bilinear_weights(0.5, 0.5, 0.0, 0.0, 1.0, 1.0);
  auto n = static_cast<int64_t>(0);
  auto z = math::bilinear_interpolation<std::complex<double>>(
      w0, w1, w2, w3, std::complex<double>(0.0, 0.0),
      std::complex<double>(1.0, 1.0), std::complex<double>(2.0, 2.0),
      std::complex<double>(3.0, 3.0), n);
  EXPECT_DOUBLE_EQ(z.real(), 1.5);
  EXPECT_DOUBLE_EQ(z.imag(), 1.5);
  EXPECT_EQ(n, 4);
}

TEST(Math, Horner) {
  // horner(x, a0, a1, a2, a3) == a0 + a1 * x + a2 * x^2 + a3 * x^3
  EXPECT_DOUBLE_EQ(math::horner(2.0, 1.0, 2.0, 3.0, 4.0), 49.0);
}

TEST(Math, DegreesMinutesSecondesToDecialDegrees) {
  // degrees_minutes_secondes_to_decimal_degrees(d, m, s) == d + m / 60 + s /
  // 3600
  EXPECT_DOUBLE_EQ(math::dms_to_degrees<double>(1, 2, 3), 1.0341666666666667);
}
