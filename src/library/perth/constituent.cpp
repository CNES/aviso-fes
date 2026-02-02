#include "fes/perth/constituent.hpp"

#include <array>
#include <cstddef>
#include <stdexcept>
#include <string>

#include "fes/detail/string.hpp"
#include "fes/enum_mapper.hpp"

namespace fes {
namespace perth {

constexpr auto kConstituentNames =
    std::array<const char* const, kNumConstituentItems>{
        "Node",   "Sa1",    "Sa",    "Ssa",    "Sta",  "MSm",     "Mm",
        "MSf",    "Mf",     "MStm",  "Mtm",    "MSqm", "Mqm",     "2Q1",
        "Sigma1", "Q1",     "Rho1",  "O1",     "Tau1", "Beta1",   "M13",
        "M1",     "Chi1",   "Pi1",   "P1",     "S1",   "K1",      "Psi1",
        "Phi1",   "Theta1", "J1",    "SO1",    "OO1",  "Ups1",    "Eps2",
        "2N2",    "2MS2",   "Mu2",   "N2",     "N2P",  "Nu2",     "Gamma2",
        "Alpa2",  "M2",     "Beta2", "Delta2", "MKS2", "Lambda2", "2MN2",
        "L2",     "L2P",    "T2",    "S2",     "R2",   "K2",      "MSN2",
        "Eta2",   "2SM2",   "2MK3",  "MO3",    "M3",   "MK3",     "N4",
        "MN4",    "M4",     "SN4",   "MS4",    "MK4",  "S4",      "R4",
        "SK4",    "2MN6",   "M6",    "MSN6",   "2MS6", "2MK6",    "2SM6",
        "MSK6",   "S6",     "M8",
    };

namespace constituents {

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

}  // namespace perth
}  // namespace fes
