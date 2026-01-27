// Copyright (c) 2026 CNES
//
// All rights reserved. Use of this source code is governed by a
// BSD-style license that can be found in the LICENSE file.
/// @file include/fes/detail/angle/astronomic/speed.hpp
/// @brief Astronomic speed
#pragma once

namespace fes {
namespace detail {
namespace angle {
namespace astronomic {
namespace speed {

/// Number of hours in one Julian century (36525 days * 24 hours).
constexpr double kHoursPerJulianCentury = 876600.0;

/// @brief s
///
/// Compute the speed in degree by hour for the moon's mean longitude.
/// Based on Simon et al. (1994) value of 481267.88123421 deg/cy.
constexpr auto s() noexcept -> double {
  return 481267.88123421 / kHoursPerJulianCentury;
}

/// @brief h
///
/// Compute the speed in degree by hour for the sun's mean longitude.
/// Based on Simon et al. (1994) value of 36000.76982779 deg/cy.
constexpr auto h() noexcept -> double {
  return 36000.76982779 / kHoursPerJulianCentury;
}

/// @brief p
///
/// Compute the speed in degree by hour for the longitude of moon's perigee.
/// Based on Simon et al. (1994) value of 4069.01372879 deg/cy.
constexpr auto p() noexcept -> double {
  return 4069.01372879 / kHoursPerJulianCentury;
}

/// @brief \f$N'\f$
///
/// Compute the speed in degree by hour for the longitude of moon's ascending
/// node. This is the negative of the rate of N.
/// Based on Simon et al. (1994) value of -1934.13626197 deg/cy for N.
constexpr auto n() noexcept -> double {
  return -(-1934.13626197 / kHoursPerJulianCentury);
}

/// @brief \f$p_1\f$
///
/// Compute the speed in degree by hour for the longitude of sun's
/// perigee.
/// Based on Simon et al. (1994) value of 1.71945766 deg/cy.
constexpr auto p1() noexcept -> double {
  return 1.71945766 / kHoursPerJulianCentury;
}

/// @brief \f$\tau\f$
///
/// Compute the speed in degree by hour for the local mean lunar time.
/// This is the rate of change of the hour angle of the mean moon.
constexpr auto tau() noexcept -> double { return 15.0 - (s() - h()); }

}  // namespace speed
}  // namespace astronomic
}  // namespace angle
}  // namespace detail
}  // namespace fes
