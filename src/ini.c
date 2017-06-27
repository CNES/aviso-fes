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

#include "ini.h"

#include <ctype.h>
#ifndef _WIN32
#include <libgen.h>
#endif
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "compat.h"
#include "error.h"
#include "fes.h"

#define BUFFER_SIZE 256

/*
 Duplicate a string.
 des Pointer to the new string.
 src String to duplicate.

 @return 0 on success otherwise an error status.
 */
static int _duplicate_string(fes_handler* fes, char** dest,
                             const char* const src) {
  if (src == NULL)
    *dest = NULL;
  else if ((*dest = STRDUP(src)) == NULL) {
    set_fes_error(fes, FES_NO_MEMORY);
    return 1;
  }

  return 0;
}

/*
 Skip spaces

 src String

 Returns a pointer to the first character
 */
static char*
_skip_space(char* str) {
  char* result = str;

  while (*result && isspace(*result))
    result++;

  return result;
}

/*
 Trim blank spaces

 src String

 Returns a pointer to the string result.
 */
static char*
_trim(char* string) {
  char* ptr = _skip_space(string);
  int ix = 0;

  if (ptr != string) {
    while (*ptr)
      string[ix++] = *(ptr++);
  } else
    ptr = string + strlen(string);

  while (ptr > string) {
    if (isspace (*(ptr - 1)) == 0)
      break;
    ptr--;
  }
  *ptr = 0;

  return string;
}

/*
 Add a new pair key, value into the table.

 ini
 key
 value

 Returns 0 on success otherwise an error status.
 */
static int _add_entry(fes_handler* fes, _ini* const ini, const char* const key,
                      const char* value) {
  ini->nItems++;

  if (ini->nItems > ini->maxItems) {
    ini->maxItems += BUFFER_SIZE;

    if (ini->maxItems == 0) {
      if ((ini->val = (char**) calloc(ini->maxItems, sizeof(char*))) == NULL) {
        set_fes_error(fes, FES_NO_MEMORY);
        return 1;
      }

      if ((ini->key = (char**) calloc(ini->maxItems, sizeof(char*))) == NULL) {
        set_fes_error(fes, FES_NO_MEMORY);
        return 1;
      }
    } else {
      if ((ini->val = (char**) realloc(ini->val, ini->maxItems * sizeof(char*)))
          == NULL) {
        set_fes_error(fes, FES_NO_MEMORY);
        return 1;
      }

      if ((ini->key = (char**) realloc(ini->key, ini->maxItems * sizeof(char*)))
          == NULL) {
        set_fes_error(fes, FES_NO_MEMORY);
        return 1;
      }
    }
  }

  if (_duplicate_string(fes, &ini->key[ini->nItems - 1], key))
    return 1;
  if (_duplicate_string(fes, &ini->val[ini->nItems - 1], value))
    return 1;

  return 0;
}

/*
 Returns the value for the key "key"

 ini
 key

 Returns the value associated with key if one exists, null otherwise.
 */
static const char*
_get_entry(_ini* const ini, const char* const key) {
  unsigned int ix;

  for (ix = 0; ix < ini->nItems; ix++)
    if (STRCASECMP(ini->key[ix], key) == 0)
      return ini->val[ix];

  return NULL;
}

/*
 Parse ini file

 stream
 ini

 Returns 0 on success otherwise an error status.
 */
