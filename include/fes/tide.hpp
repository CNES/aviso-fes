// Copyright (c) 2026 CNES
//
// All rights reserved. Use of this source code is governed by a
// BSD-style license that can be found in the LICENSE file.
/// @file include/fes/tide.hpp
/// @brief Tide prediction.
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

}  // namespace detail

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

auto evaluate_tide_from_constituents(
    const std::map<ConstituentId, Complex>& constituents,
    const Eigen::Ref<const Eigen::VectorXd>& epoch, const double latitude,
    const boost::optional<Settings>& settings = boost::none)
    -> std::tuple<Eigen::VectorXd, Eigen::VectorXd>;

auto evaluate_equilibrium_long_period(
    const Eigen::Ref<const Eigen::VectorXd>& epoch,
    const Eigen::Ref<const Eigen::VectorXd>& latitude,
    const std::vector<ConstituentId>& constituents,
    const boost::optional<Settings>& settings = boost::none) -> Eigen::VectorXd;

}  // namespace fes
