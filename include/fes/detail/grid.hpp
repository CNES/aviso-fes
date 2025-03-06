// Copyright (c) 2025 CNES
//
// All rights reserved. Use of this source code is governed by a
// BSD-style license that can be found in the LICENSE file.
/// @file include/fes/detail/grid.hpp
/// @brief Numerical grid handling.
#pragma once
#include <Eigen/Core>

#include "fes/eigen.hpp"

namespace fes {
namespace detail {

/// @brief Handle a grid (2D array) stored in a 1D array.
///
/// @tparam T The type of the grid values.
template <typename T>
class Grid {
 public:
  /// @brief Default constructor.
  /// @param[in] data The data of the grid.
  /// @param[in] nx The number of rows in the grid.
  /// @param[in] ny The number of columns in the grid.
  /// @param[in] row_major Whether the data is stored in row-major order.
  constexpr Grid(const T* data, size_t nx, size_t ny,
                 bool row_major = true) noexcept
      : nx_(nx),
        ny_(ny),
        data_(data),
        get_index_(row_major ? &Grid::index_xy : &Grid::index_yx) {}

  /// Get the value at the given coordinates.
  /// @param[in] x The x coordinate.
  /// @param[in] y The y coordinate.
  /// @return The value at the given coordinates.
  constexpr auto operator()(const Eigen::Index x,
                            const Eigen::Index y) const noexcept -> T {
    return data_[(this->*get_index_)(x, y)];
  }

  /// Get the number of rows in the grid.
  /// @return The number of rows in the grid.
  constexpr auto nx() const noexcept -> size_t { return nx_; }

  /// Get the number of columns in the grid.
  /// @return The number of columns in the grid.
  constexpr auto ny() const noexcept -> size_t { return ny_; }

  /// Get the number of elements in the grid.
  /// @return The number of elements in the grid.
  constexpr auto size() const noexcept -> size_t { return nx_ * ny_; }

  /// Get the data.
  /// @return The data.
  constexpr auto data() const noexcept -> const T* { return data_; }

  /// Set the data.
  /// @param[in] data The data.
  constexpr auto data(const T* data) noexcept -> void { data_ = data; }

 private:
  /// Typename to a function pointer for accessing an element of the grid.
  using GetIndex = Eigen::Index (Grid::*)(const Eigen::Index,
                                          const Eigen::Index) const;
  /// The number of rows in the grid.
  size_t nx_;
  /// The number of columns in the grid.
  size_t ny_;
  /// The data stored in the grid.
  const T* data_;
  /// Pointer to the function for accessing an element of the grid.
  GetIndex get_index_;

  /// The index of the element at the given row and column if the grid is
  /// stored in row-major order.
  constexpr auto index_xy(const Eigen::Index x, const Eigen::Index y) const
      -> Eigen::Index {
    return x * ny_ + y;
  }

  /// The index of the element at the given row and column if the grid is
  /// stored in column-major order.
  constexpr auto index_yx(const Eigen::Index x, const Eigen::Index y) const
      -> Eigen::Index {
    return y * nx_ + x;
  }
};

}  // namespace detail
}  // namespace fes
