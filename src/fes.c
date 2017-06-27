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

#include "fes.h"

#include <limits.h>
#include <math.h>
#include <netcdf.h>
#include <stddef.h>
#include <stdio.h>
#include <string.h>
#include <uthash.h>

#include "cache.h"
#include "compat.h"
#include "dlist.h"
#include "error.h"
#include "fes_int.h"
#include "grid.h"
#include "ini.h"
#include "prediction.h"

/* Default variable name for the amplitude data */
#define AMPLTIUDE "amplitude"

/* Default variable name for the phase data */
#define PHASE "phase"

/* Default value for the phase lag */
#define PHASE_LAG 0

/* Default variable and dimension name for latitudes */
#define LATITUDE "latitude"

/* Default variable and dimension name for longitude */
#define LONGITUDE "longitude"

/* Default size of the buffer */
#define BUFFER_SIZE 64

/*
 _get_key

 Get the key name in the parameter file

 type Type of tide
 name Wave name
 key Key

 Returns a pointer to a static string that contains the key name.
 */
static char*
_get_key(const fes_enum_tide_type tide, const char* const name,
         const char* const key) {
  static char buffer[MAX_PATH];

  snprintf(buffer, sizeof(buffer), "%s_%s_%s",
           tide == FES_RADIAL ? "RADIAL" : "TIDE", name, key);

  return buffer;
}

/*
 _get_env

 Read the environment variable defining the size of the buffer

 Returns the buffer size on success or 0 on failure.
 */
