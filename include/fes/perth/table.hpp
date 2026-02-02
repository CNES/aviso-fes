// Copyright (c) 2026 CNES
//
// All rights reserved. Use of this source code is governed by a
// BSD-style license that can be found in the LICENSE file.
/// @file include/fes/perth/table.hpp
/// @brief Perth tidal constituents table.
#pragma once

#include <Eigen/Core>
#include <array>
#include <cstddef>
#include <limits>
#include <stdexcept>
#include <string>
#include <utility>
#include <vector>

#include "fes/abstract_tidal_model.hpp"
#include "fes/perth/constituent.hpp"
#include "fes/perth/doodson.hpp"
#include "fes/perth/nodal_corrections.hpp"
#include "fes/tidal_constituents.hpp"

namespace fes {
namespace perth {

/// @brief Class representing a set of tidal constituents and their associated
/// tide components.
/// @tparam T The type of the tide component.
template <typename T>
class WaveSet : public TidalConstituents {
 public:
  /// @brief Type of the container that holds the keys of the wave set.
  using Key = std::array<Constituent, kNumConstituentItems>;
  /// @brief Type of the container that holds the items of the wave set.
  using Item = std::array<T, kNumConstituentItems>;

  /// @brief Default constructor
  constexpr WaveSet() noexcept = default;

  /// @brief Constructor with keys and items
  /// @param[in] keys The keys of the constituent set
  /// @param[in] items The items of the constituent set
  constexpr WaveSet(Key &&keys, Item &&items) noexcept
      : keys_(std::move(keys)),
        items_(std::move(items)),
        keys_vector_(keys_.begin(), keys_.end()),
        nodal_corrections_(keys_.size()) {
    std::fill(nodal_corrections_.begin(), nodal_corrections_.end(),
              NodalCorrections{std::numeric_limits<double>::quiet_NaN(),
                               std::numeric_limits<double>::quiet_NaN()});
  }

  /// @brief Accessor operator
  constexpr auto operator[](Constituent constituent) const noexcept
      -> const T & {
    return items_[static_cast<std::size_t>(constituent)];
  }

  /// @brief Accessor operator
  /// @param[in] constituent The constituent to access
  /// @return Reference to the tide component associated with the constituent
  constexpr auto operator[](Constituent constituent) noexcept -> T & {
    return items_[static_cast<std::size_t>(constituent)];
  }

  /// @brief Get the size of the constituent set
  constexpr auto size() const noexcept -> std::size_t {
    return kNumConstituentItems;
  }

  /// @brief Get the keys of the constituent set
  constexpr auto keys() const noexcept -> const Key & { return keys_; }

  /// @brief Get the keys of the constituent set
  constexpr auto items() const noexcept -> const Item & { return items_; }

  /// @brief Get the items of the constituent set
  constexpr auto items() noexcept -> Item & { return items_; }

  /// @brief Check if a constituent is in the table
  /// @param[in] ident The constituent identifier
  /// @return true if the constituent is in the table
  inline auto contains(const Constituent ident) const noexcept -> bool {
    return std::find(keys_.begin(), keys_.end(), ident) != keys_.end();
  }

  /// @brief Get the keys as a vector
  constexpr auto keys_vector() const -> const std::vector<Constituent> & {
    return keys_vector_;
  }

  /// @brief Get iterators to the keys
  constexpr auto begin() const noexcept ->
      typename std::vector<Constituent>::const_iterator {
    return keys_vector_.begin();
  }

  /// @brief Get iterators to the keys
  constexpr auto end() const noexcept ->
      typename std::vector<Constituent>::const_iterator {
    return keys_vector_.end();
  }

  /// @brief Access the constituent and its associated item by index
  /// @param[in] index The index of the constituent
  /// @return A pair of the constituent and its associated item
  constexpr auto at(const size_t index) const
      -> const std::pair<Constituent, T> {
    if (index >= static_cast<size_t>(kNumConstituentItems)) {
      throw std::out_of_range("Index out of range");
    }
    return {keys_[index], items_[index]};
  }

  /// Set the tide of a constituent
  /// @param[in] ident The constituent identifier
  /// @param[out] value The tide value
  void set_tide(ConstituentId ident, const Complex &value) override {
    if (ident >= items_.size()) {
      throw std::out_of_range("ident out of range");
    }
    set_tide(static_cast<Constituent>(ident), value);
  }

  /// Set the tide of a constituent
  /// @param[in] ident The constituent identifier
  /// @param[out] value The tide value
  void set_tide(Constituent ident, const Complex &value) {
    items_[static_cast<std::size_t>(ident)].tide = value;
  }

  /// @brief Update the nodal corrections for all constituents at a given epoch
  /// @param[in] epoch The epoch in seconds since 1970-01-01T00:00:00Z
  /// @param[in] group_modulations Whether to group modulations
  /// @param[in] acc The accelerator to use for angle calculations
  auto update_nodal_corrections(const double epoch,
                                const bool group_modulations, Accelerator &acc)
      -> void;

  //// @brief Get the nodal correction for a given constituent
  /// @param[in] constituent The constituent identifier
  /// @return The nodal correction associated with the constituent
  constexpr auto nodal_correction(const Constituent constituent) const
      -> const NodalCorrections & {
    auto index = static_cast<std::size_t>(constituent);
    return nodal_corrections_[index];
  }

 private:
  /// The keys of the constituent set.
  Key keys_{};
  /// The items of the constituent set.
  Item items_{};
  /// The keys as a vector.
  std::vector<Constituent> keys_vector_;
  /// The nodal corrections for each constituent.
  std::vector<NodalCorrections> nodal_corrections_;
};

/// @brief Data structure that holds the tide of a constituent.
struct Wave {
  Vector7b doodson_numbers;  //!< Doodson number of the constituent
  Complex tide;              //!< Tide of the constituent
  double tidal_argument;     //!< Doodson argument
  TidalType type;            //!< Type of tidal wave
  Constituent id;            //!< Constituent identifier
  bool is_inferred = false;  //!< Whether the tide was inferred from the
                             //!< constituents
};

/// @brief Alias for a constituent table.
class WaveTable : public WaveSet<Wave> {
 public:
  /// @brief Create the PERTH wave table.
  /// @param[in] constituents The list of constituent names provided by the
  ///     model. Constituents not in this list will be inferred.
  explicit WaveTable(const std::vector<Constituent> &constituents = {});

  /// @brief Create the PERTH wave table.
  /// @param[in] constituent_names The list of constituent names provided by the
  ///     model. Constituents not in this list will be inferred.
  explicit WaveTable(const std::vector<std::string> &constituent_names);
};

template <typename T>
auto WaveSet<T>::update_nodal_corrections(const double epoch,
                                          const bool group_modulations,
                                          Accelerator &acc) -> void {
  auto angle = acc.calculate_angle(epoch);
  auto args = calculate_celestial_vector(angle);
  const auto perigee = args(3);
  const auto omega = -args(4);
  if (group_modulations) {
    const auto hsolar = args(2);
    const auto psolar = args(5);
    nodal_corrections_ =
        compute_nodal_corrections(psolar, omega, perigee, hsolar, keys_vector_);
  } else {
    nodal_corrections_ =
        compute_nodal_corrections(omega, perigee, keys_vector_);
  }
  for (const auto &key : keys_) {
    auto &component = (*this)[key];
    component.tidal_argument = calculate_doodson_argument(
        angle, component.doodson_numbers.template cast<double>());
  }
}

}  // namespace perth
}  // namespace fes
