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

#include "interp.h"

#include <assert.h>

#include "fes_int.h"

/*
 _linear_weighting

 Linear Weighting.

 x Abscissa where interpolation will occur
 x1 Abscissa corresponding to the first value
 x2 Abscissa corresponding to the second value
 w1 Linear weight w1
 w2 Linear weight w2
 */
static void _linear_weighting(const double x, const double x_1,
                              const double x_2, double* w_1, double* w_2) {
  assert(x >= x_1 - EPSILON);
  assert(x <= x_2 + EPSILON);

  if (EQUALS(x_1, x_2) || EQUALS(x, x_1)) {
    *w_1 = 1.0;
    *w_2 = 0.0;
  } else if (EQUALS(x, x_2)) {
    *w_1 = 0.0;
    *w_2 = 1.0;
  } else {
    *w_1 = (x_2 - x) / (x_2 - x_1);
    *w_2 = (x - x_1) / (x_2 - x_1);
  }
}

/*
 _sum_weighting

 Sum Weighting.

 x_12
 w_12
 s
 w
 */
static int _sum_weighting(const double x_12, const double w_12, double* s,
                          double* w) {
  if (!EQUALS(x_12, DV)) {
    *s += w_12 * x_12;
    *w += w_12;
    return 1;
  }
  return 0;
}

/*
 */
int bilinear_interp(const double x_1, const double x_2, const double y_1,
                    const double y_2, const double value_11,
                    const double value_21, const double value_12,
                    const double value_22, const double x, const double y,
                    double* z) {
  int n = 0;
  double w = 0.0;
  double s = 0.0;
  double w_x1;
  double w_x2;
  double w_y1;
  double w_y2;

  _linear_weighting(x, x_1, x_2, &w_x1, &w_x2);
  _linear_weighting(y, y_1, y_2, &w_y1, &w_y2);

  n = _sum_weighting(value_11, w_x1 * w_y1, &s, &w);
  n += _sum_weighting(value_12, w_x1 * w_y2, &s, &w);
  n += _sum_weighting(value_21, w_x2 * w_y1, &s, &w);
  n += _sum_weighting(value_22, w_x2 * w_y2, &s, &w);

  *z = w == 0.0 ? DV : s / w;

  return n;
}