static size_t _get_env(fes_handler* handle) {
  char* env;
  long int value;

  /* Read the buffer size from the environment variable */
#ifdef _WIN32
  if ((env = duplicate_env("FES_BUFFER_SIZE")) != NULL) {
#else
  if ((env = getenv("FES_BUFFER_SIZE")) != NULL) {
#endif
    char* ptr;
    value = strtol(env, &ptr, 10);

    if (ptr == env || value == LONG_MIN || value == LONG_MAX || value < 0) {
      set_fes_extended_error(
          handle, FES_VALUE_ERROR,
          "FES_BUFFER_SIZE define and invalid memory size: %s", env);
      value = 0;
    }
  }
  /* Otherwise return the default size */
  else {
    value = BUFFER_SIZE;
  }
#ifdef _WIN32
  free(env);
#endif
  return (size_t) value;
}

/*
 */
int fes_new(FES* handle, const fes_enum_tide_type tide,
            const fes_enum_access mode, const char* const path) {
  int rc = 0;
  int ix;
  unsigned int n = 0;
  size_t size;
  fes_handler* fes;
  void* ini = NULL;

  /* Allocate handle */
  if ((fes = (fes_handler*) calloc(1, sizeof(fes_handler))) == NULL) {
    goto error;
  }

  /* set handle value */
  *handle = fes;

  /* Set waves properties */
  set_waves(fes->waves);

  /* Set type of tide */
  fes->type = tide;

  /* Open configuration file */
  if (ini_open(fes, path, &ini))
    goto error;

  /* Determines the number of grid has to be loaded into memory */
  for (ix = 0; ix < N_WAVES; ++ix) {
    /* Get key name for current wave */
    if (ini_get_string(ini, _get_key(fes->type, fes->waves[ix].name, "FILE"),
    NULL) != NULL)
      fes->grid.n_grids++;
  }

  if (fes->grid.n_grids == 0) {
    set_fes_extended_error(fes, FES_INI_ERROR,
                           "The configuration file defines no %s wave",
                           tide == FES_RADIAL ? "radial" : "tide");
    goto error;
  }

  /* Allocate grids buffer */
  if (mode == FES_IO) {
    /* to read data from grids */
    if ((fes->grid.file = (fes_cdf_file*) calloc(fes->grid.n_grids,
                                                 sizeof(fes_cdf_file))) == NULL) {
      set_fes_error(fes, FES_NO_MEMORY);
      goto error;
    }

    if ((fes->grid.buffer = (fes_buffer*) calloc(1, sizeof(fes_buffer))) == NULL) {
      set_fes_error(fes, FES_NO_MEMORY);
      goto error;
    }

    /* Read the size of the buffer */
    if ((size = _get_env(fes)) == 0)
      goto error;

    fes->grid.buffer->values = NULL;
    if (fes_set_buffer_size(fes, size))
      goto error;

    dlist_init(&fes->grid.buffer->list, fes_delete_cache_item);
  }

  if (mode == FES_MEM) {
    /* to read data from memory */
    if ((fes->grid.values = (fes_float_complex**) calloc(
        fes->grid.n_grids, sizeof(fes_float_complex*))) == NULL) {
      set_fes_error(fes, FES_NO_MEMORY);
      goto error;
    }
  }

  if ((fes->grid.waveIndex = (int*) calloc(fes->grid.n_grids, sizeof(int)))
      == NULL) {
    set_fes_error(fes, FES_NO_MEMORY);
    goto error;
  }

  /* Loading grids */
  for (ix = 0; ix < N_WAVES; ++ix) {
    fes_cdf_file file;
    const char* filename = ini_get_string(
        ini, _get_key(fes->type, fes->waves[ix].name, "FILE"), NULL);

    if (filename == NULL) {
      /* Wave computed by admittance or not computed */
      fes->waves[ix].dynamic = 0;
      continue;
    }

    /* Wave computed dynamically */
    fes->waves[ix].dynamic = 1;

    /* If the data are read from a grid so the wave is not computed by
     admittance. */
    fes->waves[ix].admittance = 0;

    /* Reading the name of the variable who contains the latitudes. */
    STRNCPY(
        file.lat,
        ini_get_string(ini,
                       _get_key(fes->type, fes->waves[ix].name, "LATITUDE"),
                       LATITUDE),
        sizeof(file.lat));

    /* Reading the name of the variable who contains the longitude. */
    STRNCPY(
        file.lon,
        ini_get_string(ini,
                       _get_key(fes->type, fes->waves[ix].name, "LONGITUDE"),
                       LONGITUDE),
        sizeof(file.lon));

    /* Reading the name of the variable who contains the amplitudes. */
    STRNCPY(
        file.amp,
        ini_get_string(ini,
                       _get_key(fes->type, fes->waves[ix].name, "AMPLITUDE"),
                       AMPLTIUDE),
        sizeof(file.amp));

    /* Reading the name of the variable who contains the phases. */
    STRNCPY(
        file.pha,
        ini_get_string(ini, _get_key(fes->type, fes->waves[ix].name, "PHASE"),
        PHASE),
        sizeof(file.pha));

    /* Grids contains phase lag ? */
    file.phase_lag = ini_get_integer(
        ini, _get_key(fes->type, fes->waves[ix].name, "PHASE_LAG"),
        PHASE_LAG);

    /* loading netCDF grid */
    if ((rc = load_grid(filename, n, &file, fes)) != 0)
      goto error;

    fes->grid.waveIndex[n] = ix;

    ++n;
  }

  /* Set wave order 2 to compute long-period equilibrium ocean tides */
  set_w2nd(fes->waves, fes->w2nd);

  /* Initializes the information of the cell containing the last data read
   * from the netCDF grid. This information is used to not always search the
   * properties to the cell of the matrix containing the data to interpolate.
   */
  fes->west_lon = fes->east_lon = fes->south_lat = fes->north_lat = nan("NaN");

  goto finish;

  error:
    if (rc == 0)
      rc = 1;

  finish:
    ini_close(ini);
    return rc;
}

int fes_set_buffer_size(FES handle, const size_t size) {
  fes_handler* fes = (fes_handler *) handle;

  if (fes->grid.buffer == NULL) {
    set_fes_extended_error(fes, FES_VALUE_ERROR,
                           "The buffer is not initialized");
    return 1;
  }

  if (size < BUFFER_SIZE) {
    set_fes_extended_error(fes, FES_VALUE_ERROR,
                           "The buffer size must be >= %d Mb",
                           BUFFER_SIZE);
    return 1;
  }

  fes->grid.buffer->max_size = ((size * 1024 * 1024)
      / (sizeof(fes_double_complex) * fes->grid.n_grids));
  fes->grid.buffer->max_size = (fes->grid.buffer->max_size / 8) * 8;
  return 0;
}

/*
 */
void fes_delete(FES handle) {
  size_t ix;
  fes_handler* fes = (fes_handler *) handle;

  if (fes != NULL) {
    for (ix = 0; ix < fes->grid.n_grids; ++ix) {
      if (fes->grid.file != NULL)
        nc_close(fes->grid.file[ix].id);

      if (fes->grid.values != NULL)
        free(fes->grid.values[ix]);
    }
    if (fes->grid.buffer != NULL) {
      fes_cache_item *current_item, *tmp;
      HASH_ITER (hh, fes->grid.buffer->values, current_item, tmp)
        HASH_DEL(fes->grid.buffer->values, current_item);
      dlist_destroy(&fes->grid.buffer->list);
    }

    free(fes->grid.buffer);
    free(fes->grid.file);
    free(fes->grid.values);
    free(fes->grid.waveIndex);
    free(fes);
  }
}

/*
 */
int fes_core(FES handle, const double lat, const double lon, const double time,
             double* h, double* h_long_period) {
  int ix;
  double phi;
  double t1 = julian_centuries(time);
  double delta;
  fes_handler* fes = (fes_handler *) handle;

  delta = (t1 - fes->nodal_time) * 3.6525E+04 * 24.0;

  if (fabs(delta) > 24.0) {
    compute_nodal_corrections(t1, fes->waves);

    fes->nodal_time = t1;
    delta = 0.0;
  }

  if (interp(fes, lat, lon))
    return 1;

  admittance(fes->waves);

  if (fes->is_data) {
    *h = 0.0;

    if (fes->type == FES_TIDE) {
      lpe_minus_n_waves((const float (*)[N_COEFS]) (fes->w2nd), time, lat,
                        h_long_period);
    } else
      *h_long_period = 0;

    for (ix = 0; ix < N_WAVES; ++ix) {
      double tide;

      phi = fmod(fes->waves[ix].freq * delta + fes->waves[ix].v0u, 2.0 * M_PI);

      if (phi < 0.0)
        phi = phi + 2.0 * M_PI;

      tide = fes->waves[ix].f
          * (fes->waves[ix].c.re * cos(phi) + fes->waves[ix].c.im * sin(phi));

      if (fes->waves[ix].type == SP_TIDE)
        *h += tide;
      else
        *h_long_period += tide;
    }
  } else {
    *h = nan("nan");

    if (fes->type == FES_TIDE) {
      lpe_minus_n_waves((const float (*)[N_COEFS]) (fes->w2nd), time, lat,
                        h_long_period);
    } else
      *h_long_period = 0;
    set_fes_error(fes, FES_NO_DATA);
    return 1;
  }

  return 0;
}

/*
 */
int fes_min_number(FES handle) {
  fes_handler* fes = (fes_handler *) handle;
  return fes->min_number;
}
