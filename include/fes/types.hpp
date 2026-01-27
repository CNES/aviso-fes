// Copyright (c) 2025 CNES
//
// All rights reserved. Use of this source code is governed by a
// BSD-style license that can be found in the LICENSE file.
/// @file include/fes/types.hpp
/// @brief Eigen aliases
#pragma once
#include <Eigen/Core>
#include <complex>

namespace fes {

/// @brief Complex number type used for tidal constituents.
using Complex = std::complex<double>;

/// @brief Possible type of tidal wave.
enum TidalType : uint8_t {
  kLongPeriod = 0,  //!< Long period tidal waves
  kShortPeriod      //!< Short period tidal waves
};

/// @brief Check if a matrix is a vector.
///
/// @tparam MatrixType The type of the matrix.
/// @return true if the matrix is a vector, false otherwise.
template <typename MatrixType>
constexpr auto is_vector() noexcept -> bool {
  return MatrixType::ColsAtCompileTime == 1 &&
         (MatrixType::RowsAtCompileTime == Eigen::Dynamic ||
          MatrixType::RowsAtCompileTime > 1);
}

/// @brief Alias for an Eigen vector.
/// @tparam T The type of the vector elements.
template <typename T>
using Vector = Eigen::Matrix<T, Eigen::Dynamic, 1>;

/// @brief Alias for an Eigen matrix.
/// @tparam T The type of the matrix elements.
template <typename T>
using Matrix = Eigen::Matrix<T, Eigen::Dynamic, Eigen::Dynamic>;

/// @brief Alias for a dynamic Eigen stride.
using DynamicStride = Eigen::Stride<Eigen::Dynamic, Eigen::Dynamic>;

/// @brief Alias for a dynamic Eigen reference.
template <typename T>
using DynamicRef = Eigen::Ref<T, 0, DynamicStride>;

/// @brief Alias for a boolean vector of size 7.
using Vector7b = Eigen::Vector<int8_t, 7>;

/// @brief Alias for a double vector of size 6.
using Vector6d = Eigen::Vector<double, 6>;

/// @brief Alias for a double vector of size 7.
using Vector7d = Eigen::Vector<double, 7>;

}  // namespace fes
