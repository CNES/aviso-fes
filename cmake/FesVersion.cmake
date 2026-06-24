# Pure-CMake generation of include/fes/version.hpp.
#
# The version files (include/fes/version.hpp and src/pyfes/version.py) are not
# tracked in Git; they are generated from the Git tags. The canonical generator
# is setup.py (via setuptools_scm), which is used when the project is built as a
# Python wheel. That requires a Python interpreter together with setuptools_scm,
# which is inconvenient -- or simply unavailable -- when the project is consumed
# as a pure C++ dependency through `add_subdirectory()`, a Git submodule or
# `FetchContent`.
#
# This module reproduces just enough of that logic in CMake so that version.hpp
# can be produced without Python. It mirrors setuptools_scm's default
# "guess-next-dev" scheme: an exact tag yields `MAJOR.MINOR.PATCH`, while N
# commits past the latest tag yield `MAJOR.MINOR.(PATCH+1).devN`.
#
# Two version sources are supported, in order of preference:
#   1. A live Git checkout (.git present) -- queried with `git describe`.
#   2. `.git_archival.txt`, whose `$Format:...$` placeholders are expanded by
#      `git archive` (the mechanism behind GitHub's source zip/tarball
#      downloads, e.g. FetchContent from .../archive/refs/heads/main.zip). This
#      requires the `.git_archival.txt export-subst` entry in .gitattributes.

include_guard(GLOBAL)

# Turn a `git describe` string into MAJOR/MINOR/PATCH/DEV. Accepts both the
# exact-tag form ("2026.5.2") and the ahead-of-tag form
# ("2026.5.2-5-gabcdef[-dirty]").
function(_fes_version_from_describe describe out_major out_minor out_patch
         out_dev)
  set(dev "")
  if(describe MATCHES "^(.+)-([0-9]+)-g[0-9a-f]+(-dirty)?$")
    set(tag "${CMAKE_MATCH_1}")
    set(distance "${CMAKE_MATCH_2}")
    set(dirty "${CMAKE_MATCH_3}")
  else()
    # Archived exactly on a tag: no distance/hash suffix.
    set(tag "${describe}")
    set(distance 0)
    set(dirty "")
  endif()

  _fes_parse_tag("${tag}" major minor patch)

  if(NOT distance EQUAL 0 OR dirty)
    # guess-next-dev: the working tree is ahead of the tag, so advertise the
    # upcoming release as a development build of the next patch.
    math(EXPR patch "${patch} + 1")
    set(dev ".dev${distance}")
  endif()

  set(${out_major}
      "${major}"
      PARENT_SCOPE)
  set(${out_minor}
      "${minor}"
      PARENT_SCOPE)
  set(${out_patch}
      "${patch}"
      PARENT_SCOPE)
  set(${out_dev}
      "${dev}"
      PARENT_SCOPE)
endfunction()

# Extract MAJOR.MINOR.PATCH from a tag such as "2026.5.2" or "2026.4.0rc1".
function(_fes_parse_tag tag out_major out_minor out_patch)
  if(tag MATCHES "^v?([0-9]+)\\.([0-9]+)\\.([0-9]+)")
    set(${out_major}
        "${CMAKE_MATCH_1}"
        PARENT_SCOPE)
    set(${out_minor}
        "${CMAKE_MATCH_2}"
        PARENT_SCOPE)
    set(${out_patch}
        "${CMAKE_MATCH_3}"
        PARENT_SCOPE)
  else()
    message(WARNING "FES: cannot parse version tag '${tag}'; using 0.0.0")
    set(${out_major}
        "0"
        PARENT_SCOPE)
    set(${out_minor}
        "0"
        PARENT_SCOPE)
    set(${out_patch}
        "0"
        PARENT_SCOPE)
  endif()
endfunction()

