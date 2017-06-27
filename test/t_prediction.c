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
#include "prediction.c"

static int test_admittance(const double r, const int ix, fes_wave* w) {
  int rc;

  admittance(w);
  rc = CHECK_FLOAT(w[ix].c.re, r) && CHECK_FLOAT(w[ix].c.im, r);

  w[ix].admittance = 0;
  w[ix].c.re = 1;
  w[ix].c.im = 1;

  admittance(w);
  rc |= w[ix].c.im != 1.0 && w[ix].c.re != 1.0;

  w[ix].admittance = 1;
  w[ix].c.re = 1;
  w[ix].c.im = 1;

  return rc;
}

int main(void) {
  int total_err = 0;
  int err;
  int i;
  float w2nd[N_WAVES_2ND][N_COEFS];
  double hlp = 0;
  _fes_astronomic_angle a;
  fes_wave w[N_WAVES];

  set_waves(w);

  printf("*** testing wave properties\n");

  for (i = 0; i < N_WAVES; i++) {
    switch (i) {
      case O1:
        err = CHECK_INT(strcmp(w[i].name, "O1"), 0);
        SUMMARIZE_ERR;

        err = CHECK_FLOAT(w[i].freq, 13.9430356 * RAD);
        SUMMARIZE_ERR;

        err = CHECK_INT(w[i].type, SP_TIDE);
        SUMMARIZE_ERR;
        break;
      case P1:
        err = CHECK_INT(strcmp(w[i].name, "P1"), 0);
        SUMMARIZE_ERR;

        err = CHECK_FLOAT(w[i].freq, 14.9589314 * RAD);
        SUMMARIZE_ERR;

        err = CHECK_INT(w[i].type, SP_TIDE);
        SUMMARIZE_ERR;
        break;
      case K1:
        err = CHECK_INT(strcmp(w[i].name, "K1"), 0);
        SUMMARIZE_ERR;

        err = CHECK_FLOAT(w[i].freq, 15.0410686 * RAD);
        SUMMARIZE_ERR;

        err = CHECK_INT(w[i].type, SP_TIDE);
        SUMMARIZE_ERR;
        break;
      case _2N2:
        err = CHECK_INT(strcmp(w[i].name, "2N2"), 0);
        SUMMARIZE_ERR;

        err = CHECK_FLOAT(w[i].freq, 27.8953548 * RAD);
        SUMMARIZE_ERR;

        err = CHECK_INT(w[i].type, SP_TIDE);
        SUMMARIZE_ERR;
        break;
      case MU2:
        err = CHECK_INT(strcmp(w[i].name, "MU2"), 0);
        SUMMARIZE_ERR;

        err = CHECK_FLOAT(w[i].freq, 27.9682084 * RAD);
        SUMMARIZE_ERR;

        err = CHECK_INT(w[i].type, SP_TIDE);
        SUMMARIZE_ERR;
        break;
      case N2:
        err = CHECK_INT(strcmp(w[i].name, "N2"), 0);
        SUMMARIZE_ERR;

        err = CHECK_FLOAT(w[i].freq, 28.4397295 * RAD);
        SUMMARIZE_ERR;

        err = CHECK_INT(w[i].type, SP_TIDE);
        SUMMARIZE_ERR;
        break;
      case NU2:
        err = CHECK_INT(strcmp(w[i].name, "NU2"), 0);
        SUMMARIZE_ERR;

        err = CHECK_FLOAT(w[i].freq, 28.5125831 * RAD);
        SUMMARIZE_ERR;

        err = CHECK_INT(w[i].type, SP_TIDE);
        SUMMARIZE_ERR;
        break;
      case M2:
        err = CHECK_INT(strcmp(w[i].name, "M2"), 0);
        SUMMARIZE_ERR;

        err = CHECK_FLOAT(w[i].freq, 28.9841042 * RAD);
        SUMMARIZE_ERR;

        err = CHECK_INT(w[i].type, SP_TIDE);
        SUMMARIZE_ERR;
        break;
      case L2:
        err = CHECK_INT(strcmp(w[i].name, "L2"), 0);
        SUMMARIZE_ERR;

        err = CHECK_FLOAT(w[i].freq, 29.5284789 * RAD);
        SUMMARIZE_ERR;

        err = CHECK_INT(w[i].type, SP_TIDE);
        SUMMARIZE_ERR;
        break;
      case T2:
        err = CHECK_INT(strcmp(w[i].name, "T2"), 0);
        SUMMARIZE_ERR;

        err = CHECK_FLOAT(w[i].freq, 29.9589333 * RAD);
        SUMMARIZE_ERR;

        err = CHECK_INT(w[i].type, SP_TIDE);
        SUMMARIZE_ERR;
        break;
      case S2:
        err = CHECK_INT(strcmp(w[i].name, "S2"), 0);
        SUMMARIZE_ERR;

        err = CHECK_FLOAT(w[i].freq, 30.0 * RAD);
        SUMMARIZE_ERR;

        err = CHECK_INT(w[i].type, SP_TIDE);
        SUMMARIZE_ERR;
        break;
      case K2:
        err = CHECK_INT(strcmp(w[i].name, "K2"), 0);
        SUMMARIZE_ERR;

        err = CHECK_FLOAT(w[i].freq, 30.0821373 * RAD);
        SUMMARIZE_ERR;

        err = CHECK_INT(w[i].type, SP_TIDE);
        SUMMARIZE_ERR;
        break;
      case M4:
        err = CHECK_INT(strcmp(w[i].name, "M4"), 0);
        SUMMARIZE_ERR;

        err = CHECK_FLOAT(w[i].freq, 57.9682084 * RAD);
        SUMMARIZE_ERR;

        err = CHECK_INT(w[i].type, SP_TIDE);
        SUMMARIZE_ERR;
        break;
      case S1:
        err = CHECK_INT(strcmp(w[i].name, "S1"), 0);
        SUMMARIZE_ERR;

        err = CHECK_FLOAT(w[i].freq, 15 * RAD);
        SUMMARIZE_ERR;

        err = CHECK_INT(w[i].type, SP_TIDE);
        SUMMARIZE_ERR;
        break;
      case Q1:
        err = CHECK_INT(strcmp(w[i].name, "Q1"), 0);
        SUMMARIZE_ERR;

        err = CHECK_FLOAT(w[i].freq, 13.3986609 * RAD);
        SUMMARIZE_ERR;

        err = CHECK_INT(w[i].type, SP_TIDE);
        SUMMARIZE_ERR;
        break;
      case MM:
        err = CHECK_INT(strcmp(w[i].name, "MM"), 0);
        SUMMARIZE_ERR;

        err = CHECK_FLOAT(w[i].freq, 0.5443747 * RAD);
        SUMMARIZE_ERR;

        err = CHECK_INT(w[i].type, LP_TIDE);
        SUMMARIZE_ERR;
        break;
      case MF:
        err = CHECK_INT(strcmp(w[i].name, "MF"), 0);
        SUMMARIZE_ERR;

        err = CHECK_FLOAT(w[i].freq, 1.0980331 * RAD);
        SUMMARIZE_ERR;

        err = CHECK_INT(w[i].type, LP_TIDE);
        SUMMARIZE_ERR;
        break;
      case MTM:
        err = CHECK_INT(strcmp(w[i].name, "MTM"), 0);
        SUMMARIZE_ERR;

        err = CHECK_FLOAT(w[i].freq, 1.642407762 * RAD);
        SUMMARIZE_ERR;

        err = CHECK_INT(w[i].type, LP_TIDE);
        SUMMARIZE_ERR;
        break;
      case MSQM:
        err = CHECK_INT(strcmp(w[i].name, "MSQM"), 0);
        SUMMARIZE_ERR;

        err = CHECK_FLOAT(w[i].freq, 2.113928850 * RAD);
        SUMMARIZE_ERR;

        err = CHECK_INT(w[i].type, LP_TIDE);
        SUMMARIZE_ERR;
        break;
      case EPS2:
        err = CHECK_INT(strcmp(w[i].name, "EPS2"), 0);
        SUMMARIZE_ERR;

        err = CHECK_FLOAT(w[i].freq, 27.423833730 * RAD);
        SUMMARIZE_ERR;

        err = CHECK_INT(w[i].type, SP_TIDE);
        SUMMARIZE_ERR;
        break;
      case LAMBDA2:
        err = CHECK_INT(strcmp(w[i].name, "LAMBDA2"), 0);
        SUMMARIZE_ERR;

        err = CHECK_FLOAT(w[i].freq, 29.4556253 * RAD);
        SUMMARIZE_ERR;

        err = CHECK_INT(w[i].type, SP_TIDE);
        SUMMARIZE_ERR;
        break;
      case ETA2:
        err = CHECK_INT(strcmp(w[i].name, "ETA2"), 0);
        SUMMARIZE_ERR;

        err = CHECK_FLOAT(w[i].freq, 30.6265120 * RAD);
        SUMMARIZE_ERR;

        err = CHECK_INT(w[i].type, SP_TIDE);
        SUMMARIZE_ERR;
        break;
      case _2Q1:
        err = CHECK_INT(strcmp(w[i].name, "2Q1"), 0);
        SUMMARIZE_ERR;

        err = CHECK_FLOAT(w[i].freq, 12.8542862 * RAD);
        SUMMARIZE_ERR;

        err = CHECK_INT(w[i].type, SP_TIDE);
        SUMMARIZE_ERR;
        break;
      case SIGMA1:
        err = CHECK_INT(strcmp(w[i].name, "SIGMA1"), 0);
        SUMMARIZE_ERR;

        err = CHECK_FLOAT(w[i].freq, 12.9271398 * RAD);
        SUMMARIZE_ERR;

        err = CHECK_INT(w[i].type, SP_TIDE);
        SUMMARIZE_ERR;
        break;
      case RHO1:
        err = CHECK_INT(strcmp(w[i].name, "RHO1"), 0);
        SUMMARIZE_ERR;

        err = CHECK_FLOAT(w[i].freq, 13.4715145 * RAD);
        SUMMARIZE_ERR;

        err = CHECK_INT(w[i].type, SP_TIDE);
        SUMMARIZE_ERR;
        break;
      case M11:
        err = CHECK_INT(strcmp(w[i].name, "M11"), 0);
        SUMMARIZE_ERR;

        err = CHECK_FLOAT(w[i].freq, 14.4966939 * RAD);
        SUMMARIZE_ERR;

        err = CHECK_INT(w[i].type, SP_TIDE);
        SUMMARIZE_ERR;
        break;
      case M12:
        err = CHECK_INT(strcmp(w[i].name, "M12"), 0);
        SUMMARIZE_ERR;

        err = CHECK_FLOAT(w[i].freq, 14.48741027 * RAD);
        SUMMARIZE_ERR;

        err = CHECK_INT(w[i].type, SP_TIDE);
        SUMMARIZE_ERR;
        break;
      case CHI1:
        err = CHECK_INT(strcmp(w[i].name, "CHI1"), 0);
        SUMMARIZE_ERR;

        err = CHECK_FLOAT(w[i].freq, 14.5695476 * RAD);
        SUMMARIZE_ERR;

        err = CHECK_INT(w[i].type, SP_TIDE);
        SUMMARIZE_ERR;
        break;
      case PI1:
        err = CHECK_INT(strcmp(w[i].name, "PI1"), 0);
        SUMMARIZE_ERR;

        err = CHECK_FLOAT(w[i].freq, 14.9178647 * RAD);
        SUMMARIZE_ERR;

        err = CHECK_INT(w[i].type, SP_TIDE);
        SUMMARIZE_ERR;
        break;
      case PHI1:
        err = CHECK_INT(strcmp(w[i].name, "PHI1"), 0);
        SUMMARIZE_ERR;

        err = CHECK_FLOAT(w[i].freq, 15.1232059 * RAD);
        SUMMARIZE_ERR;

        err = CHECK_INT(w[i].type, SP_TIDE);
        SUMMARIZE_ERR;
        break;
      case THETA1:
        err = CHECK_INT(strcmp(w[i].name, "THETA1"), 0);
        SUMMARIZE_ERR;

        err = CHECK_FLOAT(w[i].freq, 15.5125897 * RAD);
        SUMMARIZE_ERR;

        err = CHECK_INT(w[i].type, SP_TIDE);
        SUMMARIZE_ERR;
        break;
      case J1:
        err = CHECK_INT(strcmp(w[i].name, "J1"), 0);
        SUMMARIZE_ERR;

        err = CHECK_FLOAT(w[i].freq, 15.5854433 * RAD);
        SUMMARIZE_ERR;

        err = CHECK_INT(w[i].type, SP_TIDE);
        SUMMARIZE_ERR;
        break;
      case OO1:
        err = CHECK_INT(strcmp(w[i].name, "OO1"), 0);
        SUMMARIZE_ERR;

        err = CHECK_FLOAT(w[i].freq, 16.1391017 * RAD);
        SUMMARIZE_ERR;

        err = CHECK_INT(w[i].type, SP_TIDE);
        SUMMARIZE_ERR;
        break;
      case M3:
        err = CHECK_INT(strcmp(w[i].name, "M3"), 0);
        SUMMARIZE_ERR;

        err = CHECK_FLOAT(w[i].freq, 43.4761563 * RAD);
        SUMMARIZE_ERR;

        err = CHECK_INT(w[i].type, SP_TIDE);
        SUMMARIZE_ERR;
        break;
      case M6:
        err = CHECK_INT(strcmp(w[i].name, "M6"), 0);
        SUMMARIZE_ERR;

        err = CHECK_FLOAT(w[i].freq, 86.9523126 * RAD);
        SUMMARIZE_ERR;

        err = CHECK_INT(w[i].type, SP_TIDE);
        SUMMARIZE_ERR;
        break;
      case MN4:
        err = CHECK_INT(strcmp(w[i].name, "MN4"), 0);
        SUMMARIZE_ERR;

        err = CHECK_FLOAT(w[i].freq, 57.4238337 * RAD);
        SUMMARIZE_ERR;

        err = CHECK_INT(w[i].type, SP_TIDE);
        SUMMARIZE_ERR;
        break;
      case MS4:
        err = CHECK_INT(strcmp(w[i].name, "MS4"), 0);
        SUMMARIZE_ERR;

        err = CHECK_FLOAT(w[i].freq, 58.9841042 * RAD);
        SUMMARIZE_ERR;

        err = CHECK_INT(w[i].type, SP_TIDE);
        SUMMARIZE_ERR;
        break;
      case N4:
        err = CHECK_INT(strcmp(w[i].name, "N4"), 0);
        SUMMARIZE_ERR;

        err = CHECK_FLOAT(w[i].freq, 56.8794590 * RAD);
        SUMMARIZE_ERR;

        err = CHECK_INT(w[i].type, SP_TIDE);
        SUMMARIZE_ERR;
        break;
      case R2:
        err = CHECK_INT(strcmp(w[i].name, "R2"), 0);
        SUMMARIZE_ERR;

        err = CHECK_FLOAT(w[i].freq, 30.0410667 * RAD);
        SUMMARIZE_ERR;

        err = CHECK_INT(w[i].type, SP_TIDE);
        SUMMARIZE_ERR;
        break;
      case R4:
        err = CHECK_INT(strcmp(w[i].name, "R4"), 0);
        SUMMARIZE_ERR;

        err = CHECK_FLOAT(w[i].freq, 30.0410667 * 2 * RAD);
        SUMMARIZE_ERR;

        err = CHECK_INT(w[i].type, SP_TIDE);
        SUMMARIZE_ERR;
        break;
      case S4:
        err = CHECK_INT(strcmp(w[i].name, "S4"), 0);
        SUMMARIZE_ERR;

        err = CHECK_FLOAT(w[i].freq, 60.0 * RAD);
        SUMMARIZE_ERR;

        err = CHECK_INT(w[i].type, SP_TIDE);
        SUMMARIZE_ERR;
        break;
      case MNS2:
        err = CHECK_INT(strcmp(w[i].name, "MNS2"), 0);
        SUMMARIZE_ERR;

        err = CHECK_FLOAT(w[i].freq, 27.4238337 * RAD);
        SUMMARIZE_ERR;

        err = CHECK_INT(w[i].type, SP_TIDE);
        SUMMARIZE_ERR;
        break;
      case M13:
        err = CHECK_INT(strcmp(w[i].name, "M13"), 0);
        SUMMARIZE_ERR;

        err = CHECK_FLOAT(w[i].freq, 14.49669394 * RAD);
        SUMMARIZE_ERR;

        err = CHECK_INT(w[i].type, SP_TIDE);
        SUMMARIZE_ERR;
        break;
      case MK4:
        err = CHECK_INT(strcmp(w[i].name, "MK4"), 0);
        SUMMARIZE_ERR;

        err = CHECK_FLOAT(w[i].freq, 59.0662415 * RAD);
        SUMMARIZE_ERR;

        err = CHECK_INT(w[i].type, SP_TIDE);
        SUMMARIZE_ERR;
        break;
      case SN4:
        err = CHECK_INT(strcmp(w[i].name, "SN4"), 0);
        SUMMARIZE_ERR;

        err = CHECK_FLOAT(w[i].freq, 58.4397295 * RAD);
        SUMMARIZE_ERR;

        err = CHECK_INT(w[i].type, SP_TIDE);
        SUMMARIZE_ERR;
        break;
      case SK4:
        err = CHECK_INT(strcmp(w[i].name, "SK4"), 0);
        SUMMARIZE_ERR;

        err = CHECK_FLOAT(w[i].freq, 60.0821373 * RAD);
        SUMMARIZE_ERR;

        err = CHECK_INT(w[i].type, SP_TIDE);
        SUMMARIZE_ERR;
        break;
      case _2MN6:
        err = CHECK_INT(strcmp(w[i].name, "2MN6"), 0);
        SUMMARIZE_ERR;

        err = CHECK_FLOAT(w[i].freq, 86.4079380 * RAD);
        SUMMARIZE_ERR;

        err = CHECK_INT(w[i].type, SP_TIDE);
        SUMMARIZE_ERR;
        break;
      case _2MS6:
        err = CHECK_INT(strcmp(w[i].name, "2MS6"), 0);
        SUMMARIZE_ERR;

        err = CHECK_FLOAT(w[i].freq, 87.9682084 * RAD);
        SUMMARIZE_ERR;

        err = CHECK_INT(w[i].type, SP_TIDE);
        SUMMARIZE_ERR;
        break;
      case _2MK6:
        err = CHECK_INT(strcmp(w[i].name, "2MK6"), 0);
        SUMMARIZE_ERR;

        err = CHECK_FLOAT(w[i].freq, 88.0503457 * RAD);
        SUMMARIZE_ERR;

        err = CHECK_INT(w[i].type, SP_TIDE);
        SUMMARIZE_ERR;
        break;
      case MSN6:
        err = CHECK_INT(strcmp(w[i].name, "MSN6"), 0);
        SUMMARIZE_ERR;

        err = CHECK_FLOAT(w[i].freq, 87.4238337 * RAD);
        SUMMARIZE_ERR;

        err = CHECK_INT(w[i].type, SP_TIDE);
        SUMMARIZE_ERR;
        break;
      case _2SM6:
        err = CHECK_INT(strcmp(w[i].name, "2SM6"), 0);
        SUMMARIZE_ERR;

        err = CHECK_FLOAT(w[i].freq, 88.9841042 * RAD);
        SUMMARIZE_ERR;

        err = CHECK_INT(w[i].type, SP_TIDE);
        SUMMARIZE_ERR;
        break;
      case MSK6:
        err = CHECK_INT(strcmp(w[i].name, "MSK6"), 0);
        SUMMARIZE_ERR;

        err = CHECK_FLOAT(w[i].freq, 89.0662415 * RAD);
        SUMMARIZE_ERR;

        err = CHECK_INT(w[i].type, SP_TIDE);
        SUMMARIZE_ERR;
        break;
      case MP1:
        err = CHECK_INT(strcmp(w[i].name, "MP1"), 0);
        SUMMARIZE_ERR;

        err = CHECK_FLOAT(w[i].freq, 14.0251729 * RAD);
        SUMMARIZE_ERR;

        err = CHECK_INT(w[i].type, SP_TIDE);
        SUMMARIZE_ERR;
        break;
      case _2SM2:
        err = CHECK_INT(strcmp(w[i].name, "2SM2"), 0);
        SUMMARIZE_ERR;

        err = CHECK_FLOAT(w[i].freq, 31.0158958 * RAD);
        SUMMARIZE_ERR;

        err = CHECK_INT(w[i].type, SP_TIDE);
        SUMMARIZE_ERR;
        break;
      case PSI1:
        err = CHECK_INT(strcmp(w[i].name, "PSI1"), 0);
        SUMMARIZE_ERR;

        err = CHECK_FLOAT(w[i].freq, 15.0821353 * RAD);
        SUMMARIZE_ERR;

        err = CHECK_INT(w[i].type, SP_TIDE);
        SUMMARIZE_ERR;
        break;
      case _2MS2:
        err = CHECK_INT(strcmp(w[i].name, "2MS2"), 0);
        SUMMARIZE_ERR;

        err = CHECK_FLOAT(w[i].freq, 27.9682084 * RAD);
        SUMMARIZE_ERR;

        err = CHECK_INT(w[i].type, SP_TIDE);
        SUMMARIZE_ERR;
        break;
      case MKS2:
        err = CHECK_INT(strcmp(w[i].name, "MKS2"), 0);
        SUMMARIZE_ERR;

        err = CHECK_FLOAT(w[i].freq, 29.0662415 * RAD);
        SUMMARIZE_ERR;

        err = CHECK_INT(w[i].type, SP_TIDE);
        SUMMARIZE_ERR;
        break;
      case _2MN2:
        err = CHECK_INT(strcmp(w[i].name, "2MN2"), 0);
        SUMMARIZE_ERR;

        err = CHECK_FLOAT(w[i].freq, 29.5284789 * RAD);
        SUMMARIZE_ERR;

        err = CHECK_INT(w[i].type, SP_TIDE);
        SUMMARIZE_ERR;
        break;
      case MSN2:
        err = CHECK_INT(strcmp(w[i].name, "MSN2"), 0);
        SUMMARIZE_ERR;

        err = CHECK_FLOAT(w[i].freq, 30.5443747 * RAD);
        SUMMARIZE_ERR;

        err = CHECK_INT(w[i].type, SP_TIDE);
        SUMMARIZE_ERR;
        break;
      case MO3:
        err = CHECK_INT(strcmp(w[i].name, "MO3"), 0);
        SUMMARIZE_ERR;

        err = CHECK_FLOAT(w[i].freq, 42.9271398 * RAD);
        SUMMARIZE_ERR;

        err = CHECK_INT(w[i].type, SP_TIDE);
        SUMMARIZE_ERR;
        break;
      case _2MK3:
        err = CHECK_INT(strcmp(w[i].name, "2MK3"), 0);
        SUMMARIZE_ERR;

        err = CHECK_FLOAT(w[i].freq, 42.9271398 * RAD);
        SUMMARIZE_ERR;

        err = CHECK_INT(w[i].type, SP_TIDE);
        SUMMARIZE_ERR;
        break;
      case MK3:
        err = CHECK_INT(strcmp(w[i].name, "MK3"), 0);
        SUMMARIZE_ERR;

        err = CHECK_FLOAT(w[i].freq, 44.0251729 * RAD);
        SUMMARIZE_ERR;

        err = CHECK_INT(w[i].type, SP_TIDE);
        SUMMARIZE_ERR;
        break;
      case S6:
        err = CHECK_INT(strcmp(w[i].name, "S6"), 0);
        SUMMARIZE_ERR;

        err = CHECK_FLOAT(w[i].freq, 90.0 * RAD);
        SUMMARIZE_ERR;

        err = CHECK_INT(w[i].type, SP_TIDE);
        SUMMARIZE_ERR;
        break;
      case M8:
        err = CHECK_INT(strcmp(w[i].name, "M8"), 0);
        SUMMARIZE_ERR;

        err = CHECK_FLOAT(w[i].freq, 115.9364169 * RAD);
        SUMMARIZE_ERR;

        err = CHECK_INT(w[i].type, SP_TIDE);
        SUMMARIZE_ERR;
        break;
      case MSF:
        err = CHECK_INT(strcmp(w[i].name, "MSF"), 0);
        SUMMARIZE_ERR;

        err = CHECK_FLOAT(w[i].freq, 1.01589578 * RAD);
        SUMMARIZE_ERR;

        err = CHECK_INT(w[i].type, SP_TIDE);
        SUMMARIZE_ERR;
        break;
      case SSA:
        err = CHECK_INT(strcmp(w[i].name, "SSA"), 0);
        SUMMARIZE_ERR;

        err = CHECK_FLOAT(w[i].freq, 0.08213728 * RAD);
        SUMMARIZE_ERR;

        err = CHECK_INT(w[i].type, LP_TIDE);
        SUMMARIZE_ERR;
        break;
      default:
        printf("missing %s\n", w[i].name);
        assert(0);
    }
    SUMMARIZE_ERR;
  }

  a.T = 1;
  a.h = 1;
  a.s = 1;
  a.p1 = 1;
  a.p = 1;
  a.I = 1;
  a.xi = 1;
  a.nu = 1;
  a.x1ra = 1;
  a.r = 1;
  a.nuprim = 1;
  a.nusec = 1;

  _nodal_g(&a, w);

  printf("*** testing _nodal_g\n");
  for (i = 0; i < N_WAVES; i++) {
    switch (i) {
      case O1:
        err = CHECK_FLOAT(w[i].v0u * DEG, +91.0);
        break;
      case P1:
        err = CHECK_FLOAT(w[i].v0u * DEG, 90.0);
        break;
      case K1:
        err = CHECK_FLOAT(w[i].v0u * DEG, -89.0);
        break;
      case _2N2:
        err = CHECK_FLOAT(w[i].v0u * DEG, 2.0);
        break;
      case MU2:
        err = CHECK_FLOAT(w[i].v0u * DEG, 2.0);
        break;
      case N2:
        err = CHECK_FLOAT(w[i].v0u * DEG, 2.0);
        break;
      case NU2:
        err = CHECK_FLOAT(w[i].v0u * DEG, 2.0);
        break;
      case M2:
        err = CHECK_FLOAT(w[i].v0u * DEG, 2.0);
        break;
      case L2:
        err = CHECK_FLOAT(w[i].v0u * DEG, 181.0);
        break;
      case T2:
        err = CHECK_FLOAT(w[i].v0u * DEG, 2.0);
        break;
      case S2:
        err = CHECK_FLOAT(w[i].v0u * DEG, 2.0);
        break;
      case K2:
        err = CHECK_FLOAT(w[i].v0u * DEG, 2.0);
        break;
      case M4:
        err = CHECK_FLOAT(w[i].v0u * DEG, 4.0);
        break;
      case S1:
        err = CHECK_FLOAT(w[i].v0u * DEG, 1.0);
        break;
      case Q1:
        err = CHECK_FLOAT(w[i].v0u * DEG, 91.0);
        break;
      case MM:
        err = CHECK_FLOAT(w[i].v0u * DEG, 0.0);
        break;
      case MF:
        err = CHECK_FLOAT(w[i].v0u * DEG, 0.0);
        break;
      case MTM:
        err = CHECK_FLOAT(w[i].v0u * DEG, 0.0);
        break;
      case MSQM:
        err = CHECK_FLOAT(w[i].v0u * DEG, 0.0);
        break;
      case EPS2:
        err = CHECK_FLOAT(w[i].v0u * DEG, 2.0);
        break;
      case LAMBDA2:
        err = CHECK_FLOAT(w[i].v0u * DEG, 182.0);
        break;
      case ETA2:
        err = CHECK_FLOAT(w[i].v0u * DEG, 2.0);
        break;
      case _2Q1:
        err = CHECK_FLOAT(w[i].v0u * DEG, 91.0);
        break;
      case SIGMA1:
        err = CHECK_FLOAT(w[i].v0u * DEG, 91.0);
        break;
      case RHO1:
        err = CHECK_FLOAT(w[i].v0u * DEG, 91.0);
        break;
      case M11:
        err = CHECK_FLOAT(w[i].v0u * DEG, -89.0);
        break;
      case M12:
        err = CHECK_FLOAT(w[i].v0u * DEG, -89.0);
        break;
      case CHI1:
        err = CHECK_FLOAT(w[i].v0u * DEG, -89.0);
        break;
      case PI1:
        err = CHECK_FLOAT(w[i].v0u * DEG, 90.0);
        break;
      case PHI1:
        err = CHECK_FLOAT(w[i].v0u * DEG, -86.0);
        break;
      case THETA1:
        err = CHECK_FLOAT(w[i].v0u * DEG, -89.0);
        break;
      case J1:
        err = CHECK_FLOAT(w[i].v0u * DEG, -89.0);
        break;
      case OO1:
        err = CHECK_FLOAT(w[i].v0u * DEG, -89.0);
        break;
      case M3:
        err = CHECK_FLOAT(w[i].v0u * DEG, 3.0);
        break;
      case M6:
        err = CHECK_FLOAT(w[i].v0u * DEG, 6.0);
        break;
      case MN4:
        err = CHECK_FLOAT(w[i].v0u * DEG, 4.0);
        break;
      case MS4:
        err = CHECK_FLOAT(w[i].v0u * DEG, 4.0);
        break;
      case N4:
        err = CHECK_FLOAT(w[i].v0u * DEG, 4.0);
        break;
      case R2:
        err = CHECK_FLOAT(w[i].v0u * DEG, 182.0);
        break;
      case R4:
        err = CHECK_FLOAT(w[i].v0u * DEG, 4.0);
        break;
      case S4:
        err = CHECK_FLOAT(w[i].v0u * DEG, 4.0);
        break;
      case MNS2:
        err = CHECK_FLOAT(w[i].v0u * DEG, 2.0);
        break;
      case M13:
        err = CHECK_FLOAT(w[i].v0u * DEG, -89.51674239);
        break;
      case MK4:
        err = CHECK_FLOAT(w[i].v0u * DEG, 4.0);
        break;
      case SN4:
        err = CHECK_FLOAT(w[i].v0u * DEG, 4.0);
        break;
      case SK4:
        err = CHECK_FLOAT(w[i].v0u * DEG, 4.0);
        break;
      case _2MN6:
        err = CHECK_FLOAT(w[i].v0u * DEG, 6.0);
        break;
      case _2MS6:
        err = CHECK_FLOAT(w[i].v0u * DEG, 6.0);
        break;
      case _2MK6:
        err = CHECK_FLOAT(w[i].v0u * DEG, 6.0);
        break;
      case MSN6:
        err = CHECK_FLOAT(w[i].v0u * DEG, 6.0);
        break;
      case _2SM6:
        err = CHECK_FLOAT(w[i].v0u * DEG, 6.0);
        break;
      case MSK6:
        err = CHECK_FLOAT(w[i].v0u * DEG, 6.0);
        break;
      case MP1:
        err = CHECK_FLOAT(w[i].v0u * DEG, -89.0);
        break;
      case _2SM2:
        err = CHECK_FLOAT(w[i].v0u * DEG, 2.0);
        break;
      case PSI1:
        err = CHECK_FLOAT(w[i].v0u * DEG, -88.0);
        break;
      case _2MS2:
        err = CHECK_FLOAT(w[i].v0u * DEG, 2.0);
        break;
      case MKS2:
        err = CHECK_FLOAT(w[i].v0u * DEG, 2.0);
        break;
      case _2MN2:
        err = CHECK_FLOAT(w[i].v0u * DEG, 182.0);
        break;
      case MSN2:
        err = CHECK_FLOAT(w[i].v0u * DEG, 2.0);
        break;
      case MO3:
        err = CHECK_FLOAT(w[i].v0u * DEG, 93.0);
        break;
      case _2MK3:
        err = CHECK_FLOAT(w[i].v0u * DEG, 93.0);
        break;
      case MK3:
        err = CHECK_FLOAT(w[i].v0u * DEG, -87.0);
        break;
      case S6:
        err = CHECK_FLOAT(w[i].v0u * DEG, 6.0);
        break;
      case M8:
        err = CHECK_FLOAT(w[i].v0u * DEG, 8.0);
        break;
      case MSF:
        err = CHECK_FLOAT(w[i].v0u * DEG, 0.0);
        break;
      case SSA:
        err = CHECK_FLOAT(w[i].v0u * DEG, 2.0);
        break;
      default:
        assert(0);
    }
    SUMMARIZE_ERR;
  }

  _nodal_a(&a, w);

  printf("*** testing _nodal_a\n");

  for (i = 0; i < N_WAVES; i++) {
    switch (i) {
      case O1:
      case Q1:
      case _2Q1:
      case SIGMA1:
      case RHO1:
      case M12:
        err = CHECK_FLOAT(w[i].f, 1.705420655553602);
        break;
      case P1:
      case T2:
      case S2:
      case S1:
      case PI1:
      case PHI1:
      case R2:
      case R4:
      case S4:
      case PSI1:
      case S6:
      case SSA:
        err = CHECK_FLOAT(w[i].f, 1.0);
        break;
      case K1:
        err = CHECK_FLOAT(w[i].f, 1.0661482919660317);
        break;
      case _2N2:
      case MU2:
      case N2:
      case NU2:
      case EPS2:
      case M2:
      case SN4:
      case MS4:
      case LAMBDA2:
      case _2SM6:
      case _2SM2:
      case MSF:
        err = CHECK_FLOAT(w[i].f, 0.64794930999090794);
        break;
      case L2:
        err = CHECK_FLOAT(w[i].f, 0.64794930999090794);
        break;
      case K2:
      case SK4:
        err = CHECK_FLOAT(w[i].f, 2.9715447147317122);
        break;
      case ETA2:
        err = CHECK_FLOAT(w[i].f, 4.5244307876905507);
        break;
      case M11:
      case CHI1:
      case THETA1:
      case J1:
      case MP1:
        err = CHECK_FLOAT(w[i].f, 1.2604621941026914);
        break;
      case OO1:
        err = CHECK_FLOAT(w[i].f, 11.793361932776087);
        break;
      case M4:
      case MN4:
      case MNS2:
      case N4:
      case _2MS6:
      case MSN6:
      case _2MS2:
      case MSN2:
        err = CHECK_FLOAT(w[i].f, 0.4198383083176937);
        break;
      case MF:
      case MTM:
      case MSQM:
        err = CHECK_FLOAT(w[i].f, 4.4871572767653438);
        break;
      case MM:
        err = CHECK_FLOAT(w[i].f, -0.08246714122068223);
        break;
      case M3:
        err = CHECK_FLOAT(w[i].f, 0.5215824486233587);
        break;
      case M6:
      case _2MN6:
      case _2MN2:
        err = CHECK_FLOAT(w[i].f, 0.2720339421821998);
        break;
      case M13:
        err = CHECK_FLOAT(w[i].f, 3.300330475634);
        break;
      case MK4:
      case MSK6:
      case MKS2:
        err = CHECK_FLOAT(w[i].f, 0.64794930999090794 * 2.9715447147317122);
        break;
      case _2MK6:
        err = CHECK_FLOAT(w[i].f, 0.2720339421821998 * 2.9715447147317122);
        break;
      case MO3:
        err = CHECK_FLOAT(w[i].f, 0.64794930999090794 * 1.705420655553602);
        break;
      case _2MK3:
        err = CHECK_FLOAT(w[i].f, 0.4198383083176937 * 1.0661482919660317);
        break;
      case MK3:
        err = CHECK_FLOAT(w[i].f, 0.64794930999090794 * 1.0661482919660317);
        break;
      case M8:
        err = CHECK_FLOAT(w[i].f, POW4(0.64794930999090794));
        break;
      default:
        assert(0);
    }
    SUMMARIZE_ERR;
  }

  _astronomics(0.0, &a);

  printf("*** testing _astronomic_angle\n");

  err = CHECK_FLOAT(a.T, 3.14159265359);
  SUMMARIZE_ERR;

  err = CHECK_FLOAT(a.h, 4.8902293307);
  SUMMARIZE_ERR;

  err = CHECK_FLOAT(a.s, 4.83500919185);
  SUMMARIZE_ERR;

  err = CHECK_FLOAT(a.p1, 4.90822987644);
  SUMMARIZE_ERR;

  err = CHECK_FLOAT(a.p, 5.8360969035);
  SUMMARIZE_ERR;

  err = CHECK_FLOAT(a.I, 0.401668407249);
  SUMMARIZE_ERR;

  err = CHECK_FLOAT(a.xi, -0.208946712189);
  SUMMARIZE_ERR;

  err = CHECK_FLOAT(a.nu, -0.227235371854);
  SUMMARIZE_ERR;

  err = CHECK_FLOAT(a.x1ra, 0.787307935551);
  SUMMARIZE_ERR;

  err = CHECK_FLOAT(a.r, -0.145318382345);
  SUMMARIZE_ERR;

  err = CHECK_FLOAT(a.nuprim, -0.155257427004);
  SUMMARIZE_ERR;

  err = CHECK_FLOAT(a.nusec, -0.154608130984);
  SUMMARIZE_ERR;

  err = CHECK_FLOAT(julian_centuries(18263.0), 1);
  SUMMARIZE_ERR;

  for (i = 0; i < N_WAVES; i++) {
    w[i].c.re = 1;
    w[i].c.im = 1;
  }

  printf("*** testing admittance\n");

  err = test_admittance(0.2378, _2Q1, w);
  SUMMARIZE_ERR;

  err = test_admittance(0.2706, SIGMA1, w);
  SUMMARIZE_ERR;

  err = test_admittance(0.1688, RHO1, w);
  SUMMARIZE_ERR;

  err = test_admittance(0.0671, M11, w);
  SUMMARIZE_ERR;

  err = test_admittance(0.0241, M12, w);
  SUMMARIZE_ERR;

  err = test_admittance(0.0124, CHI1, w);
  SUMMARIZE_ERR;

  err = test_admittance(0.0201, PI1, w);
  SUMMARIZE_ERR;

  err = test_admittance(0.0137, PHI1, w);
  SUMMARIZE_ERR;

  err = test_admittance(0.009, THETA1, w);
  SUMMARIZE_ERR;

  err = test_admittance(0.0447, J1, w);
  SUMMARIZE_ERR;

  err = test_admittance(0.0182, OO1, w);
  SUMMARIZE_ERR;

  err = test_admittance(0.0796782, ETA2, w);
  SUMMARIZE_ERR;

  err = test_admittance(0.374697218357, MU2, w);
  SUMMARIZE_ERR;

  err = test_admittance(0.157529811402, NU2, w);
  SUMMARIZE_ERR;

  err = test_admittance(0.010949128375, LAMBDA2, w);
  SUMMARIZE_ERR;

  err = test_admittance(0.053354227357, L2, w);
  SUMMARIZE_ERR;

  err = test_admittance(0.16871051505, T2, w);
  SUMMARIZE_ERR;

  err = test_admittance(0.2387, _2N2, w);
  SUMMARIZE_ERR;

  err = test_admittance(0.094151295, EPS2, w); /*cas ou 2N2 calculee par admittance auparavant */
  SUMMARIZE_ERR;

  w[_2N2].admittance = 0; /* on force 2N2 a etre calcule en dynamique */
  w[_2N2].c.re = 1;
  w[_2N2].c.im = 1;
  err = test_admittance(0.499810, EPS2, w);
  SUMMARIZE_ERR;

  w[MM].dynamic = 1;
  w[MF].dynamic = 1;
  w[MTM].dynamic = 1;
  w[MSQM].dynamic = 1;

  set_w2nd(w, w2nd);

  printf("*** testing lpe_minus_5_waves\n");
  lpe_minus_n_waves((const float (*)[N_COEFS]) (w2nd), 1, 1, &hlp);
  err = CHECK_FLOAT(hlp, -1.124059213786);
  SUMMARIZE_ERR;

  FINAL_RESULTS;
}
