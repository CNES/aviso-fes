// Copyright (c) 2025 CNES
//
// All rights reserved. Use of this source code is governed by a
// BSD-style license that can be found in the LICENSE file.
#pragma once
#include <array>
#include <cstdint>
#include <string>
#include <vector>
/// @file include/fes/constituent.hpp
/// @brief Define the tidal constituents handled by the library.

namespace fes {

/// Tidal constituents known by the library.
enum Constituent : uint8_t {
  kMm,       //!< @f$Mm@f$
  kMf,       //!< @f$Mf@f$
  kMtm,      //!< @f$Mtm@f$
  kMsqm,     //!< @f$Msqm@f$
  k2Q1,      //!< @f$2Q_1@f$
  kSigma1,   //!< @f$\sigma_1@f$
  kQ1,       //!< @f$Q_1@f$
  kRho1,     //!< @f$\rho_1@f$
  kO1,       //!< @f$O_1@f$
  kMP1,      //!< @f$MP_1@f$
  kM1,       //!< @f$M_1@f$
  kM11,      //!< @f$M_{11}@f$
  kM12,      //!< @f$M_{12}@f$
  kM13,      //!< @f$M_{13}@f$
  kChi1,     //!< @f$\chi_1@f$
  kPi1,      //!< @f$\pi_1@f$
  kP1,       //!< @f$P_1@f$
  kS1,       //!< @f$S_1@f$
  kK1,       //!< @f$K_1@f$
  kPsi1,     //!< @f$\psi_1@f$
  kPhi1,     //!< @f$\phi_1@f$
  kTheta1,   //!< @f$\theta_1@f$
  kJ1,       //!< @f$J_1@f$
  kOO1,      //!< @f$OO_1@f$
  kMNS2,     //!< @f$MNS_2@f$
  kEps2,     //!< @f$\varepsilon_2@f$
  k2N2,      //!< @f$2N_2@f$
  kMu2,      //!< @f$\mu_2@f$
  k2MS2,     //!< @f$2MS_2@f$
  kN2,       //!< @f$N_2@f$
  kNu2,      //!< @f$\nu_2@f$
  kM2,       //!< @f$M_2@f$
  kMKS2,     //!< @f$MKS_2@f$
  kLambda2,  //!< @f$\lambda_2@f$
  kL2,       //!< @f$L_2@f$
  k2MN2,     //!< @f$2MN_2@f$
  kT2,       //!< @f$T_2@f$
  kS2,       //!< @f$S_2@f$
  kR2,       //!< @f$R_2@f$
  kK2,       //!< @f$K_2@f$
  kMSN2,     //!< @f$MSN_2@f$
  kEta2,     //!< @f$\eta_2@f$
  k2SM2,     //!< @f$2SM_2@f$
  kMO3,      //!< @f$MO_3@f$
  k2MK3,     //!< @f$2MK_3@f$
  kM3,       //!< @f$M_3@f$
  kMK3,      //!< @f$MK_3@f$
  kN4,       //!< @f$N_4@f$
  kMN4,      //!< @f$MN_4@f$
  kM4,       //!< @f$M_4@f$
  kSN4,      //!< @f$SN_4@f$
  kMS4,      //!< @f$MS_4@f$
  kMK4,      //!< @f$MK_4@f$
  kS4,       //!< @f$S_4@f$
  kSK4,      //!< @f$SK_4@f$
  kR4,       //!< @f$R_4@f$
  k2MN6,     //!< @f$2MN_6@f$
  kM6,       //!< @f$M_6@f$
  kMSN6,     //!< @f$MSN_6@f$
  k2MS6,     //!< @f$2MS_6@f$
  k2MK6,     //!< @f$2MK_6@f$
  k2SM6,     //!< @f$2SM_6@f$
  kMSK6,     //!< @f$MSK_6@f$
  kS6,       //!< @f$S_6@f$
  kM8,       //!< @f$M_8@f$
  kMSf,      //!< @f$MSf@f$
  kSsa,      //!< @f$Ssa@f$
  kSa,       //!< @f$Sa@f$
  kSa1,      //!< @f$Sa_1@f$
  kSta,      //!< @f$Sta@f$
  kMm1,      //!< @f$Mm_1@f$
  kMf1,      //!< @f$Mf_1@f$
  kA5,       //!< @f$A_5@f$
  kM0,       //!< @f$M_0@f$
  kMm2,      //!< @f$Mm_2@f$
  kMf2,      //!< @f$Mf_2@f$
  kL2P,      //!< @f$L2_P@f$
  kN2P,      //!< @f$N2_P@f$
};

namespace constituents {

/// @brief Returns all the tidal constituents handled by the library.
/// @return Tidal constituents.
constexpr auto all = std::array<Constituent, 76>{
    kMm,   kMf,     kMtm,  kMsqm,    k2Q1,  kSigma1, kQ1,   kRho1, kO1,   kMP1,
    kM1,   kM11,    kM12,  kM13,     kChi1, kPi1,    kP1,   kS1,   kK1,   kPsi1,
    kPhi1, kTheta1, kJ1,   kOO1,     kMNS2, kEps2,   k2N2,  kMu2,  k2MS2, kN2,
    kNu2,  kM2,     kMKS2, kLambda2, kL2,   k2MN2,   kT2,   kS2,   kR2,   kK2,
    kMSN2, kEta2,   k2SM2, kMO3,     k2MK3, kM3,     kMK3,  kN4,   kMN4,  kM4,
    kSN4,  kMS4,    kMK4,  kS4,      kSK4,  kR4,     k2MN6, kM6,   kMSN6, k2MS6,
    k2MK6, k2SM6,   kMSK6, kS6,      kM8,   kMSf,    kSsa,  kSa,   kSa1,  kSta,
    kMm1,  kMf1,    kA5,   kM0,      kMm2,  kMf2};

/// Parses the tidal constituent from a string. Parsing is case not sensitive.
/// So "Mm" and "mm" are equivalent.
///
/// @param[in] name Tide constituent name.
/// @return Tidal constituent.
auto parse(const std::string& name) -> Constituent;

/// Returns the name of a tidal constituent.
///
/// @param[in] constituent Tidal constituent.
/// @return Tidal constituent name.
auto name(Constituent constituent) -> const char*;

/// @brief Get the tidal constituent names handled by the library.
/// @return Tidal constituent names.
auto known() -> std::vector<std::string>;

}  // namespace constituents
}  // namespace fes
