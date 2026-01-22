// Copyright (c) 2025 CNES
//
// All rights reserved. Use of this source code is governed by a
// BSD-style license that can be found in the LICENSE file.
#include "fes/tide.hpp"

#include "fes/detail/broadcast.hpp"
#include "fes/detail/thread.hpp"

namespace fes {

auto evaluate_tide_from_constituents(
    const std::map<Constituent, std::complex<double>>& constituents,
    const Eigen::Ref<const Eigen::VectorXd>& epoch, const double longitude,
    const double latitude, const Settings& settings, const size_t num_threads)
    -> std::tuple<Eigen::VectorXd, Eigen::VectorXd> {
  // Allocates the result vectors
  auto tide = Eigen::VectorXd(epoch.size());
  auto long_period = Eigen::VectorXd(epoch.size());
  // Worker responsible for the calculation of the tide at a given position
  auto worker = [&](const int64_t start, const int64_t end) {
    auto acc = Accelerator<Constituent>(settings.astronomic_formulae(),
                                        settings.time_tolerance(), 0);
    auto wave_table = detail::build_wave_table_from_constituents(constituents);
    auto lpe = wave::LongPeriodEquilibrium(wave_table);

    for (auto ix = start; ix < end; ++ix) {
      std::tie(tide(ix), long_period(ix)) = detail::compute_tide_from_waves(
          wave_table, lpe, acc, epoch[ix], latitude);
    }
  };

  detail::parallel_for(worker, epoch.size(), num_threads);
  return {tide, long_period};
}

auto evaluate_equilibrium_long_period(
    const Eigen::Ref<const Eigen::VectorXd>& epoch,
    const Eigen::Ref<const Eigen::VectorXd>& latitude, const Settings& settings,
    const size_t num_threads) -> Eigen::VectorXd {
  // Checks the input parameters
  detail::check_eigen_shape("epoch", epoch, "latitude", latitude);
  auto result = Eigen::VectorXd(epoch.size());
  auto worker = [&](const int64_t start, const int64_t end) {
    auto angles = angle::Astronomic(settings.astronomic_formulae());
    auto model = wave::LongPeriodEquilibrium(fes::wave::Table());
    for (auto ix = start; ix < end; ++ix) {
      angles.update(epoch(ix));
      result(ix) = model.lpe_minus_n_waves(angles, latitude(ix));
    }
  };

  detail::parallel_for(worker, epoch.size(), num_threads);
  return result;
}

}  // namespace fes
