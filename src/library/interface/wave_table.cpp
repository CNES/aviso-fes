#include "fes/interface/wave_table.hpp"

#include <Eigen/Core>
#include <algorithm>
#include <cmath>
#include <cstddef>
#include <cstdint>
#include <memory>
#include <stdexcept>
#include <string>
#include <tuple>
#include <utility>
#include <vector>

#include "fes/constituent.hpp"
#include "fes/detail/markdown_table.hpp"
#include "fes/detail/parallel_for.hpp"
#include "fes/inference.hpp"
#include "fes/interface/wave.hpp"

namespace fes {

// Returns the approximate theoretical equilibrium amplitude (scaled M2=100).
// Values derived from the Schureman (1958) or Doodson (1921) potential.
// This provides a "Scientific Ranking" for sorting.
auto get_theoretical_amplitude(const ConstituentId ident) -> double {
  switch (ident) {
    // --- Semi-Diurnal (Species 2) ---
    case kM2:
      return 100.0;
    case kS2:
      return 46.6;
    case kN2:
      return 19.2;
    case kK2:
      return 12.7;
    case k2N2:
      return 2.5;
    case kMu2:
      return 2.4;
    case kNu2:
      return 3.6;
    case kL2:
      return 2.8;
    case kT2:
      return 2.7;
    // --- Diurnal (Species 1) ---
    case kK1:
      return 58.4;
    case kO1:
      return 41.5;
    case kP1:
      return 19.3;
    case kQ1:
      return 7.9;
    case kM1:
      return 3.1;
    case kJ1:
      return 3.3;
    case kOO1:
      return 1.6;
    case k2Q1:
      return 1.0;
    // --- Long Period (Species 0) ---
    case kMf:
      return 17.2;
    case kMm:
      return 9.1;
    case kSsa:
      return 8.0;
    case kSa:
      return 1.1;
    // --- Shallow Water / Non-Linear (No theoretical potential) ---
    // We assign them low "dummy" priorities based on typical observation.
    case kM4:
      return 0.5;
    case kMS4:
      return 0.4;
    case kM6:
      return 0.1;
    default:
      // Default for unknown or very minor constituents
      return 0.0;
  }
}

struct Candidate {
  ConstituentId id;
  double omega;
  double rank;
};

auto WaveTableInterface::select_waves_for_analysis(
    const double duration, const double rayleigh_criterion)
    -> std::vector<std::string> {
  auto candidates = std::vector<Candidate>();

  // Convert duration from seconds to hours
  const auto duration_hours = duration / 3600.0;
  // Avoid division by zero for invalid durations
  if (duration_hours <= 1e-6) {
    return {};
  }

  // Calculate the frequency resolution limit (Rayleigh frequency)
  // Formula: Delta_f >= C_R / T  (in cycles/hour)
  // Angular: Delta_w >= 2 * pi * C_R / T (in radians/hour)
  const auto min_separation =
      detail::math::two_pi<double>() * rayleigh_criterion / duration_hours;

  // 1. Filter candidates
  for (const auto& item : *this) {
    const auto& wave = item.value();
    const auto omega = wave->frequency();

    // Check separability from DC (Period check)
    if (std::abs(omega) >= min_separation) {
      candidates.push_back(
          {wave->ident(), omega, get_theoretical_amplitude(wave->ident())});
    }
  }

  // 2. Sort by Theoretical Amplitude (Descending)
  //    This ensures M2 (100.0) is processed before 2MK2 (~0.0)
  std::sort(candidates.begin(), candidates.end(),
            [](const Candidate& a, const Candidate& b) {
              if (std::abs(a.rank - b.rank) > 1e-6) {
                return a.rank > b.rank;
              }
              // Fallback to frequency for stability
              return a.omega < b.omega;
            });

  // 3. Greedy Selection (The "Rayleigh Loop")
  auto result = std::vector<std::string>();
  auto selected_frequencies = std::vector<double>();

  for (const auto& item : candidates) {
    bool separable = true;
    for (const auto& selected_omega : selected_frequencies) {
      if (std::abs(item.omega - selected_omega) < min_separation) {
        separable = false;
        break;
      }
    }

    if (separable) {
      result.emplace_back(constituents::name(item.id));
      selected_frequencies.push_back(item.omega);
    }
  }

  return result;
}

// ============================================================================

auto WaveTableInterface::tide_from_tide_series(
    const Eigen::Ref<const Eigen::VectorXd>& epoch,
    const Eigen::Ref<const Eigen::VectorXcd>& wave,
    NodalCorrectionsArgs& args) const -> Eigen::VectorXd {
  if (static_cast<size_t>(wave.rows()) != size()) {
    throw std::invalid_argument(
        "wave must contain as many elements as the number of waves in the "
        "table");
  }
  auto result = Eigen::VectorXd(epoch.rows());

  /// The object responsible for the calculation of astronomical angles.
  auto& angles = args.angles();

  // The wave properties of the object must be immutable for the provided
  // instance.
  auto wt = clone();

  for (auto ix = 0; ix < epoch.rows(); ++ix) {
    double tide = 0;
    angles.update(epoch(ix));
    wt->compute_nodal_corrections(angles, args.group_modulations());

    for (size_t jx = 0; jx < wt->size(); ++jx) {
      const auto& item = (*wt)[jx];
      double phi = item->vu();

      tide += item->f() * (wave(jx).real() * std::cos(phi) +
                           wave(jx).imag() * std::sin(phi));
    }
    result(ix) = tide;
  }
  return result;
}

// ============================================================================

auto WaveTableInterface::tide_from_mapping(
    const double epoch, const DynamicRef<const Eigen::MatrixXcd>& wave,
    NodalCorrectionsArgs& args, const size_t num_threads) const
    -> Eigen::MatrixXd {
  if (static_cast<size_t>(wave.rows()) != size()) {
    throw std::invalid_argument(
        "wave must contain as many elements as the number of waves in the "
        "table");
  }
  auto result = Eigen::MatrixXd(wave.cols(), wave.rows());
  auto worker = [&](const int64_t start, const int64_t end) {
    // The wave properties of the object must be immutable for the provided
    // instance.
    auto wt = clone();
    args.angles().update(epoch);
    wt->compute_nodal_corrections(args.angles(), args.group_modulations());
    for (auto ix = start; ix < end; ++ix) {
      for (size_t jx = 0; jx < wt->size(); ++jx) {
        const auto& item = (*wt)[jx];
        double phi = item->vu();

        result(ix, jx) += item->f() * (wave(jx, ix).real() * std::cos(phi) +
                                       wave(jx, ix).imag() * std::sin(phi));
      }
    }
  };
  detail::parallel_for(worker, wave.cols(), num_threads);
  return result;
}

// ============================================================================

auto WaveTableInterface::compute_nodal_modulations(
    const Eigen::Ref<const Eigen::VectorXd>& epoch,
    NodalCorrectionsArgs& args) const
    -> std::tuple<Eigen::MatrixXd, Eigen::MatrixXd> {
  auto f = Eigen::MatrixXd(size(), epoch.size());
  auto vu = Eigen::MatrixXd(size(), epoch.size());

  /// The object responsible for the calculation of astronomical angles.
  auto& angles = args.angles();

  // The wave properties of the object must be immutable for the provided
  // instance.
  auto wt = clone();

  for (auto ix = 0; ix < epoch.size(); ++ix) {
    angles.update(epoch(ix));
    wt->compute_nodal_corrections(angles, args.group_modulations());
    for (size_t jx = 0; jx < wt->size(); ++jx) {
      const auto& wave = (*wt)[jx];
      f(jx, ix) = wave->f();
      vu(jx, ix) = wave->vu();
    }
  }
  return std::make_tuple(f, vu);
}

// ============================================================================
auto WaveTableInterface::generate_markdown_table() const -> std::string {
  detail::MarkdownTable constituents_table(
      {"Constituent", "Speed (Deg/hr)", "XDO"});
  for (const auto& item : this->sort_by_frequency()) {
    const auto& wave = (*this)[item];

    constituents_table.add_row(
        {wave->latex_name(), std::to_string(wave->frequency<kDegreePerHour>()),
         wave->xdo_alphabetical()});
  }
  return constituents_table.to_string();
}

// ============================================================================

auto WaveTableInterface::sort_by_frequency(const bool ascending) const
    -> std::vector<ConstituentId> {
  auto result = std::vector<ConstituentId>();
  result.reserve(size());
  for (const auto& item : *this) {
    result.emplace_back(item.key());
  }
  std::sort(result.begin(), result.end(),
            [&](const ConstituentId& a, const ConstituentId& b) {
              auto frequency_a = (*this)[a]->frequency();
              auto frequency_b = (*this)[b]->frequency();
              return ascending ? frequency_a < frequency_b
                               : frequency_a > frequency_b;
            });
  return result;
}

}  // namespace fes
