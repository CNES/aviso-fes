// Copyright (c) 2024 CNES
//
// All rights reserved. Use of this source code is governed by a
// BSD-style license that can be found in the LICENSE file.
/// @file include/fes/detail/serialize.hpp
/// @brief Serialization utilities.
#pragma once
#include <Eigen/Core>
#include <map>
#include <sstream>

#include "fes/detail/isviewstream.hpp"

namespace fes {
namespace detail {
namespace serialize {

/// @brief Write data to a stringstream
/// @tparam T The type of the data to write
/// @param[in] ss The stringstream to write to
/// @param[in] data The data to write
template <typename T>
auto write_data(std::stringstream& ss, const T& data) -> void {
  ss.write(reinterpret_cast<const char*>(&data), sizeof(data));
}

/// @brief Read data from a stringstream
/// @tparam T The type of the data to read
/// @param[in] ss The stringstream to read from
/// @return The data read
template <typename T>
auto read_data(detail::isviewstream& ss) -> T {
  auto data = T{};
  ss.read(reinterpret_cast<char*>(&data), sizeof(data));
  return data;
}

/// @brief Write a string to a stringstream
/// @param[in] ss The stringstream to write to
/// @param[in] data The string to write
inline auto write_string(std::stringstream& ss, const std::string& data)
    -> void {
  auto size = data.size();
  write_data(ss, size);
  ss.write(data.data(), data.size());
}

/// @brief Read a string from a stringstream
/// @param[in] ss The stringstream to read from
/// @return The string read
inline auto read_string(detail::isviewstream& ss) -> fes::string_view {
  return ss.readview(read_data<size_t>(ss));
}

/// @brief Write an Eigen matrix to a stringstream
/// @tparam T The type of the matrix
/// @tparam ROWS The number of rows in the matrix
/// @tparam COLS The number of columns in the matrix
/// @param[in] ss The stringstream to write to
/// @param[in] data The matrix to write
template <typename T, int ROWS, int COLS>
auto write_matrix(std::stringstream& ss,
                  const Eigen::Matrix<T, ROWS, COLS>& data) -> void {
  write_data(ss, data.rows());
  write_data(ss, data.cols());
  ss.write(reinterpret_cast<const char*>(data.data()), data.size() * sizeof(T));
}

/// @brief Read an Eigen matrix from a stringstream
/// @tparam T The type of the matrix
/// @tparam ROWS The number of rows in the matrix
/// @tparam COLS The number of columns in the matrix
/// @param[in] ss The stringstream to read from
/// @return The matrix read
template <typename T, int ROWS, int COLS>
auto read_matrix(detail::isviewstream& ss) -> Eigen::Matrix<T, ROWS, COLS> {
  auto rows = read_data<Eigen::Index>(ss);
  auto cols = read_data<Eigen::Index>(ss);
  auto data = Eigen::Matrix<T, ROWS, COLS>(rows, cols);
  ss.read(reinterpret_cast<char*>(data.data()), data.size() * sizeof(T));
  return data;
}

/// @brief Write the map of constituents to a stringstream
/// @tparam T The type of the constituents
/// @tparam U The type of the data
/// @param[in] ss The stringstream to write to
/// @param[in] data The map of constituents to write
template <typename T, typename U>
auto write_constituent_map(
    std::stringstream& ss,
    const std::map<T, Eigen::Matrix<U, Eigen::Dynamic, 1>>& data) -> void {
  write_data(ss, data.size());
  for (const auto& item : data) {
    write_data(ss, item.first);
    write_matrix<U, Eigen::Dynamic, 1>(ss, item.second);
  }
}

/// @brief Read the map of constituents from a stringstream
/// @tparam T The type of the constituents
/// @tparam U The type of the data
/// @param[in] ss The stringstream to read from
/// @return The map of constituents read
template <typename T, typename U>
auto read_constituent_map(detail::isviewstream& ss)
    -> std::map<T, Eigen::Matrix<U, Eigen::Dynamic, 1>> {
  auto size = read_data<Eigen::Index>(ss);
  auto data = std::map<T, Eigen::Matrix<U, Eigen::Dynamic, 1>>{};
  for (auto ix = 0; ix < size; ++ix) {
    auto constituent = read_data<T>(ss);
    auto map = read_matrix<U, Eigen::Dynamic, 1>(ss);
    data.emplace(constituent, map);
  }
  return data;
}

}  // namespace serialize
}  // namespace detail
}  // namespace fes
