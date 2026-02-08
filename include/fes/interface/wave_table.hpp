// Copyright (c) 2026 CNES
//
// All rights reserved. Use of this source code is governed by a
// BSD-style license that can be found in the LICENSE file.
/// @file include/fes/interface/wave_table.hpp
/// @brief Wave table interface.
#pragma once

#include <memory>
#include <stdexcept>
#include <string>
#include <tuple>
#include <utility>
#include <vector>

#include "fes/angle/astronomic.hpp"
#include "fes/constituent.hpp"
#include "fes/enum_map.hpp"
#include "fes/interface/wave.hpp"
#include "fes/types.hpp"

namespace fes {

/// @brief Enumeration of available harmonic constituent notation systems
enum class EngineType : uint8_t {
  kDarwin = 0,  ///< Darwin's notation system, used by FES models
  kDoodson = 1  ///< Doodson's notation system, used by the GOT models
};

/// @brief Map of constituents to wave interfaces.
using ConstituentMap =
    EnumMap<ConstituentId, std::unique_ptr<WaveInterface>, kKnownConstituents>;

/// @brief Helper function to extract the key from a `std::vector<T>`
/// @tparam T The type of the elements in the vector
/// @param val The value from which to extract the key
/// @return The key extracted from the value
template <typename T>
constexpr auto extract_key(const T& val) -> const T& {
  return val;
}

/// @brief Helper function to extract the key from a `std::pair<K, V>`
/// @tparam K The type of the key in the pair
/// @tparam V The type of the value in the pair
/// @param p The pair from which to extract the key
/// @return The key extracted from the value
template <typename K, typename V>
constexpr auto extract_key(const std::pair<K, V>& p) -> const K& {
  return p.first;
}

/// @brief Tidal wave table interface.
class WaveTableInterface {
 public:
  /// @brief Default constructor.
  WaveTableInterface() = default;

  /// @brief Copy constructor
  WaveTableInterface(const WaveTableInterface& other) {
    for (const auto& item : other.map_) {
      map_.set(item.key(), item.value()->clone());
    }
  }

  /// @brief Move constructor
  WaveTableInterface(WaveTableInterface&& other) noexcept = default;

  /// @brief Copy assignment operator
  auto operator=(const WaveTableInterface& other) -> WaveTableInterface& {
    if (this != &other) {
      map_.clear();
      for (const auto& item : other.map_) {
        map_.set(item.key(), item.value()->clone());
      }
    }
    return *this;
  }

  /// @brief Move assignment operator
  auto operator=(WaveTableInterface&& other) noexcept
      -> WaveTableInterface& = default;

  /// @brief Destructor.
  virtual ~WaveTableInterface() = default;

  /// @brief Clones the wave table.
  /// @return A unique pointer to the cloned wave table.
  virtual auto clone() const -> std::unique_ptr<WaveTableInterface> = 0;

  /// @brief Set all provided constituents as modeled in the wave table.
  /// @tparam Container Iterable of constituent IDs or (ConstituentId, V) pairs.
  /// Elements may be a ConctiuentId or std::pair<ConstituentId, V>, where V is
  /// any associated value (e.g., amplitude).
  /// @param[in] constituents The list of constituent identifiers to set as
  /// modeled.
  template <typename Container>
  inline auto set_modeled_constituents(const Container& constituents) -> void {
    for (const auto& item : constituents) {
      const auto& ident = extract_key(item);
      auto* ptr = map_.get(ident)->get();
      if (ptr == nullptr) {
        throw out_of_range(ident);
      }
      ptr->set_is_modeled(true);
    }
  }

  /// Set the tide of a constituent
  /// @param[in] ident The constituent identifier
  /// @param[out] value The tide value
  void set_tide(ConstituentId ident, const Complex& value) {
    auto* ptr = map_.get(ident)->get();
    if (ptr == nullptr) {
      throw out_of_range(ident);
    }
    ptr->set_tide(value);
  }

  /// @brief Set the tide for the provided constituents.
  /// @tparam Container Iterable of (ConstituentId, Complex) pairs.
  /// @param[in] tides The map of constituent identifiers and their
  /// corresponding tide values to set.
  template <typename Container>
  inline auto set_tides(const Container& tides) -> void {
    for (const auto& item : tides) {
      set_tide(item.first, item.second);
    }
  }

