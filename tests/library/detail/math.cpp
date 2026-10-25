// Copyright (c) 2026 CNES
//
// All rights reserved. Use of this source code is governed by a
// BSD-style license that can be found in the LICENSE file.
#include "fes/detail/math.hpp"

#include <gtest/gtest.h>

#include <array>
#include <cmath>
#include <complex>
#include <tuple>

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

namespace fes {
namespace detail {

TEST(Math, Pi) {
  // pi<double> == π
  EXPECT_DOUBLE_EQ(math::pi<double>(), M_PI);
  // The literal must be bit-for-bit identical to atan2(0, -1), which was the
  // previous definition. Float is not checked against atan2f: some libm
  // (e.g. macOS) round it down to stay within [-pi, pi].
  EXPECT_EQ(math::pi<float>(), static_cast<float>(M_PI));
  EXPECT_EQ(math::pi<double>(), std::atan2(0.0, -1.0));
  EXPECT_EQ(math::pi<long double>(), std::atan2(0.0L, -1.0L));
  static_assert(math::pi<double>() > 3.14, "pi must be constexpr");
}

TEST(Math, PiDiv2) {
  // pi_2<double> == π / 2
  EXPECT_DOUBLE_EQ(math::pi_2<double>(), M_PI * 0.5);
}

TEST(Math, TwoPi) {
  // two_pi<double> == 2π
  EXPECT_DOUBLE_EQ(math::two_pi<double>(), 2 * M_PI);
}

TEST(Math, Pow) {
  // pow<n>(x) == x^n
  EXPECT_DOUBLE_EQ(math::pow<0>(2), 1);
  EXPECT_DOUBLE_EQ(math::pow<1>(2), 2);
  EXPECT_DOUBLE_EQ(math::pow<2>(2), 4);
  EXPECT_DOUBLE_EQ(math::pow<3>(2), 8);
  EXPECT_DOUBLE_EQ(math::pow<4>(2), 16);
}

TEST(Math, Radians) {
  // radians(180) == π
  EXPECT_DOUBLE_EQ(math::radians(180.0), M_PI);
}

TEST(Math, Arcseconds2Radians) {
  // arcseconds(3600) == π / 180
  EXPECT_DOUBLE_EQ(math::arcseconds2radians(3600.0), M_PI / 180.0);
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

  std::tie(w0, w1, w2, w3) =
      math::bilinear_weights(0.0, 1.0, 0.0, 0.0, 1.0, 1.0);
  n = static_cast<int64_t>(0);
  z = math::bilinear_interpolation<std::complex<double>>(
      w0, w1, w2, w3, std::complex<double>(0.0, 0.0),
      std::complex<double>(NAN, NAN), std::complex<double>(2.0, 2.0),
      std::complex<double>(3.0, 3.0), n);
  EXPECT_TRUE(std::isnan(z.real()));
  EXPECT_TRUE(std::isnan(z.imag()));
}

TEST(Math, BilinearInterpolationRealWeights) {
  using C = std::complex<double>;
  const auto nan = C(NAN, NAN);
  double w0 = NAN;
  double w1 = NAN;
  double w2 = NAN;
  double w3 = NAN;
  std::tie(w0, w1, w2, w3) =
      math::bilinear_weights(0.13, 0.07, 0.0, 0.0, 0.25, 0.25);
  // Same result as the generic function, to the rounding of the division.
  auto n_expected = static_cast<int64_t>(0);
  auto n = static_cast<int64_t>(0);
  const auto z =
      std::array<C, 4>{C(1.5, -2.25), C(3.0, 0.5), C(-7.0, 2.0), C(4.0, 1.0)};
  auto expected = math::bilinear_interpolation<C>(w0, w1, w2, w3, z[0], z[1],
                                                  z[2], z[3], n_expected);
  auto actual =
      math::bilinear_interpolation(w0, w1, w2, w3, z[0], z[1], z[2], z[3], n);
  EXPECT_EQ(n, 4);
  EXPECT_NEAR(actual.real(), expected.real(), 1e-15);
  EXPECT_NEAR(actual.imag(), expected.imag(), 1e-15);

  // Missing point: the weights are normalized by their sum.
  expected = math::bilinear_interpolation<C>(w0, w1, w2, w3, z[0], nan, z[2],
                                             z[3], n_expected);
  actual =
      math::bilinear_interpolation(w0, w1, w2, w3, z[0], nan, z[2], z[3], n);
  EXPECT_EQ(n, 3);
  EXPECT_NEAR(actual.real(), expected.real(), 1e-15);
  EXPECT_NEAR(actual.imag(), expected.imag(), 1e-15);

  // No point defined.
  actual = math::bilinear_interpolation(w0, w1, w2, w3, nan, nan, nan, nan, n);
  EXPECT_EQ(n, 0);
  EXPECT_TRUE(std::isnan(actual.real()));
  EXPECT_TRUE(std::isnan(actual.imag()));
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

}  // namespace detail
}  // namespace fes
