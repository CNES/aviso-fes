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

#include "prediction.h"

#include <assert.h>
#include <math.h>
#include <string.h>

#include "angle.h"

/*
 _astronomics

 Initialize some astronomic data useful for nodal corrections.

 itj Desired UTC time, in (decimal) Modified.
 a astronomic angle computed
 */
static void _astronomics(const double itj, _fes_astronomic_angle* const a) {
  static const double ct0 = 180.0;
  static const double ct1 = 360.0 * 3.6525E+04;
  static const double cn0 = 259.1560563;
  static const double cn1 = -1934.1423972;
  static const double cs0 = 277.0256206;
  static const double cs1 = 481267.892;
  static const double ch0 = 280.1895015;
  static const double ch1 = 36000.76892;
  static const double cps0 = 281.2208568;
  static const double cps1 = 1.719175;
  static const double cp0 = 334.3837214;
  static const double cp1 = 4069.0322056;
  double tgn2;
  double at1;
  double at2;
  double u;
  double tgi2;
  double n;
  double pp;

  /* T mean solar angle relative to Greenwich */
  a->T = fmod(ct0 + ct1 * itj, 360.0) * RAD;

  /* n longitude of ascending lunar node */
  n = fmod(cn0 + cn1 * itj, 360.0) * RAD;

  /* h mean solar longitude */
  a->h = fmod(ch0 + ch1 * itj, 360.0) * RAD;

  /* s mean lunar longitude */
  a->s = fmod(cs0 + cs1 * itj, 360.0) * RAD;

  /* p1 longitude of solar perigee */
  a->p1 = fmod(cps0 + cps1 * itj, 360.0) * RAD;

  /* p longitude of lunar perigee */
  a->p = fmod(cp0 + cp1 * itj, 360.0) * RAD;

  u = 9.13694997e-01 - 3.5692561e-02 * cos(n);

  a->I = acos(u);

  tgn2 = tan(n * 0.5);

  at1 = atan(1.01883 * tgn2);
  at2 = atan(6.4412e-01 * tgn2);

  a->xi = -at1 - at2 + n;

  if (n > M_PI)
    a->xi -= 2.0 * M_PI;

  a->nu = at1 - at2;

  /* for constituents l2,k1,k2 */
  tgi2 = tan(a->I * 0.5);

  pp = a->p - a->xi;

  a->x1ra = sqrt(1.0 - 12.0 * SQR(tgi2) * cos(2.0 * pp) + 36.0 * POW4(tgi2));

  a->r = atan(sin(2.0 * pp) / (1.0 / (6.0 * SQR(tgi2)) - cos(2.0 * pp)));

  a->nuprim = atan(
      sin(2.0 * (a->I)) * sin(a->nu)
          / (sin(2.0 * (a->I)) * cos(a->nu) + 3.347E-01));

  a->nusec = 0.5
      * atan(
          ((SQR(sin(a->I))) * sin(2.0 * (a->nu)))
              / (SQR (sin (a->I)) * cos(2.0 * a->nu) + 7.27E-02));
}

/*
 _f_o1  = sin I cos² ½I / 0.3800

 a Astronomic angle
 */
static double _f_o1(const _fes_astronomic_angle* const a) {
  return sin(a->I) * SQR(cos(a->I * 0.5)) / 0.3800;
}

/*
 _f_oo1  = sin I sin² ½I / 0.0164

 a Astronomic angle
 */
static double _f_oo1(const _fes_astronomic_angle* const a) {
  return sin(a->I) * SQR(sin(a->I * 0.5)) / 0.01640;
}

/*
 _f_1  = 1

 a Astronomic angle
 */
static double _f_1(UNUSED const _fes_astronomic_angle* const a) {
  return 1;
}

/*
 _f_j1  = sin 2I / 0.7214

 a Astronomic angle
 */
static double _f_j1(const _fes_astronomic_angle* const a) {
  return sin(2.0 * a->I) / 0.7214;
}

/*
 _f_m13 = (1 -10 sin² ½I + 15 sin⁴ ½I) cos² ½I / 0.5873

 a Astronomic angle
 */
static double _f_m13(const _fes_astronomic_angle* const a) {
  return _f_o1(a) * sqrt(2.310 + 1.435 * cos(2.0 * (a->p - a->xi)));
}

/*
 _f_m2 = cos⁴ ½I / 0.9154

 a Astronomic angle
 */
static double _f_m2(const _fes_astronomic_angle* const a) {
  return POW4 (cos (a->I * 0.5)) / 0.9154;
}

/*
 _f_m3 = cos⁶ ½I / 0.8758

 a Astronomic angle
 */
static double _f_m3(const _fes_astronomic_angle* const a) {
  return pow(cos(a->I * 0.5), 6.0) / 0.8758;
}

/*
 _f_mf = sin² I / 0.1578

 a Astronomic angle
 */
static double _f_mf(const _fes_astronomic_angle* const a) {
  return SQR (sin (a->I)) / 0.1578;
}

/*
 _f_mm = (2/3 - sin² I) / 0.5021

 a Astronomic angle
 */
static double _f_mm(const _fes_astronomic_angle* const a) {
  return (2.0 / 3.0 - SQR(sin(a->I))) / 0.5021;
}

/*
 _f_m22 = f²(M₂)

 a Astronomic angle
 */
static double _f_m22(const _fes_astronomic_angle* const a) {
  return SQR(_f_m2(a));
}

/*
 _f_m23 = f(M₂)³

 a Astronomic angle
 */
static double _f_m23(const _fes_astronomic_angle* const a) {
  return POW3(_f_m2(a));
}
/*
 _f_m24 = f(M₂)⁴

 a Astronomic angle
 */
static double _f_m24(const _fes_astronomic_angle* const a) {
  return POW4(_f_m2(a));
}

/*
 _f_k1 = √(0.8965 sin² 2I+0.6001 sin 2I cos ν + 0.1006)

 a Astronomic angle
 */
static double _f_k1(const _fes_astronomic_angle* const a) {
  return sqrt(
      0.8965 * SQR(sin(2.0 * a->I)) + 0.6001 * sin(2.0 * a->I) * cos(a->nu)
          + 0.1006);
}

/*
 _f_k2 = √(19.0444 sin⁴ I + 2.7702 sin² I cos 2ν + 0.0981)

 a Astronomic angle
 */
static double _f_k2(const _fes_astronomic_angle* const a) {
  return sqrt(
      19.0444 * POW4(sin(a->I)) + 2.7702 * SQR(sin(a->I)) * cos(2.0 * a->nu)
          + 0.0981);
}

/*
 _f_kj2 = sin² I / 0.1565 (formula #79)


 a Astronomic angle
 */
static double _f_kj2(const _fes_astronomic_angle* const a) {
  return SQR (sin (a->I)) / 0.1565;
}

/*
 _f_l2 = _f_m2(a) * 1 / Ra

 a Astronomic angle
 */
static double _f_l2(const _fes_astronomic_angle* const a) {
  return _f_m2(a) * a->x1ra;
}

/*
 _f_m2_k2 = f(M₂) * f(K₂)

 a Astronomic angle
 */
static double _f_m2_k2(const _fes_astronomic_angle* const a) {
  return _f_m2(a) * _f_k2(a);
}

