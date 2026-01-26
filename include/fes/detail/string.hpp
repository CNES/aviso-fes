// Copyright (c) 2025 CNES
//
// All rights reserved. Use of this source code is governed by a
// BSD-style license that can be found in the LICENSE file.
/// @file include/fes/detail/string.hpp
/// @brief String utility functions.
#pragma once

#include <algorithm>
#include <cctype>
#include <string>

namespace fes {
namespace detail {

/// @brief Case-insensitive string comparison.
/// @param[in] a First string to compare.
/// @param[in] b Second string to compare.
/// @return True if the strings are equal ignoring case, false otherwise.
inline auto iequals(const std::string &a, const std::string &b) -> bool {
  return std::equal(a.begin(), a.end(), b.begin(), b.end(),
                    [](char a, char b) { return tolower(a) == tolower(b); });
}

}  // namespace detail
}  // namespace fes
