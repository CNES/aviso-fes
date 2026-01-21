#pragma once

#include <algorithm>
#include <string>

namespace fes {
namespace detail {

/// @brief Case-insensitive string comparison.
/// @param[in] a First string to compare.
/// @param[in] b Second string to compare.
/// @return True if the strings are equal ignoring case, false otherwise.
inline auto iequals(const std::string& a, const std::string& b) -> bool {
  return std::equal(a.begin(), a.end(), b.begin(), b.end(),
                    [](char a, char b) { return tolower(a) == tolower(b); });
}

}  // namespace detail
}  // namespace fes
