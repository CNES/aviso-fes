/**
 @file fes_int.h
 @brief Contains internal definitions of the tide model.
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

#include "fes.h"
#include <math.h>
#include <netcdf.h>
#include <uthash.h>

/**
 @brief Ignores warnings about unused variables.
 */
#if (__GNUC__ > 2) || (__GNUC__ == 2 && __GNUC_MINOR__ > 4)
#define UNUSED __attribute__((__unused__))
#else
#define UNUSED
#endif

/**
 @brief Index to access the wave Mm in the internal table.
 */
#define MM 0

/**
 @brief Index to access the wave Mf in the internal table.
 */
#define MF 1

/**
 @brief Index to access the wave Mtm in the internal table.
 */
#define MTM 2

/**
 @brief Index to access the wave Msqm in the internal table.
 */
#define MSQM 3

/**
 @brief Index to access the wave 2Q₁ in the internal table.
 */
#define _2Q1 4

/**
 @brief Index to access the wave σ₁ in the internal table.
 */
#define SIGMA1 5

/**
 @brief Index to access the wave Q₁ in the internal table.
 */
#define Q1 6

/**
 @brief Index to access the wave ρ₁ in the internal table.
 */
#define RHO1 7

/**
 @brief Index to access the wave O₁ in the internal table.
 */
#define O1 8

/**
 @brief Index to access the wave MP₁ in the internal table.
 */
#define MP1 9

/**
 @brief Index to access the wave M₁₁ in the internal table.
 */
#define M11 10

/**
 @brief Index to access the wave M₁₂ in the internal table.
 */
#define M12 11

/**
 @brief Index to access the wave M₁₃ in the internal table.
 */
#define M13 12

/**
 @brief Index to access the wave χ₁ in the internal table.
 */
#define CHI1 13

/**
 @brief Index to access the wave π₁ in the internal table.
 */
#define PI1 14

/**
 @brief Index to access the wave P₁ in the internal table.
 */
#define P1 15

/**
 @brief Index to access the wave S₁ in the internal table.
 */
#define S1 16

/**
 @brief Index to access the wave K₁ in the internal table.
 */
#define K1 17

/**
 @brief Index to access the wave ψ₁ in the internal table.
 */
#define PSI1 18

/**
 @brief Index to access the wave φ₁ in the internal table.
 */
#define PHI1 19

/**
 @brief Index to access the wave θ₁ in the internal table.
 */
#define THETA1 20

/**
 @brief Index to access the wave J₁ in the internal table.
 */
#define J1 21

/**
 @brief Index to access the wave OO₁ in the internal table.
 */
#define OO1 22

/**
 @brief Index to access the wave MNS₂ in the internal table.
 */
#define MNS2 23

/**
 @brief Index to access the wave ε₂ in the internal table.
 */
#define EPS2 24

/**
 @brief Index to access the wave 2N₂ in the internal table.
 */
#define _2N2 25

/**
 @brief Index to access the wave µ₂ in the internal table.
 */
#define MU2 26

/**
 @brief Index to access the wave 2MS₂ in the internal table.
 */
#define _2MS2 27

/**
 @brief Index to access the wave N₂ in the internal table.
 */
#define N2 28

/**
 @brief Index to access the wave ν₂ in the internal table.
 */
#define NU2 29

/**
 @brief Index to access the wave M₂ in the internal table.
 */
#define M2 30

/**
 @brief Index to access the wave MKS₂ in the internal table.
 */
#define MKS2 31

/**
 @brief Index to access the wave λ₂ in the internal table.
 */
#define LAMBDA2 32

/**
 @brief Index to access the wave L₂ in the internal table.
 */
#define L2 33

/**
 @brief Index to access the wave 2MN₂ in the internal table.
 */
#define _2MN2 34

/**
 @brief Index to access the wave T₂ in the internal table.
 */
#define T2 35

/**
 @brief Index to access the wave S₂ in the internal table.
 */
#define S2 36

/**
 @brief Index to access the wave R₂ in the internal table.
 */
#define R2 37

/**
 @brief Index to access the wave K₂ in the internal table.
 */
#define K2 38

/**
 @brief Index to access the wave MSN₂ in the internal table.
 */
#define MSN2 39

/**
 @brief Index to access the wave η₂ in the internal table.
 */
#define ETA2 40

/**
 @brief Index to access the wave 2SM₂ in the internal table.
 */
#define _2SM2 41

/**
 @brief Index to access the wave MO₃ in the internal table.
 */
#define MO3 42

/**
 @brief Index to access the wave 2MK₃ in the internal table.
 */
