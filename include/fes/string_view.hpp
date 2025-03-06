// Copyright (c) 2025 CNES
//
// All rights reserved. Use of this source code is governed by a
// BSD-style license that can be found in the LICENSE file.
/// @file include/fes/string_view.hpp
/// @brief A non-owning view of a string.
#pragma once

#include <cstddef>
#include <cstring>
#include <stdexcept>

namespace fes {

/// @brief A non-owning view of a string.
class string_view {
 public:
  /// @brief The type used to represent the size of the string.
  using size_type = std::size_t;
  /// @brief The type used to represent a pointer to a character.
  using const_iterator = const char*;

  /// @brief Default constructor.
  constexpr string_view() noexcept : data_(nullptr), size_(0) {}

  /// @brief Construct a string view from a null-terminated string.
  /// @param[in] str The null-terminated string.
  constexpr string_view(const char* str) noexcept
      : data_(str), size_(string_view::compute_size(str)) {}

  /// @brief Construct a string view from a string and its size.
  /// @param[in] str The null-terminated string.
  /// @param[in] size The size of the string.
  constexpr string_view(const char* str, const size_type size) noexcept
      : data_(str), size_(size) {}

  /// @brief Copy constructor.
  /// @param[in] other The other string view.
  constexpr string_view(const string_view& other) noexcept = default;

  /// @brief Copy assignment operator.
  /// @param[in] other The other string view.
  /// @return A reference to this string view.
  constexpr string_view& operator=(const string_view& other) noexcept = default;

  /// @brief Move constructor.
  /// @return A reference to this string view.
  constexpr const char* data() const noexcept { return data_; }

  /// @brief Return the size of the string.
  /// @return The size of the string.
  constexpr size_type size() const noexcept { return size_; }

  /// @brief Return true if the string is empty.
  /// @return True if the string is empty.
  constexpr bool empty() const noexcept { return size_ == 0; }

  /// @brief Return an iterator to the beginning of the string.
  /// @return An iterator to the beginning of the string.
  constexpr const_iterator begin() const noexcept { return data_; }

  /// @brief Return an iterator to the end of the string.
  /// @return An iterator to the end of the string.
  constexpr const_iterator end() const noexcept { return data_ + size_; }

  /// @brief Return a reference to the character at the given index.
  /// @param[in] index The index of the character.
  /// @return A reference to the character at the given index.
  constexpr const char& operator[](const size_type index) const {
    if (index >= size_) {
      throw std::out_of_range("string_view index out of range");
    }
    return data_[index];
  }

 private:
  /// @brief A pointer to the string.
  const char* data_;
  /// @brief The size of the string.
  size_type size_;

  /// @brief Compute the size of a null-terminated string.
  /// @param[in] str The null-terminated string.
  /// @return The size of the string.
  static constexpr size_type compute_size(const char* str) noexcept {
    const char* end = str;
    while (*end != '\0') {
      ++end;
    }
    return end - str;
  }
};

}  // namespace fes
