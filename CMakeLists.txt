# This file is part of FES library.
#
# FES is free software: you can redistribute it and/or modify it under the terms
# of the GNU LESSER GENERAL PUBLIC LICENSE as published by the Free Software
# Foundation, either version 3 of the License, or (at your option) any later
# version.
#
# FES is distributed in the hope that it will be useful, but WITHOUT ANY
# WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR
# A PARTICULAR PURPOSE.  See the GNU LESSER GENERAL PUBLIC LICENSE for more
# details.
#
# You should have received a copy of the GNU LESSER GENERAL PUBLIC LICENSE along
# with FES.  If not, see <http://www.gnu.org/licenses/>.
cmake_minimum_required(VERSION 2.8.12)

if(POLICY CMP0042)
  cmake_policy(SET CMP0042 NEW)
endif()

cmake_policy(SET CMP0048 NEW)
if(POLICY CMP0063)
  cmake_policy(SET CMP0063 NEW)
endif()

if(POLICY CMP0074)
  cmake_policy(SET CMP0074 NEW) # CMake 3.12
endif()

if(POLICY CMP0077)
  cmake_policy(SET CMP0077 NEW)
endif()

project(fes)

include(CheckFunctionExists)
include(CheckCXXSourceRuns)

if("${CMAKE_SOURCE_DIR}" STREQUAL "${CMAKE_CURRENT_BINARY_DIR}")
  message(FATAL_ERROR "The build directory must be different from the \
        root directory of this software.")
endif()

# Options
option(BUILD_DOC "Build documentation" OFF)
option(BUILD_PYTHON "Build python extension" OFF)

# Version
file(STRINGS "${CMAKE_SOURCE_DIR}/include/fes.h" VERSION
     REGEX "define FES_VERSION")
string(REGEX REPLACE ".*([0-9]\\.[0-9]\\.[0-9]).*" "\\1" VERSION ${VERSION})
string(REPLACE "." ";" VERSION_LIST ${VERSION})
list(GET VERSION_LIST 0 LIB_VERSION_MAJOR)
list(GET VERSION_LIST 1 LIB_VERSION_MINOR)
list(GET VERSION_LIST 2 LIB_VERSION_PATCH)
set(LIB_VERSION
    "${LIB_VERSION_MAJOR}.${LIB_VERSION_MINOR}.${LIB_VERSION_PATCH}")

message(STATUS "FES version: ${LIB_VERSION}")

if(BUILD_PYTHON)
  project(fes LANGUAGES C CXX)
else()
  project(fes LANGUAGES C)
endif()

# CMake module search path
set(CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake;"
                      "${CMAKE_MODULE_PATH}")

# By default, build type is set to release, with debugging information.
if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE RELWITHDEBINFO)
endif()
message(STATUS "Build type: ${CMAKE_BUILD_TYPE}")

if(BUILD_PYTHON)
  # The library must be built using C++14 compiler.
  set(CMAKE_CXX_STANDARD 14)
  set(CMAKE_CXX_STANDARD_REQUIRED ON)
  set(CMAKE_CXX_EXTENSIONS OFF)
  set(CMAKE_MACOSX_RPATH 1)

  include(CheckCXXCompilerFlag)
  if(NOT WIN32)
    check_cxx_compiler_flag("-std=c++14" HAS_CPP14_FLAG)
  else()
    check_cxx_compiler_flag("/std:c++14" HAS_CPP14_FLAG)
  endif()

  if(NOT HAS_CPP14_FLAG)
    message(FATAL_ERROR "Unsupported compiler -- requires C++14 support!")
  endif()

  # Check CXX compiler and linker flags
  macro(CHECK_CXX_COMPILER_AND_LINKER_FLAGS result cpp_flags linker_flags)
    set(CMAKE_REQUIRED_FLAGS ${cpp_flags})
    set(CMAKE_REQUIRED_LIBRARIES ${linker_flags})
    set(CMAKE_REQUIRED_QUIET TRUE)
    check_cxx_source_runs("int main(int argc, char **argv) { return 0; }"
                          ${result})
    set(CMAKE_REQUIRED_FLAGS "")
    set(CMAKE_REQUIRED_LIBRARIES "")
  endmacro()

  # Always use libc++ on Clang
  if(CMAKE_CXX_COMPILER_ID MATCHES "Clang")
    check_cxx_compiler_and_linker_flags(HAS_LIBCPP "-stdlib=libc++"
                                        "-stdlib=libc++")
    if(HAS_LIBCPP)
      set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -stdlib=libc++")
      set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -stdlib=libc++")
      set(CMAKE_SHARED_LINKER_FLAGS
          "${CMAKE_SHARED_LINKER_FLAGS} -stdlib=libc++")
      check_cxx_compiler_and_linker_flags(HAS_LIBCPPABI "-stdlib=libc++"
                                          "-stdlib=libc++ -lc++abi")
      if(HAS_LIBCPPABI)
        set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -lc++abi")
        set(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} -lc++abi")
      endif()
    endif()
  endif()
