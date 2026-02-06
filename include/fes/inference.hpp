// Copyright (c) 2026 CNES
//
// All rights reserved. Use of this source code is governed by a
// BSD-style license that can be found in the LICENSE file.
/// @file include/fes/darwin/inference.hpp
/// @brief Table of tidal constituents handled by FES models.
#pragma once

#include <array>
#include <vector>

#include "fes/constituent.hpp"
#include "fes/interface/inference.hpp"
#include "fes/interface/wave.hpp"
#include "fes/interface/wave_table.hpp"
#include "fes/map.hpp"
#include "fes/perth/doodson.hpp"
#include "fes/perth/love_numbers.hpp"
#include "fes/types.hpp"

namespace fes {

/// @brief Enumeration of inference (admittance) types.
enum class InferenceType : uint8_t {
  kSpline,   ///< Spline interpolation of admittances.
  kZero,     ///< No interpolation of admittances (zero admittance).
  kLinear,   ///< Piecewise linear interpolation of admittances.
  kFourier,  ///< Munk-Cartwright Fourier series interpolation of admittances.
};

/// @brief Compute waves by inference from these 7 major ones : O1, Q1, K1,
/// 2N2, N2, M2, K2 using spline interpolation.
class SplineInference : public Inference<SplineInference> {
 public:
  /// @brief Apply inference to compute minor constituents.
  /// @param[in,out] wave_table The wave table to process.
  /// @param[in] lat Latitude in degrees.
  auto apply_impl(WaveTableInterface& wave_table, const double lat) -> void;

  /// @brief Returns the list of the tidal constituents inferred by the model.
  /// @return A vector of constituent identifiers.
  auto inferred_constituents() const -> std::vector<ConstituentId> override;
};

/// @brief Enumeration of inference (admittance) interpolation types.
enum class InterpolationType : uint8_t {
  kZeroAdmittance,     ///< No interpolation of admittances.
  kLinearAdmittance,   ///< Piecewise linear interpolation of admittances.
  kFourierAdmittance,  ///< Munk-Cartwright Fourier series interpolation.
};

class PerthInference : public Inference<PerthInference> {
 public:
  /// @brief Constructor.
  /// @param[in] wave_table The wave table containing the waves used for
  /// inference.
  /// @param[in] interpolation_type The type of interpolation to use, for
  /// diurnal and semidiurnal constituents. The default is linear admittance.
  /// @note Long-period constituents are always interpolated using linear
  /// admittance, except when the interpolation type is set to zero admittance,
  /// in which case they are not inferred.
  explicit PerthInference(const WaveTableInterface& wave_table,
                          const InterpolationType interpolation_type =
                              InterpolationType::kLinearAdmittance);

  /// @brief Apply inference to compute minor constituents.
  /// @param[in,out] wave_table The wave table to process.
  /// @param[in] lat Latitude in degrees.
  auto apply_impl(WaveTableInterface& wave_table, const double lat) -> void;

  /// @brief Returns the list of the tidal constituents inferred by the model.
  /// @return A vector of constituent identifiers.
  auto inferred_constituents() const -> std::vector<ConstituentId> override;

 private:
  /// @brief Type for interpolation functions.
  /// The function takes in the frequencies and amplitudes of three components
  /// and returns a complex number representing the interpolated value.
  using Interpolator =
      std::function<Complex(double, const Complex&, double, const Complex&,
                            double, const Complex&, double)>;

  /// Inferred diurnal constituents with their (frequency, amplitude) pairs.
  Map<ConstituentId, std::pair<double, double>, 19> inferred_diurnal_{};

  /// Inferred semidiurnal constituents with their (frequency, amplitude) pairs.
  Map<ConstituentId, std::pair<double, double>, 17> inferred_semidiurnal_;

  /// Inferred long-period constituents with their (frequency, amplitude) pairs.
  Map<ConstituentId, std::pair<double, double>, 12> inferred_long_period_;

  /// Keys for inferred constituents ordered by frequency.
  std::vector<ConstituentId> diurnal_keys_;

  /// Keys for inferred constituents ordered by frequency.
  std::vector<ConstituentId> semidiurnal_keys_;

