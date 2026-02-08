// Copyright (c) 2026 CNES
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
#include <boost/none.hpp>
#include <boost/optional.hpp>
#include <limits>

#include "fes/constituent.hpp"
#include "fes/detail/broadcast.hpp"
#include "fes/detail/parallel_for.hpp"
#include "fes/interface/inference.hpp"
#include "fes/interface/tidal_model.hpp"
#include "fes/interface/wave.hpp"
#include "fes/interface/wave_table.hpp"
#include "fes/long_period_equilibrium.hpp"
#include "fes/settings.hpp"

namespace fes {
namespace detail {

#ifndef DOXYGEN_SHOULD_SKIP_THIS
// Compute the tide from the waves provided by the tidal model. This function
// is used internally by the evaluate_tide function after the interpolation of
// the waves from the model.
inline auto evaluate_tide_from_waves(const double epoch, const double latitude,
                                     const bool compute_long_period_equilibrium,
                                     const Settings& settings,
                                     WaveTableInterface& wave_table,
                                     InferenceInterface& inference,
                                     LongPeriodEquilibrium& long_period,
                                     Accelerator& acc)
    -> std::tuple<double, double> {
  // Update the astronomic angle used to evaluate the tidal constituents.
  const auto& angles = acc.calculate_angle(epoch);

  // Adjusts nodal corrections to the tidal estimate date.
  wave_table.compute_nodal_corrections(angles, settings.group_modulations());

  // Initialize long period ocean tides
  auto h_long_period = compute_long_period_equilibrium
                           ? long_period.lpe_minus_n_waves(angles, latitude)
                           : 0.0;

  // Apply the inference to compute minor constituents from the major ones
  // provided by the model.
  inference.apply(wave_table, latitude);

  // Summation of all the tidal constituents to compute the tide
  auto h = 0.0;
  for (const auto& item : wave_table) {
    auto& wave = *item.value();
    auto tide = wave.corrected_tide();
    wave.type() == kShortPeriod ? h += tide : h_long_period += tide;
  }

  return {h, h_long_period};
}

// Compute the tide at a given position and time, using the provided
// tidal model
template <typename T>
inline auto evaluate_tide(const TidalModelInterface<T>& tidal_model,
                          const double epoch, const double longitude,
                          const double latitude, const Settings& settings,
                          WaveTableInterface& wave_table,
                          InferenceInterface& inference,
                          LongPeriodEquilibrium& long_period, Accelerator& acc)
    -> std::tuple<double, double, Quality> {
  const auto compute_long_period_equilibrium =
      tidal_model.tide_type() == fes::kTide &&
      settings.compute_long_period_equilibrium();

  // Interpolation, at the requested position, of the waves provided by the
  // model used.
  auto quality =
      tidal_model.interpolate({longitude, latitude}, wave_table, acc);

  // If the point is not defined by the model, we still compute the long
  // period equilibrium (it doesn't depend on the model) but set tide and long
  // period to NaN.
  if (quality == kUndefined) {
    double long_period_equilibrium = 0.0;
    if (compute_long_period_equilibrium) {
      auto& angles = acc.calculate_angle(epoch);
      wave_table.compute_nodal_corrections(angles,
                                           settings.group_modulations());
      long_period_equilibrium = long_period.lpe_minus_n_waves(angles, latitude);
    }
    return {std::numeric_limits<double>::quiet_NaN(), long_period_equilibrium,
            quality};
  }
  auto tides = evaluate_tide_from_waves(
      epoch, latitude, compute_long_period_equilibrium, settings, wave_table,
      inference, long_period, acc);
  return {std::get<0>(tides), std::get<1>(tides), quality};
}
#endif  // DOXYGEN_SHOULD_SKIP_THIS

}  // namespace detail

/// Ocean tide calculation
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
///     position using `N` data points (where `N` is the
///     number of data points used for the interpolation).
///   - <b>Negative values</b>: the tide is extrapolated at the given
///     position using `-N` data points (where `N` is the
///     number of data points used for the extrapolation).
/// @note The units of the returned tide are the same as the units of the
/// constituents loaded in the tidal model.
/// @note Computed height of the diurnal and semi-diurnal constituents is set
/// to nan if no data is available at the given position. the long period wave
/// constituents is always computed because this value does not depend on
/// model data.
/// @warning If Settings::compute_long_period_equilibrium returns true, the
/// tidal model must use centimeters. The long period equilibrium is computed
/// in centimeters and is added to the model tide; mixing units would make the
/// result inconsistent.
template <typename T>
auto evaluate_tide(const TidalModelInterface<T>* const tidal_model,
                   const Eigen::Ref<const Eigen::VectorXd>& epoch,
                   const Eigen::Ref<const Eigen::VectorXd>& longitude,
                   const Eigen::Ref<const Eigen::VectorXd>& latitude,
                   const boost::optional<Settings>& settings = boost::none)
    -> std::tuple<Eigen::VectorXd, Eigen::VectorXd, Vector<Quality>> {
  // Checks the input parameters
  detail::check_eigen_shape("epoch", epoch, "longitude", longitude, "latitude",
                            latitude);

  // Use default settings if not provided
  auto settings_instance = settings.value_or(Settings{});

  // Allocates the result vectors
  auto tide = Eigen::VectorXd(epoch.size());
  auto long_period = Eigen::VectorXd(epoch.size());
  auto quality = Vector<Quality>(epoch.size());

  // Worker function to evaluate the tide for a chunk of data. This will be
  // executed in parallel across multiple threads.
  auto worker = [&](const int64_t start, const int64_t end) -> void {
    // For each thread, we create a new instance of the wave table, inference,
    // and accelerator
    auto acc_ptr = std::unique_ptr<Accelerator>(
        tidal_model->accelerator(settings_instance.astronomic_formulae(),
                                 settings_instance.time_tolerance()));
    auto wt_ptr = tidal_model->wave_table(settings_instance.engine_type());
    auto inference_ptr =
        inference_factory(*wt_ptr, settings_instance.inference_type());

    // For optimization purposes, we capture the dereferenced pointers to avoid
    // unnecessary pointer dereferencing in the inner loop.
    const auto& tidal_model_instance = *tidal_model;
    auto& inference_instance = *inference_ptr;
    auto& acc_instance = *acc_ptr;
    auto& wt_instance = *wt_ptr;

    // Initialize the long period equilibrium handler for this thread
    auto lpe = LongPeriodEquilibrium(wt_instance);

    // Evaluate the tide for each point in the assigned chunk
    for (auto ix = start; ix < end; ++ix) {
      std::tie(tide(ix), long_period(ix), quality(ix)) =
          detail::evaluate_tide(tidal_model_instance, epoch(ix), longitude(ix),
                                latitude(ix), settings_instance, wt_instance,
                                inference_instance, lpe, acc_instance);
    }
  };

  detail::parallel_for(worker, epoch.size(), settings_instance.num_threads());
  return {tide, long_period, quality};
}

/// @brief Compute the tide from a set of tidal constituents.
///
/// @brief Compute the ocean tide from a list of known tidal constituents.
///
/// Unlike the other evaluate_tide overload which interpolates constituents from
/// a tidal model, this function computes the tidal prediction directly from a
/// list of tidal constituents whose properties (amplitude and phase) are known.
/// This is typically used for tide gauge analysis and prediction, where the
/// constituents have been previously determined from harmonic analysis of
/// observed sea level data.
///
/// @param[in] constituents A map of tidal constituents, where the key is the
/// constituent identifier and the value is a complex number representing the
/// amplitude (magnitude) and phase (argument) of the constituent.
/// @param[in] epoch Date of the tide calculation expressed in number of seconds
/// elapsed since 1970-01-01T00:00:00Z.
/// @param[in] latitude Latitude in degrees for the position at which the tide
/// is calculated. This is used to compute the long period equilibrium tide if
/// the settings indicate that it should be included in the result.
/// @param[in] settings Settings for the tide computation, including whether to
/// compute the long period equilibrium tide and the astronomical formulae to
/// use.
/// @return A tuple that contains:
/// - The height of the diurnal and semi-diurnal constituents of the tidal
///   spectrum for each epoch.
/// - The height of the long period wave constituents of the tidal spectrum
///   for each epoch.
/// @note The units of the returned tide are the same as the units of the input
/// constituents.
/// @warning If Settings::compute_long_period_equilibrium returns true, the
/// input constituents must use centimeters. The long period equilibrium is
/// computed in centimeters and is added to the constituent tide; mixing units
/// would make the result inconsistent.
auto evaluate_tide_from_constituents(
    const std::map<ConstituentId, Complex>& constituents,
    const Eigen::Ref<const Eigen::VectorXd>& epoch, const double latitude,
    const boost::optional<Settings>& settings = boost::none)
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
/// @param[in] epoch The number of seconds since 1970-01-01T00:00:00Z.
/// @param[in] latitude Latitude in degrees (positive north) for the position at
/// which tide is computed.
/// @param[in] constituents A list of tidal constituents to exclude from the
/// long period equilibrium tide computation. This is typically used to exclude
/// the tidal constituents that are already included in the model tide to avoid
/// double counting. If empty, all constituents from the Cartwright-Tayler-Edden
/// tables are included in the long period equilibrium tide computation.
/// @param[in] settings Settings for the tide computation.
/// @return The height of the long period equilibrium ocean tide at the given
/// epoch and latitude in centimeters.
/// @note Cartwright & Tayler, Geophys. J. R.A.S., 23, 45, 1971.
/// @note Cartwright & Edden, Geoph. J. R.A.S., 33, 253, 1973.
/// @note Tamura Y., Bull. d'information des marees terrestres, Vol. 99, 1987.
auto evaluate_equilibrium_long_period(
    const Eigen::Ref<const Eigen::VectorXd>& epoch,
    const Eigen::Ref<const Eigen::VectorXd>& latitude,
    const std::vector<ConstituentId>& constituents,
    const boost::optional<Settings>& settings = boost::none) -> Eigen::VectorXd;

}  // namespace fes
