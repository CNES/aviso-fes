#pragma once

#include <Eigen/Dense>

#include "fes/angle/astronomic.hpp"
#include "fes/detail/math.hpp"
#include "fes/eigen.hpp"

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
  auto tau = (-angle.t() + detail::math::pi<double>()) - angle.s() + angle.h();

  Vector6d vector;
  vector << tau, angle.s(), angle.h(), angle.p(), -angle.n(), angle.p1();
  return vector.unaryExpr([](double x) {
    return detail::math::normalize_angle(detail::math::degrees<double>(x));
  });
}

/// @brief Evaluate Doodson's tidal argument at a given time.
/// @param[in] angle Astronomic angles.
/// @param[in] component Tide component to evaluate.
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

}  // namespace perth
}  // namespace fes
