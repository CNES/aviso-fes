// Copyright (c) 2024 CNES
//
// All rights reserved. Use of this source code is governed by a
// BSD-style license that can be found in the LICENSE file.
/// @file include/fes/detail/broadcast.hpp
/// @brief Broadcasting utilities.
#pragma once
#include <sstream>
#include <stdexcept>
#include <string>

#include "fes/eigen.hpp"

namespace fes {
namespace detail {

/// @brief Get a string representing the shape of a Eigen matrix.
///
/// @param[in] array tensor to process
template <typename Array>
auto eigen_shape(const Array& array) -> std::string {
  std::stringstream ss;
  if (is_vector<Array>()) {
    ss << "(" << array.size() << ")";
  } else {
    ss << "(" << array.rows() << ", " << array.cols() << ")";
  }
  return ss.str();
}

/// @brief Automation of vector size control to ensure that all vectors have the
/// same size.
///
/// @tparam Matrix1 type of the first matrix
/// @tparam Matrix2 type of the second matrix
/// @param[in] name1 name of the variable containing the first vector
/// @param[in] m1 first matrix
/// @param[in] name2 name of the variable containing the second vector
/// @param[in] m2 second matrix
/// @throw std::invalid_argument if the size of the two vectors is different
template <typename Matrix1, typename Matrix2>
void check_eigen_shape(const std::string& name1, const Matrix1& m1,
                       const std::string& name2, const Matrix2& m2) {
  if (m1.cols() != m2.cols() || m1.rows() != m2.rows()) {
    throw std::invalid_argument(name1 + ", " + name2 +
                                " could not be broadcast together with shape " +
                                eigen_shape(m1) + ", " + eigen_shape(m2));
  }
}

/// Vector size check function pattern.
///
/// @tparam Matrix1 type of the first matrix
/// @tparam Matrix2 type of the second matrix
/// @tparam Args list of vectors to be verified
/// @param[in] name1 name of the variable containing the first vector
/// @param[in] v1 first vector
/// @param[in] name2 name of the variable containing the second vector
/// @param[in] v2 second vector
/// @param[in] args other vectors to be verified
/// @throw std::invalid_argument if the size of the two vectors is different
template <typename Matrix1, typename Matrix2, typename... Args>
void check_eigen_shape(const std::string& name1, const Matrix1& v1,
                       const std::string& name2, const Matrix2& v2,
                       Args... args) {
  static_assert(sizeof...(Args) % 2 == 0,
                "an even number of parameters is expected");
  check_eigen_shape(name1, v1, name2, v2);
  check_eigen_shape(name1, v1, args...);
}

}  // namespace detail
}  // namespace fes
