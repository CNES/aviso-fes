// Copyright (c) 2026 CNES
//
// All rights reserved. Use of this source code is governed by a
// BSD-style license that can be found in the LICENSE file.
/// @file include/fes/perth/wave.hpp
/// @brief Tidal constituent properties
#pragma once

#include <memory>
#include <string>

#include "fes/constituent.hpp"
#include "fes/interface/wave.hpp"
#include "fes/perth/doodson.hpp"
#include "fes/types.hpp"
#include "fes/xdo.hpp"

namespace fes {
namespace perth {

class Wave : public WaveInterface {
 public:
  Wave(const ConstituentId ident, WaveType type,
       Vector7b doodson_numbers) noexcept
      : WaveInterface(ident, type),
        doodson_numbers_(std::move(doodson_numbers)),
        freq_(detail::math::radians(tidal_frequency(doodson_numbers_))) {}

  /// @brief Gets the frequency in radians per hour.
  /// @return The frequency in radians per hour.
  auto frequency() const noexcept -> double final { return freq_; }

  /// @brief Clones the wave.
  /// @return A unique pointer to the cloned wave.
  inline auto clone() const -> std::unique_ptr<WaveInterface> final {
    return std::make_unique<Wave>(*this);
  }

  /// @brief Computes the nodal corrections for the wave.
  /// @param[in] angles Astronomical angles used to compute nodal corrections.
  /// @param[in] group_modulations If true, applies group modulations to nodal
  /// corrections.
  auto compute_nodal_corrections(const angle::Astronomic& angles,
                                 const bool group_modulations) -> void final;

  /// Gets the XDO numerical representation of the wave
  auto xdo_numerical() const -> std::string final {
    return fes::xdo_numerical(doodson_numbers_);
  }

  /// Gets the XDO alphabetical representation of the wave
  auto xdo_alphabetical() const -> std::string final {
    return fes::xdo_alphabetical(doodson_numbers_);
  }

  /// Gets the Doodson number of the wave
  /// @note The 7th number follows the convention established in Doodson &
  /// Warburg's 1941 book. This number can be 0, 1, 2, or -1, representing
  /// multiples of 90 degrees added to the tidal argument when using cosine
  /// functions
  auto doodson_numbers() const -> Vector7b final { return doodson_numbers_; }

