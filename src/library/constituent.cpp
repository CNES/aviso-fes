// Copyright (c) 2024 CNES
//
// All rights reserved. Use of this source code is governed by a
// BSD-style license that can be found in the LICENSE file.
#include "fes/constituent.hpp"

#include <stdexcept>
#include <string>

#include "fes/detail/wave/name.hpp"

namespace fes {
namespace constituents {

namespace wave = fes::detail::wave;

auto known() -> std::vector<std::string> {
  auto result = std::vector<std::string>();
  result.reserve(all.size());
  for (size_t ix = 0; ix < all.size(); ++ix) {
    result.emplace_back(name(static_cast<Constituent>(ix)));
  }
  return result;
}

inline auto iequals(const std::string& a, const std::string& b) -> bool {
  return std::equal(a.begin(), a.end(), b.begin(), b.end(),
                    [](char a, char b) { return tolower(a) == tolower(b); });
}

auto name(Constituent constituent) -> const char* {
  switch (constituent) {
    case kO1:
      return detail::wave::name::kO1;
    case kP1:
      return detail::wave::name::kP1;
    case kK1:
      return detail::wave::name::kK1;
    case k2N2:
      return detail::wave::name::k2N2;
    case kMu2:
      return detail::wave::name::kMu2;
    case kN2:
      return detail::wave::name::kN2;
    case kNu2:
      return detail::wave::name::kNu2;
    case kM2:
      return detail::wave::name::kM2;
    case kL2:
      return detail::wave::name::kL2;
    case kT2:
      return detail::wave::name::kT2;
    case kS2:
      return detail::wave::name::kS2;
    case kK2:
      return detail::wave::name::kK2;
    case kM4:
      return detail::wave::name::kM4;
    case kS1:
      return detail::wave::name::kS1;
    case kQ1:
      return detail::wave::name::kQ1;
    case kMm:
      return detail::wave::name::kMm;
    case kMf:
      return detail::wave::name::kMf;
    case kMtm:
      return detail::wave::name::kMtm;
    case kMsqm:
      return detail::wave::name::kMsqm;
    case kEps2:
      return detail::wave::name::kEps2;
    case kLambda2:
      return detail::wave::name::kLambda2;
    case kEta2:
      return detail::wave::name::kEta2;
    case k2Q1:
      return detail::wave::name::k2Q1;
    case kSigma1:
      return detail::wave::name::kSigma1;
    case kRho1:
      return detail::wave::name::kRho1;
    case kM11:
      return detail::wave::name::kM11;
    case kM12:
      return detail::wave::name::kM12;
    case kChi1:
      return detail::wave::name::kChi1;
    case kPi1:
      return detail::wave::name::kPi1;
    case kPhi1:
      return detail::wave::name::kPhi1;
    case kTheta1:
      return detail::wave::name::kTheta1;
    case kJ1:
      return detail::wave::name::kJ1;
    case kOO1:
      return detail::wave::name::kOO1;
    case kM3:
      return detail::wave::name::kM3;
    case kM6:
      return detail::wave::name::kM6;
    case kMN4:
      return detail::wave::name::kMN4;
    case kMS4:
      return detail::wave::name::kMS4;
    case kN4:
      return detail::wave::name::kN4;
    case kR2:
      return detail::wave::name::kR2;
    case kR4:
      return detail::wave::name::kR4;
    case kS4:
      return detail::wave::name::kS4;
    case kMNS2:
      return detail::wave::name::kMNS2;
    case kM13:
      return detail::wave::name::kM13;
    case kMK4:
      return detail::wave::name::kMK4;
    case kSN4:
      return detail::wave::name::kSN4;
    case kSK4:
      return detail::wave::name::kSK4;
    case k2MN6:
      return detail::wave::name::k2MN6;
    case k2MS6:
      return detail::wave::name::k2MS6;
    case k2MK6:
      return detail::wave::name::k2MK6;
    case kMSN6:
      return detail::wave::name::kMSN6;
    case k2SM6:
      return detail::wave::name::k2SM6;
    case kMSK6:
      return detail::wave::name::kMSK6;
    case kMP1:
      return detail::wave::name::kMP1;
    case k2SM2:
      return detail::wave::name::k2SM2;
    case kPsi1:
      return detail::wave::name::kPsi1;
    case k2MS2:
      return detail::wave::name::k2MS2;
    case kMKS2:
      return detail::wave::name::kMKS2;
    case k2MN2:
      return detail::wave::name::k2MN2;
    case kMSN2:
      return detail::wave::name::kMSN2;
    case kMO3:
      return detail::wave::name::kMO3;
    case k2MK3:
      return detail::wave::name::k2MK3;
    case kMK3:
      return detail::wave::name::kMK3;
    case kS6:
      return detail::wave::name::kS6;
    case kM8:
      return detail::wave::name::kM8;
    case kMSf:
      return detail::wave::name::kMSf;
    case kSsa:
      return detail::wave::name::kSsa;
    case kSa:
      return detail::wave::name::kSa;
    case kA5:
      return detail::wave::name::kA5;
    case kSa1:
      return detail::wave::name::kSa1;
    case kSta:
      return detail::wave::name::kSta;
    case kMm1:
      return detail::wave::name::kMm1;
    case kMm2:
      return detail::wave::name::kMm2;
    case kMf1:
      return detail::wave::name::kMf1;
    case kMf2:
      return detail::wave::name::kMf2;
    case kM0:
      return detail::wave::name::kM0;
    default:
      throw std::invalid_argument("unknown wave: " +
                                  std::to_string(constituent));
  }
}

auto parse(const std::string& name) -> Constituent {
  if (iequals(name, wave::name::kO1)) {
    return kO1;
  }
  if (iequals(name, wave::name::kP1)) {
    return kP1;
  }
  if (iequals(name, wave::name::kK1)) {
    return kK1;
  }
  if (iequals(name, wave::name::k2N2)) {
    return k2N2;
  }
  if (iequals(name, wave::name::kMu2)) {
    return kMu2;
  }
  if (iequals(name, wave::name::kN2)) {
    return kN2;
  }
  if (iequals(name, wave::name::kNu2)) {
    return kNu2;
  }
  if (iequals(name, wave::name::kM2)) {
    return kM2;
  }
  if (iequals(name, wave::name::kL2)) {
    return kL2;
  }
  if (iequals(name, wave::name::kT2)) {
    return kT2;
  }
  if (iequals(name, wave::name::kS2)) {
    return kS2;
  }
  if (iequals(name, wave::name::kK2)) {
    return kK2;
  }
  if (iequals(name, wave::name::kM4)) {
    return kM4;
  }
  if (iequals(name, wave::name::kS1)) {
    return kS1;
  }
  if (iequals(name, wave::name::kQ1)) {
    return kQ1;
  }
  if (iequals(name, wave::name::kMm)) {
    return kMm;
  }
  if (iequals(name, wave::name::kMf)) {
    return kMf;
  }
  if (iequals(name, wave::name::kMtm)) {
    return kMtm;
  }
  if (iequals(name, wave::name::kMsqm)) {
    return kMsqm;
  }
  if (iequals(name, wave::name::kEps2)) {
    return kEps2;
  }
  if (iequals(name, wave::name::kLambda2)) {
    return kLambda2;
  }
  if (iequals(name, wave::name::kEta2)) {
    return kEta2;
  }
  if (iequals(name, wave::name::k2Q1)) {
    return k2Q1;
  }
  if (iequals(name, wave::name::kSigma1)) {
    return kSigma1;
  }
  if (iequals(name, wave::name::kRho1)) {
    return kRho1;
  }
  if (iequals(name, wave::name::kM11)) {
    return kM11;
  }
  if (iequals(name, wave::name::kM12)) {
    return kM12;
  }
  if (iequals(name, wave::name::kChi1)) {
    return kChi1;
  }
  if (iequals(name, wave::name::kPi1)) {
    return kPi1;
  }
  if (iequals(name, wave::name::kPhi1)) {
    return kPhi1;
  }
  if (iequals(name, wave::name::kTheta1)) {
    return kTheta1;
  }
  if (iequals(name, wave::name::kJ1)) {
    return kJ1;
  }
  if (iequals(name, wave::name::kOO1)) {
    return kOO1;
  }
  if (iequals(name, wave::name::kM3)) {
    return kM3;
  }
  if (iequals(name, wave::name::kM6)) {
    return kM6;
  }
  if (iequals(name, wave::name::kMN4)) {
    return kMN4;
  }
  if (iequals(name, wave::name::kMS4)) {
    return kMS4;
  }
  if (iequals(name, wave::name::kN4)) {
    return kN4;
  }
  if (iequals(name, wave::name::kR2)) {
    return kR2;
  }
  if (iequals(name, wave::name::kR4)) {
    return kR4;
  }
  if (iequals(name, wave::name::kS4)) {
    return kS4;
  }
  if (iequals(name, wave::name::kMNS2)) {
    return kMNS2;
  }
  if (iequals(name, wave::name::kM13)) {
    return kM13;
  }
  if (iequals(name, wave::name::kMK4)) {
    return kMK4;
  }
  if (iequals(name, wave::name::kSN4)) {
    return kSN4;
  }
  if (iequals(name, wave::name::kSK4)) {
    return kSK4;
  }
  if (iequals(name, wave::name::k2MN6)) {
    return k2MN6;
  }
  if (iequals(name, wave::name::k2MS6)) {
    return k2MS6;
  }
  if (iequals(name, wave::name::k2MK6)) {
    return k2MK6;
  }
  if (iequals(name, wave::name::kMSN6)) {
    return kMSN6;
  }
  if (iequals(name, wave::name::k2SM6)) {
    return k2SM6;
  }
  if (iequals(name, wave::name::kMSK6)) {
    return kMSK6;
  }
  if (iequals(name, wave::name::kMP1)) {
    return kMP1;
  }
  if (iequals(name, wave::name::k2SM2)) {
    return k2SM2;
  }
  if (iequals(name, wave::name::kPsi1)) {
    return kPsi1;
  }
  if (iequals(name, wave::name::k2MS2)) {
    return k2MS2;
  }
  if (iequals(name, wave::name::kMKS2)) {
    return kMKS2;
  }
  if (iequals(name, wave::name::k2MN2)) {
    return k2MN2;
  }
  if (iequals(name, wave::name::kMSN2)) {
    return kMSN2;
  }
  if (iequals(name, wave::name::kMO3)) {
    return kMO3;
  }
  if (iequals(name, wave::name::k2MK3)) {
    return k2MK3;
  }
  if (iequals(name, wave::name::kMK3)) {
    return kMK3;
  }
  if (iequals(name, wave::name::kS6)) {
    return kS6;
  }
  if (iequals(name, wave::name::kM8)) {
    return kM8;
  }
  if (iequals(name, wave::name::kMSf)) {
    return kMSf;
  }
  if (iequals(name, wave::name::kSsa)) {
    return kSsa;
  }
  if (iequals(name, wave::name::kSa)) {
    return kSa;
  }
  if (iequals(name, wave::name::kA5)) {
    return kA5;
  }
  if (iequals(name, wave::name::kSa1)) {
    return kSa1;
  }
  if (iequals(name, wave::name::kSta)) {
    return kSta;
  }
  if (iequals(name, wave::name::kMm1)) {
    return kMm1;
  }
  if (iequals(name, wave::name::kMm2)) {
    return kMm2;
  }
  if (iequals(name, wave::name::kMf1)) {
    return kMf1;
  }
  if (iequals(name, wave::name::kMf2)) {
    return kMf2;
  }
  if (iequals(name, wave::name::kM0)) {
    return kM0;
  }
  throw std::invalid_argument("invalid tidal constituent: " + name);
}

}  // namespace constituents
}  // namespace fes
