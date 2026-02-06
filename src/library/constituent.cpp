// Copyright (c) 2026 CNES
//
// All rights reserved. Use of this source code is governed by a
// BSD-style license that can be found in the LICENSE file.
#include "fes/constituent.hpp"

#include <array>
#include <stdexcept>

#include "fes/detail/string.hpp"

namespace fes {
namespace constituents {

constexpr auto kConstituentNames =
    std::array<const char* const, kNumConstituents>{
        "2MK2",  "2MK3",  "2MK6",  "2MN2",   "2MN6", "2MNS4",   "2MP5",
        "2MS2",  "2MS6",  "2MSN4", "2N2",    "2NM6", "2NS2",    "2Q1",
        "2SM2",  "2SM6",  "2SMu2", "3MS4",   "3MS8", "A5",      "Alpha2",
        "Beta1", "Beta2", "Chi1",  "Delta2", "Eps2", "Eta2",    "Gamma2",
        "J1",    "K1",    "K2",    "L2",     "L2P",  "Lambda2", "M0",
        "M1",    "M11",   "M12",   "M13",    "M2",   "M3",      "M4",
        "M6",    "M8",    "Mf",    "Mf1",    "Mf2",  "MK3",     "MK4",
        "MKS2",  "ML4",   "Mm",    "Mm1",    "Mm2",  "MN4",     "MNK6",
        "MNS2",  "MNu4",  "MNuS2", "MO3",    "MP1",  "Mqm",     "MS4",
        "MSf",   "MSK2",  "MSK6",  "MSm",    "MSN2", "MSN6",    "MSqm",
        "MStm",  "Mtm",   "Mu2",   "N2",     "N2P",  "N4",      "NK4",
        "NKM2",  "Node",  "Nu2",   "O1",     "OO1",  "OQ2",     "P1",
        "Phi1",  "Pi1",   "Psi1",  "Q1",     "R2",   "R4",      "Rho1",
        "S1",    "S2",    "S4",    "S6",     "Sa",   "Sa1",     "Sigma1",
        "SK3",   "SK4",   "SKM2",  "SN4",    "SO1",  "SO3",     "Ssa",
        "Sta",   "T2",    "Tau1",  "Theta1", "Ups1",
    };

auto parse(const std::string& constituent_name) -> ConstituentId {
  for (size_t ix = 0; ix < kNumConstituents; ++ix) {
    if (detail::iequals(constituent_name, std::string{kConstituentNames[ix]})) {
      return static_cast<ConstituentId>(ix);
    }
  }
  throw std::invalid_argument("unknown constituent name: " + constituent_name);
}

auto name(ConstituentId constituent) -> const char* {
  const auto ix = static_cast<std::size_t>(constituent);
  if (ix >= kNumConstituents) {
    throw std::invalid_argument("unknown constituent: " +
                                std::to_string(static_cast<int>(constituent)));
  }
  return kConstituentNames[ix];
}

}  // namespace constituents
}  // namespace fes
