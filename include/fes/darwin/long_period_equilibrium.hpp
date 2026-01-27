// Copyright (c) 2026 CNES
//
// All rights reserved. Use of this source code is governed by a
// BSD-style license that can be found in the LICENSE file.
/// @file include/fes/darwin/long_period_equilibrium.hpp
/// @brief Long period equilibrium ocean tides.
#pragma once
#include <Eigen/Core>

#include "fes/darwin/table.hpp"

namespace fes {
namespace darwin {

/// @brief Compute the long-period equilibrium ocean tides.
///
/// Properties of the wave order 2 coefficients values to compute the
/// long-period equilibrium ocean tides.
class LongPeriodEquilibrium {
 public:
  /// Default constructor
  LongPeriodEquilibrium();

  /// Constructs the default object and disable the dynamic wave used for the
  /// calculation of the long-period equilibrium ocean tides.
  explicit LongPeriodEquilibrium(const WaveTable& table)
      : LongPeriodEquilibrium() {
    disable_dynamic_wave(table);
  }

  /// Disable the dynamic wave used for the calculation of the long-period
  /// equilibrium ocean tides.
  ///
  /// @param[in] table Waves to disable
  auto disable_dynamic_wave(const WaveTable& table) -> void;

  /// @brief Computes the long-period equilibrium ocean tides.
  ///
  /// The complete tidal spectral lines from the Cartwright-Tayler-Edden
  /// tables are summed over to compute the long-period tide.
  /// @n
  /// Order 2 and order 3 of the tidal potential for the long period waves is
  /// now taken into account.
  /// @n
  /// The decomposition was validated compared to the potential proposed by
  /// Tamura.
  /// @n
  /// Waves computed dynamically are removed.
  ///
  /// Technical references:
  ///   - Cartwright & Tayler, Geophys. J. R.A.S., 23, 45, 1971.
  ///   - Cartwright & Edden, Geophys. J. R.A.S., 33, 253, 1973.
  ///   - Tamura Y., Bull. d'information des marees terrestres, Vol. 99, 1987.
  ///
  /// @param[in] angles the astronomic angle, indicating the date on which the
  /// tide is to be calculated.
  /// @param[in] lat Latitude in degrees (positive north) for the position at
  /// which tide is computed.
  /// @return Computed long-period tide, in centimeters.
  auto lpe_minus_n_waves(const angle::Astronomic& angles, double lat) -> double;

 private:
  Eigen::Matrix<double, 106, 6> order2_;  // NOLINT (magic number, physics)
  Eigen::Matrix<double, 17, 6> order3_;   // NOLINT (magic number, physics)
};

}  // namespace darwin
}  // namespace fes
