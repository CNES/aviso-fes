// Copyright (c) 2026 CNES
//
// All rights reserved. Use of this source code is governed by a
// BSD-style license that can be found in the LICENSE file.
/// @file include/fes/detail/angle/astronomic/frequency.hpp
/// @brief Astronomic frequency
#pragma once

#include "fes/detail/angle/astronomic/speed.hpp"

namespace fes {
namespace detail {
namespace angle {
namespace astronomic {
namespace frequency {

/// @brief s
///
/// Compute the frequency in degree by hour for the moon's mean longitude.
constexpr auto s() noexcept -> double {
  return 1.0 / ((15.0 / speed::s()) * 24.0);
}

/// @brief h
///
/// Compute the frequency in degree by hour for the sun's mean longitude.
constexpr auto h() noexcept -> double {
  return 1.0 / ((15.0 / speed::h()) * 24.0);
}

/// @brief p
///
/// Compute the frequency in degree by hour for the longitude of the moon's
/// perigee.
constexpr auto p() noexcept -> double {
  return 1.0 / ((15.0 / speed::p()) * 24.0);
}

/// @brief \f$N'\f$
///
/// Compute the frequency in degree by hour for the longitude of the moon's
/// ascending node.
constexpr auto n() noexcept -> double {
  return 1.0 / ((15.0 / speed::n()) * 24.0);
}

/// @brief \f$p_1\f$
///
/// Compute the frequency in degree by hour for the longitude of sun's
/// perigee.
constexpr auto p1() noexcept -> double {
  return 1.0 / ((15.0 / speed::p1()) * 24.0);
}

/// @brief \f$\tau\f$
///
/// Compute the frequency in degree by hour for the local mean lunar time.
constexpr auto tau() noexcept -> double {
  return 1.0 / ((15.0 / speed::tau()) * 24.0);
}

}  // namespace frequency
}  // namespace astronomic
}  // namespace angle
}  // namespace detail
}  // namespace fes
