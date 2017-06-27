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

#include "grid.h"

#include <math.h>
#include <netcdf.h>
#include <stddef.h>
#include <stdlib.h>

#include "cache.h"
#include "error.h"
#include "fes.h"
#include "interp.h"

/*
 FILL_VALUES

 Attribute _FillValue is a netCDF reserved attribute name that
 indicates missing values.
 */
#define FILL_VALUES "_FillValue"

/*
 _normalizeLongitude

 Standardization of longitude

 base Base standardization.
 long Longitude in degree.

 Returns standardized longitude.
 */
static double _normalize_longitude(const double base, const double lon) {
  register double result = lon;

  while (result >= ((base + 360.0) - EPSILON))
    result -= 360.0;
  while (result < base - EPSILON)
    result += 360.0;
  if (fabs(result - base) <= EPSILON)
    result = base;

  return result;
}

/*
 _nearest_integer

 Whole part of a real.

 x Real value

 Returns whole part.
 */
static double _nearest_integer(const double x) {
  double integral;

  modf(x, &integral);

  return integral;
}

/*
 _get_index

 Compute index in the grid corresponding to a value.

 min Minimum value in the grid
 value Asked value
 step Step

 Returns computed index.
 */
static size_t _get_index(const double min, const double value,
                         const double step) {
  return (size_t) (_nearest_integer((value - min) / step));
}

/*
 _get_value

 Compute a value corresponding at the given index.

 idx Asked index
 min Minimum value in the grid
 step Step

 Returns computed value
 */
double _get_value(const size_t idx, const double min, const double step) {
  return min + (step * idx);
}

/*
 _read_grid_value

 Reading a value in NetCDF grid.

 grid Grid
 iLon Index of the longitude in the grid.
 iLat Index of the latitude in the grid.
 n Index of the grid file.
 value Value read.

 @return If an error occurs, the return value is 1, otherwise 0.
 */
static int _read_grid_value(fes_handler* const fes, const size_t i_lon,
                            const size_t i_lat, const size_t n,
                            fes_double_complex* value) {
  fes_float_complex z;
  fes_grid* const grid = &fes->grid;

  /* reading value from NetCDF grid */
  if (grid->file != NULL) {
    int rc;
    float amp;
    float pha;

    // Read data from the buffer, if the user wants
    if (grid->buffer && fes_get_cache(grid, i_lon, i_lat, n, value))
      return 0;

    size_t count[2] = { 1, 1 };
    size_t start[2];

    start[0] = grid->transpose ? i_lon : i_lat;
    start[1] = grid->transpose ? i_lat : i_lon;

    rc = nc_get_vars_float(grid->file[n].id, grid->file[n].amp_id, start, count,
    NULL,
                           &amp);
    if (rc) {
      set_fes_extended_error(fes, FES_NETCDF_ERROR, "%s (%s)", nc_strerror(rc),
                             grid->file[n].amp);
      return 1;
    }

    rc = nc_get_vars_float(grid->file[n].id, grid->file[n].pha_id, start, count,
    NULL,
                           &pha);
    if (rc) {
      set_fes_extended_error(fes, FES_NETCDF_ERROR, "%s (%s)", nc_strerror(rc),
                             grid->file[n].pha);
      return 1;
    }

    if (amp == grid->undef || pha == grid->undef) {
      value->re = DV;
      value->im = DV;
    } else {
      value->re = amp * cos(pha * RAD);
      value->im = amp * sin(pha * RAD);
    }

    // Store the data read into the buffer, if the user wants.
    if (grid->buffer != NULL && fes_set_cache(grid, i_lon, i_lat, n, value)) {
      set_fes_error(fes, FES_NO_MEMORY);
      return 1;
    }
  }
  /* reading values from memory */
  else {
    z = grid->transpose ?
        grid->values[n][i_lon * grid->lat_dim + i_lat] :
        grid->values[n][i_lat * grid->lon_dim + i_lon];

    if (z.re == grid->undef || z.im == grid->undef) {
      value->re = DV;
      value->im = DV;
    } else {
      value->re = z.re;
      value->im = z.im;
    }
  }
  return 0;
}