/*
 _f_m2_k1 = f(M₂) * f(K₁)

 a Astronomic angle
 */
static double _f_m2_k1(const _fes_astronomic_angle* const a) {
  return _f_m2(a) * _f_k1(a);
}

/*
 _f_m2_o1 = f(M₂) * f(O₁)

 a Astronomic angle
 */
static double _f_m2_o1(const _fes_astronomic_angle* const a) {
  return _f_m2(a) * _f_o1(a);
}

/*
 _f_m22_k1 = f(M₂)² * f(K₁)

 a Astronomic angle
 */
static double _f_m22_k1(const _fes_astronomic_angle* const a) {
  return _f_m22(a) * _f_k1(a);
}
/*
 _f_m23_k2 = f(M₂)³ * f(K₂)

 a Astronomic angle
 */
static double _f_m23_k2(const _fes_astronomic_angle* const a) {
  return _f_m23(a) * _f_k2(a);
}

/*
 _nodal_a

 Compute nodal corrections from SCHUREMAN (1958).

 Indexes used in this routine are internal to the code
 and corresponds to the "original" ondes.dat file.

 a Astronomic angle
 w Pointer to the array which contains waves definition.
 */
static void _nodal_a(const _fes_astronomic_angle* const a, fes_wave* const w) {
  int ix;

  for (ix = 0; ix < N_WAVES; ++ix) {
    w[ix].f = w[ix].f_function(a);
  }
}

/*
 _nodal_g

 Compute nodal corrections from SCHUREMAN (1958).

 Indexes used in this routine are internal to the code and corresponds to the
 "original" ondes.dat file.

 a Astronomic angle
 w Pointer to the array which contains waves definition

 */
static void _nodal_g(const _fes_astronomic_angle* const a, fes_wave* const w) {
  int ix;
  register double u;
  register double v;

  for (ix = 0; ix < N_WAVES; ix++) {
    v = w[ix].argument[0] * a->T + w[ix].argument[1] * a->s
        + w[ix].argument[2] * a->h + w[ix].argument[3] * a->p
        + w[ix].argument[5] * a->p1 + w[ix].argument[6] * 90.0;

    u = w[ix].argument[7] * a->xi + w[ix].argument[8] * a->nu
        + w[ix].argument[9] * a->nuprim + w[ix].argument[10] * a->nusec;
    if (ix == L2)
      u -= a->r;
    if (ix == M13)
      u -= 1.0 / sqrt(2.310 + 1.435 * cos(2 * (a->p - a->xi)));
    w[ix].v0u = fmod(v + u, 360.00) * RAD;
  }
}

/*

 */
void admittance(fes_wave* const waves) {
  /*
   Arrays who contains the spline coefficients needed to compute MU2, NU2,
   L2, T2 and Lambda2 by admittance.
   */
  static const double mu2[3] =
      { 0.069439968323, 0.351535557706, -0.046278307672 };
  static const double nu2[3] =
      { -0.006104695053, 0.156878802427, 0.006755704028 };
  static const double l2[3] =
      { 0.077137765667, -0.051653455134, 0.027869916824 };
  static const double t2[3] =
      { 0.180480173707, -0.020101177502, 0.008331518844 };
  static const double lda2[3] =
      { 0.016503557465, -0.013307812292, 0.007753383202 };
  register fes_double_complex* x;
  register fes_double_complex* y;
  register fes_double_complex* z;

  /*
   infer additional constituents by admittance DIURNALS
   (from Richard Ray perth2 program)
   */

  /* from Q1 and O1 (0-1) */

  x = &waves[Q1].c;
  y = &waves[O1].c;
  z = &waves[K1].c;

  /* 2Q1 */
  if (waves[_2Q1].admittance) {
    waves[_2Q1].c.re = 0.263 * x->re - 0.0252 * y->re;
    waves[_2Q1].c.im = 0.263 * x->im - 0.0252 * y->im;
  }
  /* Sigma1 */
  if (waves[SIGMA1].admittance) {
    waves[SIGMA1].c.re = 0.297 * x->re - 0.0264 * y->re;
    waves[SIGMA1].c.im = 0.297 * x->im - 0.0264 * y->im;
  }
  /* ro1 */
  if (waves[RHO1].admittance) {
    waves[RHO1].c.re = 0.164 * x->re + 0.0048 * y->re;
    waves[RHO1].c.im = 0.164 * x->im + 0.0048 * y->im;
  }

  /* from O1 and K1  (1-2) */

  /* M11 */
  if (waves[M11].admittance) {
    waves[M11].c.re = 0.0389 * y->re + 0.0282 * z->re;
    waves[M11].c.im = 0.0389 * y->im + 0.0282 * z->im;
  }
  /* M12 */
  if (waves[M12].admittance) {
    waves[M12].c.re = 0.0140 * y->re + 0.0101 * z->re;
    waves[M12].c.im = 0.0140 * y->im + 0.0101 * z->im;
  }
  /* CHI1 */
  if (waves[CHI1].admittance) {
    waves[CHI1].c.re = 0.0064 * y->re + 0.0060 * z->re;
    waves[CHI1].c.im = 0.0064 * y->im + 0.0060 * z->im;
  }
  /* pi1 */
  if (waves[PI1].admittance) {
    waves[PI1].c.re = 0.0030 * y->re + 0.0171 * z->re;
    waves[PI1].c.im = 0.0030 * y->im + 0.0171 * z->im;
  }
  /* phi1 */
  if (waves[PHI1].admittance) {
    waves[PHI1].c.re = -0.0015 * y->re + 0.0152 * z->re;
    waves[PHI1].c.im = -0.0015 * y->im + 0.0152 * z->im;
  }
  /* teta1 */
  if (waves[THETA1].admittance) {
    waves[THETA1].c.re = -0.0065 * y->re + 0.0155 * z->re;
    waves[THETA1].c.im = -0.0065 * y->im + 0.0155 * z->im;
  }
  /* J1 */
  if (waves[J1].admittance) {
    waves[J1].c.re = -0.0389 * y->re + 0.0836 * z->re;
    waves[J1].c.im = -0.0389 * y->im + 0.0836 * z->im;
  }
  /* OO1 */
  if (waves[OO1].admittance) {
    waves[OO1].c.re = -0.0431 * y->re + 0.0613 * z->re;
    waves[OO1].c.im = -0.0431 * y->im + 0.0613 * z->im;
  }

  /*
   infer additional constituents by admittance SEMI-DIURNALS
   (from Richard Ray perth3 program)
   2N2
   */
  /* from M2 - N2 */
  x = &waves[N2].c;
  y = &waves[M2].c;

  /* 2N2 */
  if (waves[_2N2].admittance) {
    waves[_2N2].c.re = 0.264 * x->re - 0.0253 * y->re;
    waves[_2N2].c.im = 0.264 * x->im - 0.0253 * y->im;
  }
  /* SEMI-DIURNAL (from Grenoble to take advantage of 2N2) */

  /* from 2N2 -N2 (3-4) */

  x = &waves[_2N2].c;
  y = &waves[N2].c;

  /* eps2 */
  if (waves[EPS2].admittance) {
    waves[EPS2].c.re = 0.53285 * x->re - 0.03304 * y->re;
    waves[EPS2].c.im = 0.53285 * x->im - 0.03304 * y->im;
  }

  /* from M2 - K2 [5-6] */

  x = &waves[N2].c;
  y = &waves[M2].c;
  z = &waves[K2].c;

  /* eta2 */
  if (waves[ETA2].admittance) {
    waves[ETA2].c.re = -0.0034925 * y->re + 0.0831707 * z->re;
    waves[ETA2].c.im = -0.0034925 * y->im + 0.0831707 * z->im;
  }

  /* from N2 -M2- K2 by spline admittances [see GRL 18[5]:845-848,1991] */

  /* mu2 */
  if (waves[MU2].admittance) {
    waves[MU2].c.re = mu2[0] * z->re + mu2[1] * x->re + mu2[2] * y->re;
    waves[MU2].c.im = mu2[0] * z->im + mu2[1] * x->im + mu2[2] * y->im;
  }
  /* nu2 */
  if (waves[NU2].admittance) {
    waves[NU2].c.re = nu2[0] * z->re + nu2[1] * x->re + nu2[2] * y->re;
    waves[NU2].c.im = nu2[0] * z->im + nu2[1] * x->im + nu2[2] * y->im;
  }
  /* lambda2 */
  if (waves[LAMBDA2].admittance) {
    waves[LAMBDA2].c.re = lda2[0] * z->re + lda2[1] * x->re + lda2[2] * y->re;
    waves[LAMBDA2].c.im = lda2[0] * z->im + lda2[1] * x->im + lda2[2] * y->im;
  }
  /* L2 */
  if (waves[L2].admittance) {
    waves[L2].c.re = l2[0] * z->re + l2[1] * x->re + l2[2] * y->re;
    waves[L2].c.im = l2[0] * z->im + l2[1] * x->im + l2[2] * y->im;
  }
  /* T2 */
  if (waves[T2].admittance) {
    waves[T2].c.re = t2[0] * z->re + t2[1] * x->re + t2[2] * y->re;
    waves[T2].c.im = t2[0] * z->im + t2[1] * x->im + t2[2] * y->im;
  }
}

