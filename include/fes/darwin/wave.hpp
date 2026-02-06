// Copyright (c) 2026 CNES
//
// All rights reserved. Use of this source code is governed by a
// BSD-style license that can be found in the LICENSE file.
/// @file include/fes/darwin/wave.hpp
/// @brief Tidal constituent properties
#pragma once

#include <array>
#include <memory>
#include <string>

#include "fes/constituent.hpp"
#include "fes/darwin.hpp"
#include "fes/detail/angle/astronomic/frequency.hpp"
#include "fes/detail/math.hpp"
#include "fes/interface/wave.hpp"
#include "fes/numbers.hpp"
#include "fes/xdo.hpp"

namespace fes {
namespace darwin {

class Wave : public WaveInterface {
 public:
  /// Typename to a function pointer for calculate the nodal factor
  using nodal_factor_t = double (angle::Astronomic::*)() const;

  /// @brief  Initializes the properties of the wave (frequency, doodson's
  /// coefficients, etc.).
  /// @param[in] ident Tidal constituent identifier.
  /// @param[in] type Type of tidal wave
  /// @param[in] darwin Darwin parameters for the wave
  /// @param[in] calculate_node_factor Function used to calculate the nodal
  /// factor
  Wave(const ConstituentId ident, WaveType type, const Darwin& darwin,
       nodal_factor_t calculate_node_factor) noexcept
      : WaveInterface(ident, type),
        calculate_node_factor_(calculate_node_factor),
        argument_({darwin.t, darwin.s, darwin.h, darwin.p, darwin.n, darwin.p1,
                   darwin.shift, darwin.eps, darwin.nu, darwin.nuprim,
                   darwin.nusec}),
        freq_(detail::math::radians(frequency(
            darwin.t, darwin.s, darwin.h, darwin.p, darwin.n, darwin.p1))) {}

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
  inline auto compute_nodal_corrections(const angle::Astronomic& angles,
                                        const bool group_modulations)
      -> void final {
    nodal_a(angles);
    nodal_g(angles);
  }

  /// Gets the XDO numerical representation of the wave
  auto xdo_numerical() const -> std::string final {
    return fes::xdo_numerical(doodson_numbers());
  }

  /// Gets the XDO alphabetical representation of the wave
  auto xdo_alphabetical() const -> std::string final {
    return fes::xdo_alphabetical(doodson_numbers());
  }

  /// Gets the Doodson number of the wave
  /// @note The 7th number follows the convention established in Doodson &
  /// Warburg's 1941 book. This number can be 0, 1, 2, or -1, representing
  /// multiples of 90 degrees added to the tidal argument when using cosine
  /// functions
  auto doodson_numbers() const -> Vector7b final;

 protected:
  /// Compute nodal corrections from SCHUREMAN (1958).
  ///
  /// @param[in] a Astronomic angle
  virtual inline void nodal_g(const angle::Astronomic& a) {
    v_ = argument_[0] * a.t() + argument_[1] * a.s() + argument_[2] * a.h() +
         argument_[3] * a.p() + argument_[5] * a.p1() +
         argument_[6] * detail::math::pi_2<double>();
    u_ = argument_[7] * a.xi() + argument_[8] * a.nu() +
         argument_[9] * a.nuprim() + argument_[10] * a.nusec();
  }

 private:
  /// Function to call for computing the node factor
  nodal_factor_t calculate_node_factor_;

  /// Harmonic constituents
  /// \f$(T, s, h, p, N', p_1, shift, \xi, \nu, \nu', \nu'')\f$
  std::array<int8_t, 11> argument_;

  /// Wave frequency in radians per hour
  double freq_;

  /// Computes the wave frequency from the doodson arguments
  ///
  /// @param[in] t Mean solar angle relative to Greenwich
  /// @param[in] s moon's mean longitude
  /// @param[in] h sun's mean longitude
  /// @param[in] p longitude of the moon's perigee
  /// @param[in] n longitude of the moon's ascending node
  /// @param[in] p1 longitude of sun's perigee
  /// @return The frequency of the wave, expressed in degrees per hour.
  static constexpr auto frequency(const int8_t t, const int8_t s,
                                  const int8_t h, const int8_t p,
                                  const int8_t n, const int8_t p1) -> double {
    namespace frequency = detail::angle::astronomic::frequency;

    return ((frequency::tau() + frequency::s() - frequency::h()) * t +
            frequency::s() * s + frequency::h() * h + frequency::p() * p +
            frequency::n() * n + frequency::p1() * p1) *
           360;
  }