# Resolve MAJOR/MINOR/PATCH/DEV from the Git history rooted at source_dir.
# Returns an empty MAJOR on failure (no Git, no tags, shallow clone, ...).
function(_fes_version_from_git source_dir out_major out_minor out_patch out_dev)
  set(${out_major}
      ""
      PARENT_SCOPE)

  find_package(Git QUIET)
  if(NOT Git_FOUND OR NOT EXISTS "${source_dir}/.git")
    return()
  endif()

  # "<tag>-<commits since tag>-g<short hash>[-dirty]"
  execute_process(
    COMMAND "${GIT_EXECUTABLE}" describe --tags --long --dirty --match
            "[0-9]*.[0-9]*.[0-9]*"
    WORKING_DIRECTORY "${source_dir}"
    OUTPUT_VARIABLE describe
    OUTPUT_STRIP_TRAILING_WHITESPACE
    ERROR_QUIET
    RESULT_VARIABLE describe_result)
  if(NOT describe_result EQUAL 0)
    return()
  endif()

  _fes_version_from_describe("${describe}" major minor patch dev)

  set(${out_major}
      "${major}"
      PARENT_SCOPE)
  set(${out_minor}
      "${minor}"
      PARENT_SCOPE)
  set(${out_patch}
      "${patch}"
      PARENT_SCOPE)
  set(${out_dev}
      "${dev}"
      PARENT_SCOPE)
endfunction()

# Resolve MAJOR/MINOR/PATCH/DEV from a `git archive`-expanded .git_archival.txt.
# Returns an empty MAJOR when the file is missing or its placeholders were never
# substituted (e.g. a plain `cp -r` of the source tree rather than an archive).
function(_fes_version_from_archive source_dir out_major out_minor out_patch
         out_dev)
  set(${out_major}
      ""
      PARENT_SCOPE)

  set(archival "${source_dir}/.git_archival.txt")
  if(NOT EXISTS "${archival}")
    return()
  endif()

  file(READ "${archival}" contents)
  if(NOT contents MATCHES "describe-name:[ \t]*([^\r\n]*)")
    return()
  endif()
  string(STRIP "${CMAKE_MATCH_1}" describe)

  # Unsubstituted ("$Format:...$") or describe found no tag: nothing usable.
  if(describe STREQUAL "" OR describe MATCHES "\\$Format:")
    return()
  endif()

  _fes_version_from_describe("${describe}" major minor patch dev)

  set(${out_major}
      "${major}"
      PARENT_SCOPE)
  set(${out_minor}
      "${minor}"
      PARENT_SCOPE)
  set(${out_patch}
      "${patch}"
      PARENT_SCOPE)
  set(${out_dev}
      "${dev}"
      PARENT_SCOPE)
endfunction()

# Ensure include/fes/version.hpp exists and reflects the current Git state.
#
# source_dir: the FES project root (the directory holding ".git" / "include").
#
# Resolution order:
#   1. The Git tags (preferred, no external tooling required).
#   2. .git_archival.txt expanded by `git archive` (GitHub zip/tarball, i.e.
#      FetchContent from an archive URL).
#   3. An already-present version.hpp (e.g. shipped in an sdist, or written by
#      setup.py during a wheel build) -- left untouched.
#   4. A 0.0.0 placeholder, so configuration can still proceed.
#
# The file is only rewritten when its contents change, to avoid spurious header
# rebuilds on every reconfigure.
function(fes_generate_version_header source_dir)
  set(header "${source_dir}/include/fes/version.hpp")

  _fes_version_from_git("${source_dir}" major minor patch dev)
  if(major STREQUAL "")
    _fes_version_from_archive("${source_dir}" major minor patch dev)
  endif()

  if(major STREQUAL "")
    if(EXISTS "${header}")
      # Nothing better to offer than what is already on disk; keep it.
      return()
    endif()
    message(
      WARNING
        "FES: unable to determine the version from Git and no version.hpp is "
        "present; defaulting to 0.0.0")
    set(major "0")
    set(minor "0")
    set(patch "0")
    set(dev "")
  endif()

  set(content
      "/// @file fes/version.hpp
/// @brief Version of the library
#pragma once
/// Major version of the library
#define FES_VERSION_MAJOR ${major}
/// Minor version of the library
#define FES_VERSION_MINOR ${minor}
/// Patch version of the library
#define FES_VERSION_PATCH ${patch}${dev}
")

  set(existing "")
  if(EXISTS "${header}")
    file(READ "${header}" existing)
  endif()

  if(NOT existing STREQUAL content)
    file(WRITE "${header}" "${content}")
    message(STATUS
            "FES: generated version.hpp (${major}.${minor}.${patch}${dev})")
  endif()
endfunction()
