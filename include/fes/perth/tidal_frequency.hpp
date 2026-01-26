#pragma once

#include <iostream>

#include "fes/angle/astronomic.hpp"
#include "fes/perth/doodson.hpp"
#include "fes/types.hpp"

namespace fes {
namespace perth {

/// @brief Compute the frequency of a tide with a given Doodson number
/// (without the 5's). The returned frequency is in units of degrees per hour.
/// @param[in] doodson_number Doodson number as a 6-dimensional vector.
/// @return Frequency in degrees per hour.
inline auto tidal_frequency(const Eigen::Ref<const Vector6d> &doodson_number)
    -> double {
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

    // Compute rates in degrees per hour
    return (beta2 - beta1) / (24.0 * del);
  }();

  // Compute frequency as dot product with Doodson number
  return rates.dot(doodson_number);
}

}  // namespace perth
}  // namespace fes