  /// Compute nodal corrections from SCHUREMAN (1958).
  ///
  /// @param[in] a Astronomic angle
  constexpr void nodal_a(const angle::Astronomic& a) {
    f_ = (a.*calculate_node_factor_)();
  }
};

namespace wave {

/// @brief @f$Mm@f$
///
/// <table>
/// <caption id="multi_row">Mm</caption>
/// <tr><th>V</th><th>u</th><th>Factor-f</th></tr>
/// <tr><td>@f$s - p@f$</td>
/// <td>@f$0@f$</td>
/// <td>@f$f(Mm)@f$</td></tr>
/// </table>
/// @note Schureman: %Table 2, Page 164, Ref. A2
class Mm : public Wave {
 public:
  Mm();
};

/// @brief @f$Mf@f$
///
/// <table>
/// <tr><th>V</th><th>u</th><th>Factor-f</th></tr>
/// <tr><td>@f$2s@f$</td>
/// <td>@f$-2\xi@f$</td>
/// <td>@f$f(Mf)@f$</td></tr>
/// </table>
/// @note Schureman: %Table 2, Page 164, Ref. A6
class Mf : public Wave {
 public:
  Mf();
};

/// @brief @f$Mtm@f$
///
/// <table>
/// <tr><th>V</th><th>u</th><th>Factor-f</th></tr>
/// <tr><td>@f$3s - p@f$</td>
/// <td>@f$-2\xi@f$</td>
/// <td>@f$f(Mf)@f$</td></tr>
/// </table>
/// @note Schureman: %Table 2, Page 164, Ref. A7
class Mtm : public Wave {
 public:
  Mtm();
};

/// @brief @f$MSqm@f$
///
/// <table>
/// <tr><th>V</th><th>u</th><th>Factor-f</th></tr>
/// <tr><td>@f$4s - 2h@f$</td>
/// <td>@f$-2\xi@f$</td>
/// <td>@f$f(Mf)@f$</td></tr>
/// </table>
/// @note Schureman: %Table 2, Page 164, Ref. A12
class MSqm : public Wave {
 public:
  MSqm();
};

/// @brief @f$Ssa@f$
///
/// <table>
/// <tr><th>V</th><th>u</th><th>Factor-f</th></tr>
/// <tr><td>@f$2h@f$</td>
/// <td>@f$0@f$</td>
/// <td>@f$1@f$</td></tr>
/// </table>
/// @note Schureman: %Table 2, Page 164, Ref. B6
class Ssa : public Wave {
 public:
  Ssa();
};

/// @brief @f$Sa@f$
///
/// <table>
/// <tr><th>V</th><th>u</th><th>Factor-f</th></tr>
/// <tr><td>@f$h@f$</td>
/// <td>@f$0@f$</td>
/// <td>@f$1@f$</td></tr>
/// </table>
/// @note Schureman: %Table 2, Page 164, Ref. B64
class Sa : public Wave {
 public:
  Sa();
};

/// @brief @f$2Q_1@f$
///
/// <table>
/// <tr><th>V</th><th>u</th><th>Factor-f</th></tr>
/// <tr><td>@f$T - 4s + h + 2p + 90^{\circ}@f$</td>
/// <td>@f$+2\xi - \nu@f$</td>
/// <td>@f$f(O_1)@f$</td></tr>
/// </table>
/// @note Schureman: %Table 2, Page 164, Ref. A17
class _2Q1 : public Wave {
 public:
  _2Q1();
};

/// @brief @f$\sigma_1@f$
///
/// <table>
/// <tr><th>V</th><th>u</th><th>Factor-f</th></tr>
/// <tr><td>@f$T - 4s + 3h + 90^{\circ}@f$</td>
/// <td>@f$+2\xi - \nu@f$</td>
/// <td>@f$f(O_1)@f$</td></tr>
/// </table>
/// @note Schureman: %Table 2, Page 164, Ref. A20
class Sigma1 : public Wave {
 public:
  Sigma1();
};

/// @brief @f$Q_1@f$
///
/// <table>
/// <tr><th>V</th><th>u</th><th>Factor-f</th></tr>
/// <tr><td>@f$T - 3s + h + p + 90^{\circ}@f$</td>
/// <td>@f$+2\xi - \nu@f$</td>
/// <td>@f$f(O_1)@f$</td></tr>
/// </table>
/// @note Schureman: %Table 2, Page 164, Ref. A15
class Q1 : public Wave {
 public:
  Q1();
};

/// @brief @f$\rho_1@f$
///
/// <table>
/// <tr><th>V</th><th>u</th><th>Factor-f</th></tr>
/// <tr><td>@f$T - 3s + 3h - p + 90^{\circ}@f$</td>
/// <td>@f$+2\xi - \nu@f$</td>
/// <td>@f$f(O_1)@f$</td></tr>
/// </table>
/// @note Schureman: %Table 2, Page 164, Ref. A18
class Rho1 : public Wave {
 public:
  Rho1();
};

/// @brief @f$O_1@f$
///
/// <table>
/// <tr><th>V</th><th>u</th><th>Factor-f</th></tr>
/// <tr><td>@f$T - 2s + h + 90^{\circ}@f$</td>
/// <td>@f$+2\xi - \nu@f$</td>
/// <td>@f$f(O_1)@f$</td></tr>
/// </table>
/// @note Schureman: %Table 2, Page 164, Ref. A14
class O1 : public Wave {
 public:
  O1();
};

/// @brief @f$MP_1@f$
///
/// <table>
/// <tr><th>V</th><th>u</th><th>Factor-f</th></tr>
/// <tr><td>@f$T - 2s + 3h - 90^{\circ}@f$</td>
/// <td>@f$-\nu@f$</td>
/// <td>@f$f(J_1)@f$</td></tr>
/// </table>
/// @note Schureman: %Table 2, Page 164, Ref. A29
class MP1 : public Wave {
 public:
  MP1();
};

/// @brief @f$M1@f$ (= @f$M_{11} + M_{12}@f$)
///
/// <table>
/// <tr><th>V</th><th>u</th><th>Factor-f</th></tr>
/// <tr><td>@f$T - s + h + p - 90@f$</td>
/// <td>@f$-\nu@f$</td>
/// <td>@f$f(M_1)@f$</td></tr>
/// </table>
class M1 : public Wave {
 public:
  M1();

