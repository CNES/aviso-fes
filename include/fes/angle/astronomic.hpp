// Copyright (c) 2026 CNES
//
// All rights reserved. Use of this source code is governed by a
// BSD-style license that can be found in the LICENSE file.
/// @file include/fes/angle/astronomic.hpp
/// @brief Astronomic angle.
#pragma once
#include <cmath>
#include <limits>
#include <sstream>

#include "fes/delta_t.hpp"
#include "fes/detail/math.hpp"
#include "fes/numbers.hpp"

namespace fes {
namespace angle {

/// @brief Astronomic formulae used to calculate the astronomic angles.
enum class Formulae {
  /// Schureman order 1
  kSchuremanOrder1,
  /// Schureman order 3
  kSchuremanOrder3,
  /// Meeus
  kMeeus,
  /// IERS (cf.
  /// https://hpiers.obspm.fr/eop-pc/models/nutations/nut_oceanic.html)
  kIERS
};

/// @brief Astronomical angles.
///
/// In tidal work the only celestial bodies that need to be considered are the
/// moon and sun. Although every other celestial body whose gravitational
/// influence reaches the Earth create a theoretical tide producing force, the
/// greater distance or smaller size of such body renders negligible any effect
/// of this force upon the tides of the Earth. In deriving mathematical
/// expressions for the tide-producing forces of the moon and sun, the
/// principal factors to be taken into consideration are the rotation of the
/// earth, the volution of the moon around the earth, the revolution of the
/// earth around the sun, the inclination of the moon's orbit to the earth's
/// equator, and the obliquity of the ecliptic.
class Astronomic {
 public:
  /// @brief Default constructor.
  /// @param[in] formulae Astronomic formulae used to calculate the astronomic
  /// angles.
  ///
  constexpr Astronomic(const Formulae formulae = Formulae::kSchuremanOrder1)
      : update_(nullptr) {
    switch (formulae) {
      case Formulae::kSchuremanOrder1:
        update_ = &Astronomic::schureman_order1;
        break;
      case Formulae::kSchuremanOrder3:
        update_ = &Astronomic::schureman_order3;
        break;
      case Formulae::kMeeus:
        update_ = &Astronomic::meeus;
        break;
      case Formulae::kIERS:
        update_ = &Astronomic::iers;
        break;
      default:
        throw std::runtime_error("unknown formulae");
    }
  };

  /// @brief Create astronomic angles for a given UTC time.
  ///
  /// @param[in] formulae Astronomic formulae used to calculate the astronomic
  /// angles.
  /// @param[in] epoch Desired UTC time in seconds since 1970-01-01T00:00:00Z.
  FES_MATH_CONSTEXPR Astronomic(const Formulae formulae, const double epoch)
      : Astronomic(formulae) {
    update(epoch);
  }

  /// @brief Updates astronomic angles for a given UTC time.
  ///
  /// @param[in] epoch Desired UTC time in seconds since 1970-01-01T00:00:00Z.
  auto FES_MATH_CONSTEXPR update(const double epoch) noexcept -> void;

  /// @brief @f$T@f$
  ///
  /// @returns Hour angle of mean sun.
  constexpr auto t() const noexcept -> double { return t_; }

  /// @brief @f$N@f$
  ///
  /// @returns  Mean longitude of the ascending node of the Moon (radians).
  constexpr auto n() const noexcept -> double { return n_; }

  /// @brief @f$h@f$
  ///
  /// @returns Mean longitude of sun (radians).
  constexpr auto h() const noexcept -> double { return h_; }

  /// @brief @f$s@f$
  ///
  /// @returns Mean longitude of moon (radians).
  constexpr auto s() const noexcept -> double { return s_; }

  /// @brief @f$p_1@f$
  ///
  /// @returns The mean longitude of solar perigee (radians).
  constexpr auto p1() const noexcept -> double { return p1_; }

  /// @brief @f$p@f$
  ///
  /// @returns Mean longitude of lunar perigee (radians).
  constexpr auto p() const noexcept -> double { return p_; }

