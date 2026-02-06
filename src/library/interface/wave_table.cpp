#include "fes/interface/wave_table.hpp"

#include <Eigen/Core>
#include <cmath>
#include <cstddef>
#include <cstdint>
#include <memory>
#include <stdexcept>
#include <string>
#include <tuple>
#include <vector>

#include "fes/detail/markdown_table.hpp"
#include "fes/detail/parallel_for.hpp"
#include "fes/interface/wave.hpp"

namespace fes {

auto WaveTableInterface::select_waves_for_analysis(const double duration,
                                                   const double f)
    -> std::vector<std::string> {
  auto result = std::vector<std::string>();
  for (auto& item : *this) {
    auto& wave = item.value();
    if (wave->period() < f * (duration / 3600.0)) {
      result.emplace_back(wave->name());
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
  for (const auto& item : *this) {
    const auto& wave = item.value();

    constituents_table.add_row(
        {wave->latex_name(), std::to_string(wave->frequency<kDegreePerHour>()),
         wave->xdo_alphabetical()});
  }
  return constituents_table.to_string();
}

}  // namespace fes
