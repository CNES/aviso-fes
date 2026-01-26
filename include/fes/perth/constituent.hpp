// Copyright (c) 2025 CNES
//
// All rights reserved. Use of this source code is governed by a
// BSD-style license that can be found in the LICENSE file.
/// @file include/fes/perth/constituent.hpp
/// @brief Perth tidal constituents.
#pragma once

#include <array>
#include <cstddef>
#include <cstdint>
#include <string>

#include "fes/enum_mapper.hpp"

namespace fes {
namespace perth {

/// @brief Enum class representing tidal constituents.
enum Constituent : uint8_t {
  kNode = 0,
  kSa1,
  kSa,
  kSsa,
  kSta,
  kMSm,
  kMm,
  kMSf,
  kMf,
  kMStm,
  kMtm,
  kMSqm,
  kMqm,
  k2Q1,
  kSigma1,
  kQ1,
  kRho1,
  kO1,
  kTau1,
  kBeta1,
  kM13,
  kM1,
  kChi1,
  kPi1,
  kP1,
  kS1,
  kK1,
  kPsi1,
  kPhi1,
  kTheta1,
  kJ1,
  kSO1,
  kOO1,
  kUps1,
  kEps2,
  k2N2,
  k2MS2,
  kMu2,
  kN2,
  kN2P,
  kNu2,
  kGamma2,
  kAlpa2,
  kM2,
  kBeta2,
  kDelta2,
  kMKS2,
  kLambda2,
  k2MN2,
  kL2,
  kL2P,
  kT2,
  kS2,
  kR2,
  kK2,
  kMSN2,
  kEta2,
  k2SM2,
  k2MK3,
  kMO3,
  kM3,
  kMK3,
  kN4,
  kMN4,
  kM4,
  kSN4,
  kMS4,
  kMK4,
  kS4,
  kR4,
  kSK4,
  k2MN6,
  kM6,
  kMSN6,
  k2MS6,
  k2MK6,
  k2SM6,
  kMSK6,
  kS6,
  kM8,
  kNumConstituents,
};

/// @brief Number of items in the Constituent enum.
constexpr std::size_t kNumConstituentItems =
    static_cast<std::size_t>(kNumConstituents);

namespace constituents {

/// @brief Returns all the tidal constituents handled by the library.
/// @return Tidal constituents.
constexpr auto kAll = std::array<Constituent, kNumConstituentItems>{
    kNode,   kSa1,   kSa,      kSsa,  kSta,  kMSm,    kMm,    kMSf,  kMf,
    kMStm,   kMtm,   kMSqm,    kMqm,  k2Q1,  kSigma1, kQ1,    kRho1, kO1,
    kTau1,   kBeta1, kM13,     kM1,   kChi1, kPi1,    kP1,    kS1,   kK1,
    kPsi1,   kPhi1,  kTheta1,  kJ1,   kSO1,  kOO1,    kUps1,  kEps2, k2N2,
    k2MS2,   kMu2,   kN2,      kN2P,  kNu2,  kGamma2, kAlpa2, kM2,   kBeta2,
    kDelta2, kMKS2,  kLambda2, k2MN2, kL2,   kL2P,    kT2,    kS2,   kR2,
    kK2,     kMSN2,  kEta2,    k2SM2, k2MK3, kMO3,    kM3,    kMK3,  kN4,
    kMN4,    kM4,    kSN4,     kMS4,  kMK4,  kS4,     kR4,    kSK4,  k2MN6,
    kM6,     kMSN6,  k2MS6,    k2MK6, k2SM6, kMSK6,   kS6,    kM8,
};

/// @brief Convert a constituent enum to its string name.
/// @param[in] constituent The constituent to convert.
/// @return The string name of the constituent.
auto name(Constituent constituent) -> const char* const;

/// @brief Parse a constituent name string to its enum value.
/// @param[in] name The name of the constituent.
/// @return The corresponding Constituent enum value.
auto parse(const std::string& name) -> Constituent;

/// @brief Return the list of known constituents by this library.
/// @return List of constituent names.
auto known() -> std::array<std::string, kNumConstituentItems>;

/// @brief Create and return an EnumMapper for the Constituent enum.
/// @return EnumMapper for Constituent.
inline auto map() -> EnumMapper<uint8_t> {
  EnumMapper<uint8_t> mapper;
  mapper.reserve(kNumConstituentItems);

  for (auto& id : kAll) {
    mapper.add_entry(static_cast<uint8_t>(id), name(id));
  }
  return mapper;
}

}  // namespace constituents

}  // namespace perth
}  // namespace fes