  /// @brief @f$I@f$
  ///
  /// @returns Obliquity of lunar orbit with respect to earth's equator
  /// (radians).
  constexpr auto i() const noexcept -> double { return i_; }

  /// @brief @f$\xi@f$
  ///
  /// @returns Longitude in the moon's orbit of lunar intersection (radians)
  constexpr auto xi() const noexcept -> double { return xi_; }

  /// @brief @f$\nu@f$
  ///
  /// @returns Right ascension of lunar intersection (radians).
  constexpr auto nu() const noexcept -> double { return nu_; }

  /// @brief @f$1/R_a@f$
  ///
  /// @returns Factor in amplitude of constituent @f$L_2@f$.
  constexpr auto x1ra() const noexcept -> double { return x1ra_; }

  /// @brief @f$R@f$
  ///
  /// @returns Term in argument of constituent @f$L_2@f$ (radians).
  constexpr auto r() const noexcept -> double { return r_; }

  /// @brief @f$\nu'@f$
  ///
  /// @returns Term in argument of lunisolar constituent @f$K_1@f$ (radians).
  constexpr auto nuprim() const noexcept -> double { return nuprim_; }

  /// @brief @f$\nu''@f$
  ///
  /// @returns Term in argument of lunisolar constituent @f$K_2@f$ (radians).
  constexpr auto nusec() const noexcept -> double { return nusec_; }

  /// @brief Gets the node factor of @f$O_1@f$.
  ///
  /// @return @f$sin(I) \times cos^{2}(\frac{I}{2})/0.3800@f$
  FES_MATH_CONSTEXPR auto f_o1() const noexcept -> double {
    // SCHUREMAN P.25 (75)
    constexpr auto factor = 1 / numbers::k67;
    return std::sin(i_) * detail::math::pow<2>(std::cos(i_ * 0.5)) * factor;
  }

  /// @brief Gets the node factor of @f$OO_1@f$.
  ///
  /// @return @f$sin(I) \times sin^2(\frac{I}{2})/0.0164@f$
  FES_MATH_CONSTEXPR auto f_oo1() const noexcept -> double {
    // SCHUREMAN P.25 (77)
    constexpr auto factor = 1 / numbers::k69;
    return std::sin(i_) * detail::math::pow<2>(std::sin(i_ * 0.5)) * factor;
  }

  /// @brief Gets the unity node factor.
  ///
  /// @return 1
  FES_MATH_CONSTEXPR auto f_1() const noexcept -> double { return 1; }

  /// @brief Gets the node factor of @f$J_1@f$.
  ///
  /// @return @f$sin(2I)/0.7214@f$
  FES_MATH_CONSTEXPR auto f_j1() const noexcept -> double {
    // SCHUREMAN P.25 (76)
    constexpr auto factor = 1 / numbers::k68;
    return std::sin(2.0 * i_) * factor;
  }

  /// @brief Gets the node factor of @f$M_{1}@f$.
  ///
  /// @return @f$f(O_1) \times (2.310 + 1.435 \times
  /// cos(2(p - \xi))^\frac{1}{2}@f$
  /// @note @f$p=P+\xi@f$
  FES_MATH_CONSTEXPR auto f_m1() const noexcept -> double {
    // SCHUREMAN P.43 (207)
    // SCHUREMAN P.41 (197)
    return f_o1() * std::sqrt(numbers::k197_1 +
                              numbers::k197_2 * std::cos(2.0 * (p_ - xi_)));
  }

  /// @brief Gets the node factor of @f$M_{2}@f$.
  ///
  /// @return @f$cos^4(\frac{I}{2})/0.9154@f$
  constexpr auto f_m2() const noexcept -> double {
    // SCHUREMAN P.25 (78)
    constexpr auto factor = 1 / numbers::k70;
    return detail::math::pow<4>(std::cos(i_ * 0.5)) * factor;
  }

  /// @brief Gets the node factor of @f$M_{3}@f$.
  ///
  /// @return @f$cos^6(\frac{I}{2})/0.8758@f$
  FES_MATH_CONSTEXPR auto f_m3() const noexcept -> double {
    // SCHUREMAN P.36 (149)
    constexpr auto factor = 1 / numbers::k149;
    return std::pow(std::cos(i_ * 0.5), 6.0) * factor;
  }

