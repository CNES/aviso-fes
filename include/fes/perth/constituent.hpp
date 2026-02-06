// Copyright (c) 2026 CNES
//
// All rights reserved. Use of this source code is governed by a
// BSD-style license that can be found in the LICENSE file.
/// @file include/fes/perth/constituent.hpp
/// @brief Tidal constituents handled by the PERTH models.
#pragma once

#include <array>
#include <string>

#include "fes/constituent.hpp"

namespace fes {
namespace perth {

/// @brief Number of constituents handled by the PERTH models.
constexpr size_t kNumConstituents = 80;

/// @brief Returns all the tidal constituents handled by the library.
/// @return Tidal constituents.
constexpr auto kAllConstituents = std::array<ConstituentId, kNumConstituents>{
    kNode,   kSa1,   kSa,      kSsa,  kSta,  kMSm,    kMm,     kMSf,  kMf,
    kMStm,   kMtm,   kMSqm,    kMqm,  k2Q1,  kSigma1, kQ1,     kRho1, kO1,
    kTau1,   kBeta1, kM13,     kM1,   kChi1, kPi1,    kP1,     kS1,   kK1,
    kPsi1,   kPhi1,  kTheta1,  kJ1,   kSO1,  kOO1,    kUps1,   kEps2, k2N2,
    k2MS2,   kMu2,   kN2,      kN2P,  kNu2,  kGamma2, kAlpha2, kM2,   kBeta2,
    kDelta2, kMKS2,  kLambda2, k2MN2, kL2,   kL2P,    kT2,     kS2,   kR2,
    kK2,     kMSN2,  kEta2,    k2SM2, k2MK3, kMO3,    kM3,     kMK3,  kN4,
    kMN4,    kM4,    kSN4,     kMS4,  kMK4,  kS4,     kR4,     kSK4,  k2MN6,
    kM6,     kMSN6,  k2MS6,    k2MK6, k2SM6, kMSK6,   kS6,     kM8,
};

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
}  // namespace perth
}  // namespace fes
