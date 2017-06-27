/**
 @file interp.h
 @brief Interpolation functions.
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

/**
 @brief Interpolate a value at a given point.

 @param x_1 X-coordinate X1
 @param x_2 X-coordinate X2
 @param y_1 Y-coordinate Y1
 @param y_2 Y-coordinate Y2
 @param value_11 Value of the point (X1, Y1)
 @param value_21 Value of the point (X2, Y1)
 @param value_12 Value of the point (X1, Y2)
 @param value_22 Value of the point (X2, Y2)
 @param x X-coordinate of the point where the interpolation is carried out
 @param y Y-coordinate of the point where the interpolation is carried out
 @param z The interpolated value at the given point.

 @return The number of points used in the interpolation
 */
int
bilinear_interp(const double x_1, const double x_2, const double y_1,
                const double y_2, const double value_11, const double value_21,
                const double value_12, const double value_22, const double x,
                const double y, double* z);
