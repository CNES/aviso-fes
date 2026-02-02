// Copyright (c) 2026 CNES
//
// All rights reserved. Use of this source code is governed by a
// BSD-style license that can be found in the LICENSE file.
#include "fes/darwin/constituent.hpp"

#include <stdexcept>
#include <string>

#include "fes/detail/string.hpp"
#include "fes/enum_mapper.hpp"

namespace fes {
namespace darwin {
namespace constituents {

constexpr auto kConstituentNames =
    std::array<const char* const, kNumConstituentItems>{
        "2MK2",   "2MK3",  "2MK6",   "2MN2", "2MN6",    "2MNS4", "2MP5", "2MS2",
        "2MS6",   "2MSN4", "2N2",    "2NM6", "2NS2",    "2Q1",   "2SM2", "2SM6",
        "2SMu2",  "3MS4",  "3MS8",   "A5",   "Chi1",    "Eps2",  "Eta2", "J1",
        "K1",     "K2",    "L2",     "L2P",  "Lambda2", "M0",    "M1",   "M11",
        "M12",    "M13",   "M2",     "M3",   "M4",      "M6",    "M8",   "Mf",
        "Mf1",    "Mf2",   "MK3",    "MK4",  "MKS2",    "ML4",   "Mm",   "Mm1",
        "Mm2",    "MN4",   "MNK6",   "MNS2", "MNu4",    "MNuS2", "MO3",  "MP1",
        "MS4",    "MSf",   "MSK2",   "MSK6", "MSN2",    "MSN6",  "MSqm", "Mtm",
        "Mu2",    "N2",    "N2P",    "N4",   "NK4",     "NKM2",  "Nu2",  "O1",
        "OO1",    "OQ2",   "P1",     "Phi1", "Pi1",     "Psi1",  "Q1",   "R2",
        "R4",     "Rho1",  "S1",     "S2",   "S4",      "S6",    "Sa",   "Sa1",
        "Sigma1", "SK3",   "SK4",    "SKM2", "SN4",     "SO1",   "SO3",  "Ssa",
        "Sta",    "T2",    "Theta1",
    };

auto name(Constituent constituent) -> const char* const {
  const auto ix = static_cast<std::size_t>(constituent);
  if (ix >= kNumConstituentItems) {
    throw std::invalid_argument("unknown constituent: " +
                                std::to_string(static_cast<int>(constituent)));
  }
  return kConstituentNames[ix];
}

auto known() -> std::array<std::string, kNumConstituentItems> {
  auto result = std::array<std::string, kNumConstituentItems>{};
  for (size_t ix = 0; ix < kNumConstituentItems; ++ix) {
    result[ix] = std::string{kConstituentNames[ix]};
  }
  return result;
}

auto parse(const std::string& constituent_name) -> Constituent {
  for (size_t ix = 0; ix < kNumConstituentItems; ++ix) {
    if (detail::iequals(constituent_name, std::string{kConstituentNames[ix]})) {
      return static_cast<Constituent>(ix);
    }
  }
  throw std::invalid_argument("unknown constituent name: " + constituent_name);
}

auto map() -> const ConstituentMap& {
  static auto mapper = []() -> const ConstituentMap {
    ConstituentMap m;
    m.reserve(kNumConstituentItems);

    for (auto& id : kAll) {
      m.add_entry(static_cast<ConstituentId>(id), name(id));
    }
    m.finalize();
    return m;
  }();
  return mapper;
}

}  // namespace constituents
}  // namespace darwin
}  // namespace fes