/*
 _get_nearest_points

 Reading the points closest to a position.

 fes FES handler
 lat Latitude of the desired position.
 lon:Latitude of the desired position.

 Returns 0 on success otherwise an error status.
 */
static int _get_nearest_points(fes_handler* const fes, const double lat,
                               const double lon) {
  double n_lon = _normalize_longitude(fes->grid.lon_min, lon);

  /* Check if asked position is in the grid */
  if ((!CONTAINS(fes->grid.lat_min, lat, fes->grid.lat_max))
      || (!CONTAINS(fes->grid.lon_min, n_lon, fes->grid.lon_max))) {
    fes->in_grid = 0;
  } else {
    size_t i_lat1 = _get_index(fes->grid.lat_min, lat, fes->grid.lat_step);
    size_t i_lon1 = _get_index(fes->grid.lon_min, n_lon, fes->grid.lon_step);
    size_t i_lat2;
    size_t i_lon2;
    size_t n;

    if (lat >= fes->grid.lat_max) {
      i_lat2 = i_lat1;
      i_lat1--;
    } else
      i_lat2 = i_lat1 + 1;

    if (n_lon >= fes->grid.lon_max) {
      i_lon2 = i_lon1;
      i_lon1--;
    } else
      i_lon2 = i_lon1 + 1;

    fes->in_grid = 1;

    fes->south_lat = _get_value(i_lat1, fes->grid.lat_min, fes->grid.lat_step);
    fes->north_lat = _get_value(i_lat2, fes->grid.lat_min, fes->grid.lat_step);
    fes->west_lon = _get_value(i_lon1, fes->grid.lon_min, fes->grid.lon_step);
    fes->east_lon = _get_value(i_lon2, fes->grid.lon_min, fes->grid.lon_step);

    if (fes->west_lon != lon) {
      double gap = fes->west_lon - fes->east_lon;

      n_lon = _normalize_longitude(lon, fes->east_lon);
      fes->east_lon = n_lon;
      fes->west_lon = n_lon + gap;
    }

    i_lon1 %= fes->grid.lon_dim;
    i_lon2 %= fes->grid.lon_dim;

    for (n = 0; n < (size_t) (fes->grid.n_grids); ++n) {
      if (_read_grid_value(fes, i_lon1, i_lat1, n, &fes->sw[n]))
        return 1;

      if (_read_grid_value(fes, i_lon2, i_lat1, n, &fes->se[n]))
        return 1;

      if (_read_grid_value(fes, i_lon1, i_lat2, n, &fes->nw[n]))
        return 1;

      if (_read_grid_value(fes, i_lon2, i_lat2, n, &fes->ne[n]))
        return 1;
    }
  }
  return 0;
}

/*
 _open_grid(const char* const path, fes_handler* const fes,
 fes_cdf_file* const  nc, size_t* const lon_dim, size_t* const lat_dim,
 double* const lon_min, double* const lat_min, double* const lon_max,
 double* const lat_max, double* const lon_step, double* const lat_step,
 double* const undef)

 Open a netCDF file who contains a wave definition.

 path Absolute or relative file name for netCDF file to be opened.
 nc Properties of the netCDF file opened.
 lon_dim Longitude dimension.
 lat_dim Latitude dimension.
 lon_min Minimum longitude (units degree).
 lat_min Minimum latitude (units degree).
 lon_max Maximum longitude (units degree).
 lat_max Maximum latitude (units degree).
 lon_step Step between each successive longitude.
 lat_step Step between each successive latitude.
 undef Value for missing data.

 Returns 0 on success otherwise an error status.
 */