#define _2MK3 43

/**
 @brief Index to access the wave M₃ in the internal table.
 */
#define M3 44

/**
 @brief Index to access the wave MK₃ in the internal table.
 */
#define MK3 45

/**
 @brief Index to access the wave N₄ in the internal table.
 */
#define N4 46

/**
 @brief Index to access the wave MN₄ in the internal table.
 */
#define MN4 47

/**
 @brief Index to access the wave M₄ in the internal table.
 */
#define M4 48

/**
 @brief Index to access the wave SN₄ in the internal table.
 */
#define SN4 49

/**
 @brief Index to access the wave MS₄ in the internal table.
 */
#define MS4 50

/**
 @brief Index to access the wave MK₄ in the internal table.
 */
#define MK4 51

/**
 @brief Index to access the wave S₄ in the internal table.
 */
#define S4 52

/**
 @brief Index to access the wave SK₄ in the internal table.
 */
#define SK4 53

/**
 @brief Index to access the wave R₄ in the internal table.
 */
#define R4 54

/**
 @brief Index to access the wave 2MN₆ in the internal table.
 */
#define _2MN6 55

/**
 @brief Index to access the wave M₆ in the internal table.
 */
#define M6 56

/**
 @brief Index to access the wave MSN₆ in the internal table.
 */
#define MSN6 57

/**
 @brief Index to access the wave 2MS₆ in the internal table.
 */
#define _2MS6 58

/**
 @brief Index to access the wave 2MK₆ in the internal table.
 */
#define _2MK6 59

/**
 @brief Index to access the wave 2SM₆ in the internal table.
 */
#define _2SM6 60

/**
 @brief Index to access the wave MSK₆ in the internal table.
 */
#define MSK6 61

/**
 @brief Index to access the wave S₆ in the internal table.
 */
#define S6 62

/**
 @brief Index to access the wave M8 in the internal table.
 */
#define M8 63

/**
 @brief Index to access the wave MSf in the internal table.
 */
#define MSF 64

/**
 @brief Index to access the wave Ssa in the internal table.
 */
#define SSA 65

/**
 @brief Index to access the wave Sa in the internal table.
 */
#define SA 66

/**
 @brief Index to access the wave Sa_1 in the internal table.
 */
#define SA_1 67

/**
 @brief Index to access the wave STA in the internal table.
 */
#define STA 68

/**
 @brief Index to access the wave MM_1 in the internal table.
 */
#define MM_1 69

/**
 @brief Index to access the wave MF_1 in the internal table.
 */
#define MF_1 70

/**
 @brief Index to access the wave MSF_LP in the internal table.
 */
#define MSF_LP 71

/**
 @brief Index to access the wave M0 in the internal table.
 */
#define M0 72

/**
 @brief Index to access the wave MM_2 in the internal table.
 */
#define MM_2 73

/**
 @brief Index to access the wave MF_2 in the internal table.
 */
#define MF_2 74

/**
 @brief The total number of waves known in the internal table.
 */
#define N_WAVES 75

/**
 @brief The number of 2nd order waves taken into account to compute long-period.
 */
#define N_WAVES_2ND 106

/**
 @brief The number of 3rd order waves taken into account to compute long-period.
 */
#define N_WAVES_3RD 17

/**
 @brief Number of coefficients
 */
#define N_COEFS 6

/**
 @brief Number of harmonic constituents.
 */
#define N_CONSTITUENTS 11

/**
 @brief Epsilon for float comparison
 */
#define EPSILON 1.0E-9

/**
 @brief Fill value for missing data
 */
#define DV 18446744073709551616.0

#ifndef M_PI
/**
 @brief PI
 */
#define M_PI 3.14159265358979323846
#endif

#ifndef MAX_PATH
/**
 @brief The maximum number of characters that a string can contain.
 */
#define MAX_PATH 1024
#endif

/**
 @brief Value to convert degree to radiant.
 */
#define RAD 0.01745329251994329576

/**
 @brief Value to convert radiant to degree.
 */
#define DEG 57.29577951308232087679

/**
 @brief Test if float values are equal.

 @param a First value to check
 @param b Second value to check
 @return 1 if values are equal, otherwise 0.
 */
#define EQUALS(a, b) (fabs(a - b) < EPSILON)

/**
 @brief Test if a value is within the defined interval.

 @param a The lower end of the interval
 @param b Value to test
 @param c The upper end of the interval
 @return 1 if value is within the interval, otherwise 0.
 */
#define CONTAINS(a, b, c) (((b) >= (a)) && ((b) <= (c)))