  /// Compute nodal corrections from SCHUREMAN (1958).
  ///
  /// @param a Astronomic angle
  inline void nodal_g(const angle::Astronomic& a) final {
    Wave::nodal_g(a);
    u_ -= detail::math::radians(
        1.0 / std::sqrt(numbers::k197_1 +
                        numbers::k197_2 * std::cos(2 * (a.p() - a.xi()))));
  }
};

/// @brief @f$M1_{1}@f$
///
/// <table>
/// <tr><th>V</th><th>u</th><th>Factor-f</th></tr>
/// <tr><td>@f$T - s + h - p - 90^{\circ}@f$</td>
/// <td>@f$+2\xi - \nu@f$</td>
/// <td>@f$f(O_1)@f$</td></tr>
/// </table>
/// @note Schureman: %Table 2, Page 164, Ref. A16
class M11 : public Wave {
 public:
  M11();
};

/// @brief @f$M1_{2}@f$
///
/// <table>
/// <tr><th>V</th><th>u</th><th>Factor-f</th></tr>
/// <tr><td>@f$T - s + h + p - 90^{\circ}@f$</td>
/// <td>@f$-\nu@f$</td>
/// <td>@f$f(J_1)@f$</td></tr>
/// </table>
/// @note Schureman: %Table 2, Page 164, Ref. A23
class M12 : public Wave {
 public:
  M12();
};

/// @brief @f$M1_{3}@f$
///
/// <table>
/// <tr><th>V</th><th>u</th><th>Factor-f</th></tr>
/// <tr><td>@f$T - s + h@f$</td>
/// <td>@f$\xi -\nu@f$</td>
/// <td>@f$f(144)@f$</td></tr>
/// </table>
/// @note Schureman: %Table 2, Page 165, Ref. A71
class M13 : public Wave {
 public:
  M13();
};

/// @brief @f$\chi_1@f$
///
/// <table>
/// <tr><th>V</th><th>u</th><th>Factor-f</th></tr>
/// <tr><td>@f$T - s + 3h - p - 90^{\circ}@f$</td>
/// <td>@f$-\nu@f$</td>
/// <td>@f$f(J_1)@f$</td></tr>
/// </table>
/// @note Schureman: %Table 2, Page 164, Ref. A27
class Chi1 : public Wave {
 public:
  Chi1();
};

/// @brief @f$\pi_1@f$
///
/// <table>
/// <tr><th>V</th><th>u</th><th>Factor-f</th></tr>
/// <tr><td>@f$T - 2h + p1 + 90^{\circ}@f$</td>
/// <td>@f$0@f$</td>
/// <td>@f$1@f$</td></tr>
/// </table>
/// @note Schureman: %Table 2, Page 165, Ref. B15
class Pi1 : public Wave {
 public:
  Pi1();
};

/// @brief @f$P_1@f$
///
/// <table>
/// <tr><th>V</th><th>u</th><th>Factor-f</th></tr>
/// <tr><td>@f$T - h + 90^{\circ}@f$</td>
/// <td>@f$0@f$</td>
/// <td>@f$1@f$</td></tr>
/// </table>
/// @note Schureman: %Table 2, Page 165, Ref. B14
class P1 : public Wave {
 public:
  P1();
};

/// @brief @f$S_1@f$
///
/// <table>
/// <tr><th>V</th><th>u</th><th>Factor-f</th></tr>
/// <tr><td>@f$T@f$</td>
/// <td>@f$0@f$</td>
/// <td>@f$1@f$</td></tr>
/// </table>
/// @note Schureman: %Table 2, Page 165, Ref. B71
class S1 : public Wave {
 public:
  S1();
};

/// @brief @f$K_1@f$
///
/// <table>
/// <tr><th>V</th><th>u</th><th>Factor-f</th></tr>
/// <tr><td>@f$T + h - 90^{\circ}@f$</td>
/// <td>@f$- \nu^{\prime}@f$</td>
/// <td>@f$f(K_1)@f$</td></tr>
/// </table>
/// @note Schureman: %Table 2, Page 165, Ref. Note 2
class K1 : public Wave {
 public:
  K1();
};

/// @brief @f$\psi_1@f$
///
/// <table>
/// <tr><th>V</th><th>u</th><th>Factor-f</th></tr>
/// <tr><td>@f$T + 2h - p1 - 90^{\circ}@f$</td>
/// <td>@f$0@f$</td>
/// <td>@f$1@f$</td></tr>
/// </table>
/// @note Schureman: %Table 2, Page 165, Ref. B24
class Psi1 : public Wave {
 public:
  Psi1();
};

/// @brief @f$\phi_1@f$
///
/// <table>
/// <tr><th>V</th><th>u</th><th>Factor-f</th></tr>
/// <tr><td>@f$T + 3h - 90^{\circ}@f$</td>
/// <td>@f$0@f$</td>
/// <td>@f$1@f$</td></tr>
/// </table>
/// @note Schureman: %Table 2, Page 165, Ref. B31
class Phi1 : public Wave {
 public:
  Phi1();
};

/// @brief @f$\theta_1@f$
///
/// <table>
/// <tr><th>V</th><th>u</th><th>Factor-f</th></tr>
/// <tr><td>@f$T + s - h + p - 90^{\circ}@f$</td>
/// <td>@f$-\nu@f$</td>
/// <td>@f$f(J_1)@f$</td></tr>
/// </table>
/// @note Schureman: %Table 2, Page 164, Ref. A28
class Theta1 : public Wave {
 public:
  Theta1();
};

/// @brief @f$J_1@f$
///
/// <table>
/// <tr><th>V</th><th>u</th><th>Factor-f</th></tr>
/// <tr><td>@f$T + s + h - p - 90^{\circ}@f$</td>
/// <td>@f$-\nu@f$</td>
/// <td>@f$f(J_1)@f$</td></tr>
/// </table>
/// @note Schureman: %Table 2, Page 164, Ref. A24
class J1 : public Wave {
 public:
  J1();
};

/// @brief @f$OO_1@f$
///
/// <table>
/// <tr><th>V</th><th>u</th><th>Factor-f</th></tr>
/// <tr><td>@f$T + 2s + h - 90^{\circ}@f$</td>
/// <td>@f$-2\xi - \nu@f$</td>
/// <td>@f$f(OO_1)@f$</td></tr>
/// </table>
/// @note Schureman: %Table 2, Page 164, Ref. A31
class OO1 : public Wave {
 public:
  OO1();
};

/// @brief @f$2MNS_2 = M_2 + N_2 + S_2@f$
///
/// <table>
/// <tr><th>V</th><th>u</th><th>Factor-f</th></tr>
/// <tr><td>@f$2T - 5s + 4h + p@f$</td>
/// <td>@f$+4\xi - 4\nu@f$</td>
/// <td>@f$f(M_2)^2@f$</td></tr>
/// </table>
/// @note Schureman: %Table 2, Page 167, Ref. MNS2
class MNS2 : public Wave {
 public:
  MNS2();
};

/// @brief @f$\varepsilon_2@f$
///
/// <table>
/// <tr><th>V</th><th>u</th><th>Factor-f</th></tr>
/// <tr><td>@f$2T - 5s + 4h + p@f$</td>
/// <td>@f$+2\xi - 2\nu@f$</td>
/// <td>@f$f(M_2)@f$</td></tr>
/// </table>
class Eps2 : public Wave {
 public:
  Eps2();
};

/// @brief @f$2N_2@f$
///
/// <table>
/// <tr><th>V</th><th>u</th><th>Factor-f</th></tr>
/// <tr><td>@f$2T - 4s + 2h + 2p@f$</td>
/// <td>@f$+2\xi - 2\nu@f$</td>
/// <td>@f$f(M_2)@f$</td></tr>
/// </table>
/// @note Schureman: %Table 2, Page 165, Ref. A42
class _2N2 : public Wave {
 public:
  _2N2();
};

/// @brief @f$\mu_2@f$
///
/// <table>
/// <tr><th>V</th><th>u</th><th>Factor-f</th></tr>
/// <tr><td>@f$2T - 4s + 4h@f$</td>
/// <td>@f$+2\xi - 2\nu@f$</td>
/// <td>@f$f(M_2)@f$</td></tr>
/// </table>
/// @note Schureman: %Table 2, Page 165, Ref. A45
class Mu2 : public Wave {
 public:
  Mu2();
};

/// @brief @f$2MS_2 = 2M_2 - S_2@f$
///
/// <table>
/// <tr><th>V</th><th>u</th><th>Factor-f</th></tr>
/// <tr><td>@f$2T - 4s + 4h@f$</td>
/// <td>@f$+4\xi - 4\nu@f$</td>
/// <td>@f$f(M_2)^2@f$</td></tr>
/// </table>
class _2MS2 : public Wave {
 public:
  _2MS2();
};

/// @brief @f$N_2@f$
///
/// <table>
/// <tr><th>V</th><th>u</th><th>Factor-f</th></tr>
/// <tr><td>@f$2T - 3s + 2h + p@f$</td>
/// <td>@f$+2\xi - 2\nu@f$</td>
/// <td>@f$f(M_2)@f$</td></tr>
/// </table>
/// @note Schureman: %Table 2, Page 165, Ref. A40
class N2 : public Wave {
 public:
  N2();
};

/// @brief @f$\nu_2@f$
///
/// <table>
/// <tr><th>V</th><th>u</th><th>Factor-f</th></tr>
/// <tr><td>@f$2T - 3s + 4h - p@f$</td>
/// <td>@f$+2\xi - 2\nu@f$</td>
/// <td>@f$f(M_2)@f$</td></tr>
/// </table>
/// @note Schureman: %Table 2, Page 165, Ref. A43
class Nu2 : public Wave {
 public:
  Nu2();
};

/// @brief @f$M_2@f$
///
/// <table>
/// <tr><th>V</th><th>u</th><th>Factor-f</th></tr>
/// <tr><td>@f$2T - 2s + 2h@f$</td>
/// <td>@f$+2\xi - 2\nu@f$</td>
/// <td>@f$f(M_2)@f$</td></tr>
/// </table>
/// @note Schureman: %Table 2, Page 165, Ref. A39
class M2 : public Wave {
 public:
  M2();
};

/// @brief @f$MKS_2 = M_2 + K_2 - S_2@f$
///
/// <table>
/// <tr><th>V</th><th>u</th><th>Factor-f</th></tr>
/// <tr><td>@f$2T - 2s + 4h@f$</td>
/// <td>@f$+2\xi - 2\nu -2\nu^{\prime \prime}@f$</td>
/// <td>@f$f(M_2) \times f(K_2)@f$</td></tr>
/// </table>
class MKS2 : public Wave {
 public:
  MKS2();
};

/// @brief @f$\lambda_2@f$
///
/// <table>
/// <tr><th>V</th><th>u</th><th>Factor-f</th></tr>
/// <tr><td>@f$2T - s + p + 180^{\circ}@f$</td>
/// <td>@f$+2\xi - 2\nu@f$</td>
/// <td>@f$f(M_2)@f$</td></tr>
/// </table>
/// @note Schureman: %Table 2, Page 165, Ref. A44
class Lambda2 : public Wave {
 public:
  Lambda2();
};

/// @brief @f$L_2@f$
///
/// <table>
/// <tr><th>V</th><th>u</th><th>Factor-f</th></tr>
/// <tr><td>@f$2T - s + 2h - p + 180^{\circ}@f$</td>
/// <td>@f$+2\xi - 2\nu - R@f$</td>
/// <td>@f$f(L_2)@f$</td></tr>
/// </table>
/// @note Schureman: %Table 2, Page 166, Ref. Note 3
class L2 : public Wave {
 public:
  L2();