int _open_grid(const char* const path, fes_handler* const fes,
               fes_cdf_file* const nc, size_t* const lon_dim,
               size_t* const lat_dim, double* const lon_min,
               double* const lat_min, double* const lon_max,
               double* const lat_max, double* const lon_step,
               double* const lat_step, double* const undef) {
  int rc;
  int x_id;
  int y_id;
  int lat_id;
  int lon_id;
  int var_id[2];
  size_t start = 0;
  size_t count = 2;
  float x[2];
  float y[2];
  float undef1;
  float undef2;

  /* Open the NetCDF file */
  rc = nc_open(path, NC_NOWRITE, &nc->id);
  if (rc) {
    set_fes_extended_error(fes, FES_NETCDF_ERROR, "%s : %s", nc_strerror(rc),
                           path);
    return 1;
  }

  /* Reading latitude, longitude samples */
  rc = nc_inq_dimid(nc->id, nc->lon, &lon_id);
  if (rc) {
    set_fes_extended_error(fes, FES_NETCDF_ERROR, "%s (%s): %s",
                           nc_strerror(rc), nc->lon, path);
    return 1;
  }

  rc = nc_inq_dimid(nc->id, nc->lat, &lat_id);
  if (rc) {
    set_fes_extended_error(fes, FES_NETCDF_ERROR, "%s (%s): %s",
                           nc_strerror(rc), nc->lat, path);
    return 1;
  }

  rc = nc_inq_dimlen(nc->id, lon_id, lon_dim);
  if (rc) {
    set_fes_extended_error(fes, FES_NETCDF_ERROR, "%s (%s): %s",
                           nc_strerror(rc), nc->lon, path);
    return 1;
  }

  rc = nc_inq_dimlen(nc->id, lat_id, lat_dim);
  if (rc) {
    set_fes_extended_error(fes, FES_NETCDF_ERROR, "%s (%s): %s",
                           nc_strerror(rc), nc->lat, path);
    return 1;
  }

  /* Read the coordinate variable data. */
  rc = nc_inq_varid(nc->id, nc->lon, &x_id);
  if (rc) {
    set_fes_extended_error(fes, FES_NETCDF_ERROR, "%s (%s): %s",
                           nc_strerror(rc), nc->lon, path);
    return 1;
  }

  rc = nc_inq_varid(nc->id, nc->lat, &y_id);
  if (rc) {
    set_fes_extended_error(fes, FES_NETCDF_ERROR, "%s (%s): %s",
                           nc_strerror(rc), nc->lat, path);
    return 1;
  }

  rc = nc_get_vara_float(nc->id, x_id, &start, &count, &x[0]);
  if (rc) {
    set_fes_extended_error(fes, FES_NETCDF_ERROR, "%s (%s): %s",
                           nc_strerror(rc), nc->lon, path);
    return 1;
  }

  rc = nc_get_vara_float(nc->id, y_id, &start, &count, &y[0]);
  if (rc) {
    set_fes_extended_error(fes, FES_NETCDF_ERROR, "%s (%s): %s",
                           nc_strerror(rc), nc->lat, path);
    return 1;
  }

  /* Set latitude, longitude minimum */
  *lon_min = x[0];
  *lat_min = y[0];

  /* Set latitude, longitude step */
  *lon_step = x[1] - x[0];
  *lat_step = y[1] - y[0];

  /* The grid covers the circumference of the sphere. */
  if (EQUALS(360.0, (*lon_step) * (*lon_dim)))
    /* Longitude max is infinite. */
    *lon_max = 1.0e+250;
  else
    /* If not longitude max is that of the last point. */
    *lon_max = _get_value((*lon_dim) - 1, *lon_min, *lon_step);

  *lat_max = _get_value((*lat_dim) - 1, *lat_min, *lat_step);

  /* Get the varids of the amplitude and phase NetCDF variables. */
  rc = nc_inq_varid(nc->id, nc->amp, &nc->amp_id);
  if (rc) {
    set_fes_extended_error(fes, FES_NETCDF_ERROR, "%s (%s): %s",
                           nc_strerror(rc), nc->amp, path);
    return 1;
  }

  rc = nc_inq_varid(nc->id, nc->pha, &nc->pha_id);
  if (rc) {
    set_fes_extended_error(fes, FES_NETCDF_ERROR, "%s (%s): %s",
                           nc_strerror(rc), nc->pha, path);
    return 1;
  }

  /* Get variable dimension IDs */
  rc = nc_inq_vardimid(nc->id, nc->pha_id, var_id);
  if (rc) {
    set_fes_extended_error(fes, FES_NETCDF_ERROR, "%s (%s): %s",
                           nc_strerror(rc), nc->pha, path);
    return 1;
  }

  /* Need to transpose data */
  fes->grid.transpose = var_id[0] == lon_id && var_id[1] == lat_id;

  /* Get variable dimension IDs */
  rc = nc_inq_vardimid(nc->id, nc->amp_id, var_id);
  if (rc) {
    set_fes_extended_error(fes, FES_NETCDF_ERROR, "%s (%s): %s",
                           nc_strerror(rc), nc->pha, path);
    return 1;
  }

  /* Check dimension for phase variable */
  if (fes->grid.transpose == 0 && var_id[0] == lon_id && var_id[1] == lat_id) {
    set_fes_extended_error(
        fes, FES_NETCDF_ERROR,
        "found %s(dim #%d, dim %d) expected %s(dim #%d, dim %d) : %s", nc->pha,
        var_id[0], var_id[1], nc->pha, lat_id, lon_id, path);
    return 1;
  }

  /* Reading default value */
  rc = nc_get_att_float(nc->id, nc->amp_id, FILL_VALUES, &undef1);
  if (rc) {
    set_fes_extended_error(fes, FES_NETCDF_ERROR, "%s (%s): %s",
                           nc_strerror(rc), nc->amp, path);
    return 1;
  }

  rc = nc_get_att_float(nc->id, nc->pha_id, FILL_VALUES, &undef2);
  if (rc) {
    set_fes_extended_error(fes, FES_NETCDF_ERROR, "%s (%s): %s",
                           nc_strerror(rc), nc->pha, path);
    return 1;
  }

  if (undef1 != undef2) {
    set_fes_extended_error(fes, FES_NETCDF_ERROR,
                           "The definition of _fillValue is not constant : %s",
                           path);
    return 1;
  }

  *undef = undef1;

  return 0;
}

