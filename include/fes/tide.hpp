// Copyright (c) 2025 CNES
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
/// @return The wave table.
template <typename T>
static auto build_wave_table(const AbstractTidalModel<T>* const tidal_model)
    -> wave::Table {
  auto result = wave::Table();

  // Add the constituents provided by the model.
  for (const auto& item : tidal_model->data()) {
    auto& wave = result[item.first];
    wave->dynamic(true);
    wave->admittance(false);
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

/// Build wave table from known constituents
///
/// @param[in] constituents A map of tidal constituents with their
/// complex-valued (amplitude and phase) properties.
/// @return The wave table.
static inline auto build_wave_table_from_constituents(
    const std::map<Constituent, std::complex<double>>& constituents)
    -> wave::Table {
  auto wave_table = wave::Table();
  for (const auto& item : constituents) {
    auto& wave = wave_table[item.first];
    wave->dynamic(true);
    wave->admittance(false);
    wave->tide(item.second);
  }
  return wave_table;
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
/// @param[in] leap_seconds Leap seconds at this epoch
/// @param[in] latitude Latitude in degrees for long period calculation
/// @param[in] compute_lpe Whether to compute long period equilibrium
/// @return Tuple of (short_period_tide, long_period_tide)
static inline auto compute_tide_from_waves(
    wave::Table& wave_table, wave::LongPeriodEquilibrium& lpe, Accelerator& acc,
    const double epoch, const uint16_t leap_seconds, const double latitude,
    const bool compute_lpe = true) -> std::tuple<double, double> {
  // Update the astronomic angle used to evaluate the tidal constituents.
  const auto& angles = acc.calculate_angle(epoch, leap_seconds);

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
      wave->type() == Wave::kShortPeriod ? h += tide : h_long_period += tide;
    }
  }

  return {h, h_long_period};
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
inline auto evaluate_tide(const AbstractTidalModel<T>* const tidal_model,
                          const double epoch, const uint16_t leap_seconds,
                          const double longitude, const double latitude,
                          wave::Table& wave_table,
                          wave::LongPeriodEquilibrium& long_period,
                          Accelerator* acc)
    -> std::tuple<double, double, Quality> {
  // Interpolation, at the requested position, of the waves provided by the
  // model used.
  auto quality =
      tidal_model->interpolate({longitude, latitude}, wave_table, acc);

  // If the point is not defined by the model, we still compute the long
  // period equilibrium (it doesn't depend on the model) but set tide to NaN.
  if (quality == kUndefined) {
    // We need to compute angles and long period for undefined points
    const auto& angles = acc->calculate_angle(epoch, leap_seconds);
    wave_table.compute_nodal_corrections(angles);
    auto h_long_period = tidal_model->tide_type() == fes::kTide
                             ? long_period.lpe_minus_n_waves(angles, latitude)
                             : 0.0;
    return {std::numeric_limits<double>::quiet_NaN(), h_long_period, quality};
  }

  // Compute the tide using the common helper function
  const auto compute_lpe = tidal_model->tide_type() == fes::kTide;
  auto tides = compute_tide_from_waves(wave_table, long_period, *acc, epoch,
                                       leap_seconds, latitude, compute_lpe);

  return {std::get<0>(tides), std::get<1>(tides), quality};
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
/// - The quality flag indicating the reliability of the tide
///   calculation at the given position:
///   - <b>0</b>: the tide is undefined (no data available at the given
///     position).
///   - <b>Positive values</b>: the tide is interpolated at the given
///     position using <pre>N</pre> data points (where <pre>N</pre> is the
///     number of data points used for the interpolation).
///   - <b>Negative values</b>: the tide is extrapolated at the given
///     position using <pre>-N</pre> data points (where <pre>N</pre> is the
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
    auto wave_table = detail::build_wave_table(tidal_model);
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
/// complex-valued (amplitude and phase) properties.
/// @param[in] epoch Date of the tide calculation expressed in number of seconds
/// elapsed since 1970-01-01T00:00:00Z (can be a vector of multiple times).
/// @param[in] leap_seconds Number of leap seconds elapsed since
/// 1970-01-01T00:00:00Z (corresponding to each epoch value).
/// @param[in] longitude Longitude in degrees for the position.
/// @param[in] latitude Latitude in degrees for the position.
/// @param[in] settings Settings for the tide computation.
/// @param[in] num_threads Number of threads to use for the computation. If 0,
/// the number of threads is automatically determined.
/// @return A tuple containing:
/// - The height of the diurnal and semi-diurnal constituents of the tidal
///   spectrum for each epoch.
/// - The height of the long period wave constituents of the tidal spectrum
///   for each epoch.
/// @note The units of the returned tide are the same as the units of the input
/// constituents.
auto evaluate_tide_from_constituents(
    const std::map<Constituent, std::complex<double>>& constituents,
    const Eigen::Ref<const Eigen::VectorXd>& epoch,
    const Eigen::Ref<const fes::Vector<uint16_t>>& leap_seconds,
    const double longitude, const double latitude,
    const Settings& settings = Settings(), const size_t num_threads = 0)
    -> std::tuple<Eigen::VectorXd, Eigen::VectorXd>;

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
/// @param[in] leap_seconds The number of leap seconds since
/// 1970-01-01T00:00:00Z.
/// @param[in] latitude Latitude in degrees (positive north) for the position at
/// which tide is computed.
/// @param[in] settings Settings for the tide computation.
/// @param[in] num_threads Number of threads to use for the computation. If 0,
/// the number of threads is automatically determined.
auto evaluate_equilibrium_long_period(
    const Eigen::Ref<const Eigen::VectorXd>& epoch,
    const Eigen::Ref<const fes::Vector<uint16_t>>& leap_seconds,
    const Eigen::Ref<const Eigen::VectorXd>& latitude,
    const Settings& settings = Settings(), const size_t num_threads = 0)
    -> Eigen::VectorXd;

}  // namespace fes