  /// @brief Gets the node factor of @f$Mf@f$.
  ///
  /// @return @f$sin^2(I)/0.1578@f$
  constexpr auto f_mf() const noexcept -> double {
    // SCHUREMAN P.25 (74)
    constexpr auto factor = 1 / numbers::k66;
    return detail::math::pow<2>(std::sin(i_)) * factor;
  }

  /// @brief Gets the node factor of @f$Mm@f$.
  ///
  /// @return @f$(\frac{2}{3} - sin^2(I))/0.5021@f$
  constexpr auto f_mm() const noexcept -> double {
    // SCHUREMAN P.25 (73)
    constexpr auto factor = 1 / numbers::k65;
    return (2.0 / 3.0 - detail::math::pow<2>(std::sin(i_))) * factor;
  }

  /// @brief Gets the node factor of @f$M_2^2@f$.
  ///
  /// @return @f$f(M_2)^2@f$
  constexpr auto f_m22() const noexcept -> double {
    return detail::math::pow<2>(f_m2());
  }

  /// @brief Gets the node factor of @f$M_2^3@f$.
  ///
  /// @return @f$f(M_2)^3@f$
  constexpr auto f_m23() const noexcept -> double {
    return detail::math::pow<3>(f_m2());
  }

  /// @brief Gets the node factor of @f$f(M_2)^4@f$.
  ///
  /// @return @f$f(M_2)^4@f$
  constexpr auto f_m24() const noexcept -> double {
    return detail::math::pow<4>(f_m2());
  }

  /// @brief Gets the node factor of @f$K_1@f$.
  ///
  /// @return @f$(0.8965 \times sin^2(2I) + 0.6001 \times sin(2I) \times
  /// cos(\nu) + 0.1006)^\frac{1}{2}@f$
  FES_MATH_CONSTEXPR auto f_k1() const noexcept -> double {
    // SCHUREMAN P.45 (227)
    auto sin_2i = std::sin(2.0 * i_);
    return std::sqrt(numbers::k227_1 * detail::math::pow<2>(sin_2i) +
                     numbers::k227_2 * sin_2i * std::cos(nu_) +
                     numbers::k227_3);
  }

  /// @brief Gets the node factor of @f$K_2@f$.
  ///
  /// @return @f$(19.0444 \times sin^4(I) + 2.7702 \times sin^2(I) \times
  /// cos(2\nu) + 0.0981)^\frac{1}{2}@f$
  FES_MATH_CONSTEXPR auto f_k2() const noexcept -> double {
    // SCHUREMAN P.46 (235)
    auto sin_i2 = detail::math::pow<2>(std::sin(i_));
    return sqrt(numbers::k235_1 * detail::math::pow<2>(sin_i2) +
                numbers::k235_2 * sin_i2 * std::cos(2.0 * nu_) +
                numbers::k235_3);
  }

  /// @brief Gets the node factor for the formula 79.
  ///
  /// @return @f$sin^2(I)/0.1565@f$
  constexpr auto f_79() const noexcept -> double {
    // SCHUREMAN P.25 (79)
    constexpr auto factor = 1 / numbers::k71;
    return detail::math::pow<2>(std::sin(i_)) * factor;
  }

  /// @brief Gets the node factor of @f$L_2@f$.
  ///
  /// @return @f$f(M_2) \times \frac{1}{R_a}@f$
  constexpr auto f_l2() const noexcept -> double {
    // SCHUREMAN P.44 (215)
    return f_m2() * x1ra_;
  }

  /// @brief Gets the node factor of @f$f(M_2) \times f(K_2)@f$.
  ///
  /// @return @f$f(M_2) \times f(K_2)@f$
  FES_MATH_CONSTEXPR auto f_m2_k2() const noexcept -> double {
    return f_m2() * f_k2();
  }

