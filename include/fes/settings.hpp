// Copyright (c) 2026 CNES
//
// All rights reserved. Use of this source code is governed by a
// BSD-style license that can be found in the LICENSE file.
/// @file include/fes/settings.hpp
/// @brief Settings for the tide computation.
#pragma once

#include <cstdint>

#include "fes/angle/astronomic.hpp"

namespace fes {

/// @brief Enumeration of available prediction engine types.
enum class EngineType : uint8_t {
  kFES = 0,    ///< FES prediction engine
  kPERTH5 = 1  ///< PERTH5 prediction engine
};

/// @brief Enumeration of inference (admittance) interpolation types.
enum class InterpolationType : uint8_t {
  kZeroAdmittance,     ///< No interpolation of admittances.
  kLinearAdmittance,   ///< Piecewise linear interpolation of admittances.
  kFourierAdmittance,  ///< Munk-Cartwright Fourier series interpolation.
};

/// @brief Settings for the tide computation.
class Settings {
 public:
  /// @brief Constructor.
  explicit constexpr Settings(EngineType engine_type)
      : engine_type_(engine_type) {}

  /// @brief Get the engine type.
  /// @return Engine type.
  constexpr auto engine_type() const noexcept -> EngineType {
    return engine_type_;
  }

  /// @brief Set the astronomic formulae.
  /// @param formulae Astronomic formulae.
  auto with_astronomic_formulae(angle::Formulae formulae) noexcept
      -> Settings& {
    astronomic_formulae_ = formulae;
    return *this;
  }

  /// @brief Get the astronomic formulae.
  /// @return Astronomic formulae.
  constexpr auto astronomic_formulae() const noexcept -> angle::Formulae {
    return astronomic_formulae_;
  }

  /// @brief Set the time tolerance.
  /// @param time_tolerance Time tolerance in seconds.
  auto with_time_tolerance(double time_tolerance) noexcept -> Settings& {
    time_tolerance_ = time_tolerance;
    return *this;
  }

  /// @brief Get the time tolerance.
  /// @return Time tolerance in seconds.
  constexpr auto time_tolerance() const noexcept -> double {
    return time_tolerance_;
  }

  /// @brief Set whether to use group modulation nodal corrections.
  auto with_group_modulations(bool group_modulations) -> Settings& {
    if (engine_type_ == EngineType::kFES) {
      throw std::runtime_error(
          "Group modulations are not supported with FES engine.");
    }
    group_modulations_ = group_modulations;
    return *this;
  }

  /// @brief Get whether to use group modulation nodal corrections.
  /// @return True if group modulation nodal corrections are used.
  constexpr auto group_modulations() const noexcept -> bool {
    return group_modulations_;
  }

  /// @brief Set the inference interpolation type.
  auto with_inference_type(InterpolationType inference_type) -> Settings& {
    if (engine_type_ == EngineType::kFES) {
      throw std::runtime_error(
          "Inference interpolation is not supported with FES engine.");
    }
    inference_type_ = inference_type;
    return *this;
  }

  /// @brief Get the inference interpolation type.
  constexpr auto inference_type() const noexcept -> InterpolationType {
    return inference_type_;
  }

  /// @brief Set the number of threads to use for computation.
  /// @param num_threads Number of threads. If 0, the number of threads is
  /// automatically determined.
  auto with_num_threads(size_t num_threads) noexcept -> Settings& {
    num_threads_ = num_threads;
    return *this;
  }

  /// @brief Get the number of threads to use for computation.
  /// @return Number of threads.
  constexpr auto num_threads() const noexcept -> size_t { return num_threads_; }

 protected:
  /// Egine type to use for the tide computation
  EngineType engine_type_;
  /// @brief Astronomic formulae used to calculate the astronomic angles.
  angle::Formulae astronomic_formulae_{angle::Formulae::kSchuremanOrder1};
  /// @brief Time in seconds for which astronomical angles are considered
  /// constant.
  double time_tolerance_{0.0};
  /// Whether to use group modulation nodal corrections
  bool group_modulations_{false};
  /// Type of inference interpolation to use
  InterpolationType inference_type_{InterpolationType::kZeroAdmittance};
  /// Number of threads to use for computation
  size_t num_threads_{0};
};

/// @brief Settings for the FES engine.
class FesRuntimeSettings : public Settings {
 public:
  /// @brief Constructor.
  constexpr FesRuntimeSettings() : Settings(EngineType::kFES) {}
};

/// @brief Settings for the PERTH5 engine.
class PerthRuntimeSettings : public Settings {
 public:
  /// @brief Constructor.
  constexpr PerthRuntimeSettings() : Settings(EngineType::kPERTH5) {
    // Initialize default settings for PERTH5 engine: use IERS astronomic
    // formulae and linear admittance interpolation. Group modulations remain
    // disabled by default, preserving the original PERTH5 behavior.
    astronomic_formulae_ = angle::Formulae::kIERS;
    inference_type_ = InterpolationType::kLinearAdmittance;
  }
};

}  // namespace fes
