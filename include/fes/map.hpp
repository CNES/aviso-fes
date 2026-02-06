// Copyright (c) 2026 CNES
//
// All rights reserved. Use of this source code is governed by a
// BSD-style license that can be found in the LICENSE file.
/// @file include/fes/map.hpp
/// @brief Map using binary search for small set.
#pragma once

#include <algorithm>
#include <array>
#include <cstddef>
#include <stdexcept>

namespace fes {

/// @brief Map using binary search for small set.
/// @tparam KeyT Type of the keys.
/// @tparam ValueT Type of the values.
/// @tparam N Number of elements in the map.
template <typename KeyT, typename ValueT, size_t N>
class Map {
  /// Array of key-value pairs.
  std::array<std::pair<KeyT, ValueT>, N> data_{};
  /// Track actual number of items in the map
  size_t size_ = 0;

 public:
  /// @brief Constructor.
  /// @param[in] data Array of key-value pairs.
  /// @note This constructor assumes keys are unique. The caller is responsible
  /// for ensuring no duplicate keys exist. Use the initializer list constructor
  /// if duplicate detection is required.
  explicit constexpr Map(std::array<std::pair<KeyT, ValueT>, N>&& data) noexcept
      : data_(std::move(data)), size_(N) {}

  /// @brief Constructor from initializer list of pairs.
  /// @param[in] init Initializer list of key-value pairs.
  /// @throws std::out_of_range if initializer list exceeds map capacity.
  /// @throws std::invalid_argument if duplicate keys are found in the
  /// initializer list.
  constexpr Map(std::initializer_list<std::pair<KeyT, ValueT>> init) {
    for (const auto& pair : init) {
      if (size_ >= N) {
        throw std::out_of_range("Initializer list exceeds map capacity");
      }
      if (!insert(pair.first, pair.second)) {
        throw std::invalid_argument("Duplicate key in initializer list");
      }
    }
  }

  /// @brief Default constructor.
  constexpr Map() noexcept = default;

  /// @brief Destructor.
  ~Map() = default;

  /// @brief Inserts a key-value pair if key not already present.
  /// @return true if inserted, false if duplicate or full.
  constexpr auto insert(KeyT key, ValueT value) noexcept -> bool {
    if (size_ >= N || contains(key)) {
      return false;
    }
    data_[size_++] = {std::move(key), std::move(value)};
    return true;
  }

  /// @brief Inserts or updates a key-value pair.
  /// @return true if inserted, false if updated or full (when new).
  constexpr auto insert_or_assign(KeyT key, ValueT value) noexcept -> bool {
    if (auto* ptr = get(key)) {
      *ptr = std::move(value);
      return false;  // Updated existing
    }
    if (size_ >= N) {
      return false;  // Full
    }
    data_[size_++] = {std::move(key), std::move(value)};
    return true;  // Inserted new
  }

  /// @brief Gets the value associated with a key.
  /// @param[in] key The key.
  /// @return Pointer to the associated value, or nullptr if the key is not
  /// present.
  constexpr auto get(KeyT key) const noexcept -> const ValueT* {
    auto end = data_.begin() + size_;
    auto it = std::find_if(data_.begin(), end, [&key](const auto& pair) {
      return pair.first == key;
    });
    return (it != end) ? &it->second : nullptr;
  }

  /// @brief Gets the mutable value associated with a key.
  /// @param[in] key The key.
  /// @return Pointer to the associated value, or nullptr if the key is not
  /// present.
  constexpr auto get(KeyT key) noexcept -> ValueT* {
    auto end = data_.begin() + size_;
    auto it = std::find_if(data_.begin(), end, [&key](const auto& pair) {
      return pair.first == key;
    });
    return (it != end) ? &it->second : nullptr;
  }

  /// @brief Gets the value associated with a key.
  /// @param[in] key The key.
  /// @return Reference to the associated value.
  constexpr auto operator[](KeyT key) const noexcept -> const ValueT& {
    return *get(key);
  }

  /// @brief Gets the mutable value associated with a key.
  /// @param[in] key The key.
  /// @return Reference to the associated value.
  constexpr auto operator[](KeyT key) noexcept -> ValueT& { return *get(key); }

  /// @brief Checks if a key is present in the map.
  /// @param[in] key The key.
  /// @return true if the key is present, false otherwise.
  constexpr auto contains(KeyT key) const noexcept -> bool {
    return get(key) != nullptr;
  }

  /// @brief Gets the number of elements in the map.
  /// @return Number of elements.
  constexpr auto size() const noexcept -> size_t { return size_; }

  /// @brief Gets an iterator to the beginning.
  /// @return Iterator to the beginning.
  constexpr auto begin() const noexcept { return data_.begin(); }

  /// @brief Gets an iterator to the end.
  /// @return Iterator to the end.
  constexpr auto end() const noexcept { return data_.begin() + size_; }

  /// @brief Gets a mutable iterator to the beginning.
  /// @return Mutable iterator to the beginning.
  constexpr auto begin() noexcept { return data_.begin(); }

  /// @brief Gets a mutable iterator to the end.
  /// @return Mutable iterator to the end.
  constexpr auto end() noexcept { return data_.begin() + size_; }
};

}  // namespace fes
