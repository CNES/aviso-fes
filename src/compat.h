/**
 @file compat.h
 @brief Compatibility layer between Windows/POSIX OS
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

#include <string.h>

#if _WIN32

/**
 * Copies characters of one string to another with security enhancements
 */
#define STRNCPY(dst, src, size)                                                \
  strncpy_s((dst), (sizeof(dst)), (src), _TRUNCATE)

/**
 * Duplicates string
 */
#define STRDUP(s) _strdup(s)

/**
 * Concatenates string
 */
#define STRNCAT_S(dest, size, src, count) strncat_s(dest, size, src, count)

/**
 * Reads formatted data from a string
 */
#define SSCANF(format, ...) sscanf_s((format), ##__VA_ARGS__)

/**
 * Performs a case-insensitive comparison of strings.
 */
#define STRCASECMP _stricmp

/**
 * Gets a value from the current environment.
 *
 * @param Environment variable name.
 * @return the value of the environment variable as a NUL-terminated string.
 * If the variable name is not in the current environment, NULL is returned.
 */
char*
duplicate_env(const char* name);

/**
 * Extract the directory part of a pathname
 *
 * @param path String to process
 * @return a pointer to the parent directory of path. The return value points
 * to an internal storage space allocated on the first call that will be
 * overwritten by subsequent calls.
 */
char*
dirname(const char* path);

#else

/**
 * Copies characters of one string to another with security enhancements
 */
#define STRNCPY(dst, src, size)                                                \
  strncpy((dst), (src), (size));                                               \
  (dst)[(size)-1] = '\0'

/**
 * Duplicates string
 */
#define STRDUP(s) strdup(s)

/**
 * Concatenates string
 */
#define STRNCAT_S(dest, size, src, count) strncat(dest, src, count)

/**
 * Reads formatted data from a string
 */
#define SSCANF(format, ...) sscanf((format), ##__VA_ARGS__)

/**
 * Performs a case-insensitive comparison of strings.
 */
#define STRCASECMP strcasecmp

#endif // _WIN32
