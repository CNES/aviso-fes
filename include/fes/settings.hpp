// Copyright (c) 2024 CNES
//
// All rights reserved. Use of this source code is governed by a
// BSD-style license that can be found in the LICENSE file.

/// @file include/fes/settings.hpp
/// @brief Settings for the tide computation.
#pragma once

#include <utility>
#include <vector>

#include "fes/angle/astronomic.hpp"
#include "fes/constituent.hpp"

namespace fes {

/// @brief Settings for the tide computation.
class Settings {
 public:
  /// @brief Constructor.
  ///
  /// @param[in] astronomic_formulae Astronomic formulae used to calculate the
  /// astronomic angles.
  /// @param[in] time_tolerance The time in seconds during which astronomical
  /// angles are considered constant. The default value is 0 seconds, indicating
  /// that astronomical angles do not remain constant with time.
  /// @warning
  /// The parameter <tt>time_tolerance</tt> allows for the adjustment of
  /// astronomical angle calculations. When its value is set to zero, the angles
  /// will be recalculated each time the date changes. Otherwise, they will be
  /// considered valid as long as the time difference between the last
  /// evaluation and the current date is within the specified tolerance.<BR> In
  /// this case, it's important to consider that the tidal evaluation may vary
  /// slightly, within an epsilon range, for the same period when calculated
  /// using a different number of threads. This is because the period will be
  /// divided differently based on the number of threads, resulting in
  /// variations in the way angles are updated depending on the number of
  /// threads used.
  Settings(const angle::Formulae& astronomic_formulae =
               angle::Formulae::kSchuremanOrder1,
           const double time_tolerance = 0.0)
      : astronomic_formulae_(astronomic_formulae),
        time_tolerance_(time_tolerance) {}

  /// @brief Returns the astronomic formulae used to calculate the astronomic
  /// angles.
  ///
  /// @return The astronomic formulae used to calculate the astronomic angles.
  constexpr auto astronomic_formulae() const noexcept
      -> const angle::Formulae& {
    return astronomic_formulae_;
  }

  /// @brief Returns the time in seconds for which astronomical angles are
  /// considered constant.
  ///
  /// @return The time in seconds for which astronomical angles are considered
  /// constant.
  constexpr auto time_tolerance() const noexcept -> double {
    return time_tolerance_;
  }

 private:
  /// @brief Astronomic formulae used to calculate the astronomic angles.
  angle::Formulae astronomic_formulae_;
  /// @brief Time in seconds for which astronomical angles are considered
  /// constant.
  double time_tolerance_;
};

}  // namespace fes
