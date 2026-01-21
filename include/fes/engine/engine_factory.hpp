// Copyright (c) 2025 CNES
//
// All rights reserved. Use of this source code is governed by a
// BSD-style license that can be found in the LICENSE file.
/// @file include/fes/engine/engine_factory.hpp
/// @brief Factory for creating prediction engines.
#pragma once
#include <memory>

#include "fes/engine/abstract_prediction_engine.hpp"
#include "fes/engine/fes_engine.hpp"

// #ifdef FES_PERTH5_ENABLED
// #include "fes/perth5/perth5_engine.hpp"
// #endif

namespace fes {

/// @brief Factory class for creating prediction engines.
///
/// This factory provides static methods for creating prediction engines
/// from various model types. It abstracts the engine creation process and
/// ensures proper initialization.
class EngineFactory {
 public:
  /// @brief Create an FES engine from an FES tidal model.
  ///
  /// @tparam T The floating-point type for tidal constituent data.
  /// @param[in] model Shared pointer to the FES tidal model.
  /// @return Unique pointer to the created FES engine.
  template <typename T>
  static auto create_fes_engine(std::shared_ptr<AbstractTidalModel<T>> model)
      -> std::unique_ptr<AbstractPredictionEngine<T>> {
    return std::make_unique<FESEngine<T>>(std::move(model));
  }

  // #ifdef FES_PERTH5_ENABLED
  //   /// @brief Create a PERTH5 engine from a PERTH5 tidal model.
  //   ///
  //   /// This method is only available when FES_PERTH5_ENABLED is defined.
  //   ///
  //   /// @tparam T The floating-point type for tidal constituent data.
  //   /// @param[in] model Shared pointer to the PERTH5 tidal model.
  //   /// @param[in] inference_type The admittance interpolation method to use.
  //   /// @return Unique pointer to the created PERTH5 engine.
  //   template <typename T>
  //   static auto create_perth5_engine(
  //       std::shared_ptr<perth5::TidalModel<T>> model,
  //       perth5::InferenceType inference_type =
  //       perth5::InferenceType::kLinear)
  //       -> std::unique_ptr<AbstractPredictionEngine<T>> {
  //     return std::make_unique<perth5::Perth5Engine<T>>(std::move(model),
  //                                                      inference_type);
  //   }
  // #endif
};

}  // namespace fes
