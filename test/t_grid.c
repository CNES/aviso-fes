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

#include <stdio.h>
#include <string.h>

#include "compat.h"
#include "grid.c"
#include "test.h"

#ifndef DUMMY_GRID
#define DUMMY_GRID "dummy.nc"
#endif

int
main(void)
{
  int total_err = 0, err;
  fes_grid grid;
  fes_cdf_file nc;
  fes_double_complex c;
  fes_handler fes;
  size_t lon_dim;
  size_t lat_dim;
  double lon_min;
  double lat_min;
  double lon_max;
  double lat_max;
  double lon_step;
  double lat_step;
  double undef;

  grid.file = &nc;
  grid.buffer = NULL;

  printf("*** testing _normalize_longitude...\n");
  err = CHECK_FLOAT(_normalize_longitude(0.0, 370), 10);
  SUMMARIZE_ERR;
  err = CHECK_FLOAT(_normalize_longitude(0.0, -10), 350);
  SUMMARIZE_ERR;
  err = CHECK_FLOAT(_normalize_longitude(0.0, 10), 10);
  SUMMARIZE_ERR;

  printf("*** testing _nearest_integer...\n");
  err = CHECK_FLOAT(_nearest_integer(M_PI), 3);
  SUMMARIZE_ERR;

  printf("*** testing _get_index...\n");
  err = CHECK_INT(_get_index(2, 3, 2), 0);
  SUMMARIZE_ERR;

  printf("*** testing _get_value...\n");
  err = CHECK_FLOAT(_get_value(10, -90, 0.5), -85);
  SUMMARIZE_ERR;

  STRNCPY(grid.file->lat, "latitude", sizeof(grid.file->lat));
  STRNCPY(grid.file->lon, "longitude", sizeof(grid.file->lon));
  STRNCPY(grid.file->amp, "amplitude", sizeof(grid.file->amp));
  STRNCPY(grid.file->pha, "phase", sizeof(grid.file->pha));

  printf("*** testing _open_grid\n");
  if (_open_grid(DUMMY_GRID,
                 &fes,
                 &nc,
                 &lon_dim,
                 &lat_dim,
                 &lon_min,
                 &lat_min,
                 &lon_max,
                 &lat_max,
                 &lon_step,
                 &lat_step,
                 &undef)) {
    printf("%s\n", "nok");
    DIE;
  }

  err = CHECK_INT(lon_dim, 2);
  SUMMARIZE_ERR;
  err = CHECK_INT(lat_dim, 3);
  SUMMARIZE_ERR;
  err = CHECK_FLOAT(lon_min, -10);
  SUMMARIZE_ERR;
  err = CHECK_FLOAT(lat_min, -5);
  SUMMARIZE_ERR;
  err = CHECK_FLOAT(lon_max, 10);
  SUMMARIZE_ERR;
  err = CHECK_FLOAT(lat_max, 5);
  SUMMARIZE_ERR;
  err = CHECK_FLOAT(lon_step, 20);
  SUMMARIZE_ERR;
  err = CHECK_FLOAT(lat_step, 5);
  SUMMARIZE_ERR;
  err = CHECK_FLOAT(undef, 1.844674e+19f);
  SUMMARIZE_ERR;

  grid.lon_dim = lon_dim;
  grid.lat_dim = lat_dim;
  grid.lon_min = lon_min;
  grid.lat_min = lat_min;
  grid.lon_max = lon_max;
  grid.lat_max = lat_max;
  grid.lon_step = lon_step;
  grid.lat_step = lat_step;
  grid.undef = undef;
  grid.n_grids = 1;
  grid.transpose = 0;

  printf("*** testing _read_grid_value..\n");
  fes.grid = grid;
  if (_read_grid_value(&fes, 1, 1, 0, &c))
    DIE;

  err = CHECK_FLOAT(c.re, 0);
  SUMMARIZE_ERR;
  err = CHECK_FLOAT(c.im, 1);
  SUMMARIZE_ERR;

  if (_read_grid_value(&fes, 1, 1, 0, &c))
    DIE;

  err = CHECK_FLOAT(c.re, 0);
  SUMMARIZE_ERR;
  err = CHECK_FLOAT(c.im, 1);
  SUMMARIZE_ERR;

  if (_read_grid_value(&fes, 0, 1, 0, &c))
    DIE;

  err = CHECK_FLOAT(c.re, DV);
  SUMMARIZE_ERR;
  err = CHECK_FLOAT(c.im, DV);
  SUMMARIZE_ERR;

  fes.grid = grid;

  printf("*** testing _get_nearest_points..\n");
  _get_nearest_points(&fes, 0, 0);

  err = CHECK_FLOAT(fes.se->re, 0);
  SUMMARIZE_ERR;
  err = CHECK_FLOAT(fes.se->im, 1);
  SUMMARIZE_ERR;
  err = CHECK_FLOAT(fes.sw->re, DV);
  SUMMARIZE_ERR;
  err = CHECK_FLOAT(fes.sw->im, DV);
  SUMMARIZE_ERR;
  err = CHECK_FLOAT(fes.nw->re, 0);
  SUMMARIZE_ERR;
  err = CHECK_FLOAT(fes.nw->im, 0);
  SUMMARIZE_ERR;
  err = CHECK_FLOAT(fes.ne->re, 0);
  SUMMARIZE_ERR;
  err = CHECK_FLOAT(fes.ne->im, 0);
  SUMMARIZE_ERR;

  FINAL_RESULTS;
}
