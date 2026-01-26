#include "fes/perth/constituent.hpp"

#include <array>
#include <cstddef>
#include <stdexcept>
#include <string>

#include "fes/detail/string.hpp"

namespace fes {
namespace perth {

constexpr auto kConstituentNames =
    std::array<const char* const, kNumConstituentItems>{
        "2MK3",   "2MK6",   "2MN2",    "2MN6",   "2MS2",  "2MS6",  "2N2",
        "2Q1",    "2SM2",   "2SM6",    "Alpa2",  "Beta1", "Beta2", "Chi1",
        "Delta2", "Eps2",   "Eta2",    "Gamma2", "J1",    "K1",    "K2",
        "L2",     "L2P",    "Lambda2", "M1",     "M13",   "M2",    "M3",
        "M4",     "M6",     "M8",      "Mf",     "MK3",   "MK4",   "MKS2",
        "Mm",     "MN4",    "MO3",     "Mqm",    "MS4",   "MSf",   "MSK6",
        "MSm",    "MSN2",   "MSN6",    "MSqm",   "MStm",  "Mtm",   "Mu2",
        "N2",     "N2P",    "N4",      "Node",   "Nu2",   "O1",    "OO1",
        "P1",     "Phi1",   "Pi1",     "Psi1",   "Q1",    "R2",    "R4",
        "Rho1",   "S1",     "S2",      "S4",     "S6",    "Sa",    "Sa1",
        "Sigma1", "SK4",    "SN4",     "SO1",    "Ssa",   "Sta",   "T2",
        "Tau1",   "Theta1", "Ups1",
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

}  // namespace constituents

}  // namespace perth
}  // namespace fes
