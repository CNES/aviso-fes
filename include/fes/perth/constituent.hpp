#pragma once

#include <Eigen/Core>
#include <cstddef>
#include <cstdint>
#include <string>

#include "fes/eigen.hpp"

namespace fes {
namespace perth {

/// @brief Possible type of tidal wave.
enum ConstituentType : uint8_t {
  kLongPeriod = 0,  //!< Long period tidal waves
  kShortPeriod      //!< Short period tidal waves
};

/// @brief Enum class representing tidal constituents.
enum Constituent : uint8_t {
  kNode,
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

/// @brief Data structure that holds the tide of a constituent.
struct Wave {
  Vector7b doodson_number;    //!< Doodson number of the constituent
  std::complex<double> tide;  //!< Tide of the constituent
  double tidal_argument;      //!< Doodson argument
  ConstituentType type;       //!< Type of tidal wave
  bool is_inferred = false;   //!< Whether the tide was inferred from the
                              //!< constituents
};

/// @brief Number of items in the Constituent enum.
constexpr std::size_t kNumConstituentItems =
    static_cast<std::size_t>(kNumConstituents);

namespace constituents {

/// @brief Convert a constituent enum to its string name.
/// @param[in] constituent The constituent to convert.
/// @return The string name of the constituent.
auto name(Constituent constituent) -> std::string;

/// @brief Parse a constituent name string to its enum value.
/// @param[in] name The name of the constituent.
/// @return The corresponding Constituent enum value.
auto parse(const std::string &name) -> Constituent;

}  // namespace constituents

}  // namespace perth
}  // namespace fes
