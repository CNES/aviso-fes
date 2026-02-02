// Copyright (c) 2026 CNES
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
enum Constituent : ConstituentId {
  kNode = 0,  //!< Node
  kSa1,       //!< @f$Sa_1@f$
  kSa,        //!< @f$Sa@f$
  kSsa,       //!< @f$Ssa@f$
  kSta,       //!< @f$Sta@f$
  kMSm,       //!< @f$MSm@f$
  kMm,        //!< @f$Mm@f$
  kMSf,       //!< @f$MSf@f$
  kMf,        //!< @f$Mf@f$
  kMStm,      //!< @f$MStm@f$
  kMtm,       //!< @f$Mtm@f$
  kMSqm,      //!< @f$MSqm@f$
  kMqm,       //!< @f$Mqm@f$
  k2Q1,       //!< @f$2Q_1@f$
  kSigma1,    //!< @f$\sigma_1@f$
  kQ1,        //!< @f$Q_1@f$
  kRho1,      //!< @f$\rho_1@f$
  kO1,        //!< @f$O_1@f$
  kTau1,      //!< @f$\tau_1@f$
  kBeta1,     //!< @f$\beta_1@f$
  kM13,       //!< @f$M_{13}@f$
  kM1,        //!< @f$M_1@f$
  kChi1,      //!< @f$\chi_1@f$
  kPi1,       //!< @f$\pi_1@f$
  kP1,        //!< @f$P_1@f$
  kS1,        //!< @f$S_1@f$
  kK1,        //!< @f$K_1@f$
  kPsi1,      //!< @f$\psi_1@f$
  kPhi1,      //!< @f$\phi_1@f$
  kTheta1,    //!< @f$\theta_1@f$
  kJ1,        //!< @f$J_1@f$
  kSO1,       //!< @f$SO_1@f$
  kOO1,       //!< @f$OO_1@f$
  kUps1,      //!< @f$\Upsilon_1@f$
  kEps2,      //!< @f$\epsilon_2@f$
  k2N2,       //!< @f$2N_2@f$
  k2MS2,      //!< @f$2MS_2@f$
  kMu2,       //!< @f$\mu_2@f$
  kN2,        //!< @f$N_2@f$
  kN2P,       //!< @f$N2_P@f$
  kNu2,       //!< @f$\nu_2@f$
  kGamma2,    //!< @f$\gamma_2@f$
  kAlpha2,    //!< @f$\alpha_2@f$
  kM2,        //!< @f$M_2@f$
  kBeta2,     //!< @f$\beta_2@f$
  kDelta2,    //!< @f$\delta_2@f$
  kMKS2,      //!< @f$MKS_2@f$
  kLambda2,   //!< @f$\lambda_2@f$
  k2MN2,      //!< @f$2MN_2@f$
  kL2,        //!< @f$L_2@f$
  kL2P,       //!< @f$L2_P@f$
  kT2,        //!< @f$T_2@f$
  kS2,        //!< @f$S_2@f$
  kR2,        //!< @f$R_2@f$
  kK2,        //!< @f$K_2@f$
  kMSN2,      //!< @f$MSN_2@f$
  kEta2,      //!< @f$\eta_2@f$
  k2SM2,      //!< @f$2SM_2@f$
  k2MK3,      //!< @f$2MK_3@f$
  kMO3,       //!< @f$MO_3@f$
  kM3,        //!< @f$M_3@f$
  kMK3,       //!< @f$MK_3@f$
  kN4,        //!< @f$N_4@f$
  kMN4,       //!< @f$MN_4@f$
  kM4,        //!< @f$M_4@f$
  kSN4,       //!< @f$SN_4@f$
  kMS4,       //!< @f$MS_4@f$
  kMK4,       //!< @f$MK_4@f$
  kS4,        //!< @f$S_4@f$
  kR4,        //!< @f$R_4@f$
  kSK4,       //!< @f$SK_4@f$
  k2MN6,      //!< @f$2MN_6@f$
  kM6,        //!< @f$M_6@f$
  kMSN6,      //!< @f$MSN_6@f$
  k2MS6,      //!< @f$2MS_6@f$
  k2MK6,      //!< @f$2MK_6@f$
  k2SM6,      //!< @f$2SM_6@f$
  kMSK6,      //!< @f$MSK_6@f$
  kS6,        //!< @f$S_6@f$
  kM8,        //!< @f$M_8@f$
  kNumConstituents,
};

/// @brief Number of items in the Constituent enum.
constexpr std::size_t kNumConstituentItems =
    static_cast<std::size_t>(kNumConstituents);

namespace constituents {

/// @brief Returns all the tidal constituents handled by the library.
/// @return Tidal constituents.
constexpr auto kAll = std::array<Constituent, kNumConstituentItems>{
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

/// @brief Convert a constituent enum to its string name.
/// @param[in] constituent The constituent to convert.
/// @return The string name of the constituent.
auto name(Constituent constituent) -> const char*;

/// @brief Parse a constituent name string to its enum value.
/// @param[in] name The name of the constituent.
/// @return The corresponding Constituent enum value.
auto parse(const std::string& name) -> Constituent;

/// @brief Return the list of known constituents by this library.
/// @return List of constituent names.
auto known() -> std::array<std::string, kNumConstituentItems>;

/// @brief Create and return an EnumMapper for the Constituent enum.
/// @return EnumMapper for Constituent.
auto map() -> const ConstituentMap&;

}  // namespace constituents

}  // namespace perth
}  // namespace fes
