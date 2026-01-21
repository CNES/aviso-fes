// Copyright (c) 2025 CNES
//
// All rights reserved. Use of this source code is governed by a
// BSD-style license that can be found in the LICENSE file.
/// @file include/fes/engine/abstract_prediction_engine.hpp
/// @brief Abstract prediction engine interface for unified tide prediction.
#pragma once
#include <tuple>

#include "fes/abstract_tidal_model.hpp"
#include "fes/eigen.hpp"
#include "fes/settings.hpp"

namespace fes {

/// @brief Enumeration of available prediction engine types.
enum class EngineType : uint8_t {
  kFES = 0,    ///< FES prediction engine
  kPERTH5 = 1  ///< PERTH5 prediction engine
};

/// @brief Abstract base class for tide prediction engines.
///
/// This interface provides a unified API for tide prediction across different
/// engine implementations (FES and PERTH5). Each engine may use different
/// internal representations (Darwin vs Doodson coefficients) and algorithms
/// (different admittance methods, nodal corrections), but they all expose the
/// same prediction interface.
///
/// @tparam T The floating-point type for tidal constituent data.
template <typename T>
class AbstractPredictionEngine {
 public:
  /// Default constructor
  AbstractPredictionEngine() = default;

  /// Virtual destructor
  virtual ~AbstractPredictionEngine() = default;

  // Disable copy
  AbstractPredictionEngine(const AbstractPredictionEngine&) = delete;
  auto operator=(const AbstractPredictionEngine&)
      -> AbstractPredictionEngine& = delete;

  // Enable move
  AbstractPredictionEngine(AbstractPredictionEngine&&) noexcept = default;
  auto operator=(AbstractPredictionEngine&&) noexcept
      -> AbstractPredictionEngine& = default;

  /// @brief Evaluate tidal heights at given positions and times.
  ///
  /// This is the main prediction method. It computes both short-period
  /// (diurnal, semi-diurnal) and long-period tidal constituents.
  ///
  /// @param[in] epoch Array of times as seconds since 1970-01-01T00:00:00Z.
  /// @param[in] longitude Array of longitudes in degrees.
  /// @param[in] latitude Array of latitudes in degrees.
  /// @param[in] settings Configuration settings for the prediction.
  /// @param[in] num_threads Number of threads for parallel computation (0 =
  /// auto).
  /// @return A tuple containing:
  ///   - Vector of short-period tide heights (diurnal + semi-diurnal)
  ///   - Vector of long-period tide heights
  ///   - Vector of quality flags indicating interpolation reliability
  virtual auto evaluate(const Eigen::Ref<const Eigen::VectorXd>& epoch,
                        const Eigen::Ref<const Eigen::VectorXd>& longitude,
                        const Eigen::Ref<const Eigen::VectorXd>& latitude,
                        const Settings& settings = Settings(),
                        size_t num_threads = 0) const
      -> std::tuple<Eigen::VectorXd, Eigen::VectorXd, Vector<Quality>> = 0;

  /// @brief Get the type of this prediction engine.
  /// @return The engine type (FES or PERTH5).
  virtual auto engine_type() const noexcept -> EngineType = 0;

  /// @brief Get the tide type handled by this engine.
  /// @return The tide type (ocean tide or radial tide).
  virtual auto tide_type() const noexcept -> TideType = 0;

  /// @brief Check if the engine has any constituent data loaded.
  /// @return true if no constituents are loaded, false otherwise.
  virtual auto empty() const noexcept -> bool = 0;

  /// @brief Get the number of constituents handled by this engine.
  /// @return The number of tidal constituents.
  virtual auto size() const noexcept -> size_t = 0;
};

}  // namespace fes