 private:
  Vector7b doodson_numbers_;  //!< Doodson number of the constituent
  double freq_;               //!< Frequency in radians per hour
};

namespace wave {

/// @brief Node
class Node : public Wave {
 public:
  Node();
};

/// @brief @f$Sa_1@f$
class Sa1 : public Wave {
 public:
  Sa1();
};

/// @brief @f$Sa@f$
class Sa : public Wave {
 public:
  Sa();
};

/// @brief @f$Ssa@f$
class Ssa : public Wave {
 public:
  Ssa();
};

/// @brief @f$Sta@f$
class Sta : public Wave {
 public:
  Sta();
};

/// @brief @f$MSm@f$
class MSm : public Wave {
 public:
  MSm();
};

/// @brief @f$Mm@f$
class Mm : public Wave {
 public:
  Mm();
};

/// @brief @f$MSf@f$
class MSf : public Wave {
 public:
  MSf();
};

/// @brief @f$Mf@f$
class Mf : public Wave {
 public:
  Mf();
};

/// @brief @f$MStm@f$
class MStm : public Wave {
 public:
  MStm();
};

/// @brief @f$Mtm@f$
class Mtm : public Wave {
 public:
  Mtm();
};

/// @brief @f$MSqm@f$
class MSqm : public Wave {
 public:
  MSqm();
};

/// @brief @f$Mqm@f$
class Mqm : public Wave {
 public:
  Mqm();
};

/// @brief @f$2Q_1@f$
class _2Q1 : public Wave {
 public:
  _2Q1();
};

/// @brief @f$\sigma_1@f$
class Sigma1 : public Wave {
 public:
  Sigma1();
};

/// @brief @f$Q_1@f$
class Q1 : public Wave {
 public:
  Q1();
};

/// @brief @f$\rho_1@f$
class Rho1 : public Wave {
 public:
  Rho1();
};

/// @brief @f$O_1@f$
class O1 : public Wave {
 public:
  O1();
};

/// @brief @f$\tau_1@f$
class Tau1 : public Wave {
 public:
  Tau1();
};

/// @brief @f$\beta_1@f$
class Beta1 : public Wave {
 public:
  Beta1();
};

/// @brief @f$M_{13}@f$
class M13 : public Wave {
 public:
  M13();
};

/// @brief @f$M_1@f$
class M1 : public Wave {
 public:
  M1();
};

/// @brief @f$\chi_1@f$
class Chi1 : public Wave {
 public:
  Chi1();
};

/// @brief @f$\pi_1@f$
class Pi1 : public Wave {
 public:
  Pi1();
};

/// @brief @f$P_1@f$
class P1 : public Wave {
 public:
  P1();
};

/// @brief @f$S_1@f$
class S1 : public Wave {
 public:
  S1();
};

/// @brief @f$K_1@f$
class K1 : public Wave {
 public:
  K1();
};

/// @brief @f$\psi_1@f$
class Psi1 : public Wave {
 public:
  Psi1();
};

/// @brief @f$\phi_1@f$
class Phi1 : public Wave {
 public:
  Phi1();
};

/// @brief @f$\theta_1@f$
class Theta1 : public Wave {
 public:
  Theta1();
};

/// @brief @f$J_1@f$
class J1 : public Wave {
 public:
  J1();
};

/// @brief @f$SO_1@f$
class SO1 : public Wave {
 public:
  SO1();
};

/// @brief @f$OO_1@f$
class OO1 : public Wave {
 public:
  OO1();
};

/// @brief @f$\Upsilon_1@f$
class Ups1 : public Wave {
 public:
  Ups1();
};

/// @brief @f$\epsilon_2@f$
class Eps2 : public Wave {
 public:
  Eps2();
};

/// @brief @f$2N_2@f$
class _2N2 : public Wave {
 public:
  _2N2();
};

/// @brief @f$2MS_2@f$
class _2MS2 : public Wave {
 public:
  _2MS2();
};

/// @brief @f$\mu_2@f$
class Mu2 : public Wave {
 public:
  Mu2();
};

/// @brief @f$N_2@f$
class N2 : public Wave {
 public:
  N2();
};

/// @brief @f$N2_P@f$
class N2P : public Wave {
 public:
  N2P();
};

/// @brief @f$\nu_2@f$
class Nu2 : public Wave {
 public:
  Nu2();
};

/// @brief @f$\gamma_2@f$
class Gamma2 : public Wave {
 public:
  Gamma2();
};

/// @brief @f$\alpha_2@f$
class Alpha2 : public Wave {
 public:
  Alpha2();
};

/// @brief @f$M_2@f$
class M2 : public Wave {
 public:
  M2();
};

/// @brief @f$\beta_2@f$
class Beta2 : public Wave {
 public:
  Beta2();
};

/// @brief @f$\delta_2@f$
class Delta2 : public Wave {
 public:
  Delta2();
};

/// @brief @f$MKS_2@f$
class MKS2 : public Wave {
 public:
  MKS2();
};

/// @brief @f$\lambda_2@f$
class Lambda2 : public Wave {
 public:
  Lambda2();
};

/// @brief @f$2MN_2@f$
class _2MN2 : public Wave {
 public:
  _2MN2();
};

/// @brief @f$L_2@f$
class L2 : public Wave {
 public:
  L2();
};

/// @brief @f$L2_P@f$
class L2P : public Wave {
 public:
  L2P();
};

/// @brief @f$T_2@f$
class T2 : public Wave {
 public:
  T2();
};

/// @brief @f$S_2@f$
class S2 : public Wave {
 public:
  S2();
};

/// @brief @f$R_2@f$
class R2 : public Wave {
 public:
  R2();
};

/// @brief @f$K_2@f$
class K2 : public Wave {
 public:
  K2();
};

/// @brief @f$MSN_2@f$
class MSN2 : public Wave {
 public:
  MSN2();
};

/// @brief @f$\eta_2@f$
class Eta2 : public Wave {
 public:
  Eta2();
};

/// @brief @f$2SM_2@f$
class _2SM2 : public Wave {
 public:
  _2SM2();
};

/// @brief @f$2MK_3@f$
class _2MK3 : public Wave {
 public:
  _2MK3();
};

/// @brief @f$MO_3@f$
class MO3 : public Wave {
 public:
  MO3();
};

/// @brief @f$M_3@f$
class M3 : public Wave {
 public:
  M3();
};

/// @brief @f$MK_3@f$
class MK3 : public Wave {
 public:
  MK3();
};

/// @brief @f$N_4@f$
class N4 : public Wave {
 public:
  N4();
};

/// @brief @f$MN_4@f$
class MN4 : public Wave {
 public:
  MN4();
};

/// @brief @f$M_4@f$
class M4 : public Wave {
 public:
  M4();
};

/// @brief @f$SN_4@f$
class SN4 : public Wave {
 public:
  SN4();
};

/// @brief @f$MS_4@f$
class MS4 : public Wave {
 public:
  MS4();
};

/// @brief @f$MK_4@f$
class MK4 : public Wave {
 public:
  MK4();
};

/// @brief @f$S_4@f$
class S4 : public Wave {
 public:
  S4();
};

/// @brief @f$R_4@f$
class R4 : public Wave {
 public:
  R4();
};

/// @brief @f$SK_4@f$
class SK4 : public Wave {
 public:
  SK4();
};

/// @brief @f$2MN_6@f$
class _2MN6 : public Wave {
 public:
  _2MN6();
};

/// @brief @f$M_6@f$
class M6 : public Wave {
 public:
  M6();
};

/// @brief @f$MSN_6@f$
class MSN6 : public Wave {
 public:
  MSN6();
};

/// @brief @f$2MS_6@f$
class _2MS6 : public Wave {
 public:
  _2MS6();
};

/// @brief @f$2MK_6@f$
class _2MK6 : public Wave {
 public:
  _2MK6();
};

/// @brief @f$2SM_6@f$
class _2SM6 : public Wave {
 public:
  _2SM6();
};

/// @brief @f$MSK_6@f$
class MSK6 : public Wave {
 public:
  MSK6();
};

/// @brief @f$S_6@f$
class S6 : public Wave {
 public:
  S6();
};

/// @brief @f$M_8@f$
class M8 : public Wave {
 public:
  M8();
};

}  // namespace wave
}  // namespace perth
}  // namespace fes
