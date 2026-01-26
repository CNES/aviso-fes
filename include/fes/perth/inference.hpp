#pragma once

#include <complex>
#include <cstdint>
#include <unordered_map>
#include <utility>
#include <vector>

#include "fes/detail/math.hpp"
#include "fes/perth/table.hpp"
#include "fes/settings.hpp"

namespace fes {
namespace perth {

/// @brief Inference class for computing minor tidal constituents from major
/// ones.
///
/// This class implements admittance interpolation to infer the values of
/// minor tidal constituents from the major constituents (Q1, O1, K1 for
/// diurnal; N2, M2, S2 for semidiurnal; Node, Mm, Mf for long-period).
class Inference {
 public:
  /// @brief Construct an Inference object.
  ///
  /// @param[in] components The constituent table to use for frequency
  ///   calculation.
  /// @param[in] inference_type The type of interpolation to use.
  Inference(const WaveTable& components,
            const InterpolationType interpolation_type =
                InterpolationType::kLinearAdmittance);

  /// @brief Apply inference to compute minor constituents.
  ///
  /// @param[in,out] constituent_table The constituent table to update.
  /// @param[in] lat Latitude in degrees (used for node tide calculation).
  auto operator()(WaveTable& constituent_table, const double lat = 0) const
      -> void;

 private:
  /// @brief Type for interpolation functions.
  /// The function takes in the frequencies and amplitudes of three components
  /// and returns a complex number representing the interpolated value.
  using Interpolator = std::function<std::complex<double>(
      double, const std::complex<double>&, double, const std::complex<double>&,
      double, const std::complex<double>&, double)>;

  /// Inferred diurnal constituents with their frequencies.
  static std::unordered_map<Constituent, double> kInferredDiurnalConstituents_;

  /// Inferred semidiurnal constituents with their frequencies.
  static std::unordered_map<Constituent, double>
      kInferredSemidiurnalConstituents_;

  /// Inferred long-period constituents with their frequencies.
  static std::unordered_map<Constituent, double>
      kInferredLongPeriodConstituents_;

  /// Inferred diurnal constituents with their (frequency, amplitude) pairs.
  std::unordered_map<Constituent, std::pair<double, double>> inferred_diurnal_;

  /// Inferred semidiurnal constituents with their (frequency, amplitude) pairs.
  std::unordered_map<Constituent, std::pair<double, double>>
      inferred_semidiurnal_;

  /// Inferred long-period constituents with their (frequency, amplitude) pairs.
  std::unordered_map<Constituent, std::pair<double, double>>
      inferred_long_period_;

  /// Keys for inferred constituents ordered by frequency.
  std::vector<Constituent> diurnal_keys_;

  /// Keys for inferred constituents ordered by frequency.
  std::vector<Constituent> semidiurnal_keys_;

  /// Keys for inferred constituents ordered by frequency.
  std::vector<Constituent> long_period_keys_;

  double x1_;    ///< Frequency for the first component.
  double x2_;    ///< Frequency for the second component.
  double x3_;    ///< Frequency for the third component.
  double x4_;    ///< Frequency for the first component (semidurnal).
  double x5_;    ///< Frequency for the second component (semidurnal).
  double x6_;    ///< Frequency for the third component (semidurnal).
  double x7_;    ///< Frequency for the first component (long-period).
  double x8_;    ///< Frequency for the second component (long-period).
  double x9_;    ///< Frequency for the third component (long-period).
  double amp1_;  ///< Amplitude for the first component.
  double amp2_;  ///< Amplitude for the second component.
  double amp3_;  ///< Amplitude for the third component.
  double amp4_;  ///< Amplitude for the first component (semidurnal).
  double amp5_;  ///< Amplitude for the second component (semidurnal).
  double amp6_;  ///< Amplitude for the third component (semidurnal).
  double amp7_;  ///< Amplitude for the first component (long-period).
  double amp8_;  ///< Amplitude for the second component (long-period).
  double amp9_;  ///< Amplitude for the third component (long-period).

  Interpolator interpolation_1_;  ///< Interpolation function for diurnal.
  Interpolator interpolation_2_;  ///< Interpolation function for semidiurnal.

  /// @brief Returns inphase/quad components of the 18.6-y equilibrium node
  /// tide. This is used only if inference is requested but the node tide is
  /// missing.
  /// @param[in,out] node The TideComponent for the node constituent.
  /// @param[in] lat The latitude for the computation.
  auto evaluate_node_tide(Wave& node, const double lat) const
      -> const std::complex<double>& {
    if (node.is_inferred) {
      constexpr auto gamma2 = 0.682;
      constexpr auto amplitude = 0.0279;  // m
      auto p20 = 0.5 - 1.5 * detail::math::pow<2>(
                                 std::sin(detail::math::radians(lat)));
      auto xi = gamma2 * p20 * std::sqrt(1.25 / detail::math::pi<double>());
      node.tide = std::complex<double>(xi * amplitude, 0.0);
    }
    return node.tide;
  }
};

}  // namespace perth
}  // namespace fes
