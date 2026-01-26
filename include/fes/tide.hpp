#pragma once

#include "darwin/tide.hpp"
#include "fes/settings.hpp"
#include "perth/tide.hpp"

namespace fes {

template <typename T>
auto evaluate_tide(const AbstractTidalModel<T>* const tidal_model,
                   const Eigen::Ref<const Eigen::VectorXd>& epoch,
                   const Eigen::Ref<const Eigen::VectorXd>& longitude,
                   const Eigen::Ref<const Eigen::VectorXd>& latitude,
                   const Settings& settings)
    -> std::tuple<Eigen::VectorXd, Eigen::VectorXd, Vector<Quality>> {
  if (settings.engine_type() == EngineType::kFES) {
    return darwin::evaluate_tide(tidal_model, epoch, longitude, latitude,
                                 settings);
  }
  if (settings.engine_type() == EngineType::kPERTH5) {
    return perth::evaluate_tide(tidal_model, epoch, longitude, latitude,
                                settings);
  }
  throw std::invalid_argument("Unsupported engine type for tidal evaluation.");
}

}  // namespace fes
