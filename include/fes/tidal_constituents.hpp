// Copyright (c) 2025 CNES
//
// All rights reserved. Use of this source code is governed by a
// BSD-style license that can be found in the LICENSE file.
/// @file include/fes/tidal_constituents.hpp
/// @brief Interface for tidal constituents.
#pragma once

#include <complex>

namespace fes {

/// @brief Interface for tidal constituents.
/// @tparam ConstituentId The type of the constituent identifier.
template <typename ConstituentId>
class TidalConstituents {
 public:
  /// Destructor
  virtual ~TidalConstituents() = default;

  /// Set the tide of a constituent
  /// @param[in] ident The constituent identifier
  /// @param[in] value The tide value
  virtual void set_tide(ConstituentId ident,
                        const std::complex<double>& value) = 0;
};

}  // namespace fes
