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
  k2MK3,
  k2MK6,
  k2MN2,
  k2MN6,
  k2MS2,
  k2MS6,
  k2N2,
  k2Q1,
  k2SM2,
  k2SM6,
  kAlpa2,
  kBeta1,
  kBeta2,
  kChi1,
  kDelta2,
  kEps2,
  kEta2,
  kGamma2,
  kJ1,
  kK1,
  kK2,
  kL2,
  kL2P,
  kLambda2,
  kM1,
  kM13,
  kM2,
  kM3,
  kM4,
  kM6,
  kM8,
  kMf,
  kMK3,
  kMK4,
  kMKS2,
  kMm,
  kMN4,
  kMO3,
  kMqm,
  kMS4,
  kMSf,
  kMSK6,
  kMSm,
  kMSN2,
  kMSN6,
  kMSqm,
  kMStm,
  kMtm,
  kMu2,
  kN2,
  kN2P,
  kN4,
  kNode,
  kNu2,
  kO1,
  kOO1,
  kP1,
  kPhi1,
  kPi1,
  kPsi1,
  kQ1,
  kR2,
  kR4,
  kRho1,
  kS1,
  kS2,
  kS4,
  kS6,
  kSa,
  kSa1,
  kSigma1,
  kSK4,
  kSN4,
  kSO1,
  kSsa,
  kSta,
  kT2,
  kTau1,
  kTheta1,
  kUps1,
  kNumConstituents,
};

/// @brief Number of items in the Constituent enum.
constexpr std::size_t kNumConstituentItems =
    static_cast<std::size_t>(kNumConstituents);

namespace constituents {

/// @brief Returns all the tidal constituents handled by the library.
/// @return Tidal constituents.
constexpr auto kAll = std::array<Constituent, kNumConstituentItems>{
    k2MK3, k2MK6,  k2MN2,  k2MN6,  k2MS2, k2MS6,    k2N2,    k2Q1,    k2SM2,
    k2SM6, kAlpa2, kBeta1, kBeta2, kChi1, kDelta2,  kEps2,   kEta2,   kGamma2,
    kJ1,   kK1,    kK2,    kL2,    kL2P,  kLambda2, kM1,     kM13,    kM2,
    kM3,   kM4,    kM6,    kM8,    kMf,   kMK3,     kMK4,    kMKS2,   kMm,
    kMN4,  kMO3,   kMqm,   kMS4,   kMSf,  kMSK6,    kMSm,    kMSN2,   kMSN6,
    kMSqm, kMStm,  kMtm,   kMu2,   kN2,   kN2P,     kN4,     kNode,   kNu2,
    kO1,   kOO1,   kP1,    kPhi1,  kPi1,  kPsi1,    kQ1,     kR2,     kR4,
    kRho1, kS1,    kS2,    kS4,    kS6,   kSa,      kSa1,    kSigma1, kSK4,
    kSN4,  kSO1,   kSsa,   kSta,   kT2,   kTau1,    kTheta1, kUps1,
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