/*
 */
int interp(fes_handler* fes, const double lat, const double lon) {
  int number;
  size_t n;
  fes_double_complex c;

  if (!CONTAINS(
      fes->west_lon, lon,
      fes->east_lon) || !CONTAINS (fes->south_lat, lat, fes->north_lat)) {
    if (_get_nearest_points(fes, lat, lon))
      return 1;
  }

  /* The zone required is not in the grid */
  if (!fes->in_grid)
    goto no_data;

  fes->min_number = 4;

  /* Interpolation */
  for (n = 0; n < fes->grid.n_grids; ++n) {
    if (fes->sw[n].re == DV && fes->se[n].re == DV && fes->nw[n].re == DV
        && fes->ne[n].re == DV)
      goto no_data;

    number = bilinear_interp(fes->west_lon, /* X1 */
                             fes->east_lon, /* X2 */
                             fes->south_lat, /* Y1 */
                             fes->north_lat, /* Y2 */
                             fes->sw[n].re, /* X1, Y1 */
                             fes->se[n].re, /* X2, Y1 */
                             fes->nw[n].re, /* X1, Y2 */
                             fes->ne[n].re, /* X2, Y2 */
                             lon, lat, &c.re);

    if (c.re == DV)
      goto no_data;

    fes->min_number = MIN(fes->min_number, number);

    number = bilinear_interp(fes->west_lon, /* X1 */
                             fes->east_lon, /* X2 */
                             fes->south_lat, /* Y1 */
                             fes->north_lat, /* Y2 */
                             fes->sw[n].im, /* X1, Y1 */
                             fes->se[n].im, /* X2, Y1 */
                             fes->nw[n].im, /* X1, Y2 */
                             fes->ne[n].im, /* X2, Y2 */
                             lon, lat, &c.im);

    if (c.im == DV)
      goto no_data;

    fes->min_number = MIN(fes->min_number, number);
    fes->waves[fes->grid.waveIndex[n]].c = c;
  }

  fes->is_data = 1;
  return 0;

  no_data: fes->is_data = 0;
  fes->min_number = 0;
  return 0;
}

