// Copyright (c) 2026 CNES
//
// All rights reserved. Use of this source code is governed by a
// BSD-style license that can be found in the LICENSE file.
/// @file include/fes/darwin/constituent.hpp
/// @brief Define the tidal constituents handled by the library.
#pragma once
#include <array>
#include <cstdint>
#include <string>

#include "fes/enum_mapper.hpp"

namespace fes {
namespace darwin {

/// Tidal constituents known by the library.
enum Constituent : ConstituentId {
  k2MK2,            //!< @f$2MK_2@f$
  k2MK3,            //!< @f$2MK_3@f$
  k2MK6,            //!< @f$2MK_6@f$
  k2MN2,            //!< @f$2MN_2@f$
  k2MN6,            //!< @f$2MN_6@f$
  k2MNS4,           //!< @f$2MNS_4@f$
  k2MP5,            //!< @f$2MP_5@f$
  k2MS2,            //!< @f$2MS_2@f$
  k2MS6,            //!< @f$2MS_6@f$
  k2MSN4,           //!< @f$2MSN_4@f$
  k2N2,             //!< @f$2N_2@f$
  k2NM6,            //!< @f$2NM_6@f$
  k2NS2,            //!< @f$2NS_2@f$
  k2Q1,             //!< @f$2Q_1@f$
  k2SM2,            //!< @f$2SM_2@f$
  k2SM6,            //!< @f$2SM_6@f$
  k2SMu2,           //!< @f$2SMu_2@f$
  k3MS4,            //!< @f$3MS_4@f$
  k3MS8,            //!< @f$3MS_8@f$
  kA5,              //!< @f$A_5@f$
  kChi1,            //!< @f$\chi_1@f$
  kEps2,            //!< @f$\varepsilon_2@f$
  kEta2,            //!< @f$\eta_2@f$
  kJ1,              //!< @f$J_1@f$
  kK1,              //!< @f$K_1@f$
  kK2,              //!< @f$K_2@f$
  kL2,              //!< @f$L_2@f$
  kL2P,             //!< @f$L2_P@f$
  kLambda2,         //!< @f$\lambda_2@f$
  kM0,              //!< @f$M_0@f$
  kM1,              //!< @f$M_1@f$
  kM11,             //!< @f$M_{11}@f$
  kM12,             //!< @f$M_{12}@f$
  kM13,             //!< @f$M_{13}@f$
  kM2,              //!< @f$M_2@f$
  kM3,              //!< @f$M_3@f$
  kM4,              //!< @f$M_4@f$
  kM6,              //!< @f$M_6@f$
  kM8,              //!< @f$M_8@f$
  kMf,              //!< @f$Mf@f$
  kMf1,             //!< @f$Mf_1@f$
  kMf2,             //!< @f$Mf_2@f$
  kMK3,             //!< @f$MK_3@f$
  kMK4,             //!< @f$MK_4@f$
  kMKS2,            //!< @f$MKS_2@f$
  kML4,             //!< @f$ML_4@f$
  kMm,              //!< @f$Mm@f$
  kMm1,             //!< @f$Mm_1@f$
  kMm2,             //!< @f$Mm_2@f$
  kMN4,             //!< @f$MN_4@f$
  kMNK6,            //!< @f$MNK_6@f$
  kMNS2,            //!< @f$MNS_2@f$
  kMNu4,            //!< @f$MNu_4@f$
  kMNuS2,           //!< @f$MNuS_2@f$
  kMO3,             //!< @f$MO_3@f$
  kMP1,             //!< @f$MP_1@f$
  kMS4,             //!< @f$MS_4@f$
  kMSf,             //!< @f$MSf@f$
  kMSK2,            //!< @f$MSK_2@f$
  kMSK6,            //!< @f$MSK_6@f$
  kMSN2,            //!< @f$MSN_2@f$
  kMSN6,            //!< @f$MSN_6@f$
  kMSqm,            //!< @f$MSqm@f$
  kMtm,             //!< @f$Mtm@f$
  kMu2,             //!< @f$\mu_2@f$
  kN2,              //!< @f$N_2@f$
  kN2P,             //!< @f$N2_P@f$
  kN4,              //!< @f$N_4@f$
  kNK4,             //!< @f$NK_4@f$
  kNKM2,            //!< @f$NKM_2@f$
  kNu2,             //!< @f$\nu_2@f$
  kO1,              //!< @f$O_1@f$
  kOO1,             //!< @f$OO_1@f$
  kOQ2,             //!< @f$OQ_2@f$
  kP1,              //!< @f$P_1@f$
  kPhi1,            //!< @f$\phi_1@f$
  kPi1,             //!< @f$\pi_1@f$
  kPsi1,            //!< @f$\psi_1@f$
  kQ1,              //!< @f$Q_1@f$
  kR2,              //!< @f$R_2@f$
  kR4,              //!< @f$R_4@f$
  kRho1,            //!< @f$\rho_1@f$
  kS1,              //!< @f$S_1@f$
  kS2,              //!< @f$S_2@f$
  kS4,              //!< @f$S_4@f$
  kS6,              //!< @f$S_6@f$
  kSa,              //!< @f$Sa@f$
  kSa1,             //!< @f$Sa_1@f$
  kSigma1,          //!< @f$\sigma_1@f$
  kSK3,             //!< @f$SK_3@f$
  kSK4,             //!< @f$SK_4@f$
  kSKM2,            //!< @f$SKM_2@f$
  kSN4,             //!< @f$SN_4@f$
  kSO1,             //!< @f$SO_1@f$
  kSO3,             //!< @f$SO_3@f$
  kSsa,             //!< @f$Ssa@f$
  kSta,             //!< @f$Sta@f$
  kT2,              //!< @f$T_2@f$
  kTheta1,          //!< @f$\theta_1@f$
  kNumConstituents  //!< Number of tidal constituents.
};

/// @brief Number of items in the Constituent enum.
constexpr std::size_t kNumConstituentItems =
    static_cast<std::size_t>(kNumConstituents);
namespace constituents {

/// @brief Returns all the tidal constituents handled by the library.
/// @return Tidal constituents.
constexpr auto kAll = std::array<Constituent, kNumConstituentItems>{
    kMm,   kMf,   kMtm,   kMSqm,   k2Q1,   kSigma1, kQ1,      kRho1,  kO1,
    kMP1,  kM1,   kM11,   kM12,    kM13,   kChi1,   kPi1,     kP1,    kS1,
    kK1,   kPsi1, kPhi1,  kTheta1, kJ1,    kOO1,    kMNS2,    kEps2,  k2N2,
    kMu2,  k2MS2, kN2,    kNu2,    kM2,    kMKS2,   kLambda2, kL2,    k2MN2,
    kT2,   kS2,   kR2,    kK2,     kMSN2,  kEta2,   k2SM2,    kMO3,   k2MK3,
    kM3,   kMK3,  kN4,    kMN4,    kM4,    kSN4,    kMS4,     kMK4,   kS4,
    kSK4,  kR4,   k2MN6,  kM6,     kMSN6,  k2MS6,   k2MK6,    k2SM6,  kMSK6,
    kS6,   kM8,   kMSf,   kSsa,    kSa,    kSa1,    kSta,     kMm1,   kMf1,
    kA5,   kM0,   kMm2,   kMf2,    kL2P,   kN2P,    kMSK2,    kSKM2,  kOQ2,
    k3MS4, kMNu4, k2MSN4, k2NS2,   kMNuS2, k2MK2,   kNKM2,    kML4,   kSO1,
    kSO3,  kNK4,  kMNK6,  k2NM6,   k3MS8,  kSK3,    k2MNS4,   k2SMu2, k2MP5};

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
auto name(Constituent constituent) -> const char* const;

/// @brief Get the tidal constituent names handled by the library.
/// @return Tidal constituent names.
auto known() -> std::array<std::string, kNumConstituentItems>;

/// @brief Create and return an EnumMapper for the Constituent enum.
/// @return EnumMapper for Constituent.
auto map() -> const ConstituentMap&;

}  // namespace constituents
}  // namespace darwin
}  // namespace fes
