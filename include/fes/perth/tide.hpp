#pragma once

#include <boost/optional.hpp>
#include <tuple>

#include "fes/abstract_tidal_model.hpp"
#include "fes/detail/broadcast.hpp"
#include "fes/detail/thread.hpp"
#include "fes/geometry/point.hpp"
#include "fes/perth/inference.hpp"
#include "fes/perth/table.hpp"
#include "fes/settings.hpp"

namespace fes {
namespace perth {
namespace detail {

/// Compute the ocean tide from the wave table.
///
/// @param[in,out] wave_table Wave table with tide data already set.
/// @param[in,out] inference Optional inference object to modify the wave table.
/// @param[in,out] acc Accelerator used for angle calculations.
/// @param[in] lat Latitude in degrees.
/// @param[in] time Time in seconds since epoch.
/// @param[in] group_modulations Whether to group modulations.
/// @return A tuple containing the short period tide and long period tide.
inline auto compute_tide_from_waves(WaveTable& wave_table, Inference* inference,
                                    Accelerator& acc, const double lat,
                                    const double time,
                                    const bool group_modulations)
    -> std::tuple<double, double> {
  if (inference) {
    // If an inference is provided, use it to fill the tide table.
    (*inference)(wave_table, lat);
  }
  wave_table.update_nodal_corrections(time, group_modulations, acc);

  double tide = 0.0;
  double tide_lp = 0.0;
  // Sum over all constituents to compute the final tide.
  for (size_t ix = 0; ix < wave_table.size(); ++ix) {
    const auto& wave = wave_table.at(ix);
    const auto& nodal_correction = wave_table.nodal_correction(wave.first);
    const auto& constituent_tide = wave.second.tide;
    auto x = fes::detail::math::radians(wave.second.tidal_argument +
                                        nodal_correction.u);
    auto h = constituent_tide.real() * nodal_correction.f * std::cos(x) +
             constituent_tide.imag() * nodal_correction.f * std::sin(x);
    if (wave.second.type == kLongPeriod) {
      tide_lp += h;
    } else {
      tide += h;
    }
  }
  return {tide, tide_lp};
}

template <typename T>
auto evaluate_tide(const double lon, const double lat, const double time,
                   const bool group_modulations, WaveTable& wave_table,
                   Inference* inference,
                   const AbstractTidalModel<T>* tidal_model, Accelerator* acc)
    -> std::tuple<double, double, Quality> {
  // Interpolation, at the requested position, of the waves provided by the
  // model used.
  auto quality =
      tidal_model->interpolate(geometry::Point{lon, lat}, wave_table, acc);
  if (quality == kUndefined) {
    // If the interpolation failed, return undefined values.
    return {std::numeric_limits<double>::quiet_NaN(),
            std::numeric_limits<double>::quiet_NaN(), quality};
  }
  auto tides = compute_tide_from_waves(wave_table, inference, *acc, lat, time,
                                       group_modulations);
  return {std::get<0>(tides), std::get<1>(tides), quality};
}

}  // namespace detail

template <typename T>
auto evaluate_tide(const AbstractTidalModel<T>* const tidal_model,
                   const Eigen::Ref<const Eigen::VectorXd>& epoch,
                   const Eigen::Ref<const Eigen::VectorXd>& longitude,
                   const Eigen::Ref<const Eigen::VectorXd>& latitude,
                   const Settings& settings = PerthRuntimeSettings())
    -> std::tuple<Eigen::VectorXd, Eigen::VectorXd, Vector<Quality>> {
  // Checks the input parameters
  fes::detail::check_eigen_shape("epoch", epoch, "longitude", longitude,
                                 "latitude", latitude);
  // Allocates the result vectors
  auto tide = Eigen::VectorXd(epoch.size());
  auto long_period = Eigen::VectorXd(epoch.size());
  auto quality = Vector<Quality>(epoch.size());

  auto worker = [&](const int64_t start, const int64_t end) {
    auto ids = std::vector<Constituent>{};
    ids.reserve(tidal_model->size());
    for (const auto& id : tidal_model->identifiers()) {
      ids.push_back(static_cast<Constituent>(id));
    }
    auto wave_table = WaveTable(ids);
    auto inference = std::unique_ptr<Inference>(
        settings.inference_type() == InterpolationType::kZeroAdmittance
            ? nullptr
            : new Inference(wave_table, settings.inference_type()));
    auto* inference_ptr = inference.get();
    auto acc = std::unique_ptr<Accelerator>(tidal_model->accelerator(
        settings.astronomic_formulae(), settings.time_tolerance()));
    auto* acc_ptr = acc.get();
    for (auto ix = start; ix < end; ++ix) {
      std::tie(tide(ix), long_period(ix), quality(ix)) = detail::evaluate_tide(
          longitude[ix], latitude[ix], epoch[ix], settings.group_modulations(),
          wave_table, inference_ptr, tidal_model, acc_ptr);
    }
  };

  fes::detail::parallel_for(worker, epoch.size(), settings.num_threads());
  return std::make_tuple(tide, long_period, quality);
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

}  // namespace perth
}  // namespace fes
