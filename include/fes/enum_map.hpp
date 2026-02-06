// Copyright (c) 2026 CNES
//
// All rights reserved. Use of this source code is governed by a
// BSD-style license that can be found in the LICENSE file.
/// @file include/fes/enum_map.hpp
/// @brief Sparse mapping between enumeration values and associated values
#pragma once

#include <array>
#include <bitset>
#include <cstddef>
#include <stdexcept>
#include <utility>

namespace fes {

/// @brief Sparse mapping between enumeration values and associated values.
/// @tparam EnumT Enumeration type
/// @tparam ValueT Type of the associated values
/// @tparam N Number of enumeration values
template <typename EnumT, typename ValueT, size_t Size>
class EnumMap {
  /// Bitset indicating which enumeration values are present in the map
  std::bitset<Size> present_;
  /// Array of associated values
  std::array<ValueT, Size> data_;

  /// Finds the next present enumeration value starting from a given position
  auto find_next(size_t pos) const noexcept -> size_t {
    while (pos < Size && !present_.test(pos)) {
      ++pos;
    }
    return pos;
  }

 public:
  /// @brief Iterator over the sparse enumeration map
  class iterator {
    /// Pointer to the sparse enumeration map
    EnumMap* map_;
    /// Current position in the enumeration
    size_t pos_;

   public:
    /// Iterator category definitions
    using iterator_category = std::forward_iterator_tag;
    /// Type of value returned by the iterator
    /// Difference type
    using difference_type = std::ptrdiff_t;
    /// Value type
    using value_type = std::pair<EnumT, ValueT>;
    /// Pointer type
    using pointer = value_type*;
    /// Reference type
    using reference = value_type&;

    /// Constructor
    /// @param[in] map Pointer to the sparse enumeration map
    /// @param[in] pos Current position in the enumeration
    iterator(EnumMap* map, size_t pos) noexcept : map_(map), pos_(pos) {}

    /// Dereference operator
    /// @return Pair of enumeration value and associated value reference
    auto operator*() noexcept -> iterator& { return *this; }

    /// Dereference operator (const)
    /// @return Pair of enumeration value and associated value reference
    auto operator*() const noexcept -> const iterator& { return *this; }

    /// Get the enumeration key at the current position
    auto key() const noexcept -> EnumT { return static_cast<EnumT>(pos_); }

    /// Get the associated value at the current position
    auto value() noexcept -> ValueT& { return map_->data_[pos_]; }

    /// Get the associated value at the current position (const)
    auto value() const noexcept -> const ValueT& { return map_->data_[pos_]; }

    /// Pre-increment operator
    /// @return Reference to the incremented iterator
    auto operator++() noexcept -> iterator& {
      pos_ = map_->find_next(pos_ + 1);
      return *this;
    }

    /// Post-increment operator
    /// @return Iterator before increment
    auto operator++(int) noexcept -> iterator {
      auto tmp = *this;
      ++(*this);
      return tmp;
    }

    /// Equality operator
    /// @param[in] other Iterator to compare
    /// @return true if the iterators are equal
    auto operator==(const iterator& other) const noexcept -> bool {
      return pos_ == other.pos_;
    }

    /// Inequality operator
    /// @param[in] other Iterator to compare
    /// @return true if the iterators are different
    auto operator!=(const iterator& other) const noexcept -> bool {
      return pos_ != other.pos_;
    }
  };

  /// @brief Constant iterator over the sparse enumeration map
  class const_iterator {
    /// Pointer to the sparse enumeration map
    const EnumMap* map_;
    /// Current position in the enumeration
    size_t pos_;

   public:
    /// Iterator category definitions
    using iterator_category = std::forward_iterator_tag;
    /// Difference type
    using difference_type = std::ptrdiff_t;
    /// Value type
    using value_type = std::pair<EnumT, ValueT>;
    /// Pointer type
    using pointer = const value_type*;
    /// Reference type
    using reference = const value_type&;

    /// Constructor
    /// @param[in] map Pointer to the sparse enumeration map
    /// @param[in] pos Current position in the enumeration
    const_iterator(const EnumMap* map, size_t pos) noexcept
        : map_(map), pos_(pos) {}

    /// Copy constructor from non-const iterator
    /// @param[in] it Non-const iterator to copy from
    explicit const_iterator(const iterator& it) noexcept
        : map_(it.map_), pos_(it.pos_) {}

    /// Dereference operator
    /// @return Pair of enumeration value and associated value reference
    auto operator*() noexcept -> const const_iterator& { return *this; }

    /// Dereference operator (const)
    /// @return Pair of enumeration value and associated value reference
    auto operator*() const noexcept -> const const_iterator& { return *this; }

