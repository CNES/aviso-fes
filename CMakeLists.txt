cmake_minimum_required(VERSION 3.10.0)

# CMake module search path
set(CMAKE_MODULE_PATH
    "${CMAKE_CURRENT_SOURCE_DIR}/cmake"
    "${CMAKE_SOURCE_DIR}/third_party//CMake-codecov/cmake"
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

message(STATUS "fes v${FES_VERSION} ${FES_VERSION_TYPE}")

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

if(POLICY CMP0063)
  cmake_policy(SET CMP0063 NEW)
endif()

if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE RELWITHDEBINFO)
endif()

message(STATUS "Build type: ${CMAKE_BUILD_TYPE}")

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

# Find boost
find_package(Boost 1.79 REQUIRED)
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

# CMake-codecov
if(FES_ENABLE_COVERAGE)
  set(ENABLE_COVERAGE ON)
  find_package(codecov)
endif()

# Find Eigen
find_package(Eigen3 3.3.1 REQUIRED)
include_directories(${EIGEN3_INCLUDE_DIR})

# Find Google Test, if unit tests are enabled
if(FES_ENABLE_TEST)
  find_package(GTest REQUIRED)
endif()

if(FES_ENABLE_FPIC)
  set(CMAKE_VISIBILITY_INLINES_HIDDEN ON)
  set(CMAKE_CXX_VISIBILITY_PRESET hidden)
  set(CMAKE_POSITION_INDEPENDENT_CODE ON)
endif()

# Find Python
if(FES_BUILD_PYTHON_BINDINGS)
  find_package(Python3 COMPONENTS Interpreter Development)
  add_subdirectory("${CMAKE_CURRENT_SOURCE_DIR}/third_party/pybind11")

  set(CMAKE_VISIBILITY_INLINES_HIDDEN ON)
  set(CMAKE_CXX_VISIBILITY_PRESET hidden)
  set(CMAKE_POSITION_INDEPENDENT_CODE ON)
endif()

file(GLOB_RECURSE LIBRARY_SOURCES "src/library/*.cpp")
add_library(fes STATIC ${LIBRARY_SOURCES})
target_include_directories(fes PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include
                                      ${CMAKE_CURRENT_BINARY_DIR}/include)
if(FES_ENABLE_COVERAGE)
  add_coverage(fes)
endif()
if(BLAS_FOUND)
  target_link_libraries(fes ${BLAS_LIBRARIES})
endif()
install(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/include/fes DESTINATION include)
install(
  TARGETS fes
  ARCHIVE DESTINATION lib
  LIBRARY DESTINATION lib COMPONENT library)

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
  message(STATUS "clang-tidy enabled.")
endif()

if(FES_ENABLE_TEST)
  add_subdirectory(tests/library)
endif()

if(FES_BUILD_PYTHON_BINDINGS)
  include(ProcessorCount)
  ProcessorCount(NUM_CORES)

  file(GLOB_RECURSE PYTHON_SOURCES "src/python/core/*.cpp")
  pybind11_add_module(core ${PYTHON_SOURCES})

  if(FES_ENABLE_COVERAGE)
    add_coverage(core)
  endif()

  target_link_libraries(core PRIVATE fes)
  if(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
    set_property(
      TARGET core
      APPEND
      PROPERTY COMPILE_OPTIONS -flto=${NUM_CORES})
    set_property(
      TARGET core
      APPEND
      PROPERTY LINK_OPTIONS -flto=${NUM_CORES})
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
