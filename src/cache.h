/**
 @file cache.h
 @brief Least Recently Used (LRU) cache
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
 @brief Allocate memory to store a new item in the cache.

 @param grid Grid
 @param index Index of the grid to be stored in the buffer.

 @return The item allocated or null if an error was detected.
 */
fes_cache_item*
fes_new_cache_item(fes_grid* const grid, const size_t index);

/**
 @brief Free memory used by an input cache.

 @param ptr Item to free
 */
void
fes_delete_cache_item(void* ptr);

/**
 @brief Get value stored in the buffer

 @param grid Grid
 @param i_lon Index of the longitude in the grid.
 @param i_lat Index of the latitude in the grid.
 @param n Index of the grid file.
 @param value Value read.

 @return 1 if the value was read from the cache otherwise 0.
 */
int
fes_get_cache(fes_grid* const grid, const size_t i_lon, const size_t i_lat,
              const size_t n, fes_double_complex* value);

/**
 @brief Store a value read in the buffer

 @param grid Grid
 @param i_lon Index of the longitude in the grid.
 @param i_lat Index of the latitude in the grid.
 @param n Index of the grid file.
 @param value Value to store.

 @return 1 if an error was thrown otherwise 0
 */
int
fes_set_cache(fes_grid* const grid, const size_t i_lon, const size_t i_lat,
              const size_t n, const fes_double_complex* value);
