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

#include "interp.c"
#include "test.h"

int
main(void)
{
  int total_err = 0, err;
  double a, b;

  a = 2;
  b = 2;

  printf("*** testing _sum_weighting...\n");
  _sum_weighting(2, 2, &a, &b);

  err = a != 6 || b != 4;

  SUMMARIZE_ERR;

  _sum_weighting(DV, 2, &a, &b);

  err = a != 6 || b != 4;

  SUMMARIZE_ERR;

  printf("*** testing _linear_weighting...\n");
  _linear_weighting(1, 1, 1, &a, &b);

  err = a != 1 || b != 0;

  SUMMARIZE_ERR;

  _linear_weighting(1, 0, 1, &a, &b);

  err = a != 0 || b != 1;

  SUMMARIZE_ERR;

  _linear_weighting(2, 1, 3, &a, &b);

  err = a != 0.5 || b != 0.5;

  SUMMARIZE_ERR;

  printf("*** testing bilinear_interp...\n");
  bilinear_interp(-1, 1, -1, 1, 0, 1, 1, 2, 0, 0, &a);

  err = a != 1.0;

  SUMMARIZE_ERR;

  FINAL_RESULTS;
}
