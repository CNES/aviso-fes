// Copyright (c) 2025 CNES
//
// All rights reserved. Use of this source code is governed by a
// BSD-style license that can be found in the LICENSE file.
#include "fes/tide.hpp"

#include "fes/detail/broadcast.hpp"
#include "fes/detail/thread.hpp"

namespace fes {

auto evaluate_equilibrium_long_period(
    const Eigen::Ref<const Eigen::VectorXd>& epoch,
    const Eigen::Ref<const fes::Vector<uint16_t>>& leap_seconds,
    const Eigen::Ref<const Eigen::VectorXd>& latitude, const Settings& settings,
    const size_t num_threads) -> Eigen::VectorXd {
  // Checks the input parameters
  detail::check_eigen_shape("epoch", epoch, "leap_seconds", leap_seconds,
                            "latitude", latitude);
  auto result = Eigen::VectorXd(epoch.size());
  auto worker = [&](const int64_t start, const int64_t end) {
    auto angles = angle::Astronomic(settings.astronomic_formulae());
    auto model = wave::LongPeriodEquilibrium(fes::wave::Table());
    for (auto ix = start; ix < end; ++ix) {
      angles.update(epoch(ix), leap_seconds(ix));
      result(ix) = model.lpe_minus_n_waves(angles, latitude(ix));
    }
  };

  detail::parallel_for(worker, epoch.size(), num_threads);
  return result;
}

}  // namespace fes