/**
 @brief Compute the square root of a number.

 @param a Number
 @return The square root.
 */
#define SQR(a) ((a) * (a))

/**
 @brief Compute the power 3 of a number.

 @param a Number
 @return  The power 3.
 */
#define POW3(a) pow(a, 3.0)

/**
 @brief Compute the power 4 of a number.

 @param a Number
 @return  The power 4.
 */
#define POW4(a) pow(a, 4.0)

/**
 @brief Returns the smaller of the given values.

 @param a Value to compare
 @param b Value to compare
 @return Returns the smaller of a and b.
 */
#define MIN(a, b) ((a) < (b) ? (a) : (b))

/**
 @brief A complex number of float type.
 */
struct _fes_float_complex
{
  /** Real part of the complex number */
  float re;
  /** Imaginary part of the complex number */
  float im;
};

/**
 @brief A utility data type for ::_fes_float_complex structure.
 */
typedef struct _fes_float_complex fes_float_complex;

/**
 @brief A complex number of double type.
 */
struct _fes_double_complex
{
  /** Real part of the complex number */
  double re;
  /** Imaginary part of the complex number */
  double im;
};

/**
 @brief A utility data type for ::_fes_double_complex structure.
 */
typedef struct _fes_double_complex fes_double_complex;

/**
 @brief Possible type of tide.
 */
typedef enum
{
  /** Ocean tide. */
  SP_TIDE = 0,
  /** Long period tidal waves. */
  LP_TIDE
} fes_enum_period_type;

/**
 @brief Astronomical angles.
 */
struct __fes_astronomic_angle
{
  /** Hour angle of mean sun. */
  double T;
  /** Mean longitude of the sun. */
  double h;
  /** Mean longitude of the moon. */
  double s;
  /** Mean longitude of solar perigee. */
  double p1;
  /** Mean longitude of lunar perigee. */
  double p;
  /** Obliquity of lunar orbit with respect to earth's equator */
  double I;
  /** Longitude in moon's orbit of lunar intersection */
  double xi;
  /** Right ascension of lunar intersection */
  double nu;
  /** Factor in amplitude of constituent L₂ */
  double x1ra;
  /** Term in argument of constituent L₂ */
  double r;
  /** Term in argument of lunisolar constituent K₁ */
  double nuprim;
  /** Term in argument of lunisolar constituent K₂ */
  double nusec;
};

/**
 @brief A utility data type for ::_fes_astronomic_angle structure.
 */
typedef struct __fes_astronomic_angle _fes_astronomic_angle;

/**
 @brief Wave definition.
 */
struct _fes_wave
{
  /** Wave name */
  char* name;
  /** True if wave is computed by admittance. */
  int admittance;
  /** True if wave is computed dynamically. */
  int dynamic;
  /** Type of tide. */
  fes_enum_period_type type;
  /** Function to call for computing the node factor */
  double (*f_function)(const _fes_astronomic_angle* const);
  /** Wave frequency. */
  double freq;
  /** v0 (greenwich argument) + u (nodal correction for phase). */
  double v0u;
  /** Nodal correction for amplitude. */
  double f;
  /** Tide value. */
  fes_double_complex c;
  /** Harmonic constituents (T, s, h, p, N′, p₁, shift, ξ, ν, ν′, ν″) */
  short argument[N_CONSTITUENTS];
};

/**
 @brief A utility data type for ::_fes_wave structure.
 */
typedef struct _fes_wave fes_wave;

/**
 @brief Properties for a netCDF file.
 */
struct _fes_cdf_file
{
  /** NetCDF file identifier */
  int id;
  /** NetCDF variable identifier that corresponds to amplitude
   data. */
  int amp_id;
  /** NetCDF variable identifier that corresponds to phase data. */
  int pha_id;
  /** NetCDF variable name that contains amplitude data. */
  char amp[NC_MAX_VAR_DIMS];
  /** NetCDF variable name that contains latitude data. */
  char lat[NC_MAX_VAR_DIMS];
  /** NetCDF variable name that contains longitude data. */
  char lon[NC_MAX_VAR_DIMS];
  /** NetCDF variable name that contains phase data. */
  char pha[NC_MAX_VAR_DIMS];
};

/**
 @brief A utility data type for ::_fes_cdf_file structure.
 */
typedef struct _fes_cdf_file fes_cdf_file;

/**
 @brief structure for items in a double linked list
 */
struct _fes_dlist_item
{
  void* data;
  struct _fes_dlist_item* prev;
  struct _fes_dlist_item* next;
};

