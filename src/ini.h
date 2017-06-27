/**
 @file ini.h
 @brief Management for configuration file.
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
#include "fes_int.h"

/**
 @brief Stores the data necessary to read a configuration file.
 */
struct __ini {
  /** Number of items read. */
  unsigned int nItems;
  /** Keys known. */
  char** key;
  /** Known values of the keys. */
  char** val;
  /** Maximum number of elements that can be stored. */
  unsigned int maxItems;
};

/**
 @brief A utility data type for #__ini structure.
 */
typedef struct __ini _ini;

/**
 @fn ini_open(fes_handler* fes, const char* const path, void** handle)

 @brief Open a configuration file.

 @param fes FES handler.
 @param path Absolute or relative file name for ini file to be opened.
 @param handle Handle for the open file

 @return 0 on success or 1 on failure.
 */
int
ini_open(fes_handler* fes, const char* const path, void** handle);

/**
 @brief Close an open file.

 @param handle Handle for the open file
 */
void
ini_close(void* handle);

/**
 @brief Read the integer value for a given key.
 If the key doesn't exists the function returns the default value.

 @param handle Handle for the open file
 @param key Key to read
 @param defValue Default value

 @return the integer value or the default value if the given key doesn't exists.
 */
int
ini_get_integer(void* const handle, const char* const key, const int defValue);

/**
 @brief Read the float value for a given key.

 @param handle Handle for the open file
 @param key Key to read
 @param defValue Default value

 @return the float value or the default value if the given key doesn't exists.
 */
double
ini_get_float(void* const handle, const char* const key, const double defValue);

/**
 @brief Read the string for a given key.
 If the key doesn't exists the function returns the default value.

 @param handle Handle for the open file
 @param key Key to read
 @param defValue Default value

 @return the string or the default value if the given key doesn't exists.
 */
const char*
ini_get_string(void* const handle, const char* const key,
               const char* const defValue);
