#include "fes/perth/constituent.hpp"

#include <algorithm>
#include <array>
#include <cstddef>
#include <stdexcept>
#include <string>
#include <utility>

#include "fes/detail/string.hpp"
#include "fes/eigen.hpp"

namespace fes {
namespace perth {

/// @brief Properties of a tidal wave.
struct Wave {
  Vector7b doodson_number;  //!< Doodson number of the constituent
  ConstituentType type;     //!< Type of tidal wave
};

// Known constituents
static const std::array<std::pair<Constituent, Wave>,
                        Constituent::kNumConstituents>
    kConstituents{{
        {Constituent::k2MK3, {{3, -1, 0, 0, 0, 0, 3}, kShortPeriod}},
        {Constituent::k2MK6, {{6, 2, 0, 0, 0, 0, 0}, kShortPeriod}},
        {Constituent::k2MN2, {{2, 1, 0, -1, 0, 0, 0}, kShortPeriod}},
        {Constituent::k2MN6, {{6, -1, 0, 1, 0, 0, 0}, kShortPeriod}},
        {Constituent::k2MS2, {{2, -2, 2, 0, 0, 0, 0}, kShortPeriod}},
        {Constituent::k2MS6, {{6, 2, -2, 0, 0, 0, 0}, kShortPeriod}},
        {Constituent::k2N2, {{2, -2, 0, 2, 0, 0, 0}, kShortPeriod}},
        {Constituent::k2Q1, {{1, -3, 0, 2, 0, 0, 3}, kShortPeriod}},
        {Constituent::k2SM2, {{2, 4, -4, 0, 0, 0, 0}, kShortPeriod}},
        {Constituent::k2SM6, {{6, 4, -4, 0, 0, 0, 0}, kShortPeriod}},
        {Constituent::kAlpa2, {{2, 0, -1, 0, 0, 1, 2}, kShortPeriod}},
        {Constituent::kBeta2, {{2, 0, 1, 0, 0, -1, 0}, kShortPeriod}},
        {Constituent::kBeta1, {{1, 0, -2, 1, 0, 0, 1}, kShortPeriod}},
        {Constituent::kChi1, {{1, 0, 2, -1, 0, 0, 1}, kShortPeriod}},
        {Constituent::kDelta2, {{2, 0, 2, 0, 0, 0, 0}, kShortPeriod}},
        {Constituent::kEps2, {{2, -3, 2, 1, 0, 0, 0}, kShortPeriod}},
        {Constituent::kEta2, {{2, 3, 0, -1, 0, 0, 0}, kShortPeriod}},
        {Constituent::kGamma2, {{2, 0, -2, 2, 0, 0, 2}, kShortPeriod}},
        {Constituent::kJ1, {{1, 2, 0, -1, 0, 0, 1}, kShortPeriod}},
        {Constituent::kK1, {{1, 1, 0, 0, 0, 0, 1}, kShortPeriod}},
        {Constituent::kK2, {{2, 2, 0, 0, 0, 0, 0}, kShortPeriod}},
        {Constituent::kL2, {{2, 1, 0, -1, 0, 0, 2}, kShortPeriod}},
        {Constituent::kL2P, {{2, 1, 0, 0, 0, 0, 3}, kShortPeriod}},
        {Constituent::kLambda2, {{2, 1, -2, 1, 0, 0, 2}, kShortPeriod}},
        {Constituent::kM1, {{1, 0, 0, 1, 0, 0, 1}, kShortPeriod}},
        {Constituent::kM13, {{1, 0, 0, 0, 0, 0, 2}, kShortPeriod}},
        {Constituent::kM2, {{2, 0, 0, 0, 0, 0, 0}, kShortPeriod}},
        {Constituent::kM3, {{3, 0, 0, 0, 0, 0, 2}, kShortPeriod}},
        {Constituent::kM4, {{4, 0, 0, 0, 0, 0, 0}, kShortPeriod}},
        {Constituent::kM6, {{6, 0, 0, 0, 0, 0, 0}, kShortPeriod}},
        {Constituent::kM8, {{8, 0, 0, 0, 0, 0, 0}, kShortPeriod}},
        {Constituent::kMK3, {{3, 1, 0, 0, 0, 0, 1}, kShortPeriod}},
        {Constituent::kMK4, {{4, 2, 0, 0, 0, 0, 0}, kShortPeriod}},
        {Constituent::kMKS2, {{2, 0, 2, 0, 0, 0, 0}, kShortPeriod}},
        {Constituent::kMN4, {{4, -1, 0, 1, 0, 0, 0}, kShortPeriod}},
        {Constituent::kMO3, {{3, -1, 0, 0, 0, 0, 3}, kShortPeriod}},
        {Constituent::kMS4, {{4, 2, -2, 0, 0, 0, 0}, kShortPeriod}},
        {Constituent::kMSK6, {{6, 4, -2, 0, 0, 0, 0}, kShortPeriod}},
        {Constituent::kMSN2, {{2, 3, -2, -1, 0, 0, 0}, kShortPeriod}},
        {Constituent::kMSN6, {{6, 1, -2, 1, 0, 0, 0}, kShortPeriod}},
        {Constituent::kMu2, {{2, -2, 2, 0, 0, 0, 0}, kShortPeriod}},
        {Constituent::kN2, {{2, -1, 0, 1, 0, 0, 0}, kShortPeriod}},
        {Constituent::kN2P, {{2, -1, 0, 0, 0, 0, 1}, kShortPeriod}},
        {Constituent::kN4, {{4, -2, 0, 2, 0, 0, 0}, kShortPeriod}},
        {Constituent::kNu2, {{2, -1, 2, -1, 0, 0, 0}, kShortPeriod}},
        {Constituent::kO1, {{1, -1, 0, 0, 0, 0, 3}, kShortPeriod}},
        {Constituent::kOO1, {{1, 3, 0, 0, 0, 0, 1}, kShortPeriod}},
        {Constituent::kP1, {{1, 1, -2, 0, 0, 0, 3}, kShortPeriod}},
        {Constituent::kPhi1, {{1, 1, 2, 0, 0, 0, 1}, kShortPeriod}},
        {Constituent::kPi1, {{1, 1, -3, 0, 0, 1, 3}, kShortPeriod}},
        {Constituent::kPsi1, {{1, 1, 1, 0, 0, -1, 1}, kShortPeriod}},
        {Constituent::kQ1, {{1, -2, 0, 1, 0, 0, 3}, kShortPeriod}},
        {Constituent::kR2, {{2, 2, -1, 0, 0, -1, 2}, kShortPeriod}},
        {Constituent::kR4, {{4, 4, -3, 0, 0, 0, 0}, kShortPeriod}},
        {Constituent::kRho1, {{1, -2, 2, -1, 0, 0, 3}, kShortPeriod}},
        {Constituent::kS1, {{1, 1, -1, 0, 0, 0, 2}, kShortPeriod}},
        {Constituent::kS2, {{2, 2, -2, 0, 0, 0, 0}, kShortPeriod}},
        {Constituent::kS4, {{4, 4, -4, 0, 0, 0, 0}, kShortPeriod}},
        {Constituent::kS6, {{6, 6, -6, 0, 0, 0, 0}, kShortPeriod}},
        {Constituent::kSigma1, {{1, -3, 2, 0, 0, 0, 3}, kShortPeriod}},
        {Constituent::kSK4, {{4, 4, -2, 0, 0, 0, 0}, kShortPeriod}},
        {Constituent::kSN4, {{4, 1, -2, 1, 0, 0, 0}, kShortPeriod}},
        {Constituent::kSO1, {{1, 3, -2, 0, 0, 0, 1}, kShortPeriod}},
        {Constituent::kT2, {{2, 2, -3, 0, 0, 1, 0}, kShortPeriod}},
        {Constituent::kTau1, {{1, -1, 2, 0, 0, 0, 1}, kShortPeriod}},
        {Constituent::kTheta1, {{1, 2, -2, 1, 0, 0, 1}, kShortPeriod}},
        {Constituent::kUps1, {{1, 4, 0, -1, 0, 0, 1}, kShortPeriod}},
        {Constituent::kMf, {{0, 2, 0, 0, 0, 0, 0}, kLongPeriod}},
        {Constituent::kMm, {{0, 1, 0, -1, 0, 0, 0}, kLongPeriod}},
        {Constituent::kMqm, {{0, 4, 0, -2, 0, 0, 0}, kLongPeriod}},
        {Constituent::kMSf, {{0, 2, -2, 0, 0, 0, 0}, kLongPeriod}},
        {Constituent::kMSm, {{0, 1, -2, 1, 0, 0, 0}, kLongPeriod}},
        {Constituent::kMSqm, {{0, 4, -2, 0, 0, 0, 0}, kLongPeriod}},
        {Constituent::kMStm, {{0, 3, -2, 1, 0, 0, 0}, kLongPeriod}},
        {Constituent::kMtm, {{0, 3, 0, -1, 0, 0, 0}, kLongPeriod}},
        {Constituent::kNode, {{0, 0, 0, 0, 1, 0, 2}, kLongPeriod}},
        {Constituent::kSa, {{0, 0, 1, 0, 0, 0, 0}, kLongPeriod}},
        {Constituent::kSa1, {{0, 0, 1, 0, 0, -1, 0}, kLongPeriod}},
        {Constituent::kSsa, {{0, 0, 2, 0, 0, 0, 0}, kLongPeriod}},
        {Constituent::kSta, {{0, 0, 3, 0, 0, 0, 0}, kLongPeriod}},
    }};

constexpr auto constituent_to_index(Constituent constituent) -> std::size_t {
  auto result = static_cast<std::size_t>(constituent);
  if (result >= Constituent::kNumConstituents) {
    throw std::out_of_range("Invalid constituent index");
  }
  return result;
}

auto constituent_to_name(Constituent constituent) -> std::string {
  switch (constituent) {
    case Constituent::k2MK3:
      return "2MK3";
    case Constituent::k2MK6:
      return "2MK6";
    case Constituent::k2MN2:
      return "2MN2";
    case Constituent::k2MN6:
      return "2MN6";
    case Constituent::k2MS2:
      return "2MS2";
    case Constituent::k2MS6:
      return "2MS6";
    case Constituent::k2N2:
      return "2N2";
    case Constituent::k2Q1:
      return "2Q1";
    case Constituent::k2SM2:
      return "2SM2";
    case Constituent::k2SM6:
      return "2SM6";
    case Constituent::kAlpa2:
      return "Alpha2";
    case Constituent::kBeta2:
      return "Beta2";
    case Constituent::kBeta1:
      return "Beta1";
    case Constituent::kChi1:
      return "Chi1";
    case Constituent::kDelta2:
      return "Delta2";
    case Constituent::kEps2:
      return "Eps2";
    case Constituent::kEta2:
      return "Eta2";
    case Constituent::kGamma2:
      return "Gamma2";
    case Constituent::kJ1:
      return "J1";
    case Constituent::kK1:
      return "K1";
    case Constituent::kK2:
      return "K2";
    case Constituent::kL2:
      return "L2";
    case Constituent::kL2P:
      return "L2P";
    case Constituent::kLambda2:
      return "Lambda2";
    case Constituent::kM1:
      return "M1";
    case Constituent::kM13:
      return "M13";
    case Constituent::kM2:
      return "M2";
    case Constituent::kM3:
      return "M3";
    case Constituent::kM4:
      return "M4";
    case Constituent::kM6:
      return "M6";
    case Constituent::kM8:
      return "M8";
    case Constituent::kMf:
      return "Mf";
    case Constituent::kMK3:
      return "MK3";
    case Constituent::kMK4:
      return "MK4";
    case Constituent::kMKS2:
      return "MKS2";
    case Constituent::kMm:
      return "Mm";
    case Constituent::kMN4:
      return "MN4";
    case Constituent::kMO3:
      return "MO3";
    case Constituent::kMqm:
      return "Mqm";
    case Constituent::kMS4:
      return "MS4";
    case Constituent::kMSf:
      return "MSf";
    case Constituent::kMSK6:
      return "MSK6";
    case Constituent::kMSm:
      return "MSm";
    case Constituent::kMSN2:
      return "MSN2";
    case Constituent::kMSN6:
      return "MSN6";
    case Constituent::kMSqm:
      return "MSqm";
    case Constituent::kMStm:
      return "MStm";
    case Constituent::kMtm:
      return "Mtm";
    case Constituent::kMu2:
      return "Mu2";
    case Constituent::kN2:
      return "N2";
    case Constituent::kN2P:
      return "N2P";
    case Constituent::kN4:
      return "N4";
    case Constituent::kNode:
      return "Node";
    case Constituent::kNu2:
      return "Nu2";
    case Constituent::kO1:
      return "O1";
    case Constituent::kOO1:
      return "OO1";
    case Constituent::kP1:
      return "P1";
    case Constituent::kPhi1:
      return "Phi1";
    case Constituent::kPi1:
      return "Pi1";
    case Constituent::kPsi1:
      return "Psi1";
    case Constituent::kQ1:
      return "Q1";
    case Constituent::kR2:
      return "R2";
    case Constituent::kR4:
      return "R4";
    case Constituent::kRho1:
      return "Rho1";
    case Constituent::kS1:
      return "S1";
    case Constituent::kS2:
      return "S2";
    case Constituent::kS4:
      return "S4";
    case Constituent::kS6:
      return "S6";
    case Constituent::kSa:
      return "Sa";
    case Constituent::kSa1:
      return "Sa1";
    case Constituent::kSigma1:
      return "Sigma1";
    case Constituent::kSK4:
      return "SK4";
    case Constituent::kSN4:
      return "SN4";
    case Constituent::kSO1:
      return "SO1";
    case Constituent::kSsa:
      return "Ssa";
    case Constituent::kSta:
      return "Sta";
    case Constituent::kT2:
      return "T2";
    case Constituent::kTau1:
      return "Tau1";
    case Constituent::kTheta1:
      return "Theta1";
    case Constituent::kUps1:
      return "Ups1";
    default:
      return "Unknown";
  }
}

auto assemble_constituent_table(const std::vector<Constituent>& constituents)
    -> ConstituentTable {
  ConstituentTable::Item items;
  ConstituentTable::Key keys;
  for (const auto& kv : kConstituents) {
    const auto& key = kv.first;
    const auto& wave = kv.second;
    auto index = constituent_to_index(key);
    auto inferred = std::find(constituents.begin(), constituents.end(), key);
    items[index] = {wave.doodson_number,
                    {0, 0},
                    0,
                    wave.type,
                    inferred == constituents.end()};
    keys[index] = key;
  }
  return ConstituentTable(std::move(keys), std::move(items));
}

auto name_to_constituent(const std::string& name) -> Constituent {
  for (const auto& kv : kConstituents) {
    if (detail::iequals(constituent_to_name(kv.first), name)) {
      return kv.first;
    }
  }
  throw std::invalid_argument("Invalid constituent name: " + name);
}

}  // namespace perth
}  // namespace fes
