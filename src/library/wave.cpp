

// Copyright (c) 2024 CNES
//
// All rights reserved. Use of this source code is governed by a
// BSD-style license that can be found in the LICENSE file.
#include "fes/wave.hpp"

#include <stdexcept>

#include "fes/detail/wave/name.hpp"

namespace fes {

// Get the doodson number
inline auto code(const int number) -> char {
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

auto Wave::doodson_number() const -> std::string {
  auto result = std::string(6, ' ');
  result[0] = code(argument_[0]);
  result[1] = code(argument_[1] + 5);
  result[2] = code(argument_[2] + 5);
  result[3] = code(argument_[3] + 5);
  result[4] = code(argument_[4] + 5);
  result[5] = code(argument_[5] + 5);
  return result;
}

}  // namespace fes
