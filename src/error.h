/**
 @file error.h
 @brief Management of errors.
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
 @brief Set the internal error message to fes handler

 @param fes FES handler
 @param errcode Error code
 */

void
set_fes_error(fes_handler* const fes, const fes_enum_error errcode);

/**
 @brief Set the internal error message to fes handler

 @param fes FES handler
 @param errcode Error code
 @param format Error message template, in sprintf format
 */
void
set_fes_extended_error(fes_handler* const fes,
                       const fes_enum_error errcode,
                       const char* format,
                       ...);

/**
 @brief Clear possible error state of struct FES

 @param fes Clear the state of the argument
 */
void
fes_clear_error(fes_handler* const fes);
