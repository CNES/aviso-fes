// Copyright (c) 2026 CNES
//
// All rights reserved. Use of this source code is governed by a
// BSD-style license that can be found in the LICENSE file.
/// @file include/fes/darwin/constituent.hpp
/// @brief Tidal constituents handled by the FES models.
#pragma once

#include <array>
#include <string>

#include "fes/constituent.hpp"

namespace fes {
namespace darwin {

/// @brief Number of constituents handled by the FES models.
constexpr size_t kNumConstituents = 99;

/// @brief Returns all the tidal constituents handled by the library.
/// @return Tidal constituents.
constexpr auto kAllConstituents = std::array<ConstituentId, kNumConstituents>{
    kMm,   kMf,   kMtm,   kMSqm,   k2Q1,   kSigma1, kQ1,      kRho1,  kO1,
    kMP1,  kM1,   kM11,   kM12,    kM13,   kChi1,   kPi1,     kP1,    kS1,
    kK1,   kPsi1, kPhi1,  kTheta1, kJ1,    kOO1,    kMNS2,    kEps2,  k2N2,
    kMu2,  k2MS2, kN2,    kNu2,    kM2,    kMKS2,   kLambda2, kL2,    k2MN2,
    kT2,   kS2,   kR2,    kK2,     kMSN2,  kEta2,   k2SM2,    kMO3,   k2MK3,
    kM3,   kMK3,  kN4,    kMN4,    kM4,    kSN4,    kMS4,     kMK4,   kS4,
    kSK4,  kR4,   k2MN6,  kM6,     kMSN6,  k2MS6,   k2MK6,    k2SM6,  kMSK6,
    kS6,   kM8,   kMSf,   kSsa,    kSa,    kSa1,    kSta,     kMm1,   kMf1,
    kA5,   kM0,   kMm2,   kMf2,    kL2P,   kN2P,    kMSK2,    kSKM2,  kOQ2,
    k3MS4, kMNu4, k2MSN4, k2NS2,   kMNuS2, k2MK2,   kNKM2,    kML4,   kSO1,
    kSO3,  kNK4,  kMNK6,  k2NM6,   k3MS8,  kSK3,    k2MNS4,   k2SMu2, k2MP5};

namespace constituents {

/// @brief Parses the tidal constituent from a string. Parsing is case not
/// sensitive. So "Mm" and "mm" are equivalent.
/// @param[in] name Tide constituent name.
/// @return Tidal constituent.
inline auto parse(const std::string& name) -> ConstituentId {
  return fes::constituents::parse(name, kAllConstituents);
}

/// @brief Get the tidal constituent name from its identifier.
/// @param[in] ident Tide constituent identifier.
/// @return Tidal constituent name.
inline auto name(const ConstituentId ident) -> std::string {
  return fes::constituents::name(ident, kAllConstituents);
}

/// @brief Get the tidal constituent names handled by the library.
/// @return Tidal constituent names.
inline auto known() -> std::array<std::string, kNumConstituents> {
  return fes::constituents::known(kAllConstituents);
}

}  // namespace constituents
}  // namespace darwin
}  // namespace fes
