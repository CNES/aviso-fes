// Copyright (c) 2024 CNES
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

/// Number of seconds in one Julian century
constexpr double kJulianCentury = 3155760000.0;

/// @brief s
///
/// Compute the speed in degree by hour for the moon's mean longitude.
constexpr auto s() noexcept -> double {
  return (((1336.0 * 360.0 + 307.892) / kJulianCentury)) * 3600.0;
}

/// @brief h
///
/// Compute the speed in degree by hour for the sun's mean longitude.
constexpr auto h() noexcept -> double {
  return (((100.0 * 360.0 + 0.769) / kJulianCentury)) * 3600.0;
}

/// @brief p
///
/// Compute the speed in degree by hour for the longitude of moon's perigee.
constexpr auto p() noexcept -> double {
  return (((11.0 * 360.0 + 109.032) / kJulianCentury)) * 3600.0;
}

/// @brief \f$N'\f$
///
/// Compute the speed in degree by hour for the longitude of moon's ascending
/// node.
constexpr auto n() noexcept -> double {
  return (((-5.0 * 360.0 - 134.142) / kJulianCentury)) * 3600.0;
}

/// @brief \f$p_1\f$
///
/// Compute the speed in degree by hour for the longitude of sun's
/// perigee.
constexpr auto p1() noexcept -> double {
  return ((1.719 / kJulianCentury)) * 3600;
}

/// @brief \f$\tau\f$
///
/// Compute the speed in degree by hour for the local mean lunar time.
constexpr auto tau() noexcept -> double { return 15.0 - s() + h(); }

}  // namespace speed
}  // namespace astronomic
}  // namespace angle
}  // namespace detail
}  // namespace fes