  /// @brief Computes the nodal corrections for all constituents in the table.
  ///
  /// @param[in] angles Astronomical angles used to compute nodal corrections.
  /// @param[in] group_modulations If true, applies group modulations to nodal
  /// corrections.
  virtual auto compute_nodal_corrections(const angle::Astronomic& angles,
                                         const bool group_modulations)
      -> void = 0;

  /// @brief Get the wave corresponding at the given index.
  /// @param index The index of the constituent, must be in the range
  /// [0, size()).
  /// @return A constant reference to the unique pointer to the wave.
  inline auto operator[](const size_t index) const
      -> const std::unique_ptr<WaveInterface>& {
    return map_[index];
  }

  /// @brief Get the wave corresponding at the given index.
  /// @param index The index of the constituent, must be in the range
  /// [0, size()).
  /// @return A reference to the unique pointer to the wave.
  inline auto operator[](const size_t index)
      -> std::unique_ptr<WaveInterface>& {
    return map_[index];
  }

  /// @brief Get the wave corresponding to the given constituent identifier.
  /// @param ident The constituent identifier.
  /// @return A constant reference to the unique pointer to the wave.
  inline auto operator[](ConstituentId ident) const
      -> const std::unique_ptr<WaveInterface>& {
    const auto* ptr = map_.get(ident);
    if (ptr == nullptr) {
      throw out_of_range(ident);
    }
    return *ptr;
  }

  /// @brief Get the wave corresponding to the given constituent identifier.
  /// @param ident The constituent identifier.
  /// @return A reference to the unique pointer to the wave.
  inline auto operator[](ConstituentId ident)
      -> std::unique_ptr<WaveInterface>& {
    auto* ptr = map_.get(ident);
    if (ptr == nullptr) {
      throw out_of_range(ident);
    }
    return *ptr;
  }

  /// @brief Returns an iterator to the beginning of the wave table
  /// @return An iterator to the beginning of the wave table
  inline auto begin() const noexcept -> ConstituentMap::const_iterator {
    return map_.begin();
  }

  /// @brief Returns an iterator to the end of the wave table
  /// @return An iterator to the end of the wave table
  inline auto end() const noexcept -> ConstituentMap::const_iterator {
    return map_.end();
  }

  /// @brief Returns an iterator to the beginning of the wave table
  /// @return An iterator to the beginning of the wave table
  inline auto begin() noexcept -> ConstituentMap::iterator {
    return map_.begin();
  }

  /// @brief Returns an iterator to the end of the wave table
  /// @return An iterator to the end of the wave table
  inline auto end() noexcept -> ConstituentMap::iterator { return map_.end(); }

  /// @brief Gets the list of constituent names in the table
  /// @return The list of constituent names in the table
  inline auto constituents() const -> std::vector<std::string> {
    auto names = std::vector<std::string>();
    names.reserve(map_.size());
    for (const auto& item : map_) {
      names.emplace_back(item.value()->name());
    }
    return names;
  }

  /// Get the list of constituent identifiers in the table
  /// @return The list of constituent identifiers in the table
  inline auto constituent_ids() const -> std::vector<ConstituentId> {
    auto ids = std::vector<ConstituentId>();
    ids.reserve(map_.size());
    for (const auto& item : map_) {
      ids.emplace_back(item.key());
    }
    return ids;
  }

  /// @brief Returns the size of the table
  inline auto size() const noexcept -> size_t { return map_.size(); }

  /// @brief Selects the tidal constituents that can be resolved from a record
  /// of a given duration using the Rayleigh criterion.
  ///
  /// Converts the record duration to hours, computes the minimum angular
  /// separation @f$2\pi C_R / T@f$ then iterates over the wave table,
  /// rejecting waves too close to @f$DC@f$ or closer than the Rayleigh limit
  /// to any already - selected wave.
  ///
  /// @param[in] duration Duration of the time series analyzed in seconds
  /// @param[in] rayleigh_criterion The Rayleigh criterion factor (C_R) to
  /// determine the minimum frequency. Default is 1.0, which corresponds to
  /// the classical Rayleigh criterion.
  /// @return List of selected tidal waves.
  auto select_waves_for_analysis(double duration,
                                 double rayleigh_criterion = 1.0)
      -> std::vector<std::string>;

  /// @brief Calculate the tide of a given time series.
  ///
  /// @param[in] epoch Desired UTC time expressed in number of seconds elapsed
  /// since 1970-01-01T00:00:00.
  /// @param[in] wave Tidal wave properties computed by an harmonic analysis.
  /// @return the tide at the given time.
  /// @param[in] args Arguments required to compute the nodal corrections.
  auto tide_from_tide_series(const Eigen::Ref<const Eigen::VectorXd>& epoch,
                             const Eigen::Ref<const Eigen::VectorXcd>& wave,
                             NodalCorrectionsArgs& args) const
      -> Eigen::VectorXd;

