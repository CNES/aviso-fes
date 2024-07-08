// Copyright (c) 2024 CNES
//
// All rights reserved. Use of this source code is governed by a
// BSD-style license that can be found in the LICENSE file.

/// @file include/fes/tide.hpp
/// @brief Prediction of tides.
///
/// The height of the tidal at any time may be represented harmonically as
/// follows:
///
/// \f[
///   h = H_0 + \sum f H cos\left[\alpha t + (V_0 + u) -k\right]
/// \f]
///
/// where:
/// - \f$h\f$ height of tide at any time.
/// - \f$H_0\f$ mean height of water levels above data used for prediction.
/// - \f$H\f$ mean amplitude of constituent \f$A\f$.
/// - \f$f\f$ factor for reducing mean amplitude H to the year of prediction.
/// - \f$\alpha\f$ speed of constituent \f$A\f$.
/// - \f$t\f$ time reckoned from some initial epoch such as the beginning of
///   the year of prediction.
/// - \f$(V_0 + u)\f$ value of equilibrium arguments of constituents \f$A\f$
///   when \f$t=0\f$.
/// - \f$k\f$ epoch of constituent \f$A\f$.
///
/// In the above formula, all quantities except \f$h\f$ and \f$t\f$ may be
/// considered as constants for any particular year and place, and when these
/// constants are known the value of \f$h\f$, or the predicted height of the
/// tide, may be calculated for any time \f$t\f$. By comparing successive
/// values of \f$h\f$ the heights of the high and low waters, together with
/// the times of their occurrence, maybe approximately determined. The
/// harmonic method of predicting tides, therefore, consists essentially of the
/// above formula.
#pragma once
#include <Eigen/Core>
#include <limits>
#include <tuple>
#include <vector>

#include "fes/abstract_tidal_model.hpp"
#include "fes/detail/broadcast.hpp"
#include "fes/detail/thread.hpp"
#include "fes/eigen.hpp"
#include "fes/settings.hpp"
#include "fes/wave.hpp"
#include "fes/wave/long_period_equilibrium.hpp"
#include "fes/wave/table.hpp"

