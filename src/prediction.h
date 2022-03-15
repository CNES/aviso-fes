/**
 @file prediction.h
 @brief Tide computation.
 @author CLS
 */

/* This file is part of FES library.

   FES is free software: you can redistribute it and/or modify
   it under the terms of the GNU LESSER GENERAL PUBLIC LICENSE as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.

   FES is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU LESSER GENERAL PUBLIC LICENSE for more details.

   You should have received a copy of the GNU LESSER GENERAL PUBLIC LICENSE
   along with FES.  If not, see <http://www.gnu.org/licenses/>.
*/

#pragma once
#include "fes_int.h"

/**
 @brief Compute waves by admittance from these 7 major ones : O1, Q1, K1, 2n2,
 N2, M2, K2.

 @param waves Pointer to the array which contains waves definition.
 */
void
admittance(fes_wave* const waves);

/**
 @brief Compute nodal corrections.

 @param t0 Desired UTC time, in (decimal) Modified.
 @param w Pointer to the array which contains waves definition.
 */
void
compute_nodal_corrections(const double t0, fes_wave* const w);

/**
 @brief Compute the elapsed time since 01/01/1900 0:0:0.0 in julian centuries.

 @param date Modified Julian day (days since 1950-01-01 00:00:00.000 UTC).

 @return The julian centuries.
 */
double
julian_centuries(const double date);

/**
 @brief Set wave order 2 coefficients values to compute the long-period
 equilibrium ocean tides.

 @param w Pointer to the array which contains waves definition.
 @param w2nd Pointer to the array which contains waves order 2 definition.
 */
void
set_w2nd(const fes_wave* const w, float w2nd[][N_COEFS]);

/**
 @brief Set wave order 3 coefficients values to compute the long-period
 equilibrium ocean tides.

 @param w Pointer to the array which contains waves definition.
 @param w3rd Pointer to the array which contains waves order 3 definition.
 */
void
set_w3rd(const fes_wave* const w, float w3rd[][N_COEFS]);

/**
 @brief Computes the long-period equilibrium ocean tides.

 The complete tidal spectral lines from the Cartwright-Tayler-Edden
 tables are summed over to compute the long-period tide.
 @n
 Order 2 and order 3 of the tidal potential for the long period waves is
 now taken into account.
 @n
 The decomposition was validated compared to the potential proposed by
 Tamura.
 @n
 Waves computed dynamically are removed.

 Technical references:
 - Cartwright & Tayler, Geophys. J. R.A.S., 23, 45, 1971.
 - Cartwright & Edden, Geophys. J. R.A.S., 33, 253, 1973.
 - Tamura Y., Bull. d'information des marees terrestres, Vol. 99, 1987.

 @param w2nd Pointer to the array which contains waves order 2 definition.
 @param w3rd Pointer to the array which contains waves order 3 definition.
 @param ts Julian day, in seconds, denoting time at which tide is to be
 computed.
 @param lat Latitude in degrees (positive north) for the position at which
 tide is computed.
 @param tlp Computed long-period tide, in centimeters.
 */
void
lpe_minus_n_waves(const float w2nd[][N_COEFS],
                  const float w3rd[][N_COEFS],
                  const double ts,
                  const double lat,
                  double* tlp);

/**
 @brief Set Waves properties.

 @param w Pointer to the destination array which contains N_WAVES items.
 */
void
set_waves(fes_wave* const w);
