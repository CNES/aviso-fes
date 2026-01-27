// Copyright (c) 2025 CNES
//
// All rights reserved. Use of this source code is governed by a
// BSD-style license that can be found in the LICENSE file.
/// @file include/fes/xdo.hpp
/// @brief XDO tidal code representations.
#pragma once

#include "fes/types.hpp"

namespace fes {

/// Get the code character for a given doodson number
/// @param[in] number Doodson number
/// @return Code character
constexpr auto code(const int8_t number) -> char {
  switch (number) {
    case -1:
      return '*';
    case 10:
      return 'X';
    case 11:
      return 'E';
    case 12:
      return 'T';
    default:
      return static_cast<char>(number + 48);
  }
}

/// Get the XDO numerical representation of the wave
/// @param[in] doodson Doodson numbers
/// @return XDO numerical representation
inline auto xdo_numerical(const Eigen::Ref<const Vector7b>& doodson)
    -> std::string {
  std::string result;
  result.reserve(7);

  // First element uses raw value, others add offset of 5
  result.push_back(code(doodson[0]));
  for (int64_t ix = 1; ix < doodson.size(); ++ix) {
    result.push_back(code(doodson[ix] + 5));
  }
  return result;
}

/// Get the XDO alphabetical representation of the wave
/// @param[in] doodson Doodson numbers
/// @return XDO alphabetical representation
inline auto xdo_alphabetical(const Eigen::Ref<const Vector7b>& doodson)
    -> std::string {
  constexpr auto xdo = std::array<char, 25>{
      'R', 'S', 'T', 'U', 'V', 'W', 'X', 'Y', 'Z', 'A', 'B', 'C', 'D',
      'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L', 'M', 'N', 'O', 'P'};
  auto result = std::string();
  result.reserve(7);
  for (const auto& value : doodson) {
    size_t ix = value + 8;
    if (ix >= xdo.size()) {
      throw std::out_of_range("Doodson number out of range for XDO code");
    }
    result.push_back(xdo[ix]);
  }
  return result;
}

}  // namespace fes