  /// Keys for inferred constituents ordered by frequency.
  std::vector<ConstituentId> long_period_keys_;

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
  Interpolator interpolation_3_;  ///< Interpolation function for long-period.

  /// @brief Returns inphase/quad components of the 18.6-y equilibrium node
  /// tide. This is used only if inference is requested but the node tide is
  /// missing.
  /// @param[in,out] node The TideComponent for the node constituent.
  /// @param[in] lat The latitude for the computation.
  static auto evaluate_node_tide(WaveInterface& node, const double lat)
      -> const Complex&;
};

/// @brief Factory function to create an inference object based on the specified
/// inference type.
/// @param wave_table The wave table containing the waves used for inference.
/// @param inference_type The type of inference interpolation to use.
/// @return A unique pointer to the created inference object.
auto inference_factory(WaveTableInterface& wave_table,
                       const InferenceType inference_type)
    -> std::unique_ptr<InferenceInterface>;

// ============================================================================
// Implementation
// ============================================================================

inline auto SplineInference::apply_impl(WaveTableInterface& wave_table,
                                        const double /* lat */) -> void {
  // Arrays who contains the spline coefficients needed to compute MU2, NU2,
  // L2, T2 and Lambda2 by admittance.
  constexpr auto mu2 =
      std::array<double, 3>{0.069439968323, 0.351535557706, -0.046278307672};
  constexpr auto nu2 =
      std::array<double, 3>{-0.006104695053, 0.156878802427, 0.006755704028};
  constexpr auto l2 =
      std::array<double, 3>{0.077137765667, -0.051653455134, 0.027869916824};
  constexpr auto t2 =
      std::array<double, 3>{0.180480173707, -0.020101177502, 0.008331518844};
  constexpr auto lda2 =
      std::array<double, 3>{0.016503557465, -0.013307812292, 0.007753383202};

  // infer additional constituents by admittance DIURNALS (from Richard Ray
  // perth2 program)

  // from Q1, O1 (0-1)

  auto* x = wave_table[kQ1].get();
  auto* y = wave_table[kO1].get();
  auto* z = wave_table[kK1].get();

  auto set_tide = [](WaveInterface* wave, const Complex& value) {
    if (!wave->is_modeled()) {
      wave->set_tide(value);
    }
  };

  // 2Q1
  set_tide(wave_table[k2Q1].get(), 0.263 * x->tide() - 0.0252 * y->tide());

  // Sigma1
  set_tide(wave_table[kSigma1].get(), 0.297 * x->tide() - 0.0264 * y->tide());

  // rho1
  set_tide(wave_table[kRho1].get(), 0.164 * x->tide() + 0.0048 * y->tide());

  // from O1, K1  (1-2)

  // M11
  set_tide(wave_table[kM11].get(), 0.0140 * y->tide() + 0.0101 * z->tide());

  // M12
  set_tide(wave_table[kM12].get(), 0.0389 * y->tide() + 0.0282 * z->tide());

  // CHI1
  set_tide(wave_table[kChi1].get(), 0.0064 * y->tide() + 0.0060 * z->tide());

  // pi1
  set_tide(wave_table[kPi1].get(), 0.0030 * y->tide() + 0.0171 * z->tide());

  // phi1
  set_tide(wave_table[kPhi1].get(), -0.0015 * y->tide() + 0.0152 * z->tide());

  // theta1
  set_tide(wave_table[kTheta1].get(), -0.0065 * y->tide() + 0.0155 * z->tide());

  // J1
  set_tide(wave_table[kJ1].get(), -0.0389 * y->tide() + 0.0836 * z->tide());

  // OO1
  set_tide(wave_table[kOO1].get(), -0.0431 * y->tide() + 0.0613 * z->tide());

  // infer additional constituents by admittance SEMI-DIURNALS
  // (from Richard Ray perth3 program)

  // from M2, N2
  x = wave_table[kN2].get();
  y = wave_table[kM2].get();

  // 2N2
  set_tide(wave_table[k2N2].get(), 0.264 * x->tide() - 0.0253 * y->tide());

  // SEMI-DIURNAL (from Grenoble to take advantage of 2N2)

  // from 2N2, N2 (3-4)
  x = wave_table[k2N2].get();
  y = wave_table[kN2].get();

  // eps2
  set_tide(wave_table[kEps2].get(), 0.53285 * x->tide() - 0.03304 * y->tide());

  // from M2, K2 [5-6]
  x = wave_table[kN2].get();
  y = wave_table[kM2].get();
  z = wave_table[kK2].get();

  // eta2
  set_tide(wave_table[kEta2].get(),
           -0.0034925 * y->tide() + 0.0831707 * z->tide());

  // from N2, M2, K2 by spline admittances [see GRL 18[5]:845-848,1991]

  // mu2
  set_tide(wave_table[kMu2].get(),
           mu2[0] * z->tide() + mu2[1] * x->tide() + mu2[2] * y->tide());
  // nu2
  set_tide(wave_table[kNu2].get(),
           nu2[0] * z->tide() + nu2[1] * x->tide() + nu2[2] * y->tide());
  // lambda2
  set_tide(wave_table[kLambda2].get(),
           lda2[0] * z->tide() + lda2[1] * x->tide() + lda2[2] * y->tide());
  // L2
  set_tide(wave_table[kL2].get(),
           l2[0] * z->tide() + l2[1] * x->tide() + l2[2] * y->tide());

  // T2
  set_tide(wave_table[kT2].get(),
           t2[0] * z->tide() + t2[1] * x->tide() + t2[2] * y->tide());
}

// ============================================================================

inline auto SplineInference::inferred_constituents() const
    -> std::vector<ConstituentId> {
  return {k2Q1,  kSigma1, kRho1,    kM11, kM12, kChi1, kPi1,
          kPhi1, kTheta1, kJ1,      kOO1, k2N2, kEps2, kEta2,
          kMu2,  kNu2,    kLambda2, kL2,  kT2};
}

// ============================================================================

template <size_t N>
auto populate_and_sort_inferred(
    Map<ConstituentId, std::pair<double, double>, N>& mutable_inferred,
    std::vector<ConstituentId>& keys,
    const Map<ConstituentId, double, N>& inferred,
    const WaveTableInterface& wave_table) -> void {
  for (const auto& item : inferred) {
    const auto ident = item.first;
    const auto ampl = item.second;
    mutable_inferred.insert(
        ident, {wave_table[ident]->template frequency<kDegreePerHour>(), ampl});
    keys.push_back(ident);
  }

  // Sort the inferred constituents by frequency
  std::sort(
      keys.begin(), keys.end(),
      [&mutable_inferred](const ConstituentId& a, const ConstituentId& b) {
        // Access the values through the map for comparison
        const auto& inferred_value_a = mutable_inferred[a];
        const auto& inferred_value_b = mutable_inferred[b];
        return inferred_value_a.first < inferred_value_b.first;
      });
}

// ============================================================================

/// @brief Computes linear admittance interpolation, based on 3 fundamental
/// frequencies.
/// @param[in] x1 Frequency for the first component.
/// @param[in] y1 Complex admittance for the first component.
/// @param[in] x2 Frequency for the second component.
/// @param[in] y2 Complex admittance for the second component.
/// @param[in] x3 Frequency for the third component.
/// @param[in] y3 Complex admittance for the third component.
/// @param[in] x Frequency at which to evaluate the interpolation.
/// @return The interpolated complex admittance at frequency `x`.
inline auto linear_interpolation(double x1, const Complex& y1, double x2,
                                 const Complex& y2, double x3,
                                 const Complex& y3, double x) -> Complex {
  if (x <= x2) {
    auto slp = (y2 - y1) / (x2 - x1);
    return y1 + slp * (x - x1);
  }
  auto slp = (y3 - y2) / (x3 - x2);
  return y2 + slp * (x - x2);
}

/// @brief Computes zero admittance interpolation, which always returns zero.
/// @param[in] x1 Frequency for the first component.
/// @param[in] y1 Complex admittance for the first component.
/// @param[in] x2 Frequency for the second component.
/// @param[in] y2 Complex admittance for the second component.
/// @param[in] x3 Frequency for the third component.
/// @param[in] y3 Complex admittance for the third component.
/// @param[in] x Frequency at which to evaluate the interpolation.
/// @return The interpolated complex admittance at frequency `x` (always zero).
constexpr auto zero_admittance(double /*x1*/, const Complex& /*y1*/,
                               double /*x2*/, const Complex& /*y2*/,
                               double /*x3*/, const Complex& /*y3*/,
                               double /*x*/) -> Complex {
  return {0.0, 0.0};
}

// ============================================================================

/// @brief Computes admittance interpolation, based on 3 fundamental
/// frequencies, using approach of Munk-Cartwright (low order Fourier series).
/// The 3 frequencies must be either (Q1,O1,K1) or (N2,M2,S2).
/// @tparam N Species number (1=diurnal; 2=semid'l).
/// @param[in] x1 Frequency for the first component.
/// @param[in] z1 Complex admittance for the first component.
/// @param[in] x2 Frequency for the second component.
/// @param[in] z2 Complex admittance for the second component.
/// @param[in] x3 Frequency for the third component.
/// @param[in] z3 Complex admittance for the third component.
/// @param[in] x Frequency at which to evaluate the interpolation.
/// @return The interpolated complex admittance at frequency `x`.
template <int N>
auto fourier_interpolation(double /*x1*/, const Complex& z1, double /*x2*/,
                           const Complex& z2, double /*x3*/, const Complex& z3,
                           double x) -> Complex {
  static_assert(N == 1 || N == 2, "N must be either 1 or 2");

  const auto p = detail::math::radians(48.0);
  const auto f = x * p;

  Eigen::Vector3cd admit(z1, z2, z3);

  Eigen::Vector3cd c;
  if (N == 1) {
    static const Eigen::Matrix3d ainv1 =
        (Eigen::Matrix3d() << 3.1214, -3.8494, 1.7280, -3.1727, 3.9559, -0.7832,
         1.4380, -3.0297, 1.5917)
            .finished();
    c = ainv1 * admit;
  }
  if (N == 2) {
    static const Eigen::Matrix3d ainv2 =
        (Eigen::Matrix3d() << 3.3133, -4.2538, 1.9405, -3.3133, 4.2538, -0.9405,
         1.5018, -3.2579, 1.7561)
            .finished();
    c = ainv2 * admit;
  }

  return c(0) + c(1) * std::cos(f) + c(2) * std::sin(f);
}

// ============================================================================

inline auto PerthInference::evaluate_node_tide(WaveInterface& node,
                                               const double lat)
    -> const Complex& {
  if (!node.is_modeled()) {
    constexpr auto gamma2 = 0.682;
    constexpr auto amplitude = 0.0279;  // m
    auto p20 =
        0.5 - 1.5 * detail::math::pow<2>(std::sin(detail::math::radians(lat)));
    auto xi = gamma2 * p20 * std::sqrt(1.25 / detail::math::pi<double>());
    node.set_tide(Complex(xi * amplitude, 0.0));
  }
  return node.tide();
}

// ============================================================================

inline PerthInference::PerthInference(
    const WaveTableInterface& wave_table,
    const InterpolationType interpolation_type) {
  /// Inferred diurnal constituents with their frequencies.
  static constexpr const Map<ConstituentId, double, 19>
      kInferredDiurnalConstituents_{{{{ConstituentId::k2Q1, 0.006638},
                                      {ConstituentId::kSigma1, 0.008023},
                                      {ConstituentId::kQ1, 0.050184},
                                      {ConstituentId::kRho1, 0.009540},
                                      {ConstituentId::kO1, 0.262163},
                                      {ConstituentId::kTau1, 0.003430},
                                      {ConstituentId::kBeta1, 0.001941},
                                      {ConstituentId::kM1, 0.020604},
                                      {ConstituentId::kChi1, 0.003925},
                                      {ConstituentId::kPi1, 0.007125},
                                      {ConstituentId::kP1, 0.122008},
                                      {ConstituentId::kK1, 0.368731},
                                      {ConstituentId::kPsi1, 0.002929},
                                      {ConstituentId::kPhi1, 0.005247},
                                      {ConstituentId::kTheta1, 0.003966},
                                      {ConstituentId::kJ1, 0.020618},
                                      {ConstituentId::kSO1, 0.003417},
                                      {ConstituentId::kOO1, 0.011293},
                                      {ConstituentId::kUps1, 0.002157}}}};

  /// Inferred semidiurnal constituents with their frequencies.
  static constexpr const Map<ConstituentId, double, 17>
      kInferredSemidiurnalConstituents_{{{
          {ConstituentId::kEps2, 0.004669},
          {ConstituentId::k2N2, 0.016011},
          {ConstituentId::kMu2, 0.019316},
          {ConstituentId::kN2, 0.121006},
          {ConstituentId::kNu2, 0.022983},
          {ConstituentId::kGamma2, 0.001902},
          {ConstituentId::kAlpha2, 0.002178},
          {ConstituentId::kM2, 0.631931},
          {ConstituentId::kBeta2, 0.001921},
          {ConstituentId::kDelta2, 0.000714},
          {ConstituentId::kLambda2, 0.004662},
          {ConstituentId::kL2, 0.017862},
          {ConstituentId::kT2, 0.017180},
          {ConstituentId::kS2, 0.294019},
          {ConstituentId::kR2, 0.002463},
          {ConstituentId::kK2, 0.079924},
          {ConstituentId::kEta2, 0.004467},
      }}};

  /// Inferred long-period constituents with their frequencies.
  static constexpr const Map<ConstituentId, double, 12>
      kInferredLongPeriodConstituents_{{{
          {ConstituentId::kNode, 0.027929},
          {ConstituentId::kSa1, 0.004922},  // grav arg.; not conventional Sa
          {ConstituentId::kSsa, 0.030988},
          {ConstituentId::kSta, 0.001809},
          {ConstituentId::kMSm, 0.006728},
          {ConstituentId::kMm, 0.035184},
          {ConstituentId::kMSf, 0.005837},
          {ConstituentId::kMf, 0.066607},
          {ConstituentId::kMStm, 0.002422},
          {ConstituentId::kMtm, 0.012753},
          {ConstituentId::kMSqm, 0.002037},
          {ConstituentId::kMqm, 0.001687},
      }}};

  populate_and_sort_inferred(inferred_diurnal_, diurnal_keys_,
                             kInferredDiurnalConstituents_, wave_table);
  populate_and_sort_inferred(inferred_semidiurnal_, semidiurnal_keys_,
                             kInferredSemidiurnalConstituents_, wave_table);
  populate_and_sort_inferred(inferred_long_period_, long_period_keys_,
                             kInferredLongPeriodConstituents_, wave_table);

  auto& q1 = inferred_diurnal_[ConstituentId::kQ1];
  auto& o1 = inferred_diurnal_[ConstituentId::kO1];
  auto& k1 = inferred_diurnal_[ConstituentId::kK1];
  auto& n2 = inferred_semidiurnal_[ConstituentId::kN2];
  auto& m2 = inferred_semidiurnal_[ConstituentId::kM2];
  auto& s2 = inferred_semidiurnal_[ConstituentId::kS2];
  auto& node = inferred_long_period_[ConstituentId::kNode];
  auto& mm = inferred_long_period_[ConstituentId::kMm];
  auto& mf = inferred_long_period_[ConstituentId::kMf];

  x1_ = q1.first;
  x2_ = o1.first;
  x3_ = k1.first;
  x4_ = n2.first;
  x5_ = m2.first;
  x6_ = s2.first;
  x7_ = node.first;
  x8_ = mm.first;
  x9_ = mf.first;

  double fk;
  double fh;
  double fl;

  std::tie(fk, fh, fl) = perth::love_pmm95b(x1_);
  amp1_ = q1.second * (1 + fk - fh);
  std::tie(fk, fh, fl) = perth::love_pmm95b(x2_);
  amp2_ = o1.second * (1 + fk - fh);
  std::tie(fk, fh, fl) = perth::love_pmm95b(x3_);
  amp3_ = k1.second * (1 + fk - fh);

  amp4_ = n2.second;
  amp5_ = m2.second;
  amp6_ = s2.second;
  amp7_ = node.second;
  amp8_ = mm.second;
  amp9_ = mf.second;

  // Long period constituents are always computed using linear admittance, like
  // in the original Perth program.
  switch (interpolation_type) {
    case InterpolationType::kZeroAdmittance:
      interpolation_1_ = zero_admittance;
      interpolation_2_ = zero_admittance;
      interpolation_3_ = zero_admittance;
      break;
    case InterpolationType::kLinearAdmittance:
      interpolation_1_ = linear_interpolation;
      interpolation_2_ = linear_interpolation;
      interpolation_3_ = linear_interpolation;
      break;
    case InterpolationType::kFourierAdmittance:
      interpolation_1_ = fourier_interpolation<1>;
      interpolation_2_ = fourier_interpolation<2>;
      interpolation_3_ = linear_interpolation;
      break;
    default:
      throw std::invalid_argument("Unknown interpolation type");
  }
}

inline auto PerthInference::apply_impl(WaveTableInterface& wave_table,
                                       const double lat) -> void {
  auto y1 = wave_table[ConstituentId::kQ1]->tide() / amp1_;
  auto y2 = wave_table[ConstituentId::kO1]->tide() / amp2_;
  auto y3 = wave_table[ConstituentId::kK1]->tide() / amp3_;
  auto y4 = wave_table[ConstituentId::kN2]->tide() / amp4_;
  auto y5 = wave_table[ConstituentId::kM2]->tide() / amp5_;
  auto y6 = wave_table[ConstituentId::kS2]->tide() / amp6_;
  auto y8 = wave_table[ConstituentId::kMm]->tide() / amp8_;
  auto y9 = wave_table[ConstituentId::kMf]->tide() / amp9_;

  auto y7 = PerthInference::evaluate_node_tide(
                *wave_table[ConstituentId::kNode], lat) /
            amp7_;

  for (const auto& constituent : diurnal_keys_) {
    if (!wave_table.contains(constituent)) {
      continue;  // Skip if the constituent is not in the wave table
    }
    double fk;
    double fh;
    double fl;

    auto& updated_item = *wave_table[constituent];
    if (updated_item.is_modeled() || updated_item.type() != kShortPeriod) {
      continue;  // Skip if the constituent is not computed by inference
    }
    const auto& inferred_item = inferred_diurnal_[constituent];

    auto x = inferred_item.first;
    auto y = interpolation_1_(x1_, y1, x2_, y2, x3_, y3, x);
    std::tie(fk, fh, fl) = perth::love_pmm95b(x);
    auto gam = 1 + fk - fh;
    updated_item.set_tide(y * gam * inferred_item.second);
  }

  for (const auto& constituent : semidiurnal_keys_) {
    if (!wave_table.contains(constituent)) {
      continue;  // Skip if the constituent is not in the wave table
    }
    auto& updated_item = *wave_table[constituent];
    if (updated_item.is_modeled() || updated_item.type() != kShortPeriod) {
      continue;  // Skip if the constituent is not computed by inference
    }
    const auto& inferred_item = inferred_semidiurnal_[constituent];
    auto y = interpolation_2_(x4_, y4, x5_, y5, x6_, y6, inferred_item.first);
    updated_item.set_tide(y * inferred_item.second);
  }

  for (const auto& constituent : long_period_keys_) {
    if (!wave_table.contains(constituent)) {
      continue;  // Skip if the constituent is not in the wave table
    }
    auto& updated_item = *wave_table[constituent];
    if (updated_item.is_modeled() || updated_item.type() != kLongPeriod) {
      continue;  // Skip if the constituent is not computed by inference
    }
    const auto& inferred_item = inferred_long_period_[constituent];
    auto y = interpolation_3_(x7_, y7, x8_, y8, x9_, y9, inferred_item.first);
    updated_item.set_tide(y * inferred_item.second);
  }
}

// ============================================================================

inline auto PerthInference::inferred_constituents() const
    -> std::vector<ConstituentId> {
  std::vector<ConstituentId> result;
  result.insert(result.end(), diurnal_keys_.begin(), diurnal_keys_.end());
  result.insert(result.end(), semidiurnal_keys_.begin(),
                semidiurnal_keys_.end());
  result.insert(result.end(), long_period_keys_.begin(),
                long_period_keys_.end());
  return result;
}

}  // namespace fes