  /// Compute nodal corrections from SCHUREMAN (1958).
  ///
  /// @param a Astronomic angle
  inline void nodal_g(const angle::Astronomic& a) final {
    Wave::nodal_g(a);
    u_ -= a.r();
  }
};

/// @brief @f$2MN_2 = 2M_2 - N_2@f$
///
/// <table>
/// <tr><th>V</th><th>u</th><th>Factor-f</th></tr>
/// <tr><td>@f$2T - s + 2h - p + 180^{\circ}@f$</td>
/// <td>@f$+2\xi - 2\nu@f$</td>
/// <td>@f$f(M_2)^3@f$</td></tr>
/// </table>
class _2MN2 : public Wave {
 public:
  _2MN2();
};

/// @brief @f$T_2@f$
///
/// <table>
/// <tr><th>V</th><th>u</th><th>Factor-f</th></tr>
/// <tr><td>@f$2T - h + p_1@f$</td>
/// <td>@f$0@f$</td>
/// <td>@f$1@f$</td></tr>
/// </table>
/// @note Schureman: %Table 2, Page 166, Ref. B40
class T2 : public Wave {
 public:
  T2();
};

/// @brief @f$S_2@f$
///
/// <table>
/// <tr><th>V</th><th>u</th><th>Factor-f</th></tr>
/// <tr><td>@f$2T@f$</td>
/// <td>@f$0@f$</td>
/// <td>@f$1@f$</td></tr>
/// </table>
/// @note Schureman: %Table 2, Page 166, Ref. B39
class S2 : public Wave {
 public:
  S2();
};

/// @brief @f$R_2@f$
///
/// <table>
/// <tr><th>V</th><th>u</th><th>Factor-f</th></tr>
/// <tr><td>@f$2T + h - p1 + 180^{\circ}@f$</td>
/// <td>@f$0@f$</td>
/// <td>@f$1@f$</td></tr>
/// </table>
/// @note Schureman: %Table 2, Page 166, Ref. B41
class R2 : public Wave {
 public:
  R2();
};

/// @brief @f$K_2@f$
///
/// <table>
/// <tr><th>V</th><th>u</th><th>Factor-f</th></tr>
/// <tr><td>@f$2T + 2h@f$</td>
/// <td>@f$-2\nu^{\prime \prime}@f$</td>
/// <td>@f$f(K_2)@f$</td></tr>
/// </table>
/// @note Schureman: %Table 2, Page 166, Ref. Note 4
class K2 : public Wave {
 public:
  K2();
};

/// @brief @f$MSN_2 = M_2 + S_2 - N_2@f$
///
/// <table>
/// <tr><th>V</th><th>u</th><th>Factor-f</th></tr>
/// <tr><td>@f$2T + s -p@f$</td>
/// <td>@f$0@f$</td>
/// <td>@f$f(M_2)^2@f$</td></tr>
/// </table>
class MSN2 : public Wave {
 public:
  MSN2();
};

/// @brief @f$\eta_2 = KJ_2@f$
///
/// <table>
/// <tr><th>V</th><th>u</th><th>Factor-f</th></tr>
/// <tr><td>@f$2T + s + 2h - p@f$</td>
/// <td>@f$-2\nu@f$</td>
/// <td>@f$f(KJ_2)@f$</td></tr>
/// </table>
/// @note Schureman: %Table 2, Page 165, Ref. A49
class Eta2 : public Wave {
 public:
  Eta2();
};

/// @brief @f$2SM_2 = 2S_2 - M_2@f$
///
/// <table>
/// <tr><th>V</th><th>u</th><th>Factor-f</th></tr>
/// <tr><td>@f$2T + 2s - 2h@f$</td>
/// <td>@f$-2\xi + 2\nu@f$</td>
/// <td>@f$f(M_2)@f$</td></tr>
/// </table>
class _2SM2 : public Wave {
 public:
  _2SM2();
};

/// @brief @f$MO_3 = M_2 + O_1@f$
///
/// <table>
/// <tr><th>V</th><th>u</th><th>Factor-f</th></tr>
/// <tr><td>@f$3T - 4s + 3h + 90^{\circ}@f$</td>
/// <td>@f$4\xi - 3\nu@f$</td>
/// <td>@f$f(M_2) \times f(O_1)@f$</td></tr>
/// </table>
class MO3 : public Wave {
 public:
  MO3();
};

/// @brief @f$2MK_3 = 2M_2 - K_1@f$
///
/// <table>
/// <tr><th>V</th><th>u</th><th>Factor-f</th></tr>
/// <tr><td>@f$3T - 4s + 3h + 90^{\circ}@f$</td>
/// <td>@f$4\xi - 4\nu + \nu^{\prime}@f$</td>
/// <td>@f$f(M_2)^2 \times f(K_1)@f$</td></tr>
/// </table>
class _2MK3 : public Wave {
 public:
  _2MK3();
};

/// @brief @f$M_3@f$
///
/// <table>
/// <tr><th>V</th><th>u</th><th>Factor-f</th></tr>
/// <tr><td>@f$3T - 3s + 3h@f$</td>
/// <td>@f$+3\xi - 3\nu@f$</td>
/// <td>@f$f(M_3)@f$</td></tr>
/// </table>
/// @note Schureman: %Table 2, Page 166, Ref. A82
class M3 : public Wave {
 public:
  M3();
};

/// @brief @f$MK_3 = M_2 + K_1@f$
///
/// <table>
/// <tr><th>V</th><th>u</th><th>Factor-f</th></tr>
/// <tr><td>@f$3T - 2s + 3h - 90^{\circ}@f$</td>
/// <td>@f$2\xi - 2\nu - \nu^{\prime}@f$</td>
/// <td>@f$f(M_2) \times f(K_1)@f$</td></tr>
/// </table>
class MK3 : public Wave {
 public:
  MK3();
};

/// @brief @f$N_4 = N_2 + N_2@f$
///
/// <table>
/// <tr><th>V</th><th>u</th><th>Factor-f</th></tr>
/// <tr><td>@f$4T - 6s + 4h + 2p@f$</td>
/// <td>@f$+4\xi - 4\nu@f$</td>
/// <td>@f$f(M_2)^2@f$</td></tr>
/// </table>
class N4 : public Wave {
 public:
  N4();
};

/// @brief @f$MN_4 = M_2 + N_2@f$
///
/// <table>
/// <tr><th>V</th><th>u</th><th>Factor-f</th></tr>
/// <tr><td>@f$4T - 5s + 4h + p@f$</td>
/// <td>@f$+4\xi - 4\nu@f$</td>
/// <td>@f$f(M_2)^2@f$</td></tr>
/// </table>
class MN4 : public Wave {
 public:
  MN4();
};

/// @brief @f$M_4 = 2M_2@f$
///
/// <table>
/// <tr><th>V</th><th>u</th><th>Factor-f</th></tr>
/// <tr><td>@f$4T - 4s + 4h@f$</td>
/// <td>@f$+4\xi - 4\nu@f$</td>
/// <td>@f$f(M_2)^2@f$</td></tr>
/// </table>
/// @note Schureman: %Table 2a, Page 167
class M4 : public Wave {
 public:
  M4();
};

/// @brief @f$SN_4 = S_2 + N_2@f$
///
/// <table>
/// <tr><th>V</th><th>u</th><th>Factor-f</th></tr>
/// <tr><td>@f$4T - 3s + 2h + p@f$</td>
/// <td>@f$2\xi - 2\nu@f$</td>
/// <td>@f$f(M_2)@f$</td></tr>
/// </table>
class SN4 : public Wave {
 public:
  SN4();
};

/// @brief @f$MS_4 = M_2 + S_2@f$
///
/// <table>
/// <tr><th>V</th><th>u</th><th>Factor-f</th></tr>
/// <tr><td>@f$4T - 2s + 2h@f$</td>
/// <td>@f$+2\xi - 2\nu@f$</td>
/// <td>@f$f(M_2)@f$</td></tr>
/// </table>
/// @note Schureman: %Table 2a, Page 167.
/// @note correct nodal factor: Pugh (1987).
/// @note Tides, surges and mean sea-level (Page 111, Table 4.4)
class MS4 : public Wave {
 public:
  MS4();
};

/// @brief @f$MK_4 = M_2 + K_2@f$
///
/// <table>
/// <tr><th>V</th><th>u</th><th>Factor-f</th></tr>
/// <tr><td>@f$4T - 2s + 4h@f$</td>
/// <td>@f$2\xi - 2\nu - 2\nu^{\prime\prime}@f$</td>
/// <td>@f$f(MK_4)@f$</td></tr>
/// </table>
/// @note Schureman: %Table 2a, Page 167
class MK4 : public Wave {
 public:
  MK4();
};

/// @brief @f$S_4 = S_2 + S_2@f$
///
/// <table>
/// <tr><th>V</th><th>u</th><th>Factor-f</th></tr>
/// <tr><td>@f$4T@f$</td>
/// <td>@f$0@f$</td>
/// <td>@f$1@f$</td></tr>
/// </table>
class S4 : public Wave {
 public:
  S4();
};

/// @brief @f$SK_4 = S_2 + K_2@f$
///
/// <table>
/// <tr><th>V</th><th>u</th><th>Factor-f</th></tr>
/// <tr><td>@f$4T + 2h@f$</td>
/// <td>@f$-2\nu^{\prime \prime}@f$</td>
/// <td>@f$f(K_2)@f$</td></tr>
/// </table>
class SK4 : public Wave {
 public:
  SK4();
};

/// @brief @f$R_4 = R_2 + R_2@f$
///
/// <table>
/// <tr><th>V</th><th>u</th><th>Factor-f</th></tr>
/// <tr><td>@f$4T + 2h - 2p_1@f$</td>
/// <td>@f$0@f$</td>
/// <td>@f$1@f$</td></tr>
/// </table>
class R4 : public Wave {
 public:
  R4();
};

/// @brief @f$2MN_6 = 2M_2 + N_2@f$
///
/// <table>
/// <tr><th>V</th><th>u</th><th>Factor-f</th></tr>
/// <tr><td>@f$6T - 7s + 6h + p@f$</td>
/// <td>@f$6\xi - 6\nu@f$</td>
/// <td>@f$f(M_2)^3@f$</td></tr>
/// </table>
class _2MN6 : public Wave {
 public:
  _2MN6();
};

/// @brief @f$M_6 = 3M_2@f$
///
/// <table>
/// <tr><th>V</th><th>u</th><th>Factor-f</th></tr>
/// <tr><td>@f$6T - 6s + 6h@f$</td>
/// <td>@f$+6\xi - 6\nu@f$</td>
/// <td>@f$f(M_2)^3@f$</td></tr>
/// </table>
class M6 : public Wave {
 public:
  M6();
};

/// @brief @f$MSN_6 = M_2 + S_2 + N_2@f$
///
/// <table>
/// <tr><th>V</th><th>u</th><th>Factor-f</th></tr>
/// <tr><td>@f$6T - 5s + 4h + p@f$</td>
/// <td>@f$4\xi - 4\nu@f$</td>
/// <td>@f$f(M_2)^2@f$</td></tr>
/// </table>
class MSN6 : public Wave {
 public:
  MSN6();
};

/// @brief @f$2MS_6 = 2M_2 + S_2@f$
///
/// <table>
/// <tr><th>V</th><th>u</th><th>Factor-f</th></tr>
/// <tr><td>@f$6T - 4s + 4h@f$</td>
/// <td>@f$4\xi - 4\nu@f$</td>
/// <td>@f$f(M_2)^2@f$</td></tr>
/// </table>
class _2MS6 : public Wave {
 public:
  _2MS6();
};

/// @brief @f$2MK_6 = 2M_2 + K_2@f$
///
/// <table>
/// <tr><th>V</th><th>u</th><th>Factor-f</th></tr>
/// <tr><td>@f$6T - 4s + 6h@f$</td>
/// <td>@f$4\xi - 4\nu - 2\nu^{\prime \prime}@f$</td>
/// <td>@f$f(M_2)^2 \times f(K_2)@f$</td></tr>
/// </table>
class _2MK6 : public Wave {
 public:
  _2MK6();
};

/// @brief @f$2SM_6 = 2S_2 + M_2@f$
///
/// <table>
/// <tr><th>V</th><th>u</th><th>Factor-f</th></tr>
/// <tr><td>@f$6T - 2s + 2h@f$</td>
/// <td>@f$2\xi - 2\nu@f$</td>
/// <td>@f$f(M_2)@f$</td></tr>
/// </table>
class _2SM6 : public Wave {
 public:
  _2SM6();
};

/// @brief @f$MSK_6 = M_2 + K_2 + S_2@f$
///
/// <table>
/// <tr><th>V</th><th>u</th><th>Factor-f</th></tr>
/// <tr><td>@f$6T - 2s + 4h@f$</td>
/// <td>@f$2\xi - 2\nu - 2\nu^{\prime}@f$</td>
/// <td>@f$f(M_2) \times f(K_2)@f$</td></tr>
/// </table>
class MSK6 : public Wave {
 public:
  MSK6();
};

/// @brief @f$S_6 = 3S_2@f$
///
/// <table>
/// <tr><th>V</th><th>u</th><th>Factor-f</th></tr>
/// <tr><td>@f$6T@f$</td>
/// <td>@f$0@f$</td>
/// <td>@f$1@f$</td></tr>
/// </table>
class S6 : public Wave {
 public:
  S6();
};

/// @brief @f$M_8 = 4M_2@f$
///
/// <table>
/// <tr><th>V</th><th>u</th><th>Factor-f</th></tr>
/// <tr><td>@f$8T - 8s + 8h@f$</td>
/// <td>@f$8\xi - 8\nu@f$</td>
/// <td>@f$f(M_2)^4@f$</td></tr>
/// </table>
class M8 : public Wave {
 public:
  M8();
};

/// @brief @f$MSf = M_2 - S_2@f$
///
/// <table>
/// <tr><th>V</th><th>u</th><th>Factor-f</th></tr>
/// <tr><td>@f$2s - 2h@f$</td>
/// <td>@f$2\xi - 2\nu@f$</td>
/// <td>@f$f(M_2) * f(S_2) = f(M_2)@f$</td></tr>
/// </table>
/// @warning Same frequency as MSf LP : 2s -2h
class MSf : public Wave {
 public:
  MSf();
};

/// @brief @f$A5@f$
///
/// <table>
/// <tr><th>V</th><th>u</th><th>Factor-f</th></tr>
/// <tr><td>@f$2s - 2h@f$</td>
/// <td>@f$0@f$</td>
/// <td>@f$f(Mm)@f$</td></tr>
/// </table>
/// @warning Same frequency as MSf Non Linear wave : 2s -2h
/// @note Schureman: %Table 2, Page 164, Ref. %A5
/// @note Second order long period equilibrium in atlas of FES2014c as MSf_LP
class A5 : public Wave {
 public:
  A5();
};

/// @brief @f$Sa_1@f$
///
/// <table>
/// <tr><th>V</th><th>u</th><th>Factor-f</th></tr>
/// <tr><td>@f$h - p_1@f$</td>
/// <td>@f$0@f$</td>
/// <td>@f$1@f$</td></tr>
/// </table>
/// @note Schureman: %Table 2, Page 164, Ref. B2
class Sa1 : public Wave {
 public:
  Sa1();
};

/// @brief @f$Sta@f$
///
/// <table>
/// <tr><th>V</th><th>u</th><th>Factor-f</th></tr>
/// <tr><td>@f$3h - p_1@f$</td>
/// <td>@f$0@f$</td>
/// <td>@f$1@f$</td></tr>
/// </table>
/// @note Schureman: %Table 2, Page 164, Ref. B7
class Sta : public Wave {
 public:
  Sta();
};

/// @brief @f$Mm_2@f$
///
/// <table>
/// <tr><th>V</th><th>u</th><th>Factor-f</th></tr>
/// <tr><td>@f$s - 90@f$</td>
/// <td>@f$-\xi@f$</td>
/// <td>@f$f(141)@f$</td></tr>
/// </table>
/// @note Schureman: %Table 2, Page 164, Ref. A64
class Mm2 : public Wave {
 public:
  Mm2();
};

/// @brief @f$Mm_1@f$
///
/// <table>
/// <tr><th>V</th><th>u</th><th>Factor-f</th></tr>
/// <tr><td>@f$s + p + 180@f$</td>
/// <td>@f$-2\xi@f$</td>
/// <td>@f$f(Mf)@f$</td></tr>
/// </table>
/// @note Schureman: %Table 2, Page 164, Ref. A8
class Mm1 : public Wave {
 public:
  // Schureman: Ref. A8, Page 164, Table 2.
  Mm1();
};

/// @brief @f$Mf_1@f$
///
/// <table>
/// <tr><th>V</th><th>u</th><th>Factor-f</th></tr>
/// <tr><td>@f$2s - 2p@f$</td>
/// <td>@f$0@f$</td>
/// <td>@f$f(Mm)@f$</td></tr>
/// </table>
/// @note Schureman: %Table 2, Page 164, Ref. A3
class Mf1 : public Wave {
 public:
  Mf1();
};

/// @brief @f$Mf_2@f$
///
/// <table>
/// <tr><th>V</th><th>u</th><th>Factor-f</th></tr>
/// <tr><td>@f$2s - p - 90@f$</td>
/// <td>@f$-\xi@f$</td>
/// <td>@f$f(141)@f$</td></tr>
/// </table>
/// @note Schureman: %Table 2, Page 164, Ref. A65
class Mf2 : public Wave {
 public:
  Mf2();
};

/// @brief @f$M_0@f$
///
/// <table>
/// <tr><th>V</th><th>u</th><th>Factor-f</th></tr>
/// <tr><td>@f$0@f$</td>
/// <td>@f$0@f$</td>
/// <td>@f$f(Mm)@f$</td></tr>
/// </table>
/// @note Schureman: %Table 2, Page 164, Ref. A1
class M0 : public Wave {
 public:
  M0();
};

/// @brief @f$N_2P@f$
///
/// <table>
/// <tr><th>V</th><th>u</th><th>Factor-f</th></tr>
/// <tr><td>@f$2T - 3s + 2h + 90^{\circ}@f$</td>
/// <td>@f$+3\xi - 2\nu@f$</td>
/// <td>@f$f(146)@f$</td></tr>
/// </table>
/// @note Schureman: %Table 2, Page 166, Ref. A76
class N2P : public Wave {
 public:
  N2P();
};

/// @brief @f$L_2P@f$
///
/// <table>
/// <tr><th>V</th><th>u</th><th>Factor-f</th></tr>
/// <tr><td>@f$2T - s + 2h - 90^{\circ}@f$</td>
/// <td>@f$+\xi - 2\nu@f$</td>
/// <td>@f$f(147)@f$</td></tr>
/// </table>
/// @note Schureman: %Table 2, Page 166, Ref. A79
class L2P : public Wave {
 public:
  L2P();
};

/// @brief @f$MSK2@f$
///
/// <table>
/// <tr><th>V</th><th>u</th><th>Factor-f</th></tr>
/// <tr><td>@f$22T - 2s@f$</td>
/// <td>@f$2\xi -2\nu + 2\nu^{\prime\prime}@f$</td>
/// <td>@f$f(M_2) \times f(K_2)@f$</td></tr>
/// </table>
class MSK2 : public Wave {
 public:
  MSK2();
};

/// @brief @f$SKM2@f$
///
/// <table>
/// <tr><th>V</th><th>u</th><th>Factor-f</th></tr>
/// <tr><td>@f$2T + 2s@f$</td>
/// <td>@f$-2\xi + 2\nu - 2\nu^{\prime\prime}@f$</td>
/// <td>@f$f(M_2) \times f(K_2)@f$</td></tr>
/// </table>
class SKM2 : public Wave {
 public:
  SKM2();
};

/// @brief @f$OQ2@f$
///
/// <table>
/// <tr><th>V</th><th>u</th><th>Factor-f</th></tr>
/// <tr><td>@f$2T - 5s + 2h + 1p + 180^{\circ}@f$</td>
/// <td>@f$0@f$</td>
/// <td>@f$f(O_1) \times f(Q_1)@f$</td></tr>
/// </table>
class OQ2 : public Wave {
 public:
  OQ2();
};

/// @brief @f$3MS4@f$
///
/// <table>
/// <tr><th>V</th><th>u</th><th>Factor-f</th></tr>
/// <tr><td>@f$4T - 6s + 6h@f$</td>
/// <td>@f$6\xi - 6\nu@f$</td>
/// <td>@f$f(M_2)^3@f$</td></tr>
/// </table>
class _3MS4 : public Wave {
 public:
  _3MS4();
};

/// @brief @f$MNu4 = M2 + Nu2@f$
///
/// <table>
/// <tr><th>V</th><th>u</th><th>Factor-f</th></tr>
/// <tr><td>@f$4T -5s + 6h -p@f$</td>
/// <td>@f$4\xi - 4\nu@f$</td>
/// <td>@f$f(M_2)^2@f$</td></tr>
/// </table>
class MNu4 : public Wave {
 public:
  MNu4();
};

/// @brief @f$2MSN4 = M2 + S2 - N2@f$
///
/// <table>
/// <tr><th>V</th><th>u</th><th>Factor-f</th></tr>
/// <tr><td>@f$4T - s + 2h - p@f$</td>
/// <td>@f$2\xi - 2\nu@f$</td>
/// <td>@f$f(M_2)^3@f$</td></tr>
/// </table>
class _2MSN4 : public Wave {
 public:
  _2MSN4();
};

/// @brief @f$2NS2@f$
///
/// <table>
/// <tr><th>V</th><th>u</th><th>Factor-f</th></tr>
/// <tr><td>@f$2T - 6s + 4h + 2p@f$</td>
/// <td>@f$4\xi - 4\nu@f$</td>
/// <td>@f$f(M_2)^2@f$</td></tr>
/// </table>
class _2NS2 : public Wave {
 public:
  _2NS2();
};

/// @brief @f$MNuS2 = M2 + Nu2 - S2@f$
///
/// <table>
/// <tr><th>V</th><th>u</th><th>Factor-f</th></tr>
/// <tr><td>@f$2T - 5s + 6h - p@f$</td>
/// <td>@f$4\xi - 4\nu@f$</td>
/// <td>@f$f(M_2)^2@f$</td></tr>
/// </table>
class MNuS2 : public Wave {
 public:
  MNuS2();
};

/// @brief @f$2MK2@f$
///
/// <table>
/// <tr><th>V</th><th>u</th><th>Factor-f</th></tr>
/// <tr><td>@f$2T - 4s + 2h@f$</td>
/// <td>@f$4\xi - 4\nu + 2\nu^{\prime\prime}@f$</td>
/// <td>@f$f(M_2)^2 \times f(K_2)@f$</td></tr>
/// </table>
class _2MK2 : public Wave {
 public:
  _2MK2();
};

/// @brief @f$NKM2 = N2 + K2 - M2@f$
///
/// <table>
/// <tr><th>V</th><th>u</th><th>Factor-f</th></tr>
/// <tr><td>@f$2T -s + 2h + p@f$</td>
/// <td>@f$-2\nu^{\prime\prime}@f$</td>
/// <td>@f$f(M_2)^2 \times f(K_2)@f$</td></tr>
/// </table>
class NKM2 : public Wave {
 public:
  NKM2();
};

/// @brief @f$ML4 = M2 + L2@f$
///
/// <table>
/// <tr><th>V</th><th>u</th><th>Factor-f</th></tr>
/// <tr><td>@f$4T - 3s + 4h - p@f$</td>
/// <td>@f$4\xi - 4\nu@f$</td>
/// <td>@f$f(M_2) \times f(L_2)@f$</td></tr>
/// </table>
class ML4 : public Wave {
 public:
  ML4();
};

/// @brief @f$SO1@f$
///
/// <table>
/// <tr><th>V</th><th>u</th><th>Factor-f</th></tr>
/// <tr><td>@f$T + 2s - h - 90^{\circ}@f$</td>
/// <td>@f$-\nu@f$</td>
/// <td>@f$f(O_1)@f$</td></tr>
/// </table>
class SO1 : public Wave {
 public:
  SO1();
};

/// @brief @f$SO3@f$
///
/// <table>
/// <tr><th>V</th><th>u</th><th>Factor-f</th></tr>
/// <tr><td>@f$3T -2s + 1h + 90^{\circ}@f$</td>
/// <td>@f$2\xi - \nu@f$</td>
/// <td>@f$f(O_1)@f$</td></tr>
/// </table>
class SO3 : public Wave {
 public:
  SO3();
};

/// @brief @f$NK4 = N2 + K2@f$
///
/// <table>
/// <tr><th>V</th><th>u</th><th>Factor-f</th></tr>
/// <tr><td>@f$4T - 3s + 4h + p@f$</td>
/// <td>@f$2\xi - 2\nu - 2\nu^{\prime\prime}@f$</td>
/// <td>@f$f(M_2) \times f(K_2)@f$</td></tr>
/// </table>
class NK4 : public Wave {
 public:
  NK4();
};

/// @brief @f$MNK6 = M2 + N2 + K2@f$
///
/// <table>
/// <tr><th>V</th><th>u</th><th>Factor-f</th></tr>
/// <tr><td>@f$6T - 5s + 6h + p@f$</td>
/// <td>@f$4\xi - 4\nu - 2\nu^{\prime\prime}@f$</td>
/// <td>@f$f(M_2)^2 \times f(K_2)@f$</td></tr>
/// </table>
class MNK6 : public Wave {
 public:
  MNK6();
};

/// @brief @f$3MNL6 = 3M2 + N2 - L2@f$
///
/// <table>
/// <tr><th>V</th><th>u</th><th>Factor-f</th></tr>
/// <tr><td>@f$6T - 8s + 6h + 2p@f$</td>
/// <td>@f$6\xi - 6\nu@f$</td>
/// <td>@f$f(M_2)^4 \times f(L_2)@f$</td></tr>
/// </table>
class _2NM6 : public Wave {
 public:
  _2NM6();
};

/// @brief @f$3MS8 = M2 + S2@f$
///
/// <table>
/// <tr><th>V</th><th>u</th><th>Factor-f</th></tr>
/// <tr><td>@f$8T - 6s + 6h@f$</td>
/// <td>@f$6\xi - 6\nu@f$</td>
/// <td>@f$f(M_2)^3@f$</td></tr>
/// </table>
class _3MS8 : public Wave {
 public:
  _3MS8();
};

/// @brief @f$SK3@f$
///
/// <table>
/// <tr><th>V</th><th>u</th><th>Factor-f</th></tr>
/// <tr><td>@f$3T + h - 90^{\circ}@f$</td>
/// <td>@f$-1\nu^{\prime}@f$</td>
/// <td>@f$f(K_1)@f$</td></tr>
/// </table>
class SK3 : public Wave {
 public:
  SK3();
};

/// @brief @f$2MNS4 = 2M2 + N2 - S2@f$
///
/// <table>
/// <tr><th>V</th><th>u</th><th>Factor-f</th></tr>
/// <tr><td>@f$4T - 7s + 6h + p@f$</td>
/// <td>@f$-6\xi - 6\nu@f$</td>
/// <td>@f$f(M_2)^3@f$</td></tr>
/// </table>
class _2MNS4 : public Wave {
 public:
  _2MNS4();
};

/// @brief @f$2SMu2 = S2 - Mu2@f$
///
/// <table>
/// <tr><th>V</th><th>u</th><th>Factor-f</th></tr>
/// <tr><td>@f$2T + 4s - 4h@f$</td>
/// <td>@f$-2\xi + 2\nu@f$</td>
/// <td>@f$f(M_2)@f$</td></tr>
/// </table>
class _2SMu2 : public Wave {
 public:
  _2SMu2();
};

/// @brief @f$2MP5 = 2M2 + P1@f$
///
/// <table>
/// <tr><th>V</th><th>u</th><th>Factor-f</th></tr>
/// <tr><td>@f$5T -4s + 3h@f$</td>
/// <td>@f$4\xi - 4\nu@f$</td>
/// <td>@f$f(M_2)^2@f$</td></tr>
/// </table>
class _2MP5 : public Wave {
 public:
  _2MP5();
};

}  // namespace wave
}  // namespace darwin
}  // namespace fes
