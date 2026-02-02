// Copyright (c) 2026 CNES
//
// All rights reserved. Use of this source code is governed by a
// BSD-style license that can be found in the LICENSE file.
/// @file include/fes/tidal_constituents.hpp
/// @brief Interface for tidal constituents.
#pragma once

#include <cstdint>

#include "fes/types.hpp"

namespace fes {

/// @brief Interface for tidal constituents.
class TidalConstituents {
 public:
  /// Destructor
  virtual ~TidalConstituents() = default;

  /// Set the tide of a constituent
  /// @param[in] ident The constituent identifier
  /// @param[in] value The tide value
  virtual void set_tide(const ConstituentId ident, const Complex& value) = 0;
};

}  // namespace fes
