/**
 @file fes.h
 @brief Main routines for the FES prediction software.
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
#ifdef __cplusplus
extern "C"
{
#endif
#include <stdlib.h>

/// Library version
#define FES_VERSION "2.9.4"
  /**
   @brief Possible type of access.
   */
  typedef enum
  {
    /** Direct access (Grids are not loaded into memory). */
    FES_IO = 0,
    /** Memory access (Grids are loaded into memory). */
    FES_MEM
  } fes_enum_access;

  /**
   @brief Possible type of tide.
   */
  typedef enum
  {
    /** Ocean tide. */
    FES_TIDE = 0,
    /** Radial tide. */
    FES_RADIAL
  } fes_enum_tide_type;

  /**
   @brief The result codes that can be returned by functions in the FES C SDK
   */
  typedef enum
  {
    /** No error reporting */
    FES_SUCCESS,
    /** Not enough memory */
    FES_NO_MEMORY,
    /** netCDF error */
    FES_NETCDF_ERROR,
    /** IO error */
    FES_IO_ERROR,
    /** Invalid configuration file */
    FES_INI_ERROR,
    /** No data available in grids for the location asked */
    FES_NO_DATA,
    /** Function receives an argument that has the right
     type but an inappropriate value */
    FES_VALUE_ERROR
  } fes_enum_error;

  /**
   @brief Fes handler
   */
  typedef void* FES;

  /**
   @brief Initializes the computation of the tide.

   @param handle FES data handler.
   @param tide Computation mode. If mode is equals to FES_TIDE, the core
   function computes the tide otherwise she computes the
   radial tide.
   @param mode One of FES_MEM, FES_IO which request loading grids into
   memory or direct access from NetCDF grids.
   @param path Path to the configuration file.

   @return 0 on success or 1 on failure.

   @note The user can set the size of the buffer when the FES_IO mode is used,
   with the FES_BUFFER_SIZE environment variable. This variable must define a
   positive integer representing the size of the memory in Mb.
   */
  int fes_new(FES* handle,
              const fes_enum_tide_type tide,
              const fes_enum_access mode,
              const char* const path);

  /**
   @brief Sets the size of the buffer

   @param handle FES data handler.
   @param size Size of the buffer in MB

   @return 0 on success or 1 on failure.
   */
  int fes_set_buffer_size(FES handle, const size_t size);

  /**
   @brief Frees the memory used.

   @param handle FES data identifier
   */
  void fes_delete(FES handle);

  /**
   @brief Driver for tidal computation.

   @param handle FES handler.
   @param lat Latitude in degrees (positive north) for the position
   at which tide is computed.
   @param lon Longitude in degrees for the position at which tide is
   computed.
   @param time Julian day (days since 1950-01-01 00:00:00.000 UTC).
   @param h Computed height of the diurnal and semi-diurnal
   constituents of the tidal spectrum (in centimeters).
   @param h_long_period Computed height of the long period wave constituents of
   the tidal spectrum (in centimeters).

   @return 0 on success or 1 on failure.

   @note h is set to nan if no data is available at the given position.
   h_long_period is always computed because this value does not depend
   on input grids.
   */
  int fes_core(FES handle,
               const double lat,
               const double lon,
               const double time,
               double* h,
               double* h_long_period);

  /**
   @brief Return the minimum number of points used to interpolate the tidal
   waves for the asked position

   @param handle FES handler.

   @return The minimum number of points used during the interpolation
   */
  int fes_min_number(FES handle);

  /**
   @brief Returns the last error code.

   @param fes FES handler

   @return the \link #fes_enum_error error code\endlink.
   */
  fes_enum_error fes_errno(FES fes);

  /**
   @brief Returns the message error.

   @param fes FES handler

   @return the message error.
   */
  const char* fes_error(FES fes);

  /**
   @brief Dump the template of the configuration file that the library uses.

   @return 0 on success otherwise 1 on error
   */
  int fes_dump_template(const char* path);

#ifdef __cplusplus
}
#endif
