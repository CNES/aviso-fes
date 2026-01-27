// Copyright (c) 2026 CNES
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

namespace fes {

/// @brief Class to map enum values to strings and vice versa.
/// @tparam T The value type to map (must be an integral type).
template <typename T>
class EnumMapper {
  static_assert(std::is_integral<T>::value,
                "EnumMapper requires an integral type");

 public:
  /// @brief Default constructor.
  EnumMapper() = default;

  /// @brief Construct and finalize from a vector of pairs.
  /// @param entries Vector of (value, name) pairs.
  explicit EnumMapper(std::vector<std::pair<T, std::string>> entries) {
    by_value_ = std::move(entries);
    finalize();
  }

  /// @brief Reserve space for n entries.
  /// @param n The number of entries to reserve space for.
  auto reserve(std::size_t n) -> void {
    by_value_.reserve(n);
    by_name_.reserve(n);
  }

  /// @brief Add an entry to the mapper (must call finalize() after all adds).
  /// @param[in] value The enum value.
  /// @param[in] name The string representation of the enum value.
  auto add_entry(T value, std::string name) -> void {
    by_value_.emplace_back(value, std::move(name));
    finalized_ = false;
  }

  /// @brief Sort internal structures for O(log n) lookups.
  /// Must be called after all add_entry() calls and before any lookups.
  auto finalize() -> void {
    // Sort by value
    std::sort(
        by_value_.begin(), by_value_.end(),
        [](const std::pair<T, std::string>& a,
           const std::pair<T, std::string>& b) { return a.first < b.first; });

    // Build name index (lowercase name -> index in by_value_)
    by_name_.clear();
    by_name_.reserve(by_value_.size());
    for (std::size_t i = 0; i < by_value_.size(); ++i) {
      by_name_.emplace_back(to_lower(by_value_[i].second), i);
    }
    std::sort(by_name_.begin(), by_name_.end(),
              [](const std::pair<std::string, std::size_t>& a,
                 const std::pair<std::string, std::size_t>& b) {
                return a.first < b.first;
              });

    finalized_ = true;
  }

  /// @brief Convert a string to an enum value.
  /// @param[in] str The string to convert.
  /// @return The corresponding enum value.
  /// @throw std::invalid_argument if key not found.
  auto from_string(const std::string& str) const -> T {
    auto result = T{};
    if (try_from_string(str, result)) {
      return result;
    }
    throw std::invalid_argument("key not found: " + str);
  }

  /// @brief Convert an enum value to a string.
  /// @param[in] value The enum value to convert.
  /// @return The corresponding string.
  /// @throw std::invalid_argument if value not found.
  auto to_string(T value) const -> const std::string& {
    auto it = std::lower_bound(by_value_.begin(), by_value_.end(), value,
                               [](const std::pair<T, std::string>& entry, T v) {
                                 return entry.first < v;
                               });

    if (it != by_value_.end() && it->first == value) {
      return it->second;
    }
    throw std::invalid_argument("value not found: " + std::to_string(value));
  }

  /// @brief Try to convert a string to an enum value without throwing.
  /// @param[in] str The string to convert.
  /// @param[out] result The result if found.
  /// @return True if found, false otherwise.
  auto try_from_string(const std::string& str, T& result) const noexcept
      -> bool {
    auto key = to_lower(str);
    auto it =
        std::lower_bound(by_name_.begin(), by_name_.end(), key,
                         [](const std::pair<std::string, std::size_t>& entry,
                            const std::string& k) { return entry.first < k; });

    if (it != by_name_.end() && it->first == key) {
      result = by_value_[it->second].first;
      return true;
    }
    return false;
  }

  /// @brief Try to convert an enum value to a string without throwing.
  /// @param[in] value The enum value to convert.
  /// @param[out] result Pointer to the result if found.
  /// @return True if found, false otherwise.
  auto try_to_string(T value, const std::string*& result) const noexcept
      -> bool {
    auto it = std::lower_bound(by_value_.begin(), by_value_.end(), value,
                               [](const std::pair<T, std::string>& entry, T v) {
                                 return entry.first < v;
                               });

    if (it != by_value_.end() && it->first == value) {
      result = &it->second;
      return true;
    }
    return false;
  }

  /// @brief Check if an enum value exists.
  /// @param[in] value The enum value to check.
  /// @return True if the value exists, false otherwise.
  auto exists(T value) const noexcept -> bool {
    auto it = std::lower_bound(by_value_.begin(), by_value_.end(), value,
                               [](const std::pair<T, std::string>& entry, T v) {
                                 return entry.first < v;
                               });
    return it != by_value_.end() && it->first == value;
  }

  /// @brief Check if a string key exists (case-insensitive).
  /// @param[in] str The string to check.
  /// @return True if the key exists, false otherwise.
  auto exists(const std::string& str) const -> bool {
    auto key = to_lower(str);
    auto it =
        std::lower_bound(by_name_.begin(), by_name_.end(), key,
                         [](const std::pair<std::string, std::size_t>& entry,
                            const std::string& k) { return entry.first < k; });
    return it != by_name_.end() && it->first == key;
  }

  /// @brief Get the number of entries in the mapper.
  /// @return The number of entries.
  auto size() const noexcept -> std::size_t { return by_value_.size(); }

  /// @brief Check if the mapper is empty.
  /// @return True if empty, false otherwise.
  auto empty() const noexcept -> bool { return by_value_.empty(); }

  /// @brief Check if the mapper has been finalized.
  /// @return True if finalized, false otherwise.
  auto is_finalized() const noexcept -> bool { return finalized_; }

  /// @brief Get all the keys (string representations) in the mapper.
  /// @return A vector of all the keys (sorted by value).
  auto keys() const -> std::vector<std::string> {
    std::vector<std::string> result;
    result.reserve(by_value_.size());
    for (const auto& entry : by_value_) {
      result.push_back(entry.second);
    }
    return result;
  }

  /// @brief Get all the values (enum values) in the mapper.
  /// @return A vector of all the values (sorted).
  auto values() const -> std::vector<T> {
    std::vector<T> result;
    result.reserve(by_value_.size());
    for (const auto& entry : by_value_) {
      result.push_back(entry.first);
    }
    return result;
  }

  /// @brief Get all the items in the mapper.
  /// @return A vector of all the items (sorted by value).
  auto items() const -> const std::vector<std::pair<T, std::string>>& {
    return by_value_;
  }

  /// @brief Returns a read-only (constant) iterator that points to the first
  /// element in the mapper.
  /// @return An iterator to the first element.
  auto begin() const noexcept ->
      typename std::vector<std::pair<T, std::string>>::const_iterator {
    return by_value_.begin();
  }

  /// @brief Returns a read-only (constant) iterator that points to one past the
  /// last element in the mapper.
  /// @return An iterator to one past the last element.
  auto end() const noexcept ->
      typename std::vector<std::pair<T, std::string>>::const_iterator {
    return by_value_.end();
  }

 private:
  /// @brief Convert string to lowercase.
  static auto to_lower(const std::string& s) -> std::string {
    std::string result;
    result.reserve(s.size());
    for (unsigned char c : s) {
      result.push_back(static_cast<char>(std::tolower(c)));
    }
    return result;
  }

  /// Entries sorted by value: (value, name)
  std::vector<std::pair<T, std::string>> by_value_;
  /// Index sorted by lowercase name: (lowercase_name, index_into_by_value)
  std::vector<std::pair<std::string, std::size_t>> by_name_;
  /// Whether finalize() has been called
  bool finalized_ = false;
};

}  // namespace fes
