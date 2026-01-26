// Copyright (c) 2025 CNES
//
// All rights reserved. Use of this source code is governed by a
// BSD-style license that can be found in the LICENSE file.
#include "fes/perth/tide.hpp"

#include "fes/abstract_tidal_model.hpp"
#include "fes/detail/thread.hpp"
#include "fes/perth/table.hpp"

namespace fes {
namespace perth {

auto evaluate_tide_from_constituents(
    const std::map<Constituent, std::complex<double>>& constituents,
    const Eigen::Ref<const Eigen::VectorXd>& epoch, const double latitude,
    const Settings& settings) -> std::tuple<Eigen::VectorXd, Eigen::VectorXd> {
  // Allocates the result vectors
  auto tide = Eigen::VectorXd(epoch.size());
  auto long_period = Eigen::VectorXd(epoch.size());
  auto group_modulations = settings.group_modulations();
  // List of the tidal waves to be used for the tide calculation
  // Worker responsible for the calculation of the tide at a given position
  auto worker = [&](const int64_t start, const int64_t end) {
    auto tide_waves = std::vector<Constituent>();
    tide_waves.reserve(constituents.size());
    for (const auto& item : constituents) {
      tide_waves.push_back(item.first);
    }
    auto wave_table = WaveTable(tide_waves);
    auto acc = Accelerator(settings.astronomic_formulae(),
                           settings.time_tolerance(), 0);
    auto inference = std::unique_ptr<Inference>(
        settings.inference_type() == InterpolationType::kZeroAdmittance
            ? nullptr
            : new Inference(wave_table, settings.inference_type()));
    auto* inference_ptr = inference.get();

    for (auto ix = start; ix < end; ++ix) {
      std::tie(tide(ix), long_period(ix)) =
          perth::detail::compute_tide_from_waves(wave_table, inference_ptr, acc,
                                                 latitude, epoch[ix],
                                                 group_modulations);
    }
  };

  fes::detail::parallel_for(worker, epoch.size(), settings.num_threads());
  return {tide, long_period};
}

}  // namespace perth
}  // namespace fes
