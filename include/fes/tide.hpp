// Copyright (c) 2025 CNES
//
// All rights reserved. Use of this source code is governed by a
// BSD-style license that can be found in the LICENSE file.
/// @file include/fes/tide.hpp
/// @brief Tidal evaluation.
#pragma once

#include "darwin/tide.hpp"
#include "fes/settings.hpp"
#include "perth/tide.hpp"

namespace fes {

/// Ocean tide prediction.
///
/// @param[in] tidal_model Tidal model used to interpolate the modelized waves
/// @param[in] epoch Date of the tide calculation expressed in number of seconds
/// elapsed since 1970-01-01T00:00:00Z
/// @param[in] longitude Longitude in degrees for the position at which the tide
/// is calculated
/// @param[in] latitude Latitude in degrees for the position at which the tide
/// is calculated
/// @param[in] settings Settings for the tide computation.
/// @return A tuple that contains:
/// - The height of the the diurnal and semi-diurnal constituents of the
///   tidal spectrum.
/// - The height of the long period wave constituents of the tidal
///   spectrum.
/// - The quality flag indicating the reliability of the tide
///   calculation at the given position:
///   - <b>0</b>: the tide is undefined (no data available at the given
///     position).
///   - <b>Positive values</b>: the tide is interpolated at the given
///     position using \c N data points (where \c N is the
///     number of data points used for the interpolation).
///   - <b>Negative values</b>: the tide is extrapolated at the given
///     position using \c -N data points (where \c N is the
///     number of data points used for the extrapolation).
/// @note The units of the returned tide are the same as the units of the
/// constituents loaded in the tidal model.
/// @note Computed height of the diurnal and semi-diurnal constituents is set
/// to nan if no data is available at the given position. the long period wave
/// constituents is always computed because this value does not depend on
/// model data.
template <typename T>
auto evaluate_tide(const AbstractTidalModel<T>* const tidal_model,
                   const Eigen::Ref<const Eigen::VectorXd>& epoch,
                   const Eigen::Ref<const Eigen::VectorXd>& longitude,
                   const Eigen::Ref<const Eigen::VectorXd>& latitude,
                   const Settings& settings)
    -> std::tuple<Eigen::VectorXd, Eigen::VectorXd, Vector<Quality>> {
  if (settings.engine_type() == EngineType::kFES) {
    return darwin::evaluate_tide(tidal_model, epoch, longitude, latitude,
                                 settings);
  }
  if (settings.engine_type() == EngineType::kPERTH5) {
    return perth::evaluate_tide(tidal_model, epoch, longitude, latitude,
                                settings);
  }
  throw std::invalid_argument("Unsupported engine type for tidal evaluation.");
}

}  // namespace fes
