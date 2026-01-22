
#include "fes/perth/inference.hpp"

#include <algorithm>
#include <cmath>
#include <stdexcept>
#include <tuple>
#include <unordered_map>
#include <utility>
#include <vector>

#include "fes/detail/math.hpp"
#include "fes/perth/constituent.hpp"
#include "fes/perth/love_numbers.hpp"
#include "fes/perth/tidal_frequency.hpp"

namespace fes {
namespace perth {

std::unordered_map<Constituent, double>
    Inference::kInferredDiurnalConstituents_{{
        // Diurnal constituents
        {Constituent::k2Q1, 0.006638},    {Constituent::kSigma1, 0.008023},
        {Constituent::kQ1, 0.050184},     {Constituent::kRho1, 0.009540},
        {Constituent::kO1, 0.262163},     {Constituent::kTau1, 0.003430},
        {Constituent::kBeta1, 0.001941},  {Constituent::kM1, 0.020604},
        {Constituent::kChi1, 0.003925},   {Constituent::kPi1, 0.007125},
        {Constituent::kP1, 0.122008},     {Constituent::kK1, 0.368731},
        {Constituent::kPsi1, 0.002929},   {Constituent::kPhi1, 0.005247},
        {Constituent::kTheta1, 0.003966}, {Constituent::kJ1, 0.020618},
        {Constituent::kSO1, 0.003417},    {Constituent::kOO1, 0.011293},
        {Constituent::kUps1, 0.002157},
    }};

std::unordered_map<Constituent, double>
    Inference::kInferredSemidiurnalConstituents_{{
        // Semidiurnal constituents
        {Constituent::kEps2, 0.004669},
        {Constituent::k2N2, 0.016011},
        {Constituent::kMu2, 0.019316},
        {Constituent::kN2, 0.121006},
        {Constituent::kNu2, 0.022983},
        {Constituent::kGamma2, 0.001902},
        {Constituent::kAlpa2, 0.002178},
        {Constituent::kM2, 0.631931},
        {Constituent::kBeta2, 0.001921},
        {Constituent::kDelta2, 0.000714},
        {Constituent::kLambda2, 0.004662},
        {Constituent::kL2, 0.017862},
        {Constituent::kT2, 0.017180},
        {Constituent::kS2, 0.294019},
        {Constituent::kR2, 0.002463},
        {Constituent::kK2, 0.079924},
        {Constituent::kEta2, 0.004467},
    }};

std::unordered_map<Constituent, double>
    Inference::kInferredLongPeriodConstituents_{{
        // Long-period constituents
        {Constituent::kNode, 0.027929},
        {Constituent::kSa1, 0.004922},  // grav arg.; not conventional Sa
        {Constituent::kSsa, 0.030988},
        {Constituent::kSta, 0.001809},
        {Constituent::kMSm, 0.006728},
        {Constituent::kMm, 0.035184},
        {Constituent::kMSf, 0.005837},
        {Constituent::kMf, 0.066607},
        {Constituent::kMStm, 0.002422},
        {Constituent::kMtm, 0.012753},
        {Constituent::kMSqm, 0.002037},
        {Constituent::kMqm, 0.001687},
    }};

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
inline auto linear_interpolation(double x1, const std::complex<double>& y1,
                                 double x2, const std::complex<double>& y2,
                                 double x3, const std::complex<double>& y3,
                                 double x) -> std::complex<double> {
  if (x <= x2) {
    auto slp = (y2 - y1) / (x2 - x1);
    return y1 + slp * (x - x1);
  }
  auto slp = (y3 - y2) / (x3 - x2);
  return y2 + slp * (x - x2);
}

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
auto fourier_interpolation(double /*x1*/, const std::complex<double>& z1,
                           double /*x2*/, const std::complex<double>& z2,
                           double /*x3*/, const std::complex<double>& z3,
                           double x) -> std::complex<double> {
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

auto populate_and_sort_inferred(
    std::unordered_map<Constituent, std::pair<double, double>>&
        mutable_inferred,
    std::vector<Constituent>& keys,
    const std::unordered_map<Constituent, double>& inferred,
    const WaveTable& components) -> void {
  for (const auto& item : inferred) {
    const auto ident = item.first;
    const auto ampl = item.second;
    const auto doodson_number =
        components[ident].doodson_number.head(6).cast<double>();
    mutable_inferred[ident] = {tidal_frequency(doodson_number), ampl};
    keys.push_back(ident);
  }

  // Sort the inferred constituents by frequency
  std::sort(keys.begin(), keys.end(),
            [&mutable_inferred](const Constituent& a, const Constituent& b) {
              // Access the values through the map for comparison
              const auto& inferred_value_a = mutable_inferred.at(a);
              const auto& inferred_value_b = mutable_inferred.at(b);
              return inferred_value_a.first < inferred_value_b.first;
            });
}

Inference::Inference(const WaveTable& components,
                     const InterpolationType interpolation_type) {
  populate_and_sort_inferred(inferred_diurnal_, diurnal_keys_,
                             kInferredDiurnalConstituents_, components);
  populate_and_sort_inferred(inferred_semidiurnal_, semidiurnal_keys_,
                             kInferredSemidiurnalConstituents_, components);
  populate_and_sort_inferred(inferred_long_period_, long_period_keys_,
                             kInferredLongPeriodConstituents_, components);

  auto& q1 = inferred_diurnal_[Constituent::kQ1];
  auto& o1 = inferred_diurnal_[Constituent::kO1];
  auto& k1 = inferred_diurnal_[Constituent::kK1];
  auto& n2 = inferred_semidiurnal_[Constituent::kN2];
  auto& m2 = inferred_semidiurnal_[Constituent::kM2];
  auto& s2 = inferred_semidiurnal_[Constituent::kS2];
  auto& node = inferred_long_period_[Constituent::kNode];
  auto& mm = inferred_long_period_[Constituent::kMm];
  auto& mf = inferred_long_period_[Constituent::kMf];

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

  std::tie(fk, fh, fl) = love_pmm95b(x1_);
  amp1_ = q1.second * (1 + fk - fh);
  std::tie(fk, fh, fl) = love_pmm95b(x2_);
  amp2_ = o1.second * (1 + fk - fh);
  std::tie(fk, fh, fl) = love_pmm95b(x3_);
  amp3_ = k1.second * (1 + fk - fh);

  amp4_ = n2.second;
  amp5_ = m2.second;
  amp6_ = s2.second;
  amp7_ = node.second;
  amp8_ = mm.second;
  amp9_ = mf.second;

  if (interpolation_type == InterpolationType::kLinearAdmittance) {
    interpolation_1_ = linear_interpolation;
    interpolation_2_ = linear_interpolation;
  } else if (interpolation_type == InterpolationType::kFourierAdmittance) {
    interpolation_1_ = fourier_interpolation<1>;
    interpolation_2_ = fourier_interpolation<2>;
  } else {
    throw std::invalid_argument("Unknown interpolation type");
  }
}

auto Inference::operator()(WaveTable& constituent_table, const double lat) const
    -> void {
  auto y1 = constituent_table[Constituent::kQ1].tide / amp1_;
  auto y2 = constituent_table[Constituent::kO1].tide / amp2_;
  auto y3 = constituent_table[Constituent::kK1].tide / amp3_;
  auto y4 = constituent_table[Constituent::kN2].tide / amp4_;
  auto y5 = constituent_table[Constituent::kM2].tide / amp5_;
  auto y6 = constituent_table[Constituent::kS2].tide / amp6_;
  auto y8 = constituent_table[Constituent::kMm].tide / amp8_;
  auto y9 = constituent_table[Constituent::kMf].tide / amp9_;

  auto y7 =
      evaluate_node_tide(constituent_table[Constituent::kNode], lat) / amp7_;

  for (const auto& constituent : diurnal_keys_) {
    double fk;
    double fh;
    double fl;

    auto& updated_item = constituent_table[constituent];
    if (!updated_item.is_inferred || updated_item.type != kShortPeriod) {
      continue;  // Skip if the constituent is not computed by inference
    }
    const auto& inferred_item = inferred_diurnal_.at(constituent);

    auto x = inferred_item.first;
    auto y = interpolation_1_(x1_, y1, x2_, y2, x3_, y3, x);
    std::tie(fk, fh, fl) = love_pmm95b(x);
    auto gam = 1 + fk - fh;
    updated_item.tide = y * gam * inferred_item.second;
  }

  for (const auto& constituent : semidiurnal_keys_) {
    auto& updated_item = constituent_table[constituent];
    if (!updated_item.is_inferred || updated_item.type != kShortPeriod) {
      continue;  // Skip if the constituent is not computed by inference
    }
    const auto& inferred_item = inferred_semidiurnal_.at(constituent);
    auto y = interpolation_2_(x4_, y4, x5_, y5, x6_, y6, inferred_item.first);
    updated_item.tide = y * inferred_item.second;
  }
  for (const auto& constituent : long_period_keys_) {
    auto& updated_item = constituent_table[constituent];
    if (!updated_item.is_inferred || updated_item.type != kLongPeriod) {
      continue;  // Skip if the constituent is not computed by inference
    }
    const auto& inferred_item = inferred_long_period_.at(constituent);
    auto y =
        linear_interpolation(x7_, y7, x8_, y8, x9_, y9, inferred_item.first);
    updated_item.tide = y * inferred_item.second;
  }
}

}  // namespace perth
}  // namespace fes
