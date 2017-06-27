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
#include <stdlib.h>

#include "test.h"
#include "fes.h"

#define INI "fes.ini"
#define FES_DATA "data"

#define CHECK_TIDE(a, b, c) fabs((a)-(b)) > c

static int total_err = 0;

int test(fes_enum_access access) {
  int err;
  int hour;
  int rc = 0;
  double lon = -7.688;
  double lat = 59.195;
  double tide;
  double time;
  double lp;
  double load;
  double loadlp;
  FES short_tide = NULL;
  FES radial_tide = NULL;

  printf(
      "*** testing libfes with %s...\n",
      access == FES_IO ?
          "direct access" :
          (access == FES_MEM ? "memory access" : "buffered access"));

  if (fes_new(&short_tide, FES_TIDE, access, INI)) {
    printf("fes error : %s\n", fes_error(short_tide));
    goto on_error;
  }

  if (fes_new(&radial_tide, FES_RADIAL, access, INI)) {
    printf("fes error : %s\n", fes_error(radial_tide));
    goto on_error;
  }

  for (hour = 0, time = 12053; hour < 24; hour++, time += 1 / 24.0) {
    /* Compute tide */
    if (fes_core(short_tide, lat, lon, time, &tide, &lp)) {
      if (fes_errno(short_tide) == FES_NO_DATA)
        continue;
      else {
        fprintf(stderr, "%s\n", fes_error(short_tide));
        goto on_error;
      }
    }

    if (fes_core(radial_tide, lat, lon, time, &load, &loadlp)) {
      if (fes_errno(radial_tide) == FES_NO_DATA)
        continue;
      else {
        fprintf(stderr, "%s\n", fes_error(radial_tide));
        goto on_error;
      }
    }

    err = fes_min_number(short_tide) < 0 || fes_min_number(short_tide) > 4;
    SUMMARIZE_ERR;

    switch (hour) {
      case 0:
        err = CHECK_TIDE(tide, -100.990629, 1E-5);
        SUMMARIZE_ERR;
        err = CHECK_TIDE(lp, 0.903299, 1E-5);
        SUMMARIZE_ERR;
        err = CHECK_TIDE(load, 3.881161, 1E-5);
        SUMMARIZE_ERR;
        break;
      case 1:
        err = CHECK_TIDE(tide, -137.104223, 1E-5);
        SUMMARIZE_ERR;
        err = CHECK_TIDE(lp, 0.876519, 1E-5);
        SUMMARIZE_ERR;
        err = CHECK_TIDE(load, 4.328335, 1E-5);
        SUMMARIZE_ERR;
        break;
      case 2:
        err = CHECK_TIDE(tide, -138.482412, 1E-5);
        SUMMARIZE_ERR;
        err = CHECK_TIDE(lp, 0.849121, 1E-5);
        SUMMARIZE_ERR;
        err = CHECK_TIDE(load, 3.710694, 1E-5);
        SUMMARIZE_ERR;
        break;
      case 3:
        err = CHECK_TIDE(tide, -104.345361, 1E-5);
        SUMMARIZE_ERR;
        err = CHECK_TIDE(lp, 0.821123, 1E-5);
        SUMMARIZE_ERR;
        err = CHECK_TIDE(load, 2.134257, 1E-5);
        SUMMARIZE_ERR;
        break;
      case 4:
        err = CHECK_TIDE(tide, -42.515586, 1E-5);
        SUMMARIZE_ERR;
        err = CHECK_TIDE(lp, 0.792543, 1E-5);
        SUMMARIZE_ERR;
        err = CHECK_TIDE(load, -0.052047, 1E-5);
        SUMMARIZE_ERR;
        break;
      case 5:
        err = CHECK_TIDE(tide, 32.374755, 1E-5);
        SUMMARIZE_ERR;
        err = CHECK_TIDE(lp, 0.763399, 1E-5);
        SUMMARIZE_ERR;
        err = CHECK_TIDE(load, -2.341404, 1E-5);
        SUMMARIZE_ERR;
        break;
      case 6:
        err = CHECK_TIDE(tide, 102.167627, 1E-5);
        SUMMARIZE_ERR;
        err = CHECK_TIDE(lp, 0.733711, 1E-5);
        SUMMARIZE_ERR;
        err = CHECK_TIDE(load, -4.194242, 1E-5);
        SUMMARIZE_ERR;
        break;
      case 7:
        err = CHECK_TIDE(tide, 149.469495, 1E-5);
        SUMMARIZE_ERR;
        err = CHECK_TIDE(lp, 0.703496, 1E-5);
        SUMMARIZE_ERR;
        err = CHECK_TIDE(load, -5.171971, 1E-5);
        SUMMARIZE_ERR;
        break;
      case 8:
        err = CHECK_TIDE(tide, 162.102726, 1E-5);
        SUMMARIZE_ERR;
        err = CHECK_TIDE(lp, 0.672774, 1E-5);
        SUMMARIZE_ERR;
        err = CHECK_TIDE(load, -5.045669, 1E-5);
        SUMMARIZE_ERR;
        break;
      case 9:
        err = CHECK_TIDE(tide, 136.505573, 1E-5);
        SUMMARIZE_ERR;
        err = CHECK_TIDE(lp, 0.641566, 1E-5);
        SUMMARIZE_ERR;
        err = CHECK_TIDE(load, -3.852375, 1E-5);
        SUMMARIZE_ERR;
        break;
      case 10:
        err = CHECK_TIDE(tide, 78.894879, 1E-5);
        SUMMARIZE_ERR;
        err = CHECK_TIDE(lp, 0.609890, 1E-5);
        SUMMARIZE_ERR;
        err = CHECK_TIDE(load, -1.884925, 1E-5);
        SUMMARIZE_ERR;
        break;
      case 11:
        err = CHECK_TIDE(tide, 3.643044, 1E-5);
        SUMMARIZE_ERR;
        err = CHECK_TIDE(lp, 0.577768, 1E-5);
        SUMMARIZE_ERR;
        err = CHECK_TIDE(load, 0.381964, 1E-5);
        SUMMARIZE_ERR;
        break;
      case 12:
        err = CHECK_TIDE(tide, -70.661218, 1E-5);
        SUMMARIZE_ERR;
        err = CHECK_TIDE(lp, 0.545220, 1E-5);
        SUMMARIZE_ERR;
        err = CHECK_TIDE(load, 2.410565, 1E-5);
        SUMMARIZE_ERR;
        break;
      case 13:
        err = CHECK_TIDE(tide, -126.154394, 1E-5);
        SUMMARIZE_ERR;
        err = CHECK_TIDE(lp, 0.512266, 1E-5);
        SUMMARIZE_ERR;
        err = CHECK_TIDE(load, 3.733913, 1E-5);
        SUMMARIZE_ERR;
        break;
      case 14:
        err = CHECK_TIDE(tide, -150.116619, 1E-5);
        SUMMARIZE_ERR;
        err = CHECK_TIDE(lp, 0.478928, 1E-5);
        SUMMARIZE_ERR;
        err = CHECK_TIDE(load, 4.070741, 1E-5);
        SUMMARIZE_ERR;
        break;
      case 15:
        err = CHECK_TIDE(tide, -137.779885, 1E-5);
        SUMMARIZE_ERR;
        err = CHECK_TIDE(lp, 0.445227, 1E-5);
        SUMMARIZE_ERR;
        err = CHECK_TIDE(load, 3.392764, 1E-5);
        SUMMARIZE_ERR;
        break;
      case 16:
        err = CHECK_TIDE(tide, -93.130691, 1E-5);
        SUMMARIZE_ERR;
        err = CHECK_TIDE(lp, 0.411184, 1E-5);
        SUMMARIZE_ERR;
        err = CHECK_TIDE(load, 1.927624, 1E-5);
        SUMMARIZE_ERR;
        break;
      case 17:
        err = CHECK_TIDE(tide, -27.816052, 1E-5);
        SUMMARIZE_ERR;
        err = CHECK_TIDE(lp, 0.376822, 1E-5);
        SUMMARIZE_ERR;
        err = CHECK_TIDE(load, 0.097163, 1E-5);
        SUMMARIZE_ERR;
        break;
      case 18:
        err = CHECK_TIDE(tide, 41.546661, 1E-5);
        SUMMARIZE_ERR;
        err = CHECK_TIDE(lp, 0.342161, 1E-5);
        SUMMARIZE_ERR;
        err = CHECK_TIDE(load, -1.592943, 1E-5);
        SUMMARIZE_ERR;
        break;
      case 19:
        err = CHECK_TIDE(tide, 97.256195, 1E-5);
        SUMMARIZE_ERR;
        err = CHECK_TIDE(lp, 0.307223, 1E-5);
        SUMMARIZE_ERR;
        err = CHECK_TIDE(load, -2.683946, 1E-5);
        SUMMARIZE_ERR;
        break;
      case 20:
        err = CHECK_TIDE(tide, 124.945282, 1E-5);
        SUMMARIZE_ERR;
        err = CHECK_TIDE(lp, 0.272032, 1E-5);
        SUMMARIZE_ERR;
        err = CHECK_TIDE(load, -2.881870, 1E-5);
        SUMMARIZE_ERR;
        break;
      case 21:
        err = CHECK_TIDE(tide, 117.469330, 1E-5);
        SUMMARIZE_ERR;
        err = CHECK_TIDE(lp, 0.236608, 1E-5);
        SUMMARIZE_ERR;
        err = CHECK_TIDE(load, -2.132584, 1E-5);
        SUMMARIZE_ERR;
        break;
      case 22:
        err = CHECK_TIDE(tide, 77.027699, 1E-5);
        SUMMARIZE_ERR;
        err = CHECK_TIDE(lp, 0.200974, 1E-5);
        SUMMARIZE_ERR;
        err = CHECK_TIDE(load, -0.635177, 1E-5);
        SUMMARIZE_ERR;
        break;
      case 23:
        err = CHECK_TIDE(tide, 14.659386, 1E-5);
        SUMMARIZE_ERR;
        err = CHECK_TIDE(lp, 0.165154, 1E-5);
        SUMMARIZE_ERR;
        err = CHECK_TIDE(load, 1.209534, 1E-5);
        SUMMARIZE_ERR;
        break;
      default:
        abort();
    }
  }

  goto on_terminate;

on_error:
  rc = 1;

on_terminate:
  fes_delete(short_tide);
  fes_delete(radial_tide);

  if (rc == 0) {
    FINAL_RESULTS;
  }
  return rc;
}

/*
 */
int main(void) {
  int total_err = 0, err;

#ifdef _WIN32
  _putenv_s("FES_DATA", FES_DATA);
#else
  setenv("FES_DATA", FES_DATA, 1);
#endif

  err = test(FES_IO);
  SUMMARIZE_ERR;

  err = test(FES_MEM);
  SUMMARIZE_ERR;

  FINAL_RESULTS;
}
