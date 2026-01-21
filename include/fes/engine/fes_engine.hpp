// Copyright (c) 2025 CNES
//
// All rights reserved. Use of this source code is governed by a
// BSD-style license that can be found in the LICENSE file.
/// @file include/fes/engine/fes_engine.hpp
/// @brief FES prediction engine implementation.
#pragma once
#include <memory>

#include "fes/abstract_tidal_model.hpp"
#include "fes/engine/abstract_prediction_engine.hpp"
#include "fes/tide.hpp"

namespace fes {

/// @brief Prediction engine using FES tidal prediction code.
///
/// This engine wraps the existing FES tidal model infrastructure, providing
/// access to the original FES prediction algorithm through the unified engine
/// interface. It supports both Cartesian grids and LGP finite element meshes.
/// @tparam T The floating-point type for tidal constituent data.
template <typename T>
class FESEngine : public AbstractPredictionEngine<T> {
 public:
  /// @brief Construct an FES engine from a tidal model.
  ///
  /// @param[in] model Shared pointer to the FES tidal model. The model must
  /// remain valid for the lifetime of the engine.
  explicit FESEngine(std::shared_ptr<AbstractTidalModel<T>> model)
      : model_(std::move(model)) {}

  /// Default destructor
  ~FESEngine() override = default;

  /// @brief Evaluate tidal heights using the FES algorithm.
  ///
  /// This method delegates to the existing fes::evaluate_tide() function,
  /// maintaining full backward compatibility with the original FES API.
  ///
  /// @param[in] epoch Array of times as seconds since 1970-01-01T00:00:00Z.
  /// @param[in] longitude Array of longitudes in degrees.
  /// @param[in] latitude Array of latitudes in degrees.
  /// @param[in] settings Configuration settings for the prediction.
  /// @param[in] num_threads Number of threads for parallel computation.
  /// @return Tuple of (short_period_tide, long_period_tide, quality).
  inline auto evaluate(const Eigen::Ref<const Eigen::VectorXd>& epoch,
                       const Eigen::Ref<const Eigen::VectorXd>& longitude,
                       const Eigen::Ref<const Eigen::VectorXd>& latitude,
                       const Settings& settings = Settings(),
                       size_t num_threads = 0) const
      -> std::tuple<Eigen::VectorXd, Eigen::VectorXd,
                    Vector<Quality>> override {
    return fes::evaluate_tide(model_.get(), epoch, longitude, latitude,
                              settings, num_threads);
  }

  /// @brief Get the engine type.
  /// @return Always returns EngineType::kFES.
  constexpr auto engine_type() const noexcept -> EngineType override {
    return EngineType::kFES;
  }

  /// @brief Get the tide type from the underlying model.
  /// @return The tide type (ocean tide or radial tide).
  constexpr auto tide_type() const noexcept -> TideType override {
    return model_->tide_type();
  }

  /// @brief Check if the underlying model is empty.
  /// @return true if no constituents are loaded.
  constexpr auto empty() const noexcept -> bool override {
    return model_->empty();
  }

  /// @brief Get the number of constituents in the underlying model.
  /// @return The number of tidal constituents.
  constexpr auto size() const noexcept -> size_t override {
    return model_->size();
  }

  /// @brief Get the underlying tidal model.
  ///
  /// This provides access to the FES-specific model for cases where
  /// engine-specific functionality is needed.
  ///
  /// @return Shared pointer to the underlying AbstractTidalModel.
  constexpr auto model() const noexcept
      -> const std::shared_ptr<AbstractTidalModel<T>>& {
    return model_;
  }

  /// @brief Get the tidal constituent identifiers from the model.
  /// @return Vector of constituent identifiers.
  inline auto identifiers() const -> std::vector<Constituent> {
    return model_->identifiers();
  }

 private:
  /// The underlying FES tidal model
  std::shared_ptr<AbstractTidalModel<T>> model_;
};

}  // namespace fes
