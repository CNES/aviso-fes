// Copyright (c) 2026 CNES
//
// All rights reserved. Use of this source code is governed by a
// BSD-style license that can be found in the LICENSE file.
/// @file include/fes/enum_map.hpp
/// @brief Sparse mapping between enumeration values and associated values
#pragma once

#include <Eigen/Core>
#include <cstddef>

#include "fes/types.hpp"

namespace fes {

/// Harmonic analysis of the tide
///
/// The harmonic analysis method consists in expressing the ocean tidal
/// variations as a sum of independent constituents accordingly to the tidal
/// potential spectrum. Then the sea surface elevation at a point
/// \f$(x, y)\f$ and time \f$(t\f$) can be expressed as a linear sum as
/// follow:
///
/// \f[
///   S_{ap} = S_{0}(x, y) + \sum_{k=0}^n f_{k}(t)S_{k}(x, y)
///     \times cos [\omega_{k}t + {v}_{k}(t) + u_{k}(t) - G_{k}(x,y)]
/// \f]
///
/// where:
///   * \f$(n\f$) is the number of constituents,
///   * \f$S_{0}(x, y)\f$ is the mean sea level,
///   * \f$S_{k}(x, y)\f$ is the amplitude of the constituent of index
///     \f$(k\f$),
///   * \f$G_{k}(x, y)\f$ is the phase lag relative to Greenwich time,
///   * \f$w_{k}\f$ is the angular frequency of the constituent of index
///     \f$(k\f$),
///   * \f$v_{k}\f$ is the astronomical argument at time \f$(t\f$),
///   * \f$f_{k}(t)\f$ is the nodal correction coefficient applied to
///     the amplitude of the constituent of index \f$(k\f$),
///   * \f$u_{k}(t)\f$ is the nodal correction coefficient applied to
///     the phase of the constituent of index \f$(k\f$).
///
///   The a priori analysis spectrum includes the most important astronomical
///   constituents in the Darwin development, completed by Schureman in 1958,
///   and many non-linear waves. The definition of tidal constants and
///   astronomical arguments is taken from FES2014 tidal prediction software
///   and a complete definition of waves is also available in
///   Schureman (1958).
///   This spectrum is the most commonly used for harmonic analysis due the
///   simplification given by the nodal correction concept (\f$(f\f$) and
///   \f$(u\f$) coefficients above) which allows dealing with slow motions of
///   the lunar ascending node and reducing the number of constituents in the
///   tidal spectrum. More details about this harmonic analysis method can be
///   found in Ponchaut et al. 1999.
///
/// @param[in] h Sea level
/// @param[in] f Nodal correction coefficient applied to the
/// amplitude of the constituents analyzed.
/// @param[in] vu Astronomical argument at time \f$(t\f$) + the
/// nodal correction coefficient applied to the phase of the
/// constituents analyzed.
auto harmonic_analysis(const Eigen::Ref<const Eigen::VectorXd>& h,
                       const DynamicRef<const Eigen::MatrixXd>& f,
                       const DynamicRef<const Eigen::MatrixXd>& vu)
    -> Eigen::VectorXcd;

}  // namespace fes