  /// @brief Gets the node factor of @f$f(M_2) \times f(K_1)@f$.
  ///
  /// @return @f$f(M_2) \times f(K_1)@f$
  FES_MATH_CONSTEXPR auto f_m2_k1() const noexcept -> double {
    return f_m2() * f_k1();
  }

  /// @brief Gets the node factor of @f$f(M_2) \times f(O_1)@f$.
  ///
  /// @return @f$f(M_2) \times f(O_1)@f$
  FES_MATH_CONSTEXPR auto f_m2_o1() const noexcept -> double {
    return f_m2() * f_o1();
  }

  /// @brief Gets the node factor of @f$f(M_2) \times f(L_2)@f$.
  ///
  /// @return @f$f(M_2) \times f(L_2)@f$
  FES_MATH_CONSTEXPR auto f_m2_l2() const noexcept -> double {
    return f_m2() * f_l2();
  }

  /// @brief Gets the node factor of @f$f(M_2)^4 \times f(L_2)@f$.
  ///
  /// @return @f$f(M_2)^4 \times f(L_2)@f$
  FES_MATH_CONSTEXPR auto f_m24_l2() const noexcept -> double {
    return f_m24() * f_l2();
  }

  /// @brief Gets the node factor of @f$f(O_1)^2@f$.
  ///
  /// @return @f$f(O_1)^2@f$
  FES_MATH_CONSTEXPR auto f_o12() const noexcept -> double {
    return detail::math::pow<2>(f_o1());
  }

  /// @brief Gets the node factor of @f$f(M_2)^2 \times f(K_1)@f$.
  ///
  /// @return @f$f(M_2)^2 \times f(K_1)@f$
  FES_MATH_CONSTEXPR auto f_m22_k1() const noexcept -> double {
    return f_m22() * f_k1();
  }

  /// @brief Gets the node factor of @f$f(M_2)^2 \times f(K_2)@f$.
  ///
  /// @return @f$f(M_2)^2 \times f(K_2)@f$
  FES_MATH_CONSTEXPR auto f_m22_k2() const noexcept -> double {
    return f_m22() * f_k2();
  }

  /// @brief Gets the node factor of @f$f(M_2)^3 \times f(K_2)@f$.
  ///
  /// @return @f$f(M_2)^3 \times f(K_2)@f$
  FES_MATH_CONSTEXPR auto f_m23_k2() const noexcept -> double {
    return f_m23() * f_k2();
  }

  /// @brief Gets the node factor for formula 141.
  ///
  /// @return @f$(\sin(I) - 5/4 sin^3(I)) / 0.3192@f$
  FES_MATH_CONSTEXPR auto f_141() const noexcept -> double {
    // SCHUREMAN P.36 (141)
    auto sin_i = std::sin(i_);
    constexpr auto factor = 1 / numbers::k141;
    return (sin_i - (5.0 / 4.0) * detail::math::pow<3>(sin_i)) * factor;
  }

  /// @brief Gets the node factor for formula 144.
  ///
  /// @return @f$(1-10 \sin^2
  /// \frac{1}{2}I+15\sin^4\frac{1}{2}I)\cos^2\frac{1}{2}I/0.5873@f$
  FES_MATH_CONSTEXPR auto f_144() const noexcept -> double {
    // SCHUREMAN P.36 (144)
    auto sin_i_2 = std::sin(0.5 * i_);
    auto cos_i_2 = std::cos(0.5 * i_);
    constexpr auto factor = 1 / numbers::k144;
    return (1 - 10 * detail::math::pow<2>(sin_i_2) +
            15 * detail::math::pow<4>(sin_i_2)) *
           detail::math::pow<2>(cos_i_2) * factor;
  }

  /// @brief Gets the node factor for formula 146.
  ///
  /// @return @f$\sin I \cos^4\frac{1}{2}I/0.3658@f$
  FES_MATH_CONSTEXPR auto f_146() const noexcept -> double {
    // SCHUREMAN P.36 (146)
    auto sin_i = std::sin(i_);
    auto cos_i_2 = std::cos(0.5 * i_);
    constexpr auto factor = 1 / numbers::k146;
    return sin_i * detail::math::pow<4>(cos_i_2) * factor;
  }

