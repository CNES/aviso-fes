// Copyright (c) 2025 CNES
//
// All rights reserved. Use of this source code is governed by a
// BSD-style license that can be found in the LICENSE file.
/// @file include/fes/enum_mapper.hpp
/// @brief Enum mapper utility.
#pragma once

#include <algorithm>
#include <cstddef>
#include <stdexcept>
#include <string>
#include <vector>

#include "fes/detail/string.hpp"

namespace fes {

/// @brief Helper class to map enum values to strings and vice versa.
/// @tparam T The value type to map.
template <typename T>
struct EnumEntry {
  T value;
  std::string name;
};

/// @brief Class to map enum values to strings and vice versa.
/// @tparam T The value type to map.
template <typename T>
class EnumMapper {
 public:
  /// @brief Default constructor.
  EnumMapper() = default;

  /// @brief Reserve space for n entries.
  /// @param n The number of entries to reserve space for.
  inline auto reserve(size_t n) -> void { data_.reserve(n); }

  /// @brief Add an entry to the mapper.
  /// @param[in] value The enum value.
  /// @param[in] name The string representation of the enum value.
  inline auto add_entry(const T value, const std::string& name) -> void {
    data_.push_back(EnumEntry<T>{value, name});
  }

  /// @brief Convert a string to an enum value.
  /// @param[in] str The string to convert.
  /// @return The corresponding enum value.
  auto from_string(const std::string& str) const -> T {
    auto it = std::find_if(data_.begin(), data_.end(),
                           [&str](const EnumEntry<T>& entry) {
                             return detail::iequals(entry.name, str);
                           });

    if (it != data_.end()) {
      return it->value;
    }
    throw std::invalid_argument("key not found: " + str);
  }

  /// @brief Convert an enum value to a string.
  /// @param[in] value The enum value to convert.
  /// @return The corresponding string.
  auto to_string(T value) const -> const std::string& {
    auto it = std::find_if(
        data_.begin(), data_.end(),
        [value](const EnumEntry<T>& entry) { return entry.value == value; });

    if (it != data_.end()) {
      return it->name;
    }
    throw std::invalid_argument("value not found: " + std::to_string(value));
  }

  /// @brief Check if an enum value exists.
  /// @param[in] value The enum value to check.
  /// @return True if the value exists, false otherwise.
  auto exists(T value) const -> bool {
    auto it = std::find_if(
        data_.begin(), data_.end(),
        [value](const EnumEntry<T>& entry) { return entry.value == value; });
    return it != data_.end();
  }

  /// @brief Get the number of entries in the mapper.
  /// @return The number of entries.
  inline auto size() const -> size_t { return data_.size(); }

  /// @brief Get all the keys (string representations) in the mapper.
  /// @return A vector of all the keys.
  auto keys() const -> std::vector<std::string> {
    std::vector<std::string> result;
    result.reserve(data_.size());
    for (const auto& entry : data_) {
      result.push_back(entry.name);
    }
    return result;
  }

  /// @brief Get all the values (enum values) in the mapper.
  /// @return A vector of all the values.
  auto values() const -> std::vector<T> {
    std::vector<T> result;
    result.reserve(data_.size());
    for (const auto& entry : data_) {
      result.push_back(entry.value);
    }
    return result;
  }

  /// @brief Get all the items (pairs of enum values and string representations)
  /// in the mapper.
  /// @return A vector of all the items.
  auto items() const -> std::vector<std::pair<T, std::string>> {
    std::vector<std::pair<T, std::string>> result;
    result.reserve(data_.size());
    for (const auto& entry : data_) {
      result.emplace_back(entry.value, entry.name);
    }
    return result;
  }

 private:
  std::vector<EnumEntry<T>> data_{};
};

}  // namespace fes