  /// @brief Calculate the tide for a given date from a grid describing the
  /// properties of tidal waves over an area.
  ///
  /// @param[in] epoch Desired UTC time expressed in number of seconds elapsed
  /// since 1970-01-01T00:00:00.
  /// @param[in] wave Tidal wave properties computed by an harmonic analysis.
  /// @param[in] args Arguments required to compute the nodal corrections.
  /// @param[in] num_threads Number of threads to use for the computation. If
  /// set to 0, the number of threads is automatically determined.
  auto tide_from_mapping(double epoch,
                         const DynamicRef<const Eigen::MatrixXcd>& wave,
                         NodalCorrectionsArgs& args,
                         size_t num_threads = 0) const -> Eigen::MatrixXd;

  /// @brief Compute nodal modulations for amplitude and phase.
  ///
  /// @param[in] epoch: Desired UTC time expressed in number of seconds elapsed
  /// since 1970-01-01T00:00:00.
  /// @param[in] args Arguments required to compute the nodal corrections.
  /// @return The nodal correction for amplitude, v greenwich argument) + u
  /// (nodal correction for phase).
  /// @throw std::invalid_argument if the size of the epoch vector is not
  /// equal to the size of the leap seconds vector.
  auto compute_nodal_modulations(const Eigen::Ref<const Eigen::VectorXd>& epoch,
                                 NodalCorrectionsArgs& args) const
      -> std::tuple<Eigen::MatrixXd, Eigen::MatrixXd>;

  /// @brief Check if a constituent is in the table
  /// @param[in] ident The constituent identifier
  /// @return true if the constituent is in the table
  inline auto contains(const ConstituentId ident) const noexcept -> bool {
    return map_.contains(ident);
  }

  /// @brief Generate a markdown table summarizing the constituents handled by
  /// the wave table.
  auto generate_markdown_table() const -> std::string;

  /// @brief Returns the constituents sorted by frequency.
  /// @param[in] ascending If true (default), sort by ascending frequency;
  /// otherwise, sort by descending frequency.
  /// @return A vector of constituent identifiers sorted by frequency.
  auto sort_by_frequency(bool ascending = true) const
      -> std::vector<ConstituentId>;

 protected:
  /// @brief Type for wave factory functions.
  using WaveFactoryFunction = std::unique_ptr<WaveInterface> (*)(ConstituentId);

  /// @brief Populate the table with the given constituents using the provided
  /// wave factory function. Call this in derived class constructors after
  /// get_factory() is available.
  /// @param[in] constituents List of constituent identifiers to include in the
  /// table
  /// @param[in] wave_factory Factory function to create wave instances for the
  /// constituents
  void populate_map(const std::vector<ConstituentId>& constituents,
                    const WaveFactoryFunction& wave_factory) {
    for (const auto& ident : constituents) {
      map_.set(ident, wave_factory(ident));
    }
  }

 private:
  ConstituentMap map_;  ///< Map of constituents

  /// @brief Generates an out_of_range exception for a missing constituent.
  /// @param[in] ident The constituent identifier.
  /// @return The out_of_range exception.
  auto out_of_range(ConstituentId ident) const -> std::out_of_range {
    return std::out_of_range("Constituent ID#" +
                             std::to_string(static_cast<uint8_t>(ident)) +
                             " not found in the wave table.");
  }
};

/// @brief Factory function to create a wave table based on the specified engine
/// type.
/// @param[in] engine_type The type of the engine for which to create the wave
/// table.
/// @return A unique pointer to the created wave table.
auto wave_table_factory(const EngineType engine_type)
    -> std::unique_ptr<WaveTableInterface>;

/// @brief Factory function to create a sparse wave table based on the specified
/// engine type and list of constituents.
/// @param[in] engine_type The type of the engine for which to create the wave
/// table.
/// @param[in] constituents The list of constituent identifiers to include in
/// the wave table. If empty, all known constituents for the engine type are
/// included.
/// @return A unique pointer to the created wave table.
auto wave_table_factory(const EngineType engine_type,
                        const std::vector<std::string>& constituents)
    -> std::unique_ptr<WaveTableInterface>;

}  // namespace fes
