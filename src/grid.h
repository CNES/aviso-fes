/**
 @file grid.h
 @brief Grid management.
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
 @brief Perform bilinear interpolation at asked point from grids

 @param fes Internal data needed to interpolate.
 @param lat Latitude asked
 @param lon Longitude asked

 @return 0 on success or 1 on failure.
 */
int
interp(fes_handler* fes, const double lat, const double lon);

/**
 @brief Read waves definition from netCDF grids files.

 @param path Absolute or relative file name for netCDF file to be opened.
 @param n Index of the current grid
 @param nc Properties of netCDF read
 @param fes Properties of grid read

 @return 0 on success or 1 on failure.
 */
int
load_grid(const char* const path, const unsigned int n, fes_cdf_file* const nc,
          fes_handler* const fes);
