/**
 @file angle.h
 @brief Compute the fundamental frequencies in the earth-moon-sun motion.
 @author CLS
 */

/* This file is part of FES library.

   FES is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.

   FES is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with FES.  If not, see <http://www.gnu.org/licenses/>.
*/

#pragma once

/**
 @def JULIAN_CENTURY
 @brief Number of seconds in one Julian century
 */
#define JULIAN_CENTURY 3155760000.0

/**
 @brief Compute the speed in degree by hour for the moon's mean
 longitude (s)
 */
double
s_speed();

/**
 @brief Compute the speed in degree by hour for the sun's mean
 longitude (h)
 */
double
h_speed();

/**
 @brief Compute the speed in degree by hour for the longitude of
 moon's perigee (p)
 */
double
p_speed();

/**
 @brief Compute the speed in degree by hour for the longitude of
 moon's ascending node (N′)
 */
double
n_speed();

/**
 @brief Compute the speed in degree by hour for the longitude of
 sun's perigee (p₁)
 */
double
p1_speed();

/**
 @brief Compute the speed in degree by hour for the local mean
 lunar time (τ)
 */
double
tau_speed();

/**
 @brief Compute the frequency in degree by hour for the moon's mean
 longitude (s)
 */
double
s_frequency();

/**
 @brief Compute the frequency in degree by hour for the sun's mean
 longitude (h)
 */
double
h_frequency();

/**
 @brief Compute the frequency in degree by hour for the longitude of
 moon's perigee (p)
 */
double
p_frequency();

/**
 @brief Compute the frequency in degree by hour for the longitude of
 moon's ascending node (N′)
 */
double
n_frequency();

/**
 @brief Compute the frequency in degree by hour for the longitude of
 sun's perigee (p₁)
 */
double
p1_frequency();

/**
 @brief Compute the frequency in degree by hour for the local mean
 lunar time (τ)
 */
double
tau_frequency();