    /// Get the enumeration key at the current position
    auto key() const noexcept -> EnumT { return static_cast<EnumT>(pos_); }

    /// Get the associated value at the current position
    auto value() const noexcept -> const ValueT& { return map_->data_[pos_]; }

    /// Pre-increment operator
    /// @return Reference to the incremented iterator
    auto operator++() noexcept -> const_iterator& {
      pos_ = map_->find_next(pos_ + 1);
      return *this;
    }

    /// Post-increment operator
    /// @return Iterator before increment
    auto operator++(int) noexcept -> const_iterator {
      auto tmp = *this;
      ++(*this);
      return tmp;
    }

    /// Equality operator
    /// @param[in] other Iterator to compare
    /// @return true if the iterators are equal
    auto operator==(const const_iterator& other) const noexcept -> bool {
      return pos_ == other.pos_;
    }

    /// Inequality operator
    /// @param[in] other Iterator to compare
    /// @return true if the iterators are different
    auto operator!=(const const_iterator& other) const noexcept -> bool {
      return pos_ != other.pos_;
    }
  };

  /// Returns an iterator to the beginning of the map
  auto begin() noexcept -> iterator { return {this, find_next(0)}; }

  /// Returns an iterator to the end of the map
  auto end() noexcept -> iterator { return {this, Size}; }

  /// Returns a constant iterator to the beginning of the map
  auto begin() const noexcept -> const_iterator { return {this, find_next(0)}; }

  /// Returns a constant iterator to the end of the map
  auto end() const noexcept -> const_iterator { return {this, Size}; }

  /// Returns a constant iterator to the beginning of the map
  auto cbegin() const noexcept -> const_iterator { return begin(); }

  /// Returns a constant iterator to the end of the map
  auto cend() const noexcept -> const_iterator { return end(); }

  /// Sets the value associated with an enumeration value
  /// @param[in] key Enumeration value
  /// @param[in] value Associated value
  auto set(EnumT key, ValueT value) -> void {
    auto idx = static_cast<size_t>(key);
    present_.set(idx);
    data_[idx] = std::move(value);
  }

  /// Gets the value associated with an enumeration value
  /// @param[in] key Enumeration value
  /// @return Pointer to the associated value, or nullptr if not present
  auto get(EnumT key) const noexcept -> const ValueT* {
    auto idx = static_cast<size_t>(key);
    return present_.test(idx) ? &data_[idx] : nullptr;
  }

  /// Gets the value associated with an enumeration value
  /// @param[in] key Enumeration value
  /// @return Pointer to the associated value, or nullptr if not present
  auto get(EnumT key) noexcept -> ValueT* {
    auto idx = static_cast<size_t>(key);
    return present_.test(idx) ? &data_[idx] : nullptr;
  }

  /// Checks if an enumeration value is present in the map
  /// @param[in] key Enumeration value
  auto contains(EnumT key) const noexcept -> bool {
    return present_.test(static_cast<size_t>(key));
  }

  /// Erases the value associated with an enumeration value
  /// @param[in] key Enumeration value
  auto erase(EnumT key) noexcept(
      std::is_nothrow_default_constructible<ValueT>::value) -> bool {
    auto idx = static_cast<size_t>(key);
    bool was_present = present_.test(idx);
    present_.reset(idx);
    return was_present;
  }

  /// Clears the map
  auto clear() noexcept(std::is_nothrow_default_constructible<ValueT>::value)
      -> void {
    for (size_t i = 0; i < Size; ++i) {
      if (present_.test(i)) {
        data_[i] = ValueT{};
      }
    }
    present_.reset();
  }

  /// Access operator by index in the sparse map
  /// @param[in] n Index in the sparse map
  /// @return Reference to the associated value
  auto operator[](size_t n) -> ValueT& {
    size_t count = 0;
    for (size_t i = 0; i < Size; ++i) {
      if (present_.test(i)) {
        if (count == n) {
          return data_[i];
        }
        ++count;
      }
    }
    throw std::out_of_range("index out of range");
  }

  /// Access operator by index in the sparse map
  /// @param[in] n Index in the sparse map
  /// @return Reference to the associated value
  auto operator[](size_t n) const -> const ValueT& {
    size_t count = 0;
    for (size_t i = 0; i < Size; ++i) {
      if (present_.test(i)) {
        if (count == n) {
          return data_[i];
        }
        ++count;
      }
    }
    throw std::out_of_range("index out of range");
  }

  /// Get the number of present enumeration values
  auto size() const noexcept -> size_t { return present_.count(); }
};

}  // namespace fes