endif()

if(${CMAKE_BUILD_TYPE} MATCHES "Debug")
  set(CMAKE_DEBUG_POSTFIX "-d")
endif()

if(NOT WIN32)
  if(NOT CMAKE_CXX_FLAGS MATCHES "-Wall$")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall")
  endif()
  if(NOT CMAKE_CXX_COMPILER MATCHES "icpc$" AND NOT CMAKE_CXX_FLAGS MATCHES
                                                "-Wpedantic$")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wpedantic")
  endif()
endif()

check_function_exists(pow POW_FUNCTION_EXISTS)
if(NOT POW_FUNCTION_EXISTS)
  unset(POW_FUNCTION_EXISTS CACHE)
  list(APPEND CMAKE_REQUIRED_LIBRARIES m)
  check_function_exists(pow POW_FUNCTION_EXISTS)
  if(POW_FUNCTION_EXISTS)
    set(MATH_LIBRARY
        m
        CACHE STRING "" FORCE)
  else()
    message(FATAL_ERROR "Failed making the pow() function available")
  endif()
endif()

enable_testing()

# API documentation if (BUILD_DOC) add_subdirectory(docs) endif()

# NetCDF
find_package(NetCDF 4.1.1 REQUIRED)
if(NOT EXISTS ${NETCDF_INCLUDE_DIRS}/netcdf.h)
  message(FATAL_ERROR "Unable to find NetCDF C-Header files")
endif()
include_directories(${NETCDF_INCLUDE_DIRS})

# uthash
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/third_party/uthash/src)

include_directories(include)
install(FILES include/fes.h DESTINATION include)

add_subdirectory(src)
add_subdirectory(test)
add_subdirectory(examples)

if(BUILD_PYTHON)
  find_package(Python3 COMPONENTS Interpreter Development)
  execute_process(
    COMMAND
      ${Python3_EXECUTABLE} -c [=[import os
import sysconfig
import sys
sys.stdout.write(os.path.dirname(sysconfig.get_config_h_filename()))
]=]
    OUTPUT_VARIABLE PYTHON_INCLUDE_DIR)
  execute_process(
    COMMAND
      ${Python3_EXECUTABLE} -c [=[import site
import sys
sys.stdout.write(site.getsitepackages()[-1])
]=]
    OUTPUT_VARIABLE PYTHON_SITE_DIR)
  find_package(PythonLibs REQUIRED)
  add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/third_party/pybind11)
  add_subdirectory(python)
endif()

if(EXISTS "/etc/debian_version")
  set(CPACK_GENERATOR "DEB")
elseif(EXISTS "/etc/redhat-release")
  set(CPACK_GENERATOR "RPM")
elseif(WIN32)
  set(CPACK_GENERATOR "ZIP")
else()
  set(CPACK_GENERATOR "TGZ")
endif()
set(CPACK_SOURCE_GENERATOR "TXZ")
list(APPEND CPACK_SOURCE_IGNORE_FILES ".git.*")
list(APPEND CPACK_SOURCE_IGNORE_FILES ".vscode")
list(APPEND CPACK_SOURCE_IGNORE_FILES "build/")
set(CPACK_PACKAGE_VENDOR "CNES/CLS/LEGOS")
set(CPACK_DEBIAN_PACKAGE_MAINTAINER "fbriol@groupcls.com")
set(CPACK_DEBIAN_PACKAGE_SHLIBDEPS ON)
set(CPACK_COMPONENT_LIBS_REQUIRED TRUE)
set(CPACK_PACKAGE_VERSION "${LIB_VERSION}")
set(CPACK_PACKAGE_VERSION_MAJOR "${LIB_VERSION_MAJOR}")
set(CPACK_PACKAGE_VERSION_MINOR "${LIB_VERSION_MINOR}")
set(CPACK_PACKAGE_VERSION_PATCH "${LIB_VERSION_PATCH}")

include(CPack)