  /// @brief Gets the node factor for formula 147.
  ///
  /// @return @f$(\cos^2(\frac{1}{2}I)-\frac{2}{3})\sin I
  /// \cos^2(\frac{1}{2}I)/0.1114@f$
  FES_MATH_CONSTEXPR auto f_147() const noexcept -> double {
    // SCHUREMAN P.36 (147)
    auto sin_i = std::sin(i_);
    auto cos_i_2 = std::cos(0.5 * i_);
    constexpr auto factor = 1 / numbers::k147;
    return (detail::math::pow<2>(cos_i_2) - (2.0 / 3.0)) * sin_i *
           detail::math::pow<2>(cos_i_2) * factor;
  }

 protected:
  /// @f$T@f$: hour angle of mean sun.
  double t_{std::numeric_limits<double>::quiet_NaN()};
  /// @f$N@f$: longitude of moon's node.
  double n_{std::numeric_limits<double>::quiet_NaN()};
  /// @f$h@f$: Mean longitude of sun.
  double h_{std::numeric_limits<double>::quiet_NaN()};
  /// @f$s@f$: mean longitude of moon.
  double s_{std::numeric_limits<double>::quiet_NaN()};
  /// @f$p_1@f$: Mean longitude of solar perigee.
  double p1_{std::numeric_limits<double>::quiet_NaN()};
  /// @f$p@f$: Mean longitude of lunar perigee.
  double p_{std::numeric_limits<double>::quiet_NaN()};
  /// @f$p@f$:Obliquity of lunar orbit with respect to earth's equator
  double i_{std::numeric_limits<double>::quiet_NaN()};
  /// @f$\xi@f$: Longitude in the moon's orbit of lunar intersection
  double xi_{std::numeric_limits<double>::quiet_NaN()};
  /// @f$\nu@f$: Right ascension of lunar intersection
  double nu_{std::numeric_limits<double>::quiet_NaN()};
  /// @f$1/R_a@f$: Factor in amplitude of constituent @f$L_2@f$
  double x1ra_{std::numeric_limits<double>::quiet_NaN()};
  /// @f$R@f$: Term in argument of constituent @f$L_2@f$
  double r_{std::numeric_limits<double>::quiet_NaN()};
  /// @f$\nu'@f$: Term in argument of lunisolar constituent @f$K_1@f$
  double nuprim_{std::numeric_limits<double>::quiet_NaN()};
  /// @f$\nu''@f$: Term in argument of lunisolar constituent @f$K_2@f$
  double nusec_{std::numeric_limits<double>::quiet_NaN()};

 private:
  /// Calculates the astronomic angles using the Schureman formulae limited to
  /// order 1.
  ///
  /// @param[in] epoch Desired UTC time in seconds since 1970-01-01T00:00:00Z.
  FES_MATH_CONSTEXPR auto schureman_order1(const double epoch) noexcept -> void;

  /// Calculates the astronomic angles using the Schureman formulae.
  ///
  /// @param[in] epoch Desired UTC time in seconds since 1970-01-01T00:00:00Z.
  FES_MATH_CONSTEXPR auto schureman_order3(const double epoch) noexcept -> void;

  /// Calculates the astronomic angles using the Meeus formulae.
  ///
  /// @param[in] epoch Desired UTC time in seconds since 1970-01-01T00:00:00Z.
  FES_MATH_CONSTEXPR auto meeus(const double epoch) noexcept -> void;

  /// Calculates the astronomic angles using the the International Earth
  /// Rotation and Reference Systems Service (IERS)
  ///
  /// @brief Calculates the lunisolar fundamental arguments using the IERS
  /// model.
  ///
  /// This method computes the fundamental astronomical arguments based on the
  /// Simon et al. (1994) model as recommended by the IERS Conventions (2010).
  ///
  /// @param[in] epoch Desired UTC time in seconds since 1970-01-01T00:00:00Z.
  /// @see IERS Conventions (2010) Chapter 5, Sections 5.7.1 - 5.7.2 (pp. 57-59)
  FES_MATH_CONSTEXPR auto iers(const double epoch) noexcept -> void;