namespace fes {
namespace detail {

/// Build the wave table used for the tidal prediction.
///
/// @tparam T The type of tidal constituents modelled.
/// @param[in] tidal_model The tidal model.
/// @param[in] disable This list specifies tidal constituents to be excluded
/// from the model. Constituents included in this list will be processed through
/// admittance calculations and in the long-period equilibrium wave
/// calculation routine (`lpe_minus_n_waves`).
/// @return The wave table.
template <typename T>
static auto build_wave_table(const AbstractTidalModel<T>* const tidal_model,
                             const std::vector<Constituent>& disable)
    -> wave::Table {
  auto result = wave::Table();

  // Add the constituents provided by the model.
  for (const auto& item : tidal_model->data()) {
    auto& wave = result[item.first];
    auto it = std::find(disable.begin(), disable.end(), item.first);
    // Set the wave as dynamic and not provided by the model only if it is not
    // in the list of disabled constituents.
    if (it == disable.end()) {
      wave->dynamic(true);
      wave->admittance(false);
    }
  }

  // Add the constituents to be be considered as dynamic but not provided by
  // the model.
  for (const auto& item : tidal_model->dynamic()) {
    auto& wave = result[item];
    wave->dynamic(true);
    wave->admittance(false);
  }

  return result;
}

/// Compute the tidal prediction for a given point.
///
/// @tparam T The type of tidal constituents modelled.
/// @param[in] tidal_model The tidal model.
/// @param[in] epoch The number of seconds since 1970-01-01T00:00:00Z.
/// @param[in] leap_seconds The number of leap seconds since
/// 1970-01-01T00:00:00Z.
/// @param[in] longitude The longitude of the point.
/// @param[in] latitude The latitude of the point.
/// @param[in] wave_table The list of tidal constituents used for the tidal
/// prediction.
/// @param[in] long_period Handler to to compute the long-period equilibrium
///   ocean tides.
/// @param[inout] acc The accelerator used to speed up the computation.
/// @return A tuple containing:
/// - The height of the the diurnal and semi-diurnal constituents of the
///   tidal spectrum (same units as the constituents).
/// - The height of the long period wave constituents of the tidal
///   spectrum (same units as the constituents).
/// - The quality of the interpolation (see Quality)
template <typename T>
inline auto evaluate_tide(
    const AbstractTidalModel<T>* const tidal_model, const double epoch,
    const uint16_t leap_seconds, const double longitude, const double latitude,
    wave::Table& wave_table, wave::LongPeriodEquilibrium& long_period,
    Accelerator* acc) -> std::tuple<double, double, Quality> {
  // Update the astronomic angle used to evaluate the tidal constituents.
  const auto& angles = acc->calculate_angle(epoch, leap_seconds);
  // Adjusts nodal corrections to the tidal estimate date.
  wave_table.compute_nodal_corrections(angles);

  // Interpolation, at the requested position, of the waves provided by the
  // model used.
  auto quality =
      tidal_model->interpolate({longitude, latitude}, wave_table, acc);
  // Initialization, depending on the type of tide calculated, of he long
  // period wave constituents of the tidal spectrum
  auto h_long_period = tidal_model->tide_type() == fes::kTide
                           ? long_period.lpe_minus_n_waves(angles, latitude)
                           : 0.0;
  // Calculation of the missing waves of the model by admittance.
  wave_table.admittance();
  // If the point is not defined by the model, the tide is set to NaN.
  if (quality == kUndefined) {
    return {std::numeric_limits<double>::quiet_NaN(), h_long_period, quality};
  }
  auto h = 0.0;
  for (const auto& wave : wave_table) {
    auto phi = wave->vu();
    auto tide = wave->f() * (wave->tide().real() * std::cos(phi) +
                             wave->tide().imag() * std::sin(phi));
    if (wave->admittance() || wave->dynamic()) {
      wave->type() == Wave::kShortPeriod ? h += tide : h_long_period += tide;
    }
  }
  return {h, h_long_period, quality};
}

}  // namespace detail

/// Ocean tide calculation
///
/// @param[in] tidal_model Tidal model used to interpolate the modelized waves
/// @param[in] epoch Date of the tide calculation expressed in number of seconds
/// elapsed since 1970-01-01T00:00:00Z
/// @param[in] leap_seconds Number of leap seconds elapsed since
/// 1970-01-01T00:00:00Z
/// @param[in] longitude Longitude in degrees for the position at which the tide
/// is calculated
/// @param[in] latitude Latitude in degrees for the position at which the tide
/// is calculated
/// @param[in] settings Settings for the tide computation.
/// @param[in] num_threads Number of threads to use for the computation. If 0,
/// the number of threads is automatically determined.
/// @return A tuple that contains:
/// - The height of the the diurnal and semi-diurnal constituents of the
///   tidal spectrum.
/// - The height of the long period wave constituents of the tidal
///   spectrum.
/// - The quality of the tide calculation. Could be kUndefined if the
///   point is not defined by the model, kInterpolated if the model is
///   interpolated, kExtrapolated if the model is extrapolated.
/// @note The units of the returned tide are the same as the units of the
/// constituents loaded in the tidal model.
/// @note Computed height of the diurnal and semi-diurnal constituents is set
/// to nan if no data is available at the given position. the long period wave
/// constituents is always computed because this value does not depend on
/// input grids.
template <typename T>
auto evaluate_tide(const AbstractTidalModel<T>* const tidal_model,
                   const Eigen::Ref<const Eigen::VectorXd>& epoch,
                   const Eigen::Ref<const fes::Vector<uint16_t>>& leap_seconds,
                   const Eigen::Ref<const Eigen::VectorXd>& longitude,
                   const Eigen::Ref<const Eigen::VectorXd>& latitude,
                   const Settings& settings = Settings(),
                   const size_t num_threads = 0)
    -> std::tuple<Eigen::VectorXd, Eigen::VectorXd, Vector<Quality>> {
  // Checks the input parameters
  detail::check_eigen_shape("epoch", epoch, "leap_seconds", leap_seconds,
                            "longitude", longitude, "latitude", latitude);

  // Allocates the result vectors
  auto tide = Eigen::VectorXd(epoch.size());
  auto long_period = Eigen::VectorXd(epoch.size());
  auto quality = Vector<Quality>(epoch.size());

  // Worker responsible for the calculation of the tide at a given position
  auto worker = [&](const int64_t start, const int64_t end) {
    auto acc = std::unique_ptr<Accelerator>(tidal_model->accelerator(
        settings.astronomic_formulae(), settings.time_tolerance()));
    auto* acc_ptr = acc.get();
    auto wave_table =
        detail::build_wave_table(tidal_model, settings.excluded());
    auto lpe = wave::LongPeriodEquilibrium(wave_table);

    for (auto ix = start; ix < end; ++ix) {
      std::tie(tide(ix), long_period(ix), quality(ix)) = detail::evaluate_tide(
          tidal_model, epoch(ix), leap_seconds(ix), longitude(ix), latitude(ix),
          wave_table, lpe, acc_ptr);
    }
  };

  detail::parallel_for(worker, epoch.size(), num_threads);
  return {tide, long_period, quality};
}

}  // namespace fes
