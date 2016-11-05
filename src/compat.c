#include "compat.h"

#ifdef _WIN32
#include <stddef.h>
#include <stdlib.h>

/*
 *
 */
char*
duplicate_env(const char* name) {
  char* value;
  size_t len;
  errno_t err = _dupenv_s(&value, &len, name);
  return err ? NULL : value;
}

/*
 *
 */
char*
dirname(const char* path) {
  static char buffer[_MAX_PATH];
  char drive[_MAX_DRIVE];
  char dir[_MAX_DIR];
  char fname[_MAX_FNAME];
  char ext[_MAX_EXT];

  _splitpath_s(path, drive, sizeof(drive), dir, sizeof(dir), fname,
               sizeof(fname), ext, sizeof(ext));
  memset(buffer, 0, sizeof(buffer));
  strcat_s(buffer, sizeof(buffer), drive);
  strcat_s(buffer, sizeof(buffer), fname);

  return buffer;    
}
#endif // _WIN32