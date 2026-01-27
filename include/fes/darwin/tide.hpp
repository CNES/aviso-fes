// Copyright (c) 2026 CNES
//
// All rights reserved. Use of this source code is governed by a
// BSD-style license that can be found in the LICENSE file.

/// @file include/fes/darwin/tide.hpp
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

#include "fes/abstract_tidal_model.hpp"
#include "fes/darwin/long_period_equilibrium.hpp"
#include "fes/darwin/table.hpp"
#include "fes/detail/broadcast.hpp"
#include "fes/detail/thread.hpp"
#include "fes/settings.hpp"
#include "fes/types.hpp"

namespace fes {
namespace darwin {
namespace detail {

/// Build the wave table used for the tidal prediction.
///
/// @tparam T The type of tidal constituents modelled.
/// @param[in] tidal_model The tidal model.
/// @return The wave table.
template <typename T>
static auto build_wave_table(const AbstractTidalModel<T>* const tidal_model)
    -> WaveTable {
  auto result = WaveTable();

  // Add the constituents provided by the model.
  for (const auto& item : tidal_model->data()) {
    auto& wave = result[item.first];
    wave->dynamic(true);
    wave->admittance(false);
  }

  // Add the constituents to be be considered as dynamic but not provided by
  // the model.
  for (const auto& item : tidal_model->dynamic_ids()) {
    auto& wave = result[item];
    wave->dynamic(true);
    wave->admittance(false);
  }

  return result;
}

/// Compute tide for a single epoch from a prepared wave table
///
/// This function handles the core tide computation logic that is common
/// to both model-based and constituent-based tide calculations.
///
/// @param[inout] wave_table Wave table with tide data already set
/// @param[inout] lpe Long period equilibrium handler
/// @param[inout] acc Accelerator for angle calculation
/// @param[in] epoch Time in seconds since 1970-01-01T00:00:00Z
/// @param[in] latitude Latitude in degrees for long period calculation
/// @param[in] compute_lpe Whether to compute long period equilibrium
/// @return Tuple of (short_period_tide, long_period_tide)
static inline auto compute_tide_from_waves(WaveTable& wave_table,
                                           LongPeriodEquilibrium& lpe,
                                           Accelerator& acc, const double epoch,
                                           const double latitude,
                                           const bool compute_lpe = true)
    -> std::tuple<double, double> {
  // Update the astronomic angle used to evaluate the tidal constituents.
  const auto& angles = acc.calculate_angle(epoch);

  // Adjusts nodal corrections to the tidal estimate date.
  wave_table.compute_nodal_corrections(angles);

  // Initialize long period equilibrium ocean tides
  auto h_long_period =
      compute_lpe ? lpe.lpe_minus_n_waves(angles, latitude) : 0.0;

  // Calculation of the missing waves of the model by admittance.
  wave_table.admittance();

  // Summation of all the tidal constituents to compute the tide
  auto h = 0.0;
  for (const auto& wave : wave_table) {
    auto phi = wave->vu();
    auto tide = wave->f() * (wave->tide().real() * std::cos(phi) +
                             wave->tide().imag() * std::sin(phi));
    if (wave->admittance() || wave->dynamic()) {
      wave->type() == kShortPeriod ? h += tide : h_long_period += tide;
    }
  }

  return {h, h_long_period};
}

/// Compute the tidal prediction for a given point.
///
/// @tparam T The type of tidal constituents modelled.
/// @param[in] tidal_model The tidal model.
/// @param[in] epoch The number of seconds since 1970-01-01T00:00:00Z.
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
inline auto evaluate_tide(const AbstractTidalModel<T>* const tidal_model,
                          const double epoch, const double longitude,
                          const double latitude, WaveTable& wave_table,
                          LongPeriodEquilibrium& long_period, Accelerator* acc)
    -> std::tuple<double, double, Quality> {
  // Interpolation, at the requested position, of the waves provided by the
  // model used.
  auto quality =
      tidal_model->interpolate({longitude, latitude}, wave_table, acc);

  // If the point is not defined by the model, we still compute the long
  // period equilibrium (it doesn't depend on the model) but set tide to NaN.
  if (quality == kUndefined) {
    // We need to compute angles and long period for undefined points
    const auto& angles = acc->calculate_angle(epoch);
    wave_table.compute_nodal_corrections(angles);
    auto h_long_period = tidal_model->tide_type() == fes::kTide
                             ? long_period.lpe_minus_n_waves(angles, latitude)
                             : 0.0;
    return {std::numeric_limits<double>::quiet_NaN(), h_long_period, quality};
  }

  // Compute the tide using the common helper function
  const auto compute_lpe = tidal_model->tide_type() == fes::kTide;
  auto tides = compute_tide_from_waves(wave_table, long_period, *acc, epoch,
                                       latitude, compute_lpe);

  return {std::get<0>(tides), std::get<1>(tides), quality};
}

}  // namespace detail

/// @copydoc fes::evaluate_tide
template <typename T>
auto evaluate_tide(const AbstractTidalModel<T>* const tidal_model,
                   const Eigen::Ref<const Eigen::VectorXd>& epoch,
                   const Eigen::Ref<const Eigen::VectorXd>& longitude,
                   const Eigen::Ref<const Eigen::VectorXd>& latitude,
                   const Settings& settings = FesRuntimeSettings())
    -> std::tuple<Eigen::VectorXd, Eigen::VectorXd, Vector<Quality>> {
  // Checks the input parameters
  fes::detail::check_eigen_shape("epoch", epoch, "longitude", longitude,
                                 "latitude", latitude);

  // Allocates the result vectors
  auto tide = Eigen::VectorXd(epoch.size());
  auto long_period = Eigen::VectorXd(epoch.size());
  auto quality = Vector<Quality>(epoch.size());

  // Worker responsible for the calculation of the tide at a given position
  auto worker = [&](const int64_t start, const int64_t end) {
    auto acc = std::unique_ptr<Accelerator>(tidal_model->accelerator(
        settings.astronomic_formulae(), settings.time_tolerance()));
    auto* acc_ptr = acc.get();
    auto wave_table = detail::build_wave_table(tidal_model);
    auto lpe = LongPeriodEquilibrium(wave_table);

    for (auto ix = start; ix < end; ++ix) {
      std::tie(tide(ix), long_period(ix), quality(ix)) =
          detail::evaluate_tide(tidal_model, epoch(ix), longitude(ix),
                                latitude(ix), wave_table, lpe, acc_ptr);
    }
  };

  fes::detail::parallel_for(worker, epoch.size(), settings.num_threads());
  return {tide, long_period, quality};
}

/// @brief Compute the ocean tide from a list of known tidal constituents.
///
/// Unlike the other evaluate_tide overload which interpolates constituents from
/// a tidal model, this function computes the tidal prediction directly from a
/// list of tidal constituents whose properties (amplitude and phase) are known.
/// This is typically used for tide gauge analysis and prediction, where the
/// constituents have been previously determined from harmonic analysis of
/// observed sea level data.
///
/// @param[in] constituents A map of tidal constituents with their
/// complex cartesian equivalent to the polar representation (amplitude and
/// phase).
/// @param[in] epoch Date of the tide calculation expressed in number of seconds
/// elapsed since 1970-01-01T00:00:00Z (can be a vector of multiple times).
/// @param[in] latitude Latitude in degrees for the position.
/// @param[in] settings Settings for the tide computation.
/// @return A tuple containing:
/// - The height of the diurnal and semi-diurnal constituents of the tidal
///   spectrum for each epoch.
/// - The height of the long period wave constituents of the tidal spectrum
///   for each epoch.
/// @note The units of the returned tide are the same as the units of the input
/// constituents.
auto evaluate_tide_from_constituents(
    const std::map<Constituent, Complex>& constituents,
    const Eigen::Ref<const Eigen::VectorXd>& epoch, const double latitude,
    const Settings& settings) -> std::tuple<Eigen::VectorXd, Eigen::VectorXd>;

/// @brief Compute the long period equilibrium ocean tides.
///
/// The complete tidal spectral lines from the Cartwright-Tayler-Edden tables
/// are summed over to compute the long-period tide.
/// @n
/// Order 2 and order 3 of the tidal potential for the long period waves is
/// now taken into account.
/// @n
/// The decomposition was validated compared to the potential proposed by
/// Tamura.
/// @n
/// Technical references:
///   - Cartwright & Tayler, Geophys. J. R.A.S., 23, 45, 1971.
///   - Cartwright & Edden, Geophys. J. R.A.S., 33, 253, 1973.
///   - Tamura Y., Bull. d'information des marees terrestres, Vol. 99, 1987.
/// @param[in] epoch The number of seconds since 1970-01-01T00:00:00Z.
/// @param[in] latitude Latitude in degrees (positive north) for the position at
/// which tide is computed.
/// @param[in] settings Settings for the tide computation.
auto evaluate_equilibrium_long_period(
    const Eigen::Ref<const Eigen::VectorXd>& epoch,
    const Eigen::Ref<const Eigen::VectorXd>& latitude,
    const Settings& settings = FesRuntimeSettings()) -> Eigen::VectorXd;

}  // namespace darwin
}  // namespace fes
