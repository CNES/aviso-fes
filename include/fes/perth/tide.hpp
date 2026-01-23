#pragma once

#include <boost/optional.hpp>
#include <tuple>

#include "fes/abstract_tidal_model.hpp"
#include "fes/angle/astronomic.hpp"
#include "fes/detail/broadcast.hpp"
#include "fes/detail/thread.hpp"
#include "fes/perth/constituent.hpp"
#include "fes/perth/inference.hpp"
#include "fes/perth/table.hpp"
#include "fes/settings.hpp"

namespace fes {
namespace perth {
namespace detail {

template <typename T>
auto evaluate_tide(const double lon, const double lat, const double time,
                   const bool group_modulations, WaveTable& wave_table,
                   Inference* inference,
                   AbstractTidalModel<T, Constituent>* tidal_model,
                   Accelerator<Constituent>* acc)
    -> std::tuple<double, double, Quality> {
  // Interpolation, at the requested position, of the waves provided by the
  // model used.
  auto quality = tidal_model->interpolate({lon, lat}, wave_table, inference);
  if (quality == kUndefined) {
    // If the interpolation failed, return undefined values.
    return {std::numeric_limits<double>::quiet_NaN(),
            std::numeric_limits<double>::quiet_NaN(), quality};
  }
  if (inference) {
    // If an inference is provided, use it to fill the tide table.
    (*inference)(wave_table, lat);
  }
  wave_table.update_nodal_corrections(time, group_modulations, *acc);

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
    if (wave.second.type == ConstituentType::kLongPeriod) {
      tide_lp += h;
    } else {
      tide += h;
    }
  }
  return {tide, tide_lp, quality};
}

}  // namespace detail

/// @brief Settings for Perth tide computation.
/// This extends the general FES settings with Perth-specific options.
class Settings : public fes::Settings {
 public:
  /// @brief Construct a Settings object for Perth tide computation.
  /// @param[in] astronomic_formulae Astronomic formulae used to calculate the
  /// astronomic angles.
  /// @param[in] time_tolerance The time in seconds during which astronomical
  /// angles are considered constant. The default value is 0 seconds, indicating
  /// that astronomical angles do not remain constant with time.
  /// @param[in] group_modulations Whether to use group modulation nodal
  /// corrections.
  /// @param[in] inference_type The type of inference interpolation to use. If
  /// boost::none, no inference interpolation is used.
  explicit Settings(
      const angle::Formulae& astronomic_formulae =
          angle::Formulae::kSchuremanOrder1,
      const double time_tolerance = 0.0, const bool group_modulations = false,
      boost::optional<InterpolationType> inference_type = boost::none)
      : fes::Settings(astronomic_formulae, time_tolerance),
        group_modulations_(group_modulations),
        inference_type_(inference_type) {}

  /// @brief Returns whether to use group modulation nodal corrections.
  ///
  /// @return True if group modulation nodal corrections are to be used.
  constexpr auto group_modulations() const noexcept -> bool {
    return group_modulations_;
  }

  /// @brief Returns the type of inference interpolation to use, if any.
  ///
  /// @return The type of inference interpolation to use, or boost::none if
  ///         no inference interpolation is to be used.
  constexpr auto inference() const noexcept
      -> const boost::optional<InterpolationType>& {
    return inference_type_;
  }

 private:
  /// Whether to use group modulation nodal corrections
  bool group_modulations_{false};
  /// Type of inference interpolation to use
  boost::optional<InterpolationType> inference_type_{boost::none};
};

template <typename T>
auto evaluate_tide(const AbstractTidalModel<T, Constituent>* const tidal_model,
                   const Eigen::Ref<const Eigen::VectorXd>& epoch,
                   const Eigen::Ref<const Eigen::VectorXd>& longitude,
                   const Eigen::Ref<const Eigen::VectorXd>& latitude,
                   const Settings& settings = Settings(),
                   const size_t num_threads = 0)
    -> std::tuple<Eigen::VectorXd, Eigen::VectorXd, Vector<Quality>> {
  // Checks the input parameters
  fes::detail::check_eigen_shape("epoch", epoch, "longitude", longitude,
                                 "latitude", latitude);
  // Allocates the result vectors
  auto tide = Eigen::VectorXd(epoch.size());
  auto long_period = Eigen::VectorXd(epoch.size());
  auto quality = Vector<Quality>(epoch.size());

  auto worker = [&](const int64_t start, const int64_t end) {
    auto wave_table = WaveTable(tidal_model->identifiers());
    auto inference = std::unique_ptr<Inference>(
        settings.inference().has_value()
            ? new Inference(wave_table, settings.inference().value())
            : nullptr);
    auto* inference_ptr = inference.get();
    auto* acc = tidal_model->accelerator();
    for (auto ix = start; ix < end; ++ix) {
      std::tie(tide(ix), long_period(ix), quality(ix)) = detail::evaluate_tide(
          longitude[ix], latitude[ix], epoch[ix], settings.group_modulations(),
          wave_table, inference_ptr, tidal_model, acc);
    }
  };

  fes::detail::parallel_for(worker, epoch.size(), num_threads);
  return std::make_tuple(tide, long_period, quality);
}

}  // namespace perth
}  // namespace fes