  /// Pointer to the function that calculates the astronomic angles.
  void (Astronomic::*update_)(const double epoch) noexcept {
      &Astronomic::schureman_order1};
};

#ifndef DOXYGEN_SHOULD_SKIP_THIS

/// @brief Converts UTC to Terrestrial Dynamical Time (TDT).
/// @param[in] epoch UTC epoch
constexpr auto utc_2_tdt(const double epoch) -> double {
  return epoch + fetch_delta_time(epoch);
}

#endif

// /////////////////////////////////////////////////////////////////////////////
auto FES_MATH_CONSTEXPR
Astronomic::schureman_order1(const double epoch) noexcept -> void {
  auto reference = 25567.5;
  auto jc =
      ((epoch / static_cast<double>(numbers::kSecondsPerDay)) + reference) /
      static_cast<double>(numbers::kDaysPerCentury);

  // Schureman, P. 162

  // Longitude of moon's node (N)
  n_ = detail::math::horner(
      jc, detail::math::dms_to_degrees(259.0, 10.0, 57.12),
      -(5 * 360 + detail::math::dms_to_degrees(0.0, 0.0, 482'912.63)));
  // Mean longitude of sun (h)
  h_ = detail::math::horner(
      jc, detail::math::dms_to_degrees(279.0, 41.0, 48.04),
      detail::math::dms_to_degrees(0.0, 0.0, 129'602'768.13));
  s_ = detail::math::horner(
      jc, detail::math::dms_to_degrees(270.0, 26.0, 14.72),
      (1336 * 360 + detail::math::dms_to_degrees(0.0, 0.0, 1'108'411.20)));
  // Longitude of solar perigee (p₁)
  p1_ =
      detail::math::horner(jc, detail::math::dms_to_degrees(281.0, 13.0, 15.0),
                           detail::math::dms_to_degrees(0.0, 0.0, 6'189.03));
  // Longitude of lunar perigee (p)
  p_ = detail::math::horner(
      jc, detail::math::dms_to_degrees(334.0, 19.0, 40.87),
      (11 * 360 + detail::math::dms_to_degrees(0.0, 0.0, 392'515.94)));
}

// /////////////////////////////////////////////////////////////////////////////
auto FES_MATH_CONSTEXPR
Astronomic::schureman_order3(const double epoch) noexcept -> void {
  auto reference = 25567.5;
  auto jc =
      ((epoch / static_cast<double>(numbers::kSecondsPerDay)) + reference) /
      static_cast<double>(numbers::kDaysPerCentury);

  // Schureman, P. 162

  // Longitude of moon's node (N)
  n_ = detail::math::horner(
      jc, detail::math::dms_to_degrees(259.0, 10.0, 57.12),
      -(5 * 360 + detail::math::dms_to_degrees(0.0, 0.0, 482'912.63)),
      detail::math::dms_to_degrees(0.0, 0.0, 7.58),
      detail::math::dms_to_degrees(0.0, 0.0, 0.008));
  // Mean longitude of sun (h)
  h_ = detail::math::horner(
      jc, detail::math::dms_to_degrees(279.0, 41.0, 48.04),
      detail::math::dms_to_degrees(0.0, 0.0, 129'602'768.13),
      detail::math::dms_to_degrees(0.0, 0.0, 1.089));
  // Mean longitude of moon (s)
  s_ = detail::math::horner(
      jc, detail::math::dms_to_degrees(270.0, 26.0, 14.72),
      (1336 * 360 + detail::math::dms_to_degrees(0.0, 0.0, 1'108'411.20)),
      detail::math::dms_to_degrees(0.0, 0.0, 9.09),
      detail::math::dms_to_degrees(0.0, 0.0, 0.006'8));
  // Longitude of solar perigee (p₁)
  p1_ =
      detail::math::horner(jc, detail::math::dms_to_degrees(281.0, 13.0, 15.0),
                           detail::math::dms_to_degrees(0.0, 0.0, 6'189.03),
                           detail::math::dms_to_degrees(0.0, 0.0, 1.63),
                           detail::math::dms_to_degrees(0.0, 0.0, 0.012));
  // Longitude of lunar perigee (p)
  p_ = detail::math::horner(
      jc, detail::math::dms_to_degrees(334.0, 19.0, 40.87),
      (11 * 360 + detail::math::dms_to_degrees(0.0, 0.0, 392'515.94)),
      -detail::math::dms_to_degrees(0.0, 0.0, 37.24),
      -detail::math::dms_to_degrees(0.0, 0.0, 0.045));
}

// /////////////////////////////////////////////////////////////////////////////
FES_MATH_CONSTEXPR auto Astronomic::meeus(const double epoch) noexcept -> void {
  // Number of seconds elapsed since 2000-01-01T12:00:00Z (J2000) to epoch
  constexpr auto j2000 = 946728000.0;
  // Julian Ephemeris Millennium
  const auto jc = (utc_2_tdt(epoch) - j2000) / 3155760000.0;

  // Longitude of moon's node (N)
  // Jean Meeus, Astronomical Algorithms, 2nd ed., 1998., Willmann-Bell, Inc.
  // Formula 47.7, page 343.
  n_ = detail::math::horner(jc, 125.0445479, -1934.1362891, 0.0020754,
                            1.0 / 467441.0, -1.0 / 60616000.0);

  // Mean longitude of moon (s)
  // Jean Meeus, Astronomical Algorithms, 2nd ed., 1998., Willmann-Bell, Inc.
  // Formula 47.1, page 338.
  s_ = detail::math::horner(jc, 218.3164477, 481267.88123421, -0.0015786,
                            1.0 / 538841.0, -1.0 / 65194000.0);

  // Mean elongation of moon (d)
  // Jean Meeus, Astronomical Algorithms, 2nd ed., 1998., Willmann-Bell, Inc.
  // Formula 47.2, page 338.
  auto d = detail::math::horner(jc, 297.8501921, 445267.1114034, -0.0018819,
                                -1.0 / 545868.0, 1.0 / 113065000.0);

  // Mean longitude of sun (h)
  h_ = s_ - d;

  // Mean anomaly of sun (g)
  // Jean Meeus, Astronomical Algorithms, 2nd ed., 1998., Willmann-Bell, Inc.
  // Formula 47.3, page 338.
  auto g = detail::math::horner(jc, 357.5291092, 35999.0502909, -0.0001536,
                                1.0 / 24490000.0);

  // Longitude of solar perigee (p₁)
  p1_ = h_ - g;

  // Longitude of lunar perigee (p)
  // Jean Meeus, Astronomical Algorithms, 2nd ed., 1998., Willmann-Bell, Inc.
  // Formula 47.7, page 343.
  p_ = detail::math::horner(jc, 83.3532465, 4069.0137287, -0.0103200,
                            -1.0 / 80053.0, 1.0 / 18999000.0);
}

auto FES_MATH_CONSTEXPR Astronomic::iers(const double epoch) noexcept -> void {
  // Number of seconds elapsed since 2000-01-01T12:00:00Z (J2000) to epoch
  constexpr auto j2000 = 946728000.0;
  // Julian Ephemeris Millennium
  const auto jc = (utc_2_tdt(epoch) - j2000) / 3155760000.0;
  // Arcseconds in a full circle
  constexpr auto arcseconds_in_circle = 1296000.0;

  // Mean anomaly of the moon (L)
  auto l = detail::math::arcseconds2radians(
      std::remainder(detail::math::horner(jc, 485868.249036, 1717915923.2178,
                                          31.8792, 0.051635, 0.00024470),
                     arcseconds_in_circle));

  // Mean anomaly of the sun (LP)
  auto lp = detail::math::arcseconds2radians(
      std::remainder(detail::math::horner(jc, 1287104.79305, 129596581.0481,
                                          -0.5532, 0.000136, -0.00001149),
                     arcseconds_in_circle));

  // L - OM (Mean longitude of the ascending node of the moon)
  auto f = detail::math::arcseconds2radians(
      std::remainder(detail::math::horner(jc, 335779.526232, 1739527262.8478,
                                          -12.7512, -0.001037, 0.00000417),
                     arcseconds_in_circle));

  // Mean elongation of the moon from the sun (D)
  auto d = detail::math::arcseconds2radians(
      std::remainder(detail::math::horner(jc, 1072260.70369, 1602961601.2090,
                                          -6.3706, 0.006593, -0.00003169),
                     arcseconds_in_circle));

  // Mean longitude of the ascending node of the moon (OM)
  auto omega = detail::math::arcseconds2radians(
      std::remainder(detail::math::horner(jc, 450160.398036, -6962890.5431,
                                          7.4722, 0.007702, -0.00005939),
                     arcseconds_in_circle));

  // mean longitude of the moon
  s_ = detail::math::degrees(f + omega);

  // mean longitude of the sun
  h_ = detail::math::degrees(f + omega - d);

  // longitude of lunear perigee (P)
  p_ = detail::math::degrees(f + omega - l);

  // Longitude of solar perigee (p₁)
  p1_ = detail::math::degrees(-lp + f - d + omega);

  // Longitude of the moon's node (N)
  n_ = detail::math::degrees(omega);
}

auto FES_MATH_CONSTEXPR Astronomic::update(const double epoch) noexcept
    -> void {
  ((*this).*update_)(epoch);

  // T mean solar angle relative to Greenwich
  t_ = std::remainder(
      180.0 + 15.0 * (std::fmod(epoch, numbers::kSecondsPerDay) / 3600), 360.0);

  // Normalize angles to [0, 2π)
  t_ = detail::math::radians(t_);
  n_ = detail::math::radians(detail::math::normalize_angle(n_, 0.0));
  s_ = detail::math::radians(detail::math::normalize_angle(s_, 0.0));
  h_ = detail::math::radians(detail::math::normalize_angle(h_, 0.0));
  p_ = detail::math::radians(detail::math::normalize_angle(p_, 0.0));
  p1_ = detail::math::radians(detail::math::normalize_angle(p1_, 0.0));

  // SCHUREMAN FORMULAE P. 156
  auto u = numbers::kCosICosW - numbers::kSinISinW * std::cos(n_);

  // Inclination of the moon's orbit to the celestial equator
  i_ = std::acos(u);

  // SCHUREMAN FORMULAE P. 156
  auto tgn2 = std::tan(n_ * 0.5);
  auto at1 = std::atan(numbers::kTan1 * tgn2);
  auto at2 = std::atan(numbers::kTan2 * tgn2);

  // Longitude in the moon's orbit of lunar intersection
  xi_ = -at1 - at2 + n_;

  if (n_ > detail::math::pi<double>()) {
    xi_ -= detail::math::two_pi<double>();
  }

  // Right ascension of lunar intersection
  nu_ = at1 - at2;

  // for constituents l2, k1, k2
  auto tgi2 = detail::math::pow<2>(std::tan(i_ * 0.5));

  // SCHUREMAN P. 41 (191)
  // Mean longitude  of the lunar perigee reckoned from the lunar intersection
  auto p = p_ - xi_;

  // SCHUREMAN P. 44 (213)
  x1ra_ = std::sqrt(1.0 + tgi2 * (36.0 * tgi2 - 12.0 * std::cos(2.0 * p)));

  // SCHUREMAN P. 41 (196)
  r_ = std::atan(std::sin(2.0 * p) / (1.0 / (6.0 * tgi2) - std::cos(2.0 * p)));

  // SCHUREMAN P. 45 (224)
  nuprim_ = std::atan(std::sin(2.0 * i_) * std::sin(nu_) /
                      (std::sin(2.0 * i_) * std::cos(nu_) + numbers::k224));

  // SCHUREMAN P. 46 (232)
  nusec_ =
      0.5 *
      std::atan((detail::math::pow<2>(std::sin(i_)) * std::sin(2.0 * nu_)) /
                (detail::math::pow<2>(std::sin(i_)) * std::cos(2.0 * nu_) +
                 numbers::k232));
}

}  // namespace angle
}  // namespace fes
