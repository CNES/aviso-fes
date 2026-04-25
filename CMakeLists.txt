cmake_minimum_required(VERSION 3.15.0)

# CMake module search path
set(CMAKE_MODULE_PATH
    "${CMAKE_CURRENT_SOURCE_DIR}/cmake"
    "${CMAKE_SOURCE_DIR}/third_party/CMake-codecov/cmake"
    "${CMAKE_MODULE_PATH}")

if("${CMAKE_SOURCE_DIR}" STREQUAL "${CMAKE_CURRENT_BINARY_DIR}")
  message(FATAL_ERROR "The build directory must be different from the \
        root directory of this software.")
endif()

if(POLICY CMP0077)
  cmake_policy(SET CMP0077 NEW)
endif()

if(POLICY CMP0167)
  cmake_policy(SET CMP0167 NEW)
endif()

# On development machines, generate the version file. On other machines, ignore
# errors.
execute_process(COMMAND ${CMAKE_CURRENT_SOURCE_DIR}/setup.py ERROR_QUIET)

# Extract project version from source
file(STRINGS "${CMAKE_CURRENT_SOURCE_DIR}/include/fes/version.hpp"
     fes_version_defines REGEX "#define FES_VERSION_(MAJOR|MINOR|PATCH) ")

foreach(item ${fes_version_defines})
  if(item MATCHES [[#define FES_VERSION_(MAJOR|MINOR|PATCH) +([^ ]+)$]])
    set(FES_VERSION_${CMAKE_MATCH_1} "${CMAKE_MATCH_2}")
  endif()
endforeach()

if(FES_VERSION_PATCH MATCHES [[[0-9]*\.?(dev[0-9]+)]])
  set(FES_VERSION_TYPE "${CMAKE_MATCH_1}")
endif()
string(REGEX MATCH "^[0-9]+" FES_VERSION_PATCH "${FES_VERSION_PATCH}")

project(
  FES
  LANGUAGES CXX
  VERSION "${FES_VERSION_MAJOR}.${FES_VERSION_MINOR}.${FES_VERSION_PATCH}"
  DESCRIPTION "FES Tidal Prediction Library")

# Define the build type Asan to enable address sanitizer if the compiler matches
# the requirements (GCC or Clang)
if(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
  set(SANITIZE "address,undefined")
  if(UNIX AND NOT APPLE)
    set(SANITIZE "${SANITIZE},leak")
  endif()

  set(CMAKE_C_FLAGS_ASAN
      "${CMAKE_C_FLAGS_RELWITHDEBINFO} -fsanitize=${SANITIZE} \
      -fno-omit-frame-pointer -fno-common"
      CACHE STRING "" FORCE)
  set(CMAKE_CXX_FLAGS_ASAN
      "${CMAKE_CXX_FLAGS_RELWITHDEBINFO} -fsanitize=${SANITIZE} \
      -fno-omit-frame-pointer -fno-common"
      CACHE STRING "" FORCE)
  set(CMAKE_EXE_LINKER_FLAGS_ASAN
      "${CMAKE_EXE_LINKER_FLAGS_RELWITHDEBINFO} -fsanitize=${SANITIZE}"
      CACHE STRING "" FORCE)
  set(CMAKE_SHARED_LINKER_FLAGS_ASAN
      "${CMAKE_SHARED_LINKER_FLAGS_RELWITHDEBINFO} -fsanitize=${SANITIZE}"
      CACHE STRING "" FORCE)

  set_property(
    CACHE CMAKE_BUILD_TYPE PROPERTY STRINGS "Debug" "Release" "RelWithDebInfo"
                                    "MinSizeRel" "Asan")
endif()

option(FES_BUILD_PYTHON_BINDINGS "Build Python bindings" OFF)
option(FES_ENABLE_CLANG_TIDY "Enable clang-tidy" OFF)
option(FES_ENABLE_FPIC "Enable position independent code" ON)
option(FES_ENABLE_OPTIMIZATION "Enable optimization" ON)
option(FES_ENABLE_TEST "Build unit tests" OFF)
option(FES_ENABLE_COVERAGE "Enable coverage" OFF)
option(FES_USE_IERS_CONSTANTS "Use IERS 2010 constants" OFF)

if(POLICY CMP0063)
  cmake_policy(SET CMP0063 NEW)
endif()

if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE RELWITHDEBINFO)
endif()

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

# Enable all warnings
if(NOT WIN32)
  if(NOT CMAKE_CXX_FLAGS MATCHES "-Wall$")
    string(APPEND CMAKE_CXX_FLAGS " -Wall")
  endif()

  if(NOT CMAKE_CXX_COMPILER MATCHES "icpc$" AND NOT CMAKE_CXX_FLAGS MATCHES
                                                "-Wpedantic$")
    string(APPEND CMAKE_CXX_FLAGS " -Wpedantic")
  endif()
endif()

# Helper function to resolve the Boost include directory if find_package fails.
# It looks for boost/version.hpp in the Boost include directories and in the
# CMAKE_PREFIX_PATH.
function(resolve_boost_include_dir out_var)
  set(boost_search_hints)
  if(Boost_INCLUDE_DIRS)
    list(APPEND boost_search_hints "${Boost_INCLUDE_DIRS}")
  endif()
  foreach(prefix IN LISTS CMAKE_PREFIX_PATH)
    list(APPEND boost_search_hints "${prefix}/include" "${prefix}")
  endforeach()

  find_path(
    _BOOST_INCLUDE_DIR
    NAMES boost/version.hpp
    HINTS ${boost_search_hints})

  set(${out_var}
      "${_BOOST_INCLUDE_DIR}"
      PARENT_SCOPE)
endfunction()

# Helper function to check the Boost version by reading the boost/version.hpp
# file
function(check_boost_version boost_include_dir min_version)
  if(NOT EXISTS "${boost_include_dir}/boost/version.hpp")
    message(FATAL_ERROR "Boost version.hpp not found at "
                        "${boost_include_dir}/boost/version.hpp")
  endif()

  file(READ "${boost_include_dir}/boost/version.hpp" BOOST_VERSION_CONTENT)
  string(REGEX MATCH "BOOST_LIB_VERSION \"([0-9_]+)\"" _
               "${BOOST_VERSION_CONTENT}")
  set(boost_version_string "${CMAKE_MATCH_1}")
  message(STATUS "Boost version: ${boost_version_string}")

  string(REPLACE "_" "." BOOST_VERSION_FORMATTED "${boost_version_string}")
  if(BOOST_VERSION_FORMATTED VERSION_LESS "${min_version}")
    message(FATAL_ERROR "Boost version must be at least ${min_version}, "
                        "found ${BOOST_VERSION_FORMATTED}")
  endif()
endfunction()

# Search for Boost using find_package. If it fails (e.g. CMake 4.x removed
# FindBoost.cmake and only CONFIG mode is available), fall back to resolving the
# include directory manually and checking boost/version.hpp.
find_package(Boost 1.79 QUIET)

if(NOT Boost_FOUND)
  resolve_boost_include_dir(Boost_INCLUDE_DIRS)

  if(NOT Boost_INCLUDE_DIRS)
    message(
      FATAL_ERROR
        "Boost not found with find_package, and boost/version.hpp was not "
        "found in Boost_INCLUDE_DIRS/CMAKE_PREFIX_PATH.")
  endif()

  message(STATUS "Boost headers: ${Boost_INCLUDE_DIRS}")
  check_boost_version("${Boost_INCLUDE_DIRS}" "1.79.0")
endif()

include_directories(${Boost_INCLUDE_DIRS})

# BLAS: first try to use MKL as a single dynamic library
set(BLA_VENDOR Intel10_64_dyn)
find_package(BLAS)
if(NOT BLAS_FOUND)
  # Otherwise try to use MKL lp64 model with sequential code
  set(BLA_VENDOR Intel10_64lp_seq)
  find_package(BLAS)
endif()

if(BLAS_FOUND)
  # MKL
  if(DEFINED ENV{MKLROOT})
    find_path(
      MKL_INCLUDE_DIR
      NAMES mkl.h
      HINTS $ENV{MKLROOT}/include)
    if(MKL_INCLUDE_DIR)
      add_definitions(-DEIGEN_USE_MKL_ALL)
      add_definitions(-DMKL_LP64)
      include_directories(${MKL_INCLUDE_DIR})
    endif()
  endif()
else()
  set(BLA_VENDOR_LIST "Apple" "OpenBLAS" "Generic")
  foreach(item IN LISTS BLA_VENDOR_LIST)
    set(BLA_VENDOR ${item})
    find_package(BLAS)
    if(BLAS_FOUND)
      break()
    endif()
  endforeach()
  if(BLAS_FOUND)
    add_definitions(-DEIGEN_USE_BLAS)
  else()
    message(
      WARNING "No BLAS library has been found. Eigen will use its own BLAS "
              "implementation.")
  endif()
endif()

if(APPLE)
  list(APPEND CMAKE_INSTALL_RPATH "@loader_path")
elseif(UNIX)
  list(APPEND CMAKE_INSTALL_RPATH "$ORIGIN")
endif()

# CMake-codecov
if(FES_ENABLE_COVERAGE)
  set(ENABLE_COVERAGE ON)
  find_package(codecov)
endif()

# Find Eigen.
find_package(Eigen3 5 CONFIG QUIET)
if(NOT TARGET Eigen3::Eigen)
  find_package(Eigen3 3.4.0 REQUIRED)
endif()
if(NOT EIGEN3_INCLUDE_DIR AND EIGEN3_INCLUDE_DIRS)
  set(EIGEN3_INCLUDE_DIR ${EIGEN3_INCLUDE_DIRS})
endif()
if(NOT EIGEN3_INCLUDE_DIR AND TARGET Eigen3::Eigen)
  get_target_property(EIGEN3_INCLUDE_DIR Eigen3::Eigen
                      INTERFACE_INCLUDE_DIRECTORIES)
endif()
include_directories(SYSTEM ${EIGEN3_INCLUDE_DIR})

# Find Google Test, if unit tests are enabled
if(FES_ENABLE_TEST)
  find_package(GTest REQUIRED)
endif()

if((FES_ENABLE_FPIC OR FES_BUILD_PYTHON_BINDINGS) AND NOT BUILD_SHARED_LIBS)
  set(CMAKE_VISIBILITY_INLINES_HIDDEN ON)
  set(CMAKE_CXX_VISIBILITY_PRESET hidden)
  set(CMAKE_POSITION_INDEPENDENT_CODE ON)
endif()

# Find Python
if(FES_BUILD_PYTHON_BINDINGS)
  # Honour the Python3_EXECUTABLE hint passed by setup.py instead of greedily
  # picking the highest version found on PATH (manylinux images ship several).
  set(Python3_FIND_STRATEGY LOCATION) # cmake-lint: disable=C0103
  set(Python_FIND_STRATEGY LOCATION) # cmake-lint: disable=C0103
  if(DEFINED Python3_EXECUTABLE AND NOT DEFINED Python_EXECUTABLE)
    set(Python_EXECUTABLE "${Python3_EXECUTABLE}") # cmake-lint: disable=C0103
  endif()
  find_package(Python3 3.8 COMPONENTS Interpreter Development)
  set(PYBIND11_FINDPYTHON ON)
  set(PYBIND11_USE_SMART_HOLDER ON)
  add_subdirectory("${CMAKE_CURRENT_SOURCE_DIR}/third_party/pybind11")
endif()

file(GLOB_RECURSE LIBRARY_SOURCES "src/library/*.cpp")
if(BUILD_SHARED_LIBS)
  add_library(fes SHARED ${LIBRARY_SOURCES})
else()
  add_library(fes STATIC ${LIBRARY_SOURCES})
endif()

target_include_directories(
  fes PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
             $<INSTALL_INTERFACE:include>)
if(FES_USE_IERS_CONSTANTS)
  target_compile_definitions(
    fes PUBLIC FES_USE_IERS_CONSTANTS=${FES_USE_IERS_CONSTANTS})
endif()

if(FES_ENABLE_COVERAGE)
  add_coverage(fes)
endif()
if(BLAS_FOUND)
  target_link_libraries(fes ${BLAS_LIBRARIES})
endif()

install(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/include/fes DESTINATION include)
install(
  TARGETS fes
  LIBRARY DESTINATION lib
  ARCHIVE DESTINATION lib
  RUNTIME DESTINATION bin
  INCLUDES
  DESTINATION include)
set(CMAKE_INSTALL_RPATH "${CMAKE_INSTALL_PREFIX}/lib")
set(CMAKE_INSTALL_RPATH_USE_LINK_PATH TRUE)

set(CMAKE_BUILD_RPATH_USE_ORIGIN ON)

if(GTest_FOUND OR FES_ENABLE_CLANG_TIDY)
  include(CTest)
endif()

# Enable clang-tidy
if(FES_ENABLE_CLANG_TIDY)
  find_program(
    CLANG_TIDY_EXE
    NAMES "clang-tidy"
    DOC "/usr/bin/clang-tidy")
  if(NOT CLANG_TIDY_EXE)
    message(
      FATAL_ERROR
        "clang-tidy not found. Please set CLANG_TIDY_EXE to clang-tidy "
        "executable.")
  endif()
  string(
    CONCAT
      CLANG_TIDY_CMD
      "clang-tidy;-checks=-*,boost-*,concurrency-*,modernize-*,performance-*,"
      "cppcoreguidelines-*,-cppcoreguidelines-avoid-magic-numbers,"
      "clang-analyzer-*,portability-*,-portability-simd-intrinsics,google-*,"
      "readability-*,-readability-identifier-length,-readability-magic-numbers;"
      "-fix")
  set(CMAKE_CXX_CLANG_TIDY "${CLANG_TIDY_CMD}")
  unset(CLANG_TIDY_EXE CACHE)
  unset(CLANG_TIDY_CMD CACHE)
endif()

if(FES_ENABLE_TEST)
  add_subdirectory(tests/library)
endif()

if(FES_BUILD_PYTHON_BINDINGS)
  include(ProcessorCount)
  ProcessorCount(NUM_CORES)

  file(GLOB_RECURSE PYTHON_SOURCES "src/core/*.cpp")
  pybind11_add_module(core ${PYTHON_SOURCES})

  if(FES_ENABLE_COVERAGE)
    add_coverage(core)
  endif()

  target_link_libraries(core PRIVATE fes)
  message(STATUS ${CMAKE_CXX_COMPILER_ID})
  if(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
    # Skip LTO if using Clang from conda (does not support Gold plugin)
    if(CMAKE_CXX_COMPILER_ID STREQUAL "Clang" AND CMAKE_CXX_COMPILER MATCHES
                                                  "conda")
      message(STATUS "Skipping LTO options for Clang from conda.")
    else()
      set_property(
        TARGET core
        APPEND
        PROPERTY COMPILE_OPTIONS -flto=${NUM_CORES})
      set_property(
        TARGET core
        APPEND
        PROPERTY LINK_OPTIONS -flto=${NUM_CORES})
    endif()
  elseif(MSVC)
    set_property(
      TARGET core
      APPEND
      PROPERTY COMPILE_OPTIONS /GL)
    set_property(
      TARGET core
      APPEND
      PROPERTY LINK_OPTIONS /LTCG)
  endif()
endif()

if(FES_ENABLE_COVERAGE)
  list(APPEND LCOV_REMOVE_PATTERNS "'${CMAKE_CURRENT_SOURCE_DIR}/test/*'")
  coverage_evaluate()
endif()

# Try to find the prerequisites to compile the tide prediction example. If not
# found, skip it without error.
find_package(PkgConfig)
if(PkgConfig_FOUND)
  pkg_check_modules(NETCDF_CXX netcdf-cxx4)
endif()
if(NOT NETCDF_CXX_FOUND AND DEFINED ENV{CONDA_PREFIX})
  set(NETCDF_CXX_INCLUDE_DIRS "$ENV{CONDA_PREFIX}/include")
  find_library(
    NETCDF_CXX_LIBRARIES
    NAMES netcdf-cxx4 netcdf_c++4
    HINTS "$ENV{CONDA_PREFIX}/lib")
  if(NETCDF_CXX_LIBRARIES)
    add_executable(tide_prediction
                   ${CMAKE_CURRENT_SOURCE_DIR}/examples/prediction.cpp)
    target_compile_features(tide_prediction PRIVATE cxx_std_20)
    target_link_libraries(tide_prediction fes ${NETCDF_CXX_LIBRARIES}
                          Boost::boost)
    target_include_directories(
      tide_prediction PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include
                              ${EIGEN3_INCLUDE_DIR} ${NETCDF_CXX_INCLUDE_DIRS})
  endif()
endif()

message(STATUS "")
message(
  STATUS
    "====================[ FES Configuration Summary ]=====================")
message(STATUS "Version                : ${FES_VERSION}.${FES_VERSION_TYPE}")
if(FES_VERSION_TYPE MATCHES "dev")
  message(STATUS "Release Type           : Development")
else()
  message(STATUS "Release Type           : Release")
endif()
message(STATUS "Build Type             : ${CMAKE_BUILD_TYPE}")
if(BUILD_SHARED_LIBS)
  message(STATUS "Library Type           : Shared")
else()
  message(STATUS "Library Type           : Static")
endif()
if(FES_USE_IERS_CONSTANTS)
  message(STATUS "Constants              : IERS 2010")
else()
  message(STATUS "Constants              : Schureman 1958")
endif()
message(STATUS "Python Bindings        : ${FES_BUILD_PYTHON_BINDINGS}")
message(STATUS "Clang-Tidy             : ${FES_ENABLE_CLANG_TIDY}")
message(STATUS "Unit Tests             : ${FES_ENABLE_TEST}")
message(STATUS "Code Coverage          : ${FES_ENABLE_COVERAGE}")
message(STATUS "C++ Standard           : ${CMAKE_CXX_STANDARD}")
message(STATUS "C++ Standard Required  : ${CMAKE_CXX_STANDARD_REQUIRED}")
message(STATUS "C++ Extensions         : ${CMAKE_CXX_EXTENSIONS}")
message(STATUS "Position Independent   : ${CMAKE_POSITION_INDEPENDENT_CODE}")
message(STATUS "CXX Flags              : ${CMAKE_CXX_FLAGS}")
message(STATUS "Install RPATH          : ${CMAKE_INSTALL_RPATH}")
message(
  STATUS
    "=======================================================================")
message(STATUS "")
