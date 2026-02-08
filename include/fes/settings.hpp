// Copyright (c) 2026 CNES
//
// All rights reserved. Use of this source code is governed by a
// BSD-style license that can be found in the LICENSE file.
/// @file include/fes/settings.hpp
/// @brief Settings for the tide computation.
#pragma once

#include <string>
#include <vector>

#include "fes/angle/astronomic.hpp"
#include "fes/inference.hpp"
#include "fes/interface/wave_table.hpp"

namespace fes {

/// @brief Settings for the tide computation.
class Settings {
 public:
  /// @brief Constructor.
  constexpr Settings() = default;

  /// @brief Set the engine type for the tidal constituent notation system.
  /// @param engine_type The engine type.
  auto with_engine_type(EngineType engine_type) noexcept -> Settings& {
    engine_type_ = engine_type;
    return *this;
  }

  /// @brief Get the engine type for the tidal constituent notation system.
  /// @return The engine type.
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
  /// @note Only effective with the Perth WaveTable.
  auto with_group_modulations(bool group_modulations) -> Settings& {
    group_modulations_ = group_modulations;
    return *this;
  }

  /// @brief Get whether to use group modulation nodal corrections.
  /// @return True if group modulation nodal corrections are used.
  constexpr auto group_modulations() const noexcept -> bool {
    return group_modulations_;
  }

  /// @brief Set whether to compute the long period equilibrium tide.
  constexpr auto with_compute_long_period_equilibrium(
      bool compute_long_period_equilibrium) -> Settings& {
    compute_long_period_equilibrium_ = compute_long_period_equilibrium;
    return *this;
  }

  /// @brief Get whether to compute the long period equilibrium tide.
  /// @return True if the long period equilibrium tide is computed.
  constexpr auto compute_long_period_equilibrium() const noexcept -> bool {
    return compute_long_period_equilibrium_;
  }

  /// @brief Set the inference interpolation type.
  auto with_inference_type(InferenceType inference_type) -> Settings& {
    inference_type_ = inference_type;
    return *this;
  }

  /// @brief Get the inference interpolation type.
  constexpr auto inference_type() const noexcept -> InferenceType {
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
  /// @brief Engine type for the tidal constituent notation system.
  EngineType engine_type_{EngineType::kDarwin};
  /// @brief Astronomic formulae used to calculate the astronomic angles.
  angle::Formulae astronomic_formulae_{angle::Formulae::kSchuremanOrder1};
  /// Type of inference interpolation to use
  InferenceType inference_type_{InferenceType::kSpline};
  /// @brief Time in seconds for which astronomical angles are considered
  /// constant.
  double time_tolerance_{0.0};
  /// Whether to use group modulation nodal corrections
  bool group_modulations_{false};
  /// Whether to compute the long period equilibrium tide.
  bool compute_long_period_equilibrium_{true};
  /// Number of threads to use for computation
  size_t num_threads_{0};
};

/// @brief Default settings for FES models.
class FESSettings : public Settings {
 public:
  /// @brief Constructor.
  constexpr FESSettings() : Settings() {
    // Default constructor of Settings already initializes the settings with the
    // default values for FES models
  }
};

/// @brief Default settings for GOT models.
class PerthSettings : public Settings {
 public:
  /// @brief Constructor.
  constexpr PerthSettings() : Settings() {
    // Override the default settings for FES models with the default settings
    // for GOT models
    engine_type_ = EngineType::kDoodson;
    astronomic_formulae_ = angle::Formulae::kIERS;
    inference_type_ = InferenceType::kLinear;
    compute_long_period_equilibrium_ = false;
  }
};

/// @brief Generate a Markdown table describing the settings, the constiuents
/// provided by the model and inferred.
/// @param[in] settings The settings for which to generate the table.
/// @param[in] ascending If true (default), sort the table by ascending
/// frequency; otherwise, sort by descending frequency.
/// @param[in] modeled_constituents The list of constituents provided by
/// the model. Default is empty, in which case the table will only contain
/// the inferred constituents.
/// @return A string containing the Markdown table.
auto generate_markdown_table(
    const Settings& settings, bool ascending = true,
    const std::vector<ConstituentId>& modeled_constituents = {}) -> std::string;

}  // namespace fes