/**
 @brief A utility data type for ::_fes_dlist_item structure.
 */
typedef struct _fes_dlist_item fes_dlist_item;

/**
 @brief structure for double linked lists
 */
struct _fes_dlist
{
  size_t size;
  void (*del)(void* p);
  fes_dlist_item* head;
  fes_dlist_item* tail;
};

/**
 @brief A utility data type for ::_fes_dlist structure.
 */
typedef struct _fes_dlist fes_dlist;
/**
 @brief Properties of a value read from a netCDF file and stored in the
 buffer.
 */
struct _fes_cache_item
{
  /** True if all the data has been filled */
  size_t filled;
  /** Grid index cached */
  size_t index;
  /** Grid values cached */
  fes_double_complex* value;
  /** Cached item list */
  fes_dlist_item* list_item;
  /** Hash handler */
  UT_hash_handle hh;
};

/**
 @brief A utility data type for ::_fes_cache_item structure.
 */
typedef struct _fes_cache_item fes_cache_item;

/**
 @brief Structure of the buffer used to cache the values read in netCDF files.
 */
struct _fes_buffer
{
  /** Maximum size of the buffer in bytes */
  size_t max_size;
  /** List sorted from the most recently used to the least recently used item */
  fes_dlist list;
  /** Cache entries (map from the grid index read to the value read) */
  fes_cache_item* values;
};

/**
 @brief A utility data type for ::_fes_buffer structure.
 */
typedef struct _fes_buffer fes_buffer;

/**
 @brief Properties for tides grids.
 */
struct _fes_grid
{
  /** Latitude dimension. */
  size_t lat_dim;
  /** Longitude dimension. */
  size_t lon_dim;
  /** Number of grids. */
  size_t n_grids;
  /** True if dimensions of variables are (lon_dim, lat_dim) instead of
   (lat_dim, lon_dim) */
  int transpose;
  /**  Minimum latitude (units degree) */
  double lat_min;
  /**  Maximum latitude (units degree) */
  double lat_max;
  /**  Minimum longitude (units degree) */
  double lon_min;
  /**  Maximum longitude (units degree) */
  double lon_max;
  /**  Step between each successive latitude */
  double lat_step;
  /**  Step between each successive longitude */
  double lon_step;
  /**  Value for missing data */
  double undef;
  /** Wave index */
  int* waveIndex;
  /** Array that contains the opened grids. */
  fes_cdf_file* file;
  /** Array that contains n_grids matrix of dimension
   lat_dim * lon_dim that contains tide values. */
  fes_float_complex** values;
  /** Buffer used to cache the values read in netCDF files */
  fes_buffer* buffer;
};

/**
 @brief A utility data type for ::_fes_grid structure.
 */
typedef struct _fes_grid fes_grid;

/**
 @brief Stores the data necessary to compute the tide prediction.
 */
struct _fes_handler
{
  /** Last error number */
  fes_enum_error last_errno;
  /** Type of tide computed */
  fes_enum_tide_type type;
  /** True if the location requested is located in the grid. */
  int in_grid;
  /** True if the tide model is defined for the location requested. */
  int is_data;
  /** The minimum number of points used to interpolate the value of
   tidal waves. */
  int min_number;
  /** Time for the nodal corrections (elapsed time since 01/01/1900 0:0:0.0
   in julian centuries) */
  double nodal_time;
  /** Last error message */
  char last_error[MAX_PATH];
  /** The latitude of grid point located south of the point asked. */
  double south_lat;
  /** The latitude of grid point located north of the point asked. */
  double north_lat;
  /** The longitude of grid point located west of the point asked. */
  double west_lon;
  /** The longitude of grid point located east of the point asked. */
  double east_lon;
  /** Data read for the point located at the south-west of the point
   asked. */
  fes_double_complex sw[N_WAVES];
  /** Data read for the point located at the south-east of the point
   asked. */
  fes_double_complex se[N_WAVES];
  /** Data read for the point located at the north-west of the point
   asked. */
  fes_double_complex nw[N_WAVES];
  /** Data read for the point located at the north-east of the point
   asked. */
  fes_double_complex ne[N_WAVES];
  /** Properties of waves computed */
  fes_wave waves[N_WAVES];
  /** table with 2nd order LPequi waves */
  float w2nd[N_WAVES_2ND][N_COEFS];
  /** table with 3rd order LPequi waves */
  float w3rd[N_WAVES_3RD][N_COEFS];

  /** Tide grids read */
  fes_grid grid;
};

/**
 @brief A utility data type for ::_fes_handler structure.
 */
typedef struct _fes_handler fes_handler;
