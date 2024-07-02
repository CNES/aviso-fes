// Copyright (c) 2024 CNES
//
// All rights reserved. Use of this source code is governed by a
// BSD-style license that can be found in the LICENSE file.
/// @file include/fes/eigen.hpp
/// @brief Eigen aliases
#pragma once
#include <Eigen/Core>

namespace fes {

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

}  // namespace fes