/*
 */
void compute_nodal_corrections(const double t0, fes_wave* const w) {
  _fes_astronomic_angle astro;

  _astronomics(t0, &astro);

  _nodal_a(&astro, w);

  astro.T *= DEG;
  astro.h *= DEG;
  astro.s *= DEG;
  astro.p1 *= DEG;
  astro.p *= DEG;
  astro.xi *= DEG;
  astro.nu *= DEG;
  astro.r *= DEG;
  astro.nuprim *= DEG;
  astro.nusec *= DEG;

  _nodal_g(&astro, w);
}

/*
 */
double julian_centuries(const double date) {
  /*
   * 18262.0 = number of day elapsed between 1950-01-01 00:00:00.000 UTC and
   * 1900-01-01 00:00:00.000 UTC
   */
  return (18262.0 + date) / 36525.0;
}

/*
 */
void set_w2nd(const fes_wave* const w, float w2nd[][N_COEFS]) {
  // Table below = Doodson coefficients
  // tau s   h   p   N'  p1  coef  => Doodson = several combination for
  //    waves/secondary waves = variation for N' factor in comparison with
  //    Darwin
  // T   s   h   p   p1      coef  => Darwin = only combination for each
  //    waves, secondary waves are not explicitely described
  static const float cst_w2nd[N_WAVES_2ND][N_COEFS] = {
      {/* 0,*/ 0,  0,  0,  1,  0,  0.02793f }, /*   0 */
      {/* 0,*/ 0,  0,  0,  2,  0, -0.00027f }, /*   1 */
      {/* 0,*/ 0,  0,  2,  1,  0,  0.00004f }, /*   2 */
      {/* 0,*/ 0,  1,  0, -1, -1, -0.00004f }, /*   3 */
      {/* 0,*/ 0,  1,  0,  0, -1, -0.00492f }, /*   4 */
      {/* 0,*/ 0,  1,  0,  0,  1,  0.00026f }, /*   5 */
      {/* 0,*/ 0,  1,  0,  1, -1,  0.00005f }, /*   6 */
      {/* 0,*/ 0,  2, -2, -1,  0,  0.00002f }, /*   7 */
      {/* 0,*/ 0,  2, -2,  0,  0, -0.00031f }, /*   8 */
      {/* 0,*/ 0,  2,  0,  0,  0, -0.03095f }, /*   9 - ssa */
      {/* 0,*/ 0,  2,  0,  0, -2, -0.00008f }, /*  10 */
      {/* 0,*/ 0,  2,  0,  1,  0,  0.00077f }, /*  11 - ssa */
      {/* 0,*/ 0,  2,  0,  2,  0,  0.00017f }, /*  12 - ssa */
      {/* 0,*/ 0,  3,  0,  0, -1, -0.00181f }, /*  13 */
      {/* 0,*/ 0,  3,  0,  1, -1,  0.00003f }, /*  14 */
      {/* 0,*/ 0,  4,  0,  0, -2, -0.00007f }, /*  15 */
      {/* 0,*/ 1, -3,  1, -1,  1,  0.00002f }, /*  16 */
      {/* 0,*/ 1, -3,  1,  0,  1, -0.00029f }, /*  17 */
      {/* 0,*/ 1, -3,  1,  1,  1,  0.00002f }, /*  18 */
      {/* 0,*/ 1, -2, -1, -2,  0,  0.00003f }, /*  19 */
      {/* 0,*/ 1, -2, -1, -1,  0,  0.00007f }, /*  20 */
      {/* 0,*/ 1, -2,  1, -1,  0,  0.00048f }, /*  21 */
      {/* 0,*/ 1, -2,  1,  0,  0, -0.00673f }, /*  22 */
      {/* 0,*/ 1, -2,  1,  1,  0,  0.00043f }, /*  23 */
      {/* 0,*/ 1, -1, -1, -1,  1,  0.00002f }, /*  24 */
      {/* 0,*/ 1, -1, -1,  0,  1, -0.00021f }, /*  25 */
      {/* 0,*/ 1, -1, -1,  1,  1,  0.00000f }, /*  26 */
      {/* 0,*/ 1, -1,  0,  0,  0,  0.00020f }, /*  27 */
      {/* 0,*/ 1, -1,  1,  0, -1,  0.00005f }, /*  28 */
      {/* 0,*/ 1,  0, -1, -2,  0, -0.00003f }, /*  29 - Mm for FES2014 */
      {/* 0,*/ 1,  0, -1, -1,  0,  0.00231f }, /*  30 - Mm for FES2014 */
      {/* 0,*/ 1,  0, -1,  0,  0, -0.03518f }, /*  31 - Mm */
      {/* 0,*/ 1,  0, -1,  1,  0,  0.00228f }, /*  32 - Mm */
      {/* 0,*/ 1,  0,  1,  0,  0,  0.00189f }, /*  33 */
      {/* 0,*/ 1,  0,  1,  1,  0,  0.00077f }, /*  34 */
      {/* 0,*/ 1,  0,  1,  2,  0,  0.00021f }, /*  35 */
      {/* 0,*/ 1,  1, -1,  0, -1,  0.00018f }, /*  36 */
      {/* 0,*/ 1,  2, -1,  0,  0,  0.00049f }, /*  37 */
      {/* 0,*/ 1,  2, -1,  1,  0,  0.00024f }, /*  38 */
      {/* 0,*/ 1,  2, -1,  2,  0,  0.00004f }, /*  39 */
      {/* 0,*/ 1,  3, -1,  0, -1,  0.00003f }, /*  40 */
      {/* 0,*/ 2, -4,  2,  0,  0, -0.00011f }, /*  41 */
      {/* 0,*/ 2, -3,  0,  0,  1, -0.00038f }, /*  42 */
      {/* 0,*/ 2, -3,  0,  1,  1,  0.00002f }, /*  43 */
      {/* 0,*/ 2, -2,  0, -1,  0, -0.00042f }, /*  44 */
      {/* 0,*/ 2, -2,  0,  0,  0, -0.00582f }, /*  45 */
      {/* 0,*/ 2, -2,  0,  1,  0,  0.00037f }, /*  46 */
      {/* 0,*/ 2, -2,  2,  0,  0,  0.00004f }, /*  47 */
      {/* 0,*/ 2, -1, -2,  0,  1, -0.00004f }, /*  48 */
      {/* 0,*/ 2, -1, -1,  0,  0,  0.00003f }, /*  49 */
      {/* 0,*/ 2, -1,  0,  0, -1,  0.00007f }, /*  50 */
      {/* 0,*/ 2, -1,  0,  0,  1, -0.00020f }, /*  51 */
      {/* 0,*/ 2, -1,  0,  1,  1, -0.00004f }, /*  52 */
      {/* 0,*/ 2,  0, -2, -1,  0,  0.00015f }, /*  53 */
      {/* 0,*/ 2,  0, -2,  0,  0, -0.00288f }, /*  54 */
      {/* 0,*/ 2,  0, -2,  1,  0,  0.00019f }, /*  55 */
      {/* 0,*/ 2,  0,  0,  0,  0, -0.06662f }, /*  56 - Mf */
      {/* 0,*/ 2,  0,  0,  1,  0, -0.02762f }, /*  57 - Mf */
      {/* 0,*/ 2,  0,  0,  2,  0, -0.00258f }, /*  58 - Mf */
      {/* 0,*/ 2,  0,  0,  3,  0,  0.00007f }, /*  59 - Mf */
      {/* 0,*/ 2,  1, -2,  0, -1,  0.00003f }, /*  60 */
      {/* 0,*/ 2,  1,  0,  0, -1,  0.00023f }, /*  61 */
      {/* 0,*/ 2,  1,  0,  1, -1,  0.00006f }, /*  62 */
      {/* 0,*/ 2,  2, -2,  0,  0,  0.00020f }, /*  63 */
      {/* 0,*/ 2,  2, -2,  1,  0,  0.00008f }, /*  64 */
      {/* 0,*/ 2,  2,  0,  2,  0,  0.00003f }, /*  65 */
      {/* 0,*/ 3, -5,  1,  0,  1, -0.00002f }, /*  66 */
      {/* 0,*/ 3, -4,  1,  0,  0, -0.00017f }, /*  67 */
      {/* 0,*/ 3, -3, -1,  0,  1, -0.00007f }, /*  68 */
      {/* 0,*/ 3, -3,  1,  0,  1, -0.00012f }, /*  69 */
      {/* 0,*/ 3, -3,  1,  1,  1, -0.00004f }, /*  70 */
      {/* 0,*/ 3, -2, -1, -1,  0, -0.00010f }, /*  71 */
      {/* 0,*/ 3, -2, -1,  0,  0, -0.00091f }, /*  72 */
      {/* 0,*/ 3, -2, -1,  1,  0,  0.00006f }, /*  73 */
      {/* 0,*/ 3, -2,  1,  0,  0, -0.00242f }, /*  74 */
      {/* 0,*/ 3, -2,  1,  1,  0, -0.00100f }, /*  75 */
      {/* 0,*/ 3, -2,  1,  2,  0, -0.00009f }, /*  76 */
      {/* 0,*/ 3, -1, -1,  0,  1, -0.00013f }, /*  77 */
      {/* 0,*/ 3, -1, -1,  1,  1, -0.00004f }, /*  78 */
      {/* 0,*/ 3, -1,  0,  0,  0,  0.00006f }, /*  79 */
      {/* 0,*/ 3, -1,  0,  1,  0,  0.00003f }, /*  80 */
      {/* 0,*/ 3, -1,  1,  0, -1,  0.00003f }, /*  81 */
      {/* 0,*/ 3,  0, -3,  0,  0, -0.00023f }, /*  82 */
      {/* 0,*/ 3,  0, -3,  1, -1,  0.00004f }, /*  83 */
      {/* 0,*/ 3,  0, -3,  1,  1,  0.00004f }, /*  84 */
      {/* 0,*/ 3,  0, -1,  0,  0, -0.01275f }, /*  85 - Mtm */
      {/* 0,*/ 3,  0, -1,  1,  0, -0.00528f }, /*  86 - Mtm */
      {/* 0,*/ 3,  0, -1,  2,  0, -0.00051f }, /*  87 - Mtm */
      {/* 0,*/ 3,  0,  1,  2,  0,  0.00005f }, /*  88 */
      {/* 0,*/ 3,  0,  1,  3,  0,  0.00002f }, /*  89 */
      {/* 0,*/ 3,  1, -1,  0, -1,  0.00011f }, /*  90 */
      {/* 0,*/ 3,  1, -1,  1, -1,  0.00004f }, /*  91 */
      {/* 0,*/ 4, -4,  0,  0,  0, -0.00008f }, /*  92 */
      {/* 0,*/ 4, -4,  2,  0,  0, -0.00006f }, /*  93 */
      {/* 0,*/ 4, -4,  2,  1,  0, -0.00002f }, /*  94 */
      {/* 0,*/ 4, -3,  0,  0,  1, -0.00014f }, /*  95 */
      {/* 0,*/ 4, -3,  0,  1,  1, -0.00006f }, /*  96 */
      {/* 0,*/ 4, -2, -2,  0,  0, -0.00011f }, /*  97 */
      {/* 0,*/ 4, -2,  0,  0,  0, -0.00205f }, /*  98 - Msqm */
      {/* 0,*/ 4, -2,  0,  1,  0, -0.00085f }, /*  99 - Msqm */
      {/* 0,*/ 4, -2,  0,  2,  0, -0.00008f }, /* 100 - Msqm : for FES2014*/
      {/* 0,*/ 4, -1, -2,  0,  1, -0.00003f }, /* 101 */
      {/* 0,*/ 4, -1,  0,  0, -1,  0.00003f }, /* 102 */
      {/* 0,*/ 4,  0, -2,  0,  0, -0.00169f }, /* 103 */
      {/* 0,*/ 4,  0, -2,  1,  0, -0.00070f }, /* 104 */
      {/* 0,*/ 4,  0, -2,  2,  0, -0.00006f }, /* 105 */
  };
  int ix;

  for (ix = 0; ix < N_WAVES_2ND; ix++) {
    memcpy(w2nd[ix], cst_w2nd[ix], sizeof(float) * N_COEFS);
  }

  if (w[MM].dynamic == 1) {
    memset(w2nd[29], 0, sizeof(float) * N_COEFS);
    memset(w2nd[30], 0, sizeof(float) * N_COEFS);
    memset(w2nd[31], 0, sizeof(float) * N_COEFS);
    memset(w2nd[32], 0, sizeof(float) * N_COEFS);
  }
  if (w[MF].dynamic == 1) {
    memset(w2nd[56], 0, sizeof(float) * N_COEFS);
    memset(w2nd[57], 0, sizeof(float) * N_COEFS);
    memset(w2nd[58], 0, sizeof(float) * N_COEFS);
    memset(w2nd[59], 0, sizeof(float) * N_COEFS);
  }
  if (w[MTM].dynamic == 1) {
    memset(w2nd[85], 0, sizeof(float) * N_COEFS);
    memset(w2nd[86], 0, sizeof(float) * N_COEFS);
    memset(w2nd[87], 0, sizeof(float) * N_COEFS);
  }
  if (w[MSQM].dynamic == 1) {
    memset(w2nd[98], 0, sizeof(float) * N_COEFS);
    memset(w2nd[99], 0, sizeof(float) * N_COEFS);
    memset(w2nd[100], 0, sizeof(float) * N_COEFS);
  }
  if (w[SSA].dynamic == 1) {
    memset(w2nd[9], 0, sizeof(float) * N_COEFS);
    memset(w2nd[11], 0, sizeof(float) * N_COEFS);
    memset(w2nd[12], 0, sizeof(float) * N_COEFS);
  }
}

