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
#include "fes_int.h"

#define CHECK_INT(a, b) ((a) != (b))
#define CHECK_FLOAT(a, b) (EQUALS((a), (b)) != 1)
#define SUMMARIZE_ERR                                                          \
  do {                                                                         \
    if (err) {                                                                 \
      printf("%d failures at line %d\n", err, __LINE__);                       \
      total_err += err;                                                        \
      err = 0;                                                                 \
    }                                                                          \
  } while (0)
#define FINAL_RESULTS                                                          \
  do {                                                                         \
    if (total_err) {                                                           \
      printf("%d errors detected !\n", total_err);                             \
      return 2;                                                                \
    }                                                                          \
    printf("*** Tests successful!\n");                                         \
    return 0;                                                                  \
  } while (0)
#define DIE                                                                    \
  do {                                                                         \
    fprintf(stderr, "FATAL ERROR !");                                          \
    return 3;                                                                  \
  } while (0)
