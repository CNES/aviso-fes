// Copyright (c) 2026 CNES
//
// All rights reserved. Use of this source code is governed by a
// BSD-style license that can be found in the LICENSE file.
/// @file include/fes/perth/doodson.hpp
/// @brief Doodson's tidal argument and frequency calculations.
#pragma once

#include "fes/angle/astronomic.hpp"
#include "fes/types.hpp"

namespace fes {
namespace perth {

/// @brief Evaluate Doodson's 6 astronomical variables.
/// Computed angles are returned in units of degrees, between [-180,180).
/// \f$\text{vector} = [ \tau, s, h, p, N^{\prime}, p_s ]\f$
/// @param[in] angle Astronomic angles.
/// @return Doodson's 6 astronomical variables in degrees.
inline auto calculate_celestial_vector(const angle::Astronomic& angle) noexcept
    -> Vector6d {
  // Doodson's tau variable
  auto tau = (angle.t() - angle.s() + angle.h()) - detail::math::pi<double>();

  Vector6d vector;
  vector << tau, angle.s(), angle.h(), angle.p(), -angle.n(), angle.p1();
  return vector.unaryExpr([](double x) {
    return detail::math::normalize_angle(detail::math::degrees<double>(x));
  });
}

/// @brief Evaluate Doodson's tidal argument at a given time.
/// @param[in] angle Astronomic angles.
/// @param[in] doodson_number Doodson number as a 7-dimensional vector.
/// @return Doodson's tidal argument
inline auto calculate_doodson_argument(
    const angle::Astronomic& angle,
    const Eigen::Ref<const Vector7d>& doodson_number) -> double {
  Eigen::Vector<double, 7> beta;
  beta << calculate_celestial_vector(angle), 90.0;

  // Compute argument as dot product with Doodson number
  double arg = doodson_number.dot(beta);
  return detail::math::normalize_angle(arg);
}

/// @brief Compute the frequency of a tide with a given Doodson number
/// (without the 5's). The returned frequency is in units of degrees per hour.
/// @param[in] doodson_number Doodson number as a 6-dimensional vector.
/// @return Frequency in degrees per hour.
inline auto tidal_frequency(const Vector7b& doodson_number) -> double {
  static const auto rates = []() -> Vector6d {
    // Time interval in days
    constexpr double del = 0.05;

    // J2000 epoch in seconds since 1970-01-01T00:00:00Z
    constexpr double t1 = 946728000;
    constexpr double t2 = t1 + (del * 24 * 3600);

    auto astro = angle::Astronomic(angle::Formulae::kIERS);

    // Compute Doodson vectors at two times
    astro.update(t1);
    const auto beta1 = calculate_celestial_vector(astro);

    astro.update(t2);
    const auto beta2 = calculate_celestial_vector(astro);

    // Compute rates in degrees per hour, unwrapping to avoid ±180° jumps
    const auto delta = (beta2 - beta1).unaryExpr([](double x) {
      return detail::math::normalize_angle(x);
    });
    return delta / (24.0 * del);
  }();

  // Compute frequency as dot product with Doodson number
  return rates.dot(doodson_number.head(6).cast<double>());
}

}  // namespace perth
}  // namespace fes