/*
 */
void lpe_minus_n_waves(const float w2nd[][N_COEFS], const double ts,
                       const double lat, double* tlp) {
#define N_WAVES_3RD        17

  int ix;
  double td;
  double shpn[5];
  double tmp;
  double h20 = 0.0;
  double h30 = 0.0;
  double c20;
  double c30;

  static const float w3rd[N_WAVES_3RD][N_COEFS] = {
    {/* 0,*/0, 0, 1, 0, 0, -0.00021f },
    {/* 0,*/0, 2, -1, 0, 0, -0.00004f },
    {/* 0,*/1, -2, 0, 0, 0, 0.00004f },
    {/* 0,*/1, 0, 0, -1, 0, 0.00019f },
    {/* 0,*/1, 0, 0, 0, 0, -0.00375f },
    {/* 0,*/1, 0, 0, 1, 0, -0.00059f },
    {/* 0,*/1, 0, 0, 2, 0, 0.00005f },
    {/* 0,*/2, -2, 1, 0, 0, -0.00012f },
    {/* 0,*/2, 0, -1, 0, 0, -0.00061f },
    {/* 0,*/2, 0, -1, 1, 0, -0.00010f },
    {/* 0,*/3, -2, 0, 0, 0, -0.00010f },
    {/* 0,*/3, 0, -2, 0, 0, -0.00007f },
    {/* 0,*/3, 0, 0, 0, 0, -0.00030f },
    {/* 0,*/3, 0, 0, 1, 0, -0.00019f },
    {/* 0,*/3, 0, 0, 2, 0, -0.00004f },
    {/* 0,*/4, 0, -1, 0, 0, -0.00008f },
    {/* 0,*/4, 0, -1, 1, 0, -0.00005f }
  };

  /* Compute 4 principal mean longitudes in radians at time TD */
  td = ((ts + 33282.0) * 86400.0 - 4043174400.0) / 86400.0;

  /* Coeff from R. Ray at 0h on 1/1/1990 in degree */
  shpn[0] = RAD * fmod(290.210 + (td * 13.17639650), 360.0); /* S  */
  shpn[1] = RAD * fmod(280.120 + (td * 0.98564730), 360.0);  /* h  */
  shpn[2] = RAD * fmod(274.350 + (td * 0.11140410), 360.0);  /* p  */
  shpn[3] = RAD * fmod(343.510 + (td * 0.05295390), 360.0);  /* n  */
  shpn[4] = RAD * fmod(283.000 + (td * 0.00000000), 360.0);  /* p1 */

  /* Tidal potential V20 */
  for (ix = 0; ix < N_WAVES_2ND; ++ix) {
    tmp = w2nd[ix][0] * shpn[0] + w2nd[ix][1] * shpn[1]
        + w2nd[ix][2] * shpn[2] + w2nd[ix][3] * shpn[3]
        + w2nd[ix][4] * shpn[4];

    h20 += cos(tmp) * w2nd[ix][5];
  }

  /* Tidal potential V30 */
  for (ix = 0; ix < N_WAVES_3RD; ++ix) {
    tmp = w3rd[ix][0] * shpn[0] + w3rd[ix][1] * shpn[1]+ w3rd[ix][2] * shpn[2]
        + w3rd[ix][3] * shpn[3] + w3rd[ix][4] * shpn[4];

    h30 += sin(tmp) * w3rd[ix][5];
  }

  tmp = sin(lat * RAD);

  c20 = sqrt(5.0 / (4.0 * M_PI)) * (1.5 * SQR(tmp) - 0.5);
  c30 = sqrt(7.0 / (4.0 * M_PI)) * (2.5 * SQR(tmp) - 1.5) * tmp;

  /* m -> cm */
  *tlp = ((1.0 - 0.609 /* H2 */+ 0.302 /* K2 */) * c20 * h20 + (1.0 - 0.291
  /* H3 */+ 0.093 /* K3 */) * c30 * h30) * 1e2;

}

