/* This file is part of FES library.

   FES is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.

   FES is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with FES.  If not, see <http://www.gnu.org/licenses/>.
*/

#include <config.h>
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <libgen.h>
#ifdef HAVE_BSD_STRING
#include <bsd/string.h>
#endif

#include "ini.h"
#include "error.h"

#define BUFFER_SIZE 256

/*
 duplicateString

 Duplicate a string.
 des Pointer to the new string.
 src String to duplicate.

 @return 0 on success otherwise an error status.
 */
static int _duplicate_string(fes_handler* fes, char** dest,
                             const char* const src) {
#ifdef _WIN32
#define STRDUP _strdup
#else
#define STRDUP strdup
#endif

  if (src == NULL)
    *dest = NULL;
  else if ((*dest = STRDUP(src)) == NULL) {
    set_fes_error(fes, FES_NO_MEMORY);
    return 1;
  }

  return 0;
}

/*
 skipSpace

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
 trim

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
 addEntry

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
 getEntry

 Returns the value for the key "key"

 ini
 key

 Returns the value associated with key if one exists, null otherwise.
 */
static const char*
_get_entry(_ini* const ini, const char* const key) {
#ifdef _WIN32
#define strcasecmp _stricmp
#endif

  unsigned int ix;

  for (ix = 0; ix < ini->nItems; ix++)
    if (strcasecmp(ini->key[ix], key) == 0)
      return ini->val[ix];

  return NULL;
}

/*
 parseIni

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
    if (sscanf(first_char, "%[^=] = \"%[^\"]\"", key, val) == 2
        || sscanf(first_char, "%[^=] = '%[^\']'", key, val) == 2
        || sscanf(first_char, "%[^=] = %[^;]", key, val) == 2) {
      /* If the value contains an environment variable turns on the
       string. */
      first_char = val;

      if (sscanf(first_char, "${%[^${}]}", tmp) == 1) {
        char* ptr = getenv(tmp);

        if (ptr == NULL) {
          set_fes_extended_error(fes, FES_INI_ERROR,
                                 "%s environment variable is not set.", tmp);
          return 1;
        }

        strlcpy(tmp, ptr, sizeof(tmp) - 1);
        strlcat(tmp, strstr(val, "}") + 1, sizeof(tmp) - strlen(tmp) - 1);
        strlcpy(val, tmp, sizeof(val));
      }
      if (sscanf(first_char, "./%[^;]", tmp) == 1) {
        snprintf(val, sizeof(val), "%s/%s", root, tmp);
      }

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
  _ini* ini;
  FILE* stream;
  char* path_copy;

  if ((stream = fopen(path, "r")) == NULL) {
    set_fes_extended_error(fes, FES_IO_ERROR,
                           "Can't open file `%s' for reading.", path);
    return 1;
  }

  if((path_copy = STRDUP(path)) == NULL) {
    set_fes_error(fes, FES_NO_MEMORY);
    return 1;
  }

  if ((ini = (_ini*) calloc(1, sizeof(_ini))) == NULL) {
    free(path_copy);
    set_fes_error(fes, FES_NO_MEMORY);
    return 1;
  }

  if (parse_ini_file(fes, stream, dirname(path_copy), ini)) {
    free(path_copy);
    free(ini);
    return 1;
  }

  free(path_copy);
  fclose(stream);

  *handle = ini;

  return 0;
}

/*
 */
void ini_close(void* handle) {
  unsigned int ix;
  _ini* ini = (_ini*) handle;

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
    if (sscanf(value, "%d", &result) != 1)
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
    if (sscanf(value, "%lf", &result) != 1)
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
