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

#include "test.h"
#include "angle.h"

int main(void) {
  int total_err = 0;
  int err;

  printf("*** testing the fundamental frequencies\n");

  err = CHECK_FLOAT(tau_speed(), 14.4920521070043);
  SUMMARIZE_ERR;
  err = CHECK_FLOAT(s_speed(), 0.5490165320557);
  SUMMARIZE_ERR;
  err = CHECK_FLOAT(h_speed(), 0.0410686390600);
  SUMMARIZE_ERR;
  err = CHECK_FLOAT(p_speed(), 0.0046418343600);
  SUMMARIZE_ERR;
  err = CHECK_FLOAT(n_speed(), -0.0022064134155);
  SUMMARIZE_ERR;
  err = CHECK_FLOAT(p1_speed(), 0.00000196098563);
  SUMMARIZE_ERR;

  err = CHECK_FLOAT(tau_frequency(), 0.040255700297);
  SUMMARIZE_ERR;
  err = CHECK_FLOAT(s_frequency(), 0.001525045922);
  SUMMARIZE_ERR;
  err = CHECK_FLOAT(h_frequency(), 0.000114079553);
  SUMMARIZE_ERR;
  err = CHECK_FLOAT(p_frequency(), 0.000012893984);
  SUMMARIZE_ERR;
  err = CHECK_FLOAT(n_frequency(), -0.000006128926);
  SUMMARIZE_ERR;
  err = CHECK_FLOAT(p1_frequency(), 0.000000005447);
  SUMMARIZE_ERR;

  FINAL_RESULTS;
}