/*
 _frequency

 Computes the wave frequency from the doodson arguments

 t Mean solar angle relative to Greenwich
 s moon's mean longitude
 h sun's mean longitude
 p longitude of moon's perigee
 n longitude of moon's ascending node
 p1 longitude of sun's perigee
 */
static double _frequency(const short t, const short s, const short h,
                         const short p, const short n, const short p1) {
  return ((tau_frequency() + s_frequency() - h_frequency()) * t
      + s_frequency() * s + h_frequency() * h + p_frequency() * p
      + n_frequency() * n + p1_frequency() * p1) * 360;
}

/*
 _init_wave

 Initializes the properties of the wave (frequency, doodson's coefficients,
 etc.).

 t Mean solar angle relative to Greenwich
 s moon's mean longitude
 h sun's mean longitude
 p longitude of moon's perigee
 n longitude of moon's ascending node
 p1 longitude of sun's perigee
 shift TODO
 eps Coefficient for the longitude in moon's orbit of lunar intersection
 nu Coefficient for the right ascension of lunar intersection
 nuprim Coefficient for the term in argument of lunisolar constituent K₁
 nusec Coefficient for the term in argument of lunisolar constituent K₂
 */
static void _init_wave(const char* const name, const int admittance,
                       const short t, const short s, const short h,
                       const short p, const short n, const short p1,
                       const short shift, const short eps, const short nu,
                       const short nuprim, const short nusec,
                       fes_enum_period_type type,
                       double (*f_function)(const _fes_astronomic_angle* const),
                       fes_wave* const w) {

  w->admittance = admittance;
  w->f_function = f_function;
  w->freq = _frequency(t, s, h, p, n, p1) * RAD;
  w->name = (char*) name;
  w->type = type;

  w->argument[0] = t;
  w->argument[1] = s;
  w->argument[2] = h;
  w->argument[3] = p;
  w->argument[4] = n;
  w->argument[5] = p1;
  w->argument[6] = shift;
  w->argument[7] = eps;
  w->argument[8] = nu;
  w->argument[9] = nuprim;
  w->argument[10] = nusec;
}

