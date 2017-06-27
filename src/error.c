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

#include "error.h"

#include <stdarg.h>
#include <stdio.h>
#include <string.h>

#include "fes.h"
#include "compat.h"

/* Error code message */
static const char* err[] = { "Success", "Not enough memory", "netCDF error",
    "IO error", "Configuration file contains error", "Tide is undefined",
    "Value error" };

/*
 */
void set_fes_error(fes_handler* const fes, const fes_enum_error errcode) {
  fes->last_errno = errcode;
  STRNCPY(fes->last_error, err[errcode], sizeof(fes->last_error));
}

/*
 */
void set_fes_extended_error(fes_handler* const fes,
                            const fes_enum_error errcode, const char* format,
                            ...) {
  va_list argp;

  fes->last_errno = errcode;

  va_start(argp, format);
  vsnprintf(fes->last_error, sizeof(fes->last_error), format, argp);
  va_end(argp);
}

/*
 */
fes_enum_error fes_errno(FES fes) {
  return ((fes_handler*) fes)->last_errno;
}

/*
 */
const char*
fes_error(FES fes) {
  return ((fes_handler*) fes)->last_error;
}
