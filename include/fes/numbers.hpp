// Copyright (c) 2026 CNES
//
// All rights reserved. Use of this source code is governed by a
// BSD-style license that can be found in the LICENSE file.
/// @file include/fes/numbers.hpp
/// @brief Constants used in the FES library.
#pragma once

#include <cstdint>

namespace fes {
namespace numbers {

/// @brief Set to true to use constants from Schureman 1958 instead of those
/// from the IERS 2010 conventions.
#ifndef FES_USE_IERS_CONSTANTS
constexpr auto use_schureman_constants = true;
#else
constexpr auto use_schureman_constants = false;
#endif

/// @brief The Modified Julian Date epoch in seconds.
constexpr double kModifiedJulianEpoch = 2400000.5;

/// @brief The number of seconds in a day.
constexpr int64_t kSecondsPerDay = 86400;

/// @brief The number of microseconds in a second.
constexpr int64_t kMicrosecondsPerSecond = 1000000;

/// @brief The number of days in a century.
constexpr int64_t kDaysPerCentury = 36525;

/// @brief Julian Day Number for the J2000.0 epoch.
constexpr int64_t kJ2000JulianDay = 2451545;

/// @brief Julian Day Number for the Unix epoch (1970-01-01T00:00:00Z).
constexpr double kUnixEpochJulianDay = 2440587.5;

/// @brief Mean value of the obliquity factor for the lunar long-period
/// constituent Mm
/// Formulae 65 & 73 (P. 24, 25)
constexpr auto k65 = use_schureman_constants ? 0.5021 : 0.502306;

/// @brief Mean obliquity factor for the lunar long-period constituent Mf
/// Formulae 66 & 74 (P. 24, 25)
constexpr auto k66 = use_schureman_constants ? 0.1578 : 0.157590;

/// @brief Mean obliquity factor for the lunar diurnal constituent O1
/// Formulae 67 & 75 (P. 25)
constexpr auto k67 = use_schureman_constants ? 0.3800 : 0.379830;

/// @brief Mean obliquity factor for the lunar diurnal constituent J1
/// Formulae 68 & 76 (P. 25)
constexpr auto k68 = use_schureman_constants ? 0.7214 : 0.721100;

/// @brief Mean obliquity factor for the lunar diurnal constituent OO1
/// Formulae 69 & 77 (P. 25)
constexpr auto k69 = use_schureman_constants ? 0.0164 : 0.016346;

/// @brief Mean obliquity factor for the principal lunar semidiurnal constituent
/// M2 Formulae 70 & 78 (P. 25)
constexpr auto k70 = use_schureman_constants ? 0.9154 : 0.915484;

/// @brief Mean obliquity factor used for several semidiurnal constituents
/// Formulae 71 & 79 (P. 25)
constexpr auto k71 = use_schureman_constants ? 0.1565 : 0.156318;

/// @brief Mean obliquity factor for certain long-period constituents derived
/// from the 4th power of the moon’s parallax.
/// Formulae 141 & 137 (P. 35, 36)
constexpr auto k141 = use_schureman_constants ? 0.3192 : 0.319103;

/// @brief Mean obliquity factor for certain diurnal constituents (like M1)
/// derived from the 4th power of the moon’s parallax.
/// Formulae 144 & 138 (P. 35, 36)
constexpr auto k144 = use_schureman_constants ? 0.5873 : 0.587656;

/// @brief Mean obliquity factor for certain semidiurnal constituents derived
/// from the 4th power of the moon’s parallax.
/// Formulae 146 & 139 (P. 35, 36)
constexpr auto k146 = use_schureman_constants ? 0.3658 : 0.365630;

/// @brief Another mean obliquity factor for semidiurnal constituents.
/// Formulae 147 & 139 (P. 35, 36)
constexpr auto k147 = use_schureman_constants ? 0.1114 : 0.111387;

/// @brief Node factor for the terdiurnal constituent M3
/// Formula 149 (P. 36)
constexpr auto k149 = use_schureman_constants ? 0.8758 : 0.875944;

/// @brief First component for the amplitude factor 1/Qa of the M1 tide
/// Formula 197 (P. 41)
constexpr auto k197_1 = use_schureman_constants ? 2.310 : 2.310512;
/// @brief Second component for the amplitude factor 1/Qa of the M1 tide
/// Formula 197 (P. 41)
constexpr auto k197_2 = use_schureman_constants ? 1.435 : 1.435448;

/// @brief First coefficient for the terms that combine to form the lunisolar
/// constituents K1 and K2.
/// Formulae 216-219 (P. 45)
constexpr auto k216 = use_schureman_constants ? 0.5023 : 0.502261;
/// @brief Second coefficient for the terms that combine to form the lunisolar
/// constituents K1 and K2.
/// Formulae 216-219 (P. 45)
constexpr auto k217 = use_schureman_constants ? 0.1681 : 0.167675;
/// @brief Third coefficient for the terms that combine to form the lunisolar
/// constituents K1 and K2.
/// Formulae 216-219 (P. 45)
constexpr auto k218 = use_schureman_constants ? 0.5023 : 0.502261;
/// @brief Fourth coefficient for the terms that combine to form the lunisolar
/// constituents K1 and K2.
/// Formulae 216-219 (P. 45)
constexpr auto k219 = use_schureman_constants ? 0.0365 : 0.036348;

/// @brief Ratio of the solar coefficient to the lunar coefficient for the
/// K1 tide.
/// Formulae 224 (P. 45)
constexpr auto k224 = use_schureman_constants ? 0.3347 : 0.333840;

/// @brief Mean value of the K1 coefficient.
/// Formula 226 (P. 45)
constexpr auto k226 = use_schureman_constants ? 0.5305 : 0.529855;

/// @brief First component for the node factor K1
/// Formula 227 (P. 45)
constexpr auto k227_1 = use_schureman_constants ? 0.8965 : 0.898554;
/// @brief Second component for the node factor K1
/// Formula 227 (P. 45)
constexpr auto k227_2 = use_schureman_constants ? 0.6001 : 0.599946;
/// @brief Third component for the node factor K1
/// Formula 227 (P. 45)
constexpr auto k227_3 = use_schureman_constants ? 0.1006 : 0.100143;

/// @brief Ratio of the solar coefficient to the lunar coefficient factor for
/// the K2 tide
/// Formulae 232 (P. 46)
constexpr auto k232 = use_schureman_constants ? 0.0727 : 0.072369;

/// @brief Mean value of the K2 coefficient
/// Formula 234 (P. 46)
constexpr auto k234 = use_schureman_constants ? 0.1151 : 0.114860;

/// @brief First component for the node factor K2
/// Formula 235 (P. 46)
constexpr auto k235_1 = use_schureman_constants ? 19.0444 : 19.121423;
/// @brief Second component for the node factor K2
/// Formula 235 (P. 46)
constexpr auto k235_2 = use_schureman_constants ? 2.7702 : 2.767581;
/// @brief Third component for the node factor K2
/// Formula 235 (P. 46)
constexpr auto k235_3 = use_schureman_constants ? 0.0981 : 0.100143;

/// @brief @f$\sin(I) \sin(\omega)@f$
/// Formulae Page 156
constexpr auto kCosICosW = use_schureman_constants ? 0.91370 : 0.913785;
/// @brief @f$\sin(I) \cos(\omega)@f$
/// Formulae Page 156
constexpr auto kSinISinW = use_schureman_constants ? 0.03569 : 0.035674;
/// @brief @f$\tan \frac{1}{2} \left(N - \xi + \nu\right)@f$
/// Formulae Page 156
constexpr auto kTan1 = use_schureman_constants ? 1.01883 : 1.018818;
/// @brief @f$\tan^2 \frac{1}{2} \left(N - \xi - \nu\right)@f$
/// Formulae Page 156
constexpr auto kTan2 = use_schureman_constants ? 0.64412 : 0.643932;

}  // namespace numbers
}  // namespace fes
