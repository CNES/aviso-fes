// Copyright (c) 2026 CNES
//
// All rights reserved. Use of this source code is governed by a
// BSD-style license that can be found in the LICENSE file.
/// @file include/fes/perth/wave_table.hpp
/// @brief Table of tidal constituents handled by PERTH models.
#pragma once

#include <memory>
#include <string>
#include <vector>

#include "fes/interface/wave_table.hpp"

namespace fes {
namespace perth {

/// Properties of tide waves handled by PERTH models.
class WaveTable : public WaveTableInterface {
 public:
  /// @brief Default constructor.
  explicit WaveTable();

  /// @brief Constructor with a list of constituent names.
  /// @param[in] names List of constituent names.
  explicit WaveTable(const std::vector<std::string>& names);

  /// @brief Computes the nodal corrections for all constituents in the table.
  /// @param[in] angles Astronomical angles used to compute nodal corrections.
  /// @param[in] group_modulations If true, applies group modulations to nodal
  /// corrections.
  auto compute_nodal_corrections(const angle::Astronomic& angles,
                                 const bool group_modulations) -> void final;

  /// @brief Clones the wave table.
  /// @return A unique pointer to the cloned wave table.
  auto clone() const -> std::unique_ptr<WaveTableInterface> final {
    return std::make_unique<WaveTable>(*this);
  }
};

}  // namespace perth
}  // namespace fes