/*
 */
int load_grid(const char* const path, const unsigned int n,
              fes_cdf_file* const nc, fes_handler* const fes) {
  int rc;
  size_t lon_dim;
  size_t lat_dim;
  double lon_min;
  double lat_min;
  double lon_max;
  double lat_max;
  double lon_step;
  double lat_step;
  double undef;

  if (_open_grid(path, fes, nc, &lon_dim, &lat_dim, &lon_min, &lat_min,
                 &lon_max, &lat_max, &lon_step, &lat_step, &undef))
    return 1;

  /* First call */
  if (n == 0) {
    fes->grid.lat_min = lat_min;
    fes->grid.lon_min = lon_min;
    fes->grid.lat_max = lat_max;
    fes->grid.lon_max = lon_max;
    fes->grid.lat_step = lat_step;
    fes->grid.lon_step = lon_step;
    fes->grid.lat_dim = lat_dim;
    fes->grid.lon_dim = lon_dim;
    fes->grid.undef = undef;
  } else if (fes->grid.lat_min != lat_min || fes->grid.lon_min != lon_min
      || fes->grid.lat_max != lat_max || fes->grid.lon_max != lon_max
      || fes->grid.lat_step != lat_step || fes->grid.lon_step != lon_step
      || fes->grid.lat_dim != lat_dim || fes->grid.lon_dim != lon_dim
      || fes->grid.undef != undef) {
    set_fes_extended_error(fes, FES_IO_ERROR,
                           "The definition of grids isn't constant : %s", path);
  }

  /* Loading grid into memory */
  if (fes->grid.values != NULL) {
    size_t ix;
    size_t size = lat_dim * lon_dim;
    float* amp;
    float* pha;

    /* Allocate the current grid */
    if ((fes->grid.values[n] = (fes_float_complex*) calloc(
        size, sizeof(fes_float_complex))) == NULL) {
      set_fes_error(fes, FES_NO_MEMORY);
      return 1;
    }

    amp = (float*) calloc(size, sizeof(float));
    pha = (float*) calloc(size, sizeof(float));
    if (amp == NULL || pha == NULL) {
      free(amp);
      free(pha);
      set_fes_error(fes, FES_NO_MEMORY);
      return 1;
    }

    /* reading all values */
    rc = nc_get_var_float(nc->id, nc->amp_id, amp);
    if (rc) {
      set_fes_extended_error(fes, FES_NETCDF_ERROR, "%s (%s) : %s",
                             nc_strerror(rc), nc->amp, path);
    } else {
      rc = nc_get_var_float(nc->id, nc->pha_id, pha);
      if (rc)
        set_fes_extended_error(fes, FES_NETCDF_ERROR, "%s (%s) : %s",
                               nc_strerror(rc), nc->pha, path);
    }

    /* if an error was caught */
    if (rc) {
      free(amp);
      free(pha);
      return 1;
    }

    for (ix = 0; ix < size; ++ix) {
      if (amp[ix] != undef && pha[ix] != undef) {
        fes->grid.values[n][ix].re = amp[ix] * (float) (cos(pha[ix] * RAD));
        fes->grid.values[n][ix].im = amp[ix] * (float) (sin(pha[ix] * RAD));
      } else {
        fes->grid.values[n][ix].re = (float) (undef);
        fes->grid.values[n][ix].im = (float) (undef);
      }
    }

    free(amp);
    free(pha);
  } else {
    fes->grid.file[n] = *nc;
  }

  return 0;
}