/*
 */
void set_waves(fes_wave* const w) {
  memset(w, 0, sizeof(fes_wave) * N_WAVES);

  /*
   * Mm
   *
   * V = s - p;
   * u = 0;
   * f = f(Mm)
   */
  _init_wave("MM", 0, 0, 1, 0, -1, 0, 0, 0, 0, 0, 0, 0, LP_TIDE, _f_mm, &w[MM]);
  /*
   * Mf
   *
   * V = 2s
   * u = -2ξ
   * f = f(Mf)
   */
  _init_wave("MF", 0, 0, 2, 0, 0, 0, 0, 0, -2, 0, 0, 0, LP_TIDE, _f_mf, &w[MF]);
  /*
   * Mtm
   *
   * V = 3s - p
   * u = -2ξ
   * f = f(Mf)
   */
  _init_wave("MTM", 0, 0, 3, 0, -1, 0, 0, 0, -2, 0, 0, 0, LP_TIDE, _f_mf,
             &w[MTM]);
  /*
   * Msqm
   *
   * V = 4s - 2h
   * u = -2ξ
   * f = f(Mf)
   */
  _init_wave("MSQM", 0, 0, 4, -2, 0, 0, 0, 0, -2, 0, 0, 0, LP_TIDE, _f_mf,
             &w[MSQM]);
  /*
   * Ssa
   *
   * V = 2h
   * u = 0
   * f = 1
   */
  _init_wave("SSA", 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 0, LP_TIDE, _f_1, &w[SSA]);
  /*
   * 2Q₁
   *
   * V = T - 4s + h + 2p + 90°
   * u = +2ξ - ν
   * f = f(O₁)
   */
  _init_wave("2Q1", 1, 1, -4, 1, 2, 0, 0, 1, 2, -1, 0, 0, SP_TIDE, _f_o1,
             &w[_2Q1]);
  /*
   * σ₁
   *
   * V = T - 4s + 3h + 90°
   * u = +2ξ - ν
   * f = f(O₁)
   */
  _init_wave("SIGMA1", 1, 1, -4, 3, 0, 0, 0, 1, 2, -1, 0, 0, SP_TIDE, _f_o1,
             &w[SIGMA1]);
  /*
   * Q₁
   *
   * V = T - 3s + h + p + 90°
   * u = +2ξ - ν
   * f = f(O₁)
   */
  _init_wave("Q1", 0, 1, -3, 1, 1, 0, 0, 1, 2, -1, 0, 0, SP_TIDE, _f_o1,
             &w[Q1]);
  /*
   * ρ₁
   *
   * V = T - 3s + 3h - p + 90°
   * u = +2ξ - ν
   * f = f(O₁)
   */
  _init_wave("RHO1", 1, 1, -3, 3, -1, 0, 0, 1, 2, -1, 0, 0, SP_TIDE, _f_o1,
             &w[RHO1]);
  /*
   * O₁
   *
   * V = T - 2s + h + 90°
   * u = +2ξ - ν
   * f = f(O₁)
   *
   */
  _init_wave("O1", 0, 1, -2, 1, 0, 0, 0, 1, 2, -1, 0, 0, SP_TIDE, _f_o1,
             &w[O1]);
  /*
   * MP₁
   *
   * V = T - 2s + 3h - 90°
   * u = -ν
   * f = f(J₁)
   */
  _init_wave("MP1", 0, 1, -2, 3, 0, 0, 0, -1, 0, -1, 0, 0, SP_TIDE, _f_j1,
             &w[MP1]);
  /*
   * M₁₂ (Formula A16)
   *
   * V = T - s + h - p - 90°
   * u = +2ξ - ν
   * f = f(O₁)
   */
  _init_wave("M12", 1, 1, -1, 1, -1, 0, 0, -1, 2, -1, 0, 0, SP_TIDE, _f_o1,
             &w[M12]);
  /*
   * M13 (=M11 + M12)
   *
   * V = T - s + h + p - 90
   * u = -ν
   * f = f(M₁₃)
   */
  _init_wave("M13", 0, 1, -1, 1, 1, 0, 0, -1, 0, -1, 0, 0, SP_TIDE, _f_m13,
             &w[M13]);
  /*
   * M₁₁ (Formula A23)
   *
   * V = T - s + h + p - 90°
   * u = -ν
   * f = f(J₁)
   */
  _init_wave("M11", 1, 1, -1, 1, 1, 0, 0, -1, 0, -1, 0, 0, SP_TIDE, _f_j1,
             &w[M11]);
  /*
   * χ₁
   *
   * V = T - s + 3h - p - 90°
   * u = -ν
   * f = f(J₁)
   */
  _init_wave("CHI1", 1, 1, -1, 3, -1, 0, 0, -1, 0, -1, 0, 0, SP_TIDE, _f_j1,
             &w[CHI1]);
  /*
   * π₁
   *
   * V = T - 2h + p1 + 90°
   * u = 0
   * f = 1
   */
  _init_wave("PI1", 1, 1, 0, -2, 0, 0, 1, 1, 0, 0, 0, 0, SP_TIDE, _f_1,
             &w[PI1]);
  /*
   * P₁
   *
   * V = T - h + 90°
   * u = 0
   * f = 1
   */
  _init_wave("P1", 0, 1, 0, -1, 0, 0, 0, 1, 0, 0, 0, 0, SP_TIDE, _f_1, &w[P1]);
  /*
   * S₁
   *
   * V = T
   * u = 0
   * f = 1
   */
  _init_wave("S1", 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, SP_TIDE, _f_1, &w[S1]);
  /*
   * K₁
   *
   * V = T + h - 90°
   * u = - ν'
   * f = f(k₁)
   */
  _init_wave("K1", 0, 1, 0, 1, 0, 0, 0, -1, 0, 0, -1, 0, SP_TIDE, _f_k1,
             &w[K1]);
  /*
   * ψ₁
   *
   * V = T + 2h - p1 - 90°
   * u = 0
   * f = 1
   */
  _init_wave("PSI1", 0, 1, 0, 2, 0, 0, -1, -1, 0, 0, 0, 0, SP_TIDE, _f_1,
             &w[PSI1]);
  /*
   * φ₁
   *
   * V = T + 3h - 90°
   * u = 0
   * f = 1
   */
  _init_wave("PHI1", 1, 1, 0, 3, 0, 0, 0, -1, 0, 0, 0, 0, SP_TIDE, _f_1,
             &w[PHI1]);
  /*
   * θ₁
   *
   * V = T + s - h + p - 90°
   * u = -ν
   * f = f(J₁)
   */
  _init_wave("THETA1", 1, 1, 1, -1, 1, 0, 0, -1, 0, -1, 0, 0, SP_TIDE, _f_j1,
             &w[THETA1]);
  /*
   * J₁
   *
   * V = T + s + h - p - 90°
   * u = -ν
   * f = f(J₁)
   */
  _init_wave("J1", 1, 1, 1, 1, -1, 0, 0, -1, 0, -1, 0, 0, SP_TIDE, _f_j1,
             &w[J1]);
  /*
   * OO₁
   *
   * V = T + 2s + h - 90°
   * u = -2ξ - ν
   * f = f(OO₁)
   */
  _init_wave("OO1", 1, 1, 2, 1, 0, 0, 0, -1, -2, -1, 0, 0, SP_TIDE, _f_oo1,
             &w[OO1]);
  /*
   * MNS₂ = M₂ + N₂ + S₂
   *
   * V = 2T - 5s + 4h + p
   * u = +4ξ - 4ν
   * f = f(M₂)²
   */
  _init_wave("MNS2", 0, 2, -5, 4, 1, 0, 0, 0, 4, -4, 0, 0, SP_TIDE, _f_m22,
             &w[MNS2]);
  /*
   * ε₂
   *
   * V = 2T - 5s + 4h + p
   * u = +2ξ - 2ν
   * f = f(M₂)
   */
  _init_wave("EPS2", 1, 2, -5, 4, 1, 0, 0, 0, 2, -2, 0, 0, SP_TIDE, _f_m2,
             &w[EPS2]);
  /*
   * 2N₂
   *
   * V = 2T - 4s + 2h + 2p
   * u = +2ξ - 2ν
   * f = f(M₂)
   */
  _init_wave("2N2", 1, 2, -4, 2, 2, 0, 0, 0, 2, -2, 0, 0, SP_TIDE, _f_m2,
             &w[_2N2]);
  /*
   * µ₂
   *
   * V = 2T - 4s + 4h
   * u = +2ξ - 2ν
   * f = f(M₂)
   */
  _init_wave("MU2", 1, 2, -4, 4, 0, 0, 0, 0, 2, -2, 0, 0, SP_TIDE, _f_m2,
             &w[MU2]);
  /*
   * 2MS₂ = 2M₂ - S₂
   *
   * V = 2T - 4s + 4h
   * u = +4ξ - 4ν
   * f = f(M₂)²
   */
  _init_wave("2MS2", 0, 2, -4, 4, 0, 0, 0, 0, 4, -4, 0, 0, SP_TIDE, _f_m22,
             &w[_2MS2]);
  /*
   * N₂
   *
   * V = 2T - 3s + 2h + p
   * u = +2ξ - 2ν
   * f = f(M₂)
   */
  _init_wave("N2", 0, 2, -3, 2, 1, 0, 0, 0, 2, -2, 0, 0, SP_TIDE, _f_m2,
             &w[N2]);
  /*
   * ν₂
   *
   * V = 2T - 3s + 4h - p
   * u = +2ξ - 2ν
   * f = f(M₂)
   */
  _init_wave("NU2", 1, 2, -3, 4, -1, 0, 0, 0, 2, -2, 0, 0, SP_TIDE, _f_m2,
             &w[NU2]);
  /*
   * M₂
   *
   * V = 2T - 2s + 2h
   * u = +2ξ - 2ν
   * f = f(M₂)
   */
  _init_wave("M2", 0, 2, -2, 2, 0, 0, 0, 0, 2, -2, 0, 0, SP_TIDE, _f_m2,
             &w[M2]);
  /*
   * MKS₂ = M₂ + K₂ - S₂
   *
   * V = 2T - 2s + 4h
   * u = +2ξ - 2ν -2ν''
   * f = f(M₂) × f(K₂)
   */
  _init_wave("MKS2", 0, 2, -2, 4, 0, 0, 0, 0, 2, -2, 0, -2, SP_TIDE, _f_m2_k2,
             &w[MKS2]);
  /*
   * λ₂
   *
   * V = 2T - s + p + 180°
   * u = +2ξ - 2ν
   * f = f(M₂)
   */
  _init_wave("LAMBDA2", 1, 2, -1, 0, 1, 0, 0, 2, 2, -2, 0, 0, SP_TIDE, _f_m2,
             &w[LAMBDA2]);
  /*
   * L₂
   *
   * V = 2T - s + 2h - p + 180°
   * u = +2ξ - 2ν - R
   * f = f(L₂)
   */
  _init_wave("L2", 1, 2, -1, 2, -1, 0, 0, 2, 2, -2, 0, 0, SP_TIDE, _f_l2,
             &w[L2]);
  /*
   * 2MN₂ = 2M₂ - N₂
   *
   * V = 2T - s + 2h - p + 180°
   * u = +2ξ - 2ν
   * f = f(M₂)³
   */
  _init_wave("2MN2", 0, 2, -1, 2, -1, 0, 0, 2, 2, -2, 0, 0, SP_TIDE, _f_m23,
             &w[_2MN2]);
  /*
   * T₂
   *
   * V = 2T - h + p₁
   * u = 0
   * f = 1
   */
  _init_wave("T2", 1, 2, 0, -1, 0, 0, 1, 0, 0, 0, 0, 0, SP_TIDE, _f_1, &w[T2]);
  /*
   * S₂
   *
   * V = 2T
   * u = 0
   * f = 1
   */
  _init_wave("S2", 0, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, SP_TIDE, _f_1, &w[S2]);
  /*
   * R₂
   *
   * V = 2T + h - p1 + 180°
   * u = 0
   * f = 1
   */
  _init_wave("R2", 0, 2, 0, 1, 0, 0, -1, 2, 0, 0, 0, 0, SP_TIDE, _f_1, &w[R2]);
  /*
   * K₂
   *
   * V = 2T + 2h
   * u = -2ν″
   * f = f(K₂)
   */
  _init_wave("K2", 0, 2, 0, 2, 0, 0, 0, 0, 0, 0, 0, -2, SP_TIDE, _f_k2, &w[K2]);
  /*
   * MSN₂ = M2 + S2 - N2
   *
   * V = 2T + s -p
   * u = 0
   * f = f(M₂)²
   */
  _init_wave("MSN2", 0, 2, 1, 0, -1, 0, 0, 0, 0, 0, 0, 0, SP_TIDE, _f_m22,
             &w[MSN2]);
  /*
   * η₂ = KJ₂
   *
   * V = 2T + s + 2h - p
   * u = -2ν
   * f = f(KJ₂)
   */
  _init_wave("ETA2", 1, 2, 1, 2, -1, 0, 0, 0, 0, -2, 0, 0, SP_TIDE, _f_kj2,
             &w[ETA2]);
  /*
   * 2SM₂ = 2S₂ - M₂
   *
   * V = 2T + 2s - 2h
   * u = -2ξ + 2ν
   * f = f(M₂)
   */
  _init_wave("2SM2", 0, 2, 2, -2, 0, 0, 0, 0, 2, -2, 0, 0, SP_TIDE, _f_m2,
             &w[_2SM2]);
  /*
   * MO₃ = M₂ + O₁
   *
   * V = 3T - 4s + 3h + 90°
   * u = 4ξ - 3ν
   * f = f(M₂) × f(O₁)
   */
  _init_wave("MO3", 0, 3, -4, 3, 0, 0, 0, 1, 4, -3, 0, 0, SP_TIDE, _f_m2_o1,
             &w[MO3]);
  /*
   * 2MK₃ = 2M₂ - K₁
   *
   * V = 3T - 4s + 3h + 90°
   * u = 4ξ - 4ν + ν′
   * f = f(M₂)² × f(K₁)
   */
  _init_wave("2MK3", 0, 3, -4, 3, 0, 0, 0, 1, 4, -4, 1, 0, SP_TIDE, _f_m22_k1,
             &w[_2MK3]);
  /*
   * M₃
   *
   * V = 3T - 3s + 3h
   * u = +3ξ - 3ν
   * f = f(M₃)
   */
  _init_wave("M3", 0, 3, -3, 3, 0, 0, 0, 0, 3, -3, 0, 0, SP_TIDE, _f_m3,
             &w[M3]);
  /*
   * MK₃ = M₂ + K₁
   *
   * V = 3T - 2s + 3h - 90°
   * u = 2ξ - 2ν - ν′
   * f = f(M₂) × f(K₁)
   */
  _init_wave("MK3", 0, 3, -2, 3, 0, 0, 0, -1, 2, -2, -1, 0, SP_TIDE, _f_m2_k1,
             &w[MK3]);
  /*
   * N4 = N₂ + N₂
   *
   * V = 4T - 6s + 4h + 2p
   * u = +4ξ - 4ν
   * f = f(M₂)²
   */
  _init_wave("N4", 0, 4, -6, 4, 2, 0, 0, 0, 4, -4, 0, 0, SP_TIDE, _f_m22,
             &w[N4]);
  /*
   * MN₄ = M₂ + N₂
   *
   * V = 4T - 5s + 4h + p
   * u = +4ξ - 4ν
   * f = f(M₂)²
   */
  _init_wave("MN4", 0, 4, -5, 4, 1, 0, 0, 0, 4, -4, 0, 0, SP_TIDE, _f_m22,
             &w[MN4]);
  /*
   * M₄ = 2M₂
   *
   * V = 4T - 4s + 4h
   * u = +4ξ - 4ν
   * f = f²(M₂)
   */
  _init_wave("M4", 0, 4, -4, 4, 0, 0, 0, 0, 4, -4, 0, 0, SP_TIDE, _f_m22,
             &w[M4]);
  /*
   * SN₄ = S₂ + N₂
   *
   * V = 4T - 3s + 2h + p
   * u = 2ξ - 2ν
   * f = f(M₂)
   */
  _init_wave("SN4", 0, 4, -3, 2, 1, 0, 0, 0, 2, -2, 0, 0, SP_TIDE, _f_m2,
             &w[SN4]);
  /*
   * MS₄ = M₂ + S₂
   *
   * V = 4T - 2s + 2h
   * u = +2ξ - 2ν
   * f = f(M₂)
   */
  _init_wave("MS4", 0, 4, -2, 2, 0, 0, 0, 0, 2, -2, 0, 0, SP_TIDE, _f_m2,
             &w[MS4]);
  /*
   * MK₄ = M₂ + K₂
   *
   * V = 4T - 2s + 4h
   * u = 2ξ - 2ν - 2ν''
   * f = f(MK₄)
   */
  _init_wave("MK4", 0, 4, -2, 4, 0, 0, 0, 0, 2, -2, -2, 0, SP_TIDE, _f_m2_k2,
             &w[MK4]);
  /*
   * S₄ = S₂ + S₂
   *
   * V = 4T
   * u = 0
   * f = 1
   */
  _init_wave("S4", 0, 4, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, SP_TIDE, _f_1, &w[S4]);
  /*
   * SK₄ = S₂ + K₂
   *
   * V = 4T + 2h
   * u = -2ν''
   * f = f(K₂)
   */
  _init_wave("SK4", 0, 4, 0, 2, 0, 0, 0, 0, 0, 0, 0, -2, SP_TIDE, _f_k2,
             &w[SK4]);
  /*
   * R₄ = R₂ + R₂
   *
   * V = 4T + 2h - 2p1
   * u = 0
   * f = 1
   */
  _init_wave("R4", 0, 4, 0, 2, 0, 0, -2, 0, 0, 0, 0, 0, SP_TIDE, _f_1, &w[R4]);
  /*
   * 2MN₆ = 2M₂ + N₂
   *
   * V = 6T - 7s + 6h + p
   * u = 6ξ - 6ν
   * f = f(M₂)³
   */
  _init_wave("2MN6", 0, 6, -7, 6, 1, 0, 0, 0, 6, -6, 0, 0, SP_TIDE, _f_m23,
             &w[_2MN6]);
  /*
   * M₆ = 3M₂
   *
   * V = 6T - 6s + 6h
   * u = +6ξ - 6ν
   * f = f(M₂)³
   */
  _init_wave("M6", 0, 6, -6, 6, 0, 0, 0, 0, 6, -6, 0, 0, SP_TIDE, _f_m23,
             &w[M6]);
  /*
   * MSN₆ = M₂ + S₂ + N₂
   *
   * V = 6T - 5s + 4h + p
   * u = 4ξ - 4ν
   * f = f(M₂)²
   */
  _init_wave("MSN6", 0, 6, -5, 4, 1, 0, 0, 0, 4, -4, 0, 0, SP_TIDE, _f_m22,
             &w[MSN6]);
  /*
   * 2MS₆ = 2M₂ + S₂
   *
   * V = 6T - 4s + 4h
   * u = 4ξ - 4ν
   * f = f(M₂)²
   */
  _init_wave("2MS6", 0, 6, -4, 4, 0, 0, 0, 0, 4, -4, 0, 0, SP_TIDE, _f_m22,
             &w[_2MS6]);
  /*
   * 2MK₆ = 2M₂ + K₂
   *
   * V = 6T - 4s + 6h
   * u = 4ξ - 4ν - 2ν''
   * f = f(M₂)² × f(K₂)
   */
  _init_wave("2MK6", 0, 6, -4, 6, 0, 0, 0, 0, 4, -4, 0, -2, SP_TIDE, _f_m23_k2,
             &w[_2MK6]);
  /*
   * 2SM₆ = 2S₂ + M₂
   *
   * V = 6T - 2s + 2h
   * u = 2ξ - 2ν
   * f = f(M₂)
   */
  _init_wave("2SM6", 0, 6, -2, 2, 0, 0, 0, 0, 2, -2, 0, 0, SP_TIDE, _f_m2,
             &w[_2SM6]);
  /*
   * MSK₆ = M₂ + K₂ + S₂
   *
   * V = 6T - 2s + 4h
   * u = 2ξ - 2ν - 2ν''
   * f = f(M₂) × f(K₂)
   */
  _init_wave("MSK6", 0, 6, -2, 4, 0, 0, 0, 0, 2, -2, -2, 0, SP_TIDE, _f_m2_k2,
             &w[MSK6]);
  /*
   * S₆ = 3S₂
   *
   * V = 6T
   * u = 0
   * f = 1
   */
  _init_wave("S6", 0, 6, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, SP_TIDE, _f_1, &w[S6]);
  /*
   * M₈ = 4M₂
   *
   * V = 8T - 8s + 8h
   * u = 8ξ - 8ν
   * f = f(M₂)⁴
   */
  _init_wave("M8", 0, 8, -8, 8, 0, 0, 0, 0, 8, -8, 0, 0, SP_TIDE, _f_m24,
             &w[M8]);
  /*
   * MSf = M₂ - S₂
   * WARNING: Same frequency as MSf LP : 2s -2h
   * V = 2s - 2h
   * u = 2ξ - 2ν (=celui de M₂ - S₂)
   * f = f(M₂) * f(S2) = f(M₂)
   */
  _init_wave("MSF", 0, 0, 2, -2, 0, 0, 0, 0, 2, -2, 0, 0, SP_TIDE, _f_m2,
             &w[MSF]);
}
