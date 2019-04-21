#  UTHASH_FOUND - system has uthash
#  UTHASH_INCLUDE_DIRS - the uthash include directory

#search starting from user editable cache var
if (UTHASH_INCLUDE_DIRS)
  # Already in cache, be silent
  set (UTHASH_FIND_QUIETLY TRUE)
endif ()

find_path (UTHASH_INCLUDE_DIRS uthash.h
  PATHS
  "${UTHASH_DIR}/include"
  PATH_SUFFIXES
    uthash)
mark_as_advanced (UTHASH_INCLUDE_DIRS)
set (UTHASH_C_INCLUDE_DIRS ${UTHASH_INCLUDE_DIRS})

if( EXISTS "${UTHASH_INCLUDE_DIRS}/uthash.h")
  file( STRINGS "${UTHASH_INCLUDE_DIRS}/uthash.h" _version_contents REGEX "define UTHASH_VERSION" )
  string( REGEX REPLACE ".*([0-9]\\.[0-9]\\.[0-9]).*" "\\1" UTHASH_VERSION ${_version_contents} )
  unset(_version_contents)
endif()

include (FindPackageHandleStandardArgs)
find_package_handle_standard_args (uthash
FOUND_VAR
  UTHASH_FOUND
REQUIRED_VARS
  UTHASH_INCLUDE_DIRS
VERSION_VAR
  UTHASH_VERSION)
