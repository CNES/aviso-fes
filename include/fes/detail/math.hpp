// Copyright (c) 2024 CNES
//
// All rights reserved. Use of this source code is governed by a
// BSD-style license that can be found in the LICENSE file.
/// @file include/fes/detail/math.hpp
/// @brief Mathematical functions
#pragma once
#include <array>
#include <cmath>
#include <complex>
#include <iostream>
#include <limits>
#include <tuple>
#include <type_traits>

#if defined(__GNUC__) && (__GNUC__ >= 11) || \
    defined(__clang__) && (__cplusplus >= 202303L)
#define FES_MATH_CONSTEXPR constexpr
#else
#define FES_MATH_CONSTEXPR inline
#endif

namespace fes {
namespace detail {
namespace math {

/// π
///
/// @tparam T The type of the result.
/// @return π
template <typename T>
constexpr auto pi() noexcept -> T {
  return std::atan2(T(0), T(-1));
}

/// π/2
///
/// @tparam T The type of the result.
/// @return π/2
template <typename T>
constexpr auto pi_2() noexcept -> T {
  return 0.5 * pi<T>();
}

/// 2π
///
/// @tparam T The type of the result.
/// @return 2π
template <typename T>
constexpr auto two_pi() noexcept -> T {
  return T(2) * pi<T>();
}

/// Power of 2.
///
/// @tparam T The type of the result.
/// @return \f$x^2\f$
template <typename T>
constexpr auto sqr(const T& x) -> T {
  return x * x;
}

/// Power of 3.
///
/// @tparam T The type of the result.
/// @return \f$x^3\f$
template <typename T>
constexpr auto pow3(const T& x) -> T {
  return sqr(x) * x;
}

/// Power of 4.
///
/// @tparam T The type of the result.
/// @return \f$x^4\f$
template <typename T>
constexpr auto pow4(const T& x) -> T {
  return sqr(x) * sqr(x);
}

/// Convert angle x from radians to degrees.
///
/// @tparam T The type of the result.
/// @param[in] x Angle in radians.
/// @return Angle in degrees.
template <typename T>
constexpr auto radians(const T& x) noexcept -> T {
  return x * pi<T>() / T(180);
}

/// Convert angle x from degrees to radians.
///
/// @tparam T The type of the result.
/// @param[in] x Angle in degrees.
/// @return Angle in radians.
template <typename T>
constexpr auto degrees(const T& x) noexcept -> T {
  return x * T(180) / pi<T>();
}

/// Evaluate the sine function with the argument in degrees
///
/// In order to minimize round-off errors, this function exactly reduces the
/// argument to the range [-45, 45] before converting it to radians.
///
/// @tparam T The type of the result.
/// @param[in] x x in degrees.
/// @return sin(x).
template <typename T>
constexpr auto sind(const T& x) noexcept -> T {
  int quotient{};
  T result = radians(std::remquo(x, T(90), &quotient));
  // now |result| <= π/4
  auto quadrant = static_cast<unsigned int>(quotient);
  result = (quadrant & 1U) ? std::cos(result) : std::sin(result);
  if (quadrant & 2U) {
    result = -result;
  }
  return result;
}

/// Evaluate the cosine function with the argument in degrees
///
/// @tparam T The type of the result.
/// @param[in] x in degrees.
/// @return cos(x).
template <typename T>
constexpr auto cosd(const T& x) noexcept -> T {
  int quotient{};
  T result = radians(std::remquo(x, T(90), &quotient));
  // now |result| <= π/4
  auto quadrant = static_cast<unsigned int>(quotient + 1);
  result = (quadrant & 1U) ? std::cos(result) : std::sin(result);
  if (quadrant & 2U) {
    result = -result;
  }
  return result;
}

/// Evaluate the sine and cosine function with the argument in degrees
///
/// @tparam T The type of the result.
/// @param[in] x in degrees.
/// @return a tuple that contains sin(x) and cos(x)
template <typename T>
constexpr auto sincosd(const T& x) noexcept -> std::tuple<T, T> {
  int quotient{};
  T angle = radians(std::remquo(x, T(90), &quotient));
  // now |angle| <= π/4
  switch (static_cast<unsigned int>(quotient) & 3U) {
    case 0U:
      return std::make_tuple(std::sin(angle), std::cos(angle));
    case 1U:
      return std::make_tuple(std::cos(angle), -std::sin(angle));
    case 2U:
      return std::make_tuple(-std::sin(angle), -std::cos(angle));
    // case 3U
    default:
      return std::make_tuple(-std::cos(angle), std::sin(angle));
  }
}

/// True if a is almost zero to epsilon
///
/// @tparam T Data type
/// @param[in] a Value to test
/// @param[in] epsilon Epsilon
/// @return true if a is almost zero to epsilon
template <typename T, typename std::enable_if<std::is_floating_point<T>::value,
                                              T>::type* = nullptr>
constexpr auto is_almost_zero(
    const T& a,
    const T& epsilon = std::numeric_limits<T>::epsilon()) noexcept -> bool {
  return std::fabs(a) < epsilon;
}

/// Computes the remainder of the operation x/y
///
/// @tparam T Data type
/// @param[in] x Numerator
/// @param[in] y Denominator
/// @return a result with the same sign as its second operand
template <typename T, typename std::enable_if<std::is_integral<T>::value,
                                              T>::type* = nullptr>
constexpr auto remainder(const T& x, const T& y) noexcept -> T {
  auto result = x % y;
  return result != 0 && (result ^ y) < 0 ? result + y : result;
}

template <typename T, typename std::enable_if<std::is_floating_point<T>::value,
                                              T>::type* = nullptr>
constexpr auto remainder(const T& x, const T& y) noexcept -> T {
  auto result = std::remainder(x, y);
  if (result < T(0) && y > T(0)) {
    result += y;
  }
  return result;
}

/// Normalize an angle.
///
/// @param[in] x The angle in degrees.
/// @param[in] min Minimum circle value
/// @param[in] circle Circle value
/// @return the angle reduced to the range [min, circle + min[
template <typename T>
constexpr auto normalize_angle(const T& x, const T& min = T(-180),
                               const T& circle = T(360)) noexcept -> T {
  return remainder(x - min, circle) + min;
}

/// True if a and b are two values identical to an epsilon.
///
/// @tparam T Data type
/// @param[in] a First value
/// @param[in] b Second value
/// @param[in] epsilon Epsilon
/// @return true if a and b are two values identical to an epsilon
template <typename T, typename std::enable_if<std::is_floating_point<T>::value,
                                              T>::type* = nullptr>
constexpr auto is_same(
    const T& a, const T& b,
    const T& epsilon = std::numeric_limits<T>::epsilon()) noexcept -> bool {
  auto diff = std::fabs(a - b);
  if (diff <= epsilon) {
    return true;
  }
  if (diff < std::fmax(std::fabs(a), std::fabs(b)) * epsilon) {
    return true;
  }
  return false;
}

/// Calculate the bilinear weights
///
/// @param[in] x x coordinate
/// @param[in] y y coordinate
/// @param[in] x1 x coordinate of the first point
/// @param[in] y1 y coordinate of the first point
/// @param[in] x2 x coordinate of the second point
/// @param[in] y2 y coordinate of the second point
/// @return a tuple that contains the weights for all four points
template <typename T>
constexpr auto bilinear_weights(const T& x, const T& y, const T& x1,
                                const T& y1, const T& x2, const T& y2) noexcept
    -> std::tuple<T, T, T, T> {
  const auto dx = T(1) / (x2 - x1);
  const auto dy = T(1) / (y2 - y1);
  const auto wx1 = (x2 - x) * dx;
  const auto wx2 = (x - x1) * dx;
  const auto wy1 = (y2 - y) * dy;
  const auto wy2 = (y - y1) * dy;
  return std::make_tuple(wx1, wx2, wy1, wy2);
}

/// Calculate the bilinear interpolation
///
/// @param[in] wx1 weight for the first point in x
/// @param[in] wx2 weight for the second point in x
/// @param[in] wy1 weight for the first point in y
/// @param[in] wy2 weight for the second point in y
/// @param[in] z11 value of the first point
/// @param[in] z12 value of the second point
/// @param[in] z21 value of the third point
/// @param[in] z22 value of the fourth point
/// @param[out] n The number of points used for the interpolation
/// @return the interpolated value
template <typename T>
constexpr auto bilinear_interpolation(const T& wx1, const T& wx2, const T& wy1,
                                      const T& wy2, const T& z11, const T& z12,
                                      const T& z21, const T& z22,
                                      int64_t& n) noexcept -> T {
  auto result = T(0);
  auto sum_w = T(0);
  n = 0;
  auto add_sample = [&result, &sum_w, &n](const T& z, const T& w) {
    if (z - z == T(0)) {
      result += (z * w);
      sum_w += w;
      ++n;
    }
  };
  add_sample(z11, wx1 * wy1);
  add_sample(z12, wx1 * wy2);
  add_sample(z21, wx2 * wy1);
  add_sample(z22, wx2 * wy2);
  return std::abs(sum_w) > 0 ? result / sum_w
                             : std::numeric_limits<T>::quiet_NaN();
}

/// @brief Evaluates a polynomial using Horner's method.
///
/// @tparam T The type of the input and output values.
/// @tparam Args The types of the polynomial coefficients.
/// @param[in] x The input value.
/// @param[in] args The polynomial coefficients.
/// @return The value of the polynomial evaluated at x.
/// @remark This function evaluates the polynomial using Horner's method.
/// The polynomial coefficients are passed as variadic arguments.
/// The first argument is the input value x, followed by the coefficients
/// in decreasing order of degree.
/// For example, to evaluate the polynomial `3x^3 + 2x^2 + x + 1 at x = 2`,
/// call `horner(2, 3, 2, 1, 1)`.
template <typename T, typename... Args>
constexpr auto horner(const T x, Args... args) -> T {
  static_assert(sizeof...(args) > 0, "At least one coefficient is required.");
  const T coefficients[] = {args...};
  auto ix = sizeof...(args) - 1;
  auto result = coefficients[ix];
  while (ix > 0) {
    ix--;
    result = result * x + coefficients[ix];
  }
  return result;
}

/// @brief Converts an angle expressed in degrees, minutes and seconds to
/// decimal degrees.
///
/// @tparam T The type of the input and output values.
/// @param[in] degrees The angle in degrees.
/// @param[in] minutes The angle in minutes.
/// @param[in] seconds The angle in seconds.
/// @return The angle in decimal degrees.
template <typename T>
constexpr auto dms_to_degrees(const T degrees, const T minutes,
                              const T seconds) noexcept -> T {
  return degrees + (minutes + seconds / T(60)) / T(60);
}

}  // namespace math
}  // namespace detail
}  // namespace fes
