// Copyright (c) 2025 CNES
//
// All rights reserved. Use of this source code is governed by a
// BSD-style license that can be found in the LICENSE file.
/// @file include/fes/numbers.hpp
/// @brief Constants used in the FES library.
#pragma once

namespace fes {
namespace numbers {
/// @brief Mean value of the obliquity factor for the lunar long-period
/// constituent Mm
/// Formulae 65 & 73 (P. 24, 25)
constexpr auto k65 = 0.5021;

/// @brief Mean obliquity factor for the lunar long-period constituent Mf
/// Formulae 66 & 74 (P. 24, 25)
constexpr auto k66 = 0.1578;

/// @brief Mean obliquity factor for the lunar diurnal constituent O1
/// Formulae 67 & 75 (P. 25)
constexpr auto k67 = 0.3800;

/// @brief Mean obliquity factor for the lunar diurnal constituent J1
/// Formulae 68 & 76 (P. 25)
constexpr auto k68 = 0.7214;

/// @brief Mean obliquity factor for the lunar diurnal constituent OO1
/// Formulae 69 & 77 (P. 25)
constexpr auto k69 = 0.0164;

/// @brief Mean obliquity factor for the principal lunar semidiurnal constituent
/// M2 Formulae 70 & 78 (P. 25)
constexpr auto k70 = 0.9154;

/// @brief Mean obliquity factor used for several semidiurnal constituents
/// Formulae 71 & 79 (P. 25)
constexpr auto k71 = 0.1565;

/// @brief Mean obliquity factor for certain long-period constituents derived
/// from the 4th power of the moon’s parallax.
/// Formulae 141 & 137 (P. 35, 36)
constexpr auto k141 = 0.3192;

/// @brief Mean obliquity factor for certain diurnal constituents (like M1)
/// derived from the 4th power of the moon’s parallax.
/// Formulae 144 & 138 (P. 35, 36)
constexpr auto k144 = 0.5873;

/// @brief Mean obliquity factor for certain semidiurnal constituents derived
/// from the 4th power of the moon’s parallax.
/// Formulae 146 & 139 (P. 35, 36)
constexpr auto k146 = 0.3658;

/// @brief Another mean obliquity factor for semidiurnal constituents.
/// Formulae 147 & 139 (P. 35, 36)
constexpr auto k147 = 0.1114;

/// @brief Node factor for the terdiurnal constituent M3
/// Formula 149 (P. 36)
constexpr auto k149 = 0.8758;

/// @brief components for the amplitude factor 1/Qa of the M1 tide
/// Formula 197 (P. 41)
constexpr auto k197_1 = 2.310;
constexpr auto k197_2 = 1.435;

/// @brief Coefficients for the terms that combine to form the lunisolar
/// constituents K1 and K2.
/// Formulae 216-219 (P. 45)
constexpr auto k216 = 0.5023;
constexpr auto k217 = 0.1681;
constexpr auto k218 = 0.5023;
constexpr auto k219 = 0.0365;

/// @brief Ratio of the solar coefficient to the lunar coefficient for the
/// K1 tide.
/// Formulae 224 (P. 45)
constexpr auto k224 = 0.3347;

/// @brief Mean value of the K1 coefficient.
/// Formula 226 (P. 45)
constexpr auto k226 = 0.5305;

/// @brief Components for the node factor K1
/// Formula 227 (P. 45)
constexpr auto k227_1 = 0.8965;
constexpr auto k227_2 = 0.6001;
constexpr auto k227_3 = 0.1006;

/// @brief Ratio of the solar coefficient to the lunar coefficient factor for
/// the K2 tide
/// Formulae 232 (P. 46)
constexpr auto k232 = 0.0727;

/// @brief Mean value of the K2 coefficient
/// Formula 234 (P. 46)
constexpr auto k234 = 0.1151;

/// @brief Components for the node factor K2
/// Formula 235 (P. 46)
constexpr auto k235_1 = 19.0444;
constexpr auto k235_2 = 2.7702;
constexpr auto k235_3 = 0.0981;

/// @brief Formulae from Page 156
constexpr auto kCosICosW = 0.91370;
constexpr auto kSinISinW = 0.03569;
constexpr auto kTan1 = 1.01883;
constexpr auto kTan2 = 0.64412;

}  // namespace numbers
}  // namespace fes