static int parse_ini_file(fes_handler* fes, FILE* stream, char *root, _ini* ini) {
  char buffer[MAX_PATH] = { 0, };

  while (fgets(buffer, sizeof(buffer), stream) != NULL) {
    char* first_char = _skip_space(buffer);
    char key[MAX_PATH];
    char val[MAX_PATH];
    char tmp[MAX_PATH];

    /* If the string is too long then resize it. */
    if (buffer[MAX_PATH - 1] != 0)
      buffer[MAX_PATH - 1] = 0;

    if (*first_char == ';' || *first_char == 0)
      continue;

    /* Lookup pair key = "value" our key ='value' or key = value. */
#ifdef _WIN32
    if (sscanf_s(first_char, "%[^=] = \"%[^\"]\"", key, (unsigned)sizeof(key),
          val, (unsigned)sizeof(val)) == 2 ||
        sscanf_s(first_char, "%[^=] = '%[^\']'", key, (unsigned)sizeof(key),
          val, (unsigned)sizeof(val)) == 2 ||
        sscanf_s(first_char, "%[^=] = %[^;]", key, (unsigned)sizeof(key),
          val, (unsigned)sizeof(val)) == 2) {
      first_char = val;

      /* If the value contains an environment variable turns on the
       string. */
      if (sscanf_s(first_char, "${%[^${}]}", tmp,
          (unsigned)sizeof(tmp)) == 1) {
        char* ptr = duplicate_env(tmp);

        if (ptr == NULL) {
          set_fes_extended_error(fes, FES_INI_ERROR,
                                 "%s environment variable is not set.", tmp);
          return 1;
        }

        strncpy_s(tmp, sizeof(tmp) - 1, ptr, _TRUNCATE);
        strncat_s(tmp, sizeof(tmp) - strlen(tmp) - 1, strstr(val, "}") + 1,
                  _TRUNCATE);
        strncpy_s(val, sizeof(val), tmp, _TRUNCATE);
        free(ptr);
      }
      if (sscanf_s(first_char, "./%[^;]", tmp, (unsigned)sizeof(tmp)) == 1) {
        _snprintf_s(val, sizeof(val), _TRUNCATE, "%s/%s", root, tmp);
      }
#else
    if (sscanf(first_char, "%[^=] = \"%[^\"]\"", key, val) == 2
        || sscanf(first_char, "%[^=] = '%[^\']'", key, val) == 2
        || sscanf(first_char, "%[^=] = %[^;]", key, val) == 2) {
      first_char = val;

      /* If the value contains an environment variable turns on the
       string. */
      if (sscanf(first_char, "${%[^${}]}", tmp) == 1) {
        char* ptr = getenv(tmp);

        if (ptr == NULL) {
          set_fes_extended_error(fes, FES_INI_ERROR,
                                 "%s environment variable is not set.", tmp);
          return 1;
        }

        strncpy(tmp, ptr, sizeof(tmp) - 1);
        strncat(tmp, strstr(val, "}") + 1, sizeof(tmp) - strlen(tmp) - 1);
        STRNCPY(val, tmp, sizeof(val));
      }
      if (sscanf(first_char, "./%[^;]", tmp) == 1) {
        snprintf(val, sizeof(val), "%s/%s", root, tmp);
      }
#endif

      /* We check that the string read is not empty ( "" or''), if this
       is the case we add an empty string for the reading, otherwise
       we add the value read. */
      if (_add_entry(
          fes, ini, _trim(key),
          strcmp(val, "\"\"") == 0 || strcmp(val, "''") == 0 ? 0 : _trim(val)))
        return 1;
    }
  }
  return 0;
}

/*
 */
int ini_open(fes_handler* fes, const char* const path, void** handle) {
  int rc = 0;
  FILE* stream;
  _ini* ini = NULL;
  char* path_copy = NULL;

#ifdef _WIN32
  if (fopen_s(&stream, path, "r"))
    stream = NULL;
#else
  stream = fopen(path, "r");
#endif
  if (stream == NULL) {
    set_fes_extended_error(fes, FES_IO_ERROR,
                           "Can't open file `%s' for reading.", path);
    goto error;
  }

  if ((path_copy = STRDUP(path)) == NULL) {
    set_fes_error(fes, FES_NO_MEMORY);
    goto error;
  }

  if ((ini = (_ini*) calloc(1, sizeof(_ini))) == NULL) {
    set_fes_error(fes, FES_NO_MEMORY);
    goto error;
  }

  if (parse_ini_file(fes, stream, dirname(path_copy), ini))
    goto error;

  *handle = ini;

  goto finish;

  error:
    rc = 1;
    free(ini);

  finish:
    free(path_copy);
    if (stream != NULL)
      fclose(stream);
    return rc;
}

/*
 */
void ini_close(void* handle) {
  unsigned int ix;
  _ini* ini = (_ini*) handle;

  if (ini != NULL) {
    if (ini->maxItems > 0) {
      for (ix = 0; ix < ini->nItems; ix++) {
        free(ini->key[ix]);
        free(ini->val[ix]);
      }
      free(ini->key);
      free(ini->val);
    }
    free(ini);
  }
}

/*
 */
int ini_get_integer(void* const handle, const char* const key,
                    const int defValue) {
  _ini* ini = (_ini*) handle;
  const char* const value = _get_entry(ini, key);
  int result;

  if (value == NULL)
    result = defValue;
  else {
    if (SSCANF(value, "%d", &result) != 1)
      return defValue;
  }

  return result;
}

/*
 */
double ini_get_float(void* const handle, const char* const key,
                     const double defValue) {
  _ini* ini = (_ini*) handle;
  const char* const value = _get_entry(ini, key);
  double result;

  if (value == NULL)
    result = defValue;
  else {
    if (SSCANF(value, "%lf", &result) != 1)
      return defValue;
  }

  return result;
}

/*
 */
const char*
ini_get_string(void* const handle, const char* const key,
               const char* const defValue) {
  _ini* ini = (_ini*) handle;
  const char* const value = _get_entry(ini, key);

  if (value == NULL)
    return defValue;

  return value;
}
