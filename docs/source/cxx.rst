C++ API
=======

This library allows you to make tide predictions using C++ and Python. The
Python part is documented in the rest of this documentation. Here, we want to
explain to readers who only want to use the C++ part of this library.

This documentation presents how to use the API to calculate tides in C++ using
the FES 2022 model. This model is used to make tide predictions in SWOT KaRIn
products.

The management part is not provided in this library, but we will illustrate it
with the example. In a more advanced use case, this library must be integrated
with CMake, but here we will focus on explaining basic usage using command line
commands to make it as simple as possible.

Overview
--------

The FES2022 C++ library provides high-performance tide prediction capabilities
using two types of tidal models:

* **Ocean Tide Model (LGP2)**: Uses unstructured triangular mesh with Lagrange
  P2 finite elements for ocean tide predictions
* **Radial Load Tide Model (Cartesian)**: Uses regular cartesian grids for
  computing radial loading effects

The library supports all major tidal constituents and provides accurate
predictions for any location and time.

Prerequisites
-------------

Before using the C++ library, ensure you have the following dependencies
installed:

* **C++20 compatible compiler** (GCC 10+, Clang 11+, or MSVC 2019+)
* **Eigen3** library for linear algebra operations
* **NetCDF-CXX4** library for reading tide model data files
* **FES2022 model data files** (both ocean and load tide datasets)

Main Components
---------------

The FES C++ API consists of several key components:

1. **Tidal Model Classes**:

   * ``fes::tidal_model::LGP1<T>``: For ocean tide models using triangular mesh
     with LGP1 discretization
   * ``fes::tidal_model::LGP2<T>``: For ocean tide models using triangular mesh
     with LGP2 discretization
   * ``fes::tidal_model::Cartesian<T>``: For load tide models using regular
     grids

2. **Core Functions**:

   * ``fes::evaluate_tide()``: Main function for tide prediction

3. **Configuration Classes**:

   * ``fes::Settings``: Configuration for astronomical formulae and precision
   * ``fes::Constituent``: Enumeration of tidal constituents

Step-by-Step Usage Guide
------------------------

Using the FES C++ library involves the following main steps:

Step 1: Include Required Headers
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

.. literalinclude:: ../../examples/prediction.cpp
   :language: c++
   :lines: 1-15

Step 2: Configure Data Paths
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Define the paths to your FES2022 model data files:

.. literalinclude:: ../../examples/prediction.cpp
   :language: c++
   :lines: 17-24

Step 3: Load Tidal Constituents
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Create functions to specify which tidal constituents to load:

.. literalinclude:: ../../examples/prediction.cpp
   :language: c++
   :lines: 26-74

Step 4: Data Loading Functions
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Create utility functions for loading NetCDF data:

.. literalinclude:: ../../examples/prediction.cpp
   :language: c++
   :lines: 76-102

Step 5: Load Cartesian Model
~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Load the radial load tide model from individual NetCDF files:

.. literalinclude:: ../../examples/prediction.cpp
   :language: c++
   :lines: 108-187

Key points for Cartesian model loading:

* Each tidal constituent is stored in a separate NetCDF file
* The grid coordinates must be consistent across all constituents
* Amplitude and phase data are combined into complex waves

Step 6: Load LGP2 Model
~~~~~~~~~~~~~~~~~~~~~~~

Load the ocean tide model from the LGP2 format file:

.. literalinclude:: ../../examples/prediction.cpp
   :language: c++
   :lines: 191-268

Key points for LGP2 model loading:

* All constituents are stored in a single NetCDF file
* Uses unstructured triangular mesh with Lagrange P2 elements
* Requires mesh topology (triangles and LGP2 codes)
* Supports spatial indexing for efficient interpolation

Step 7: Model Initialization
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Create model instances and configure settings:

.. literalinclude:: ../../examples/prediction.cpp
   :language: c++
   :lines: 271-276

The ``fes::Settings`` class allows you to configure:

* **Astronomical formulae**: IERS or Meeus algorithms
* **Time tolerance**: This parameter allows for the adjustment of astronomical
  angle calculations. When its value is set to zero, the angles will be
  recalculated each time the date changes. Otherwise, they will be considered
  valid as long as the time difference between the last evaluation and the
  current date is within the specified tolerance.

Step 8: Prepare Input Data
~~~~~~~~~~~~~~~~~~~~~~~~~~

Set up your prediction parameters:

.. literalinclude:: ../../examples/prediction.cpp
   :language: c++
   :lines: 277-292

Input requirements:

* **Times**: Unix timestamps (seconds since 1970-01-01)
* **Leap seconds**: TAI-UTC offset for each time point
* **Coordinates**: Longitude and latitude in degrees
* **All vectors must have the same size**

Step 9: Perform Tide Evaluation
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Call the main prediction function:

.. literalinclude:: ../../examples/prediction.cpp
   :language: c++
   :lines: 294-305

The ``fes::evaluate_tide()`` function returns a tuple containing:

1. **Short-period tide**: High-frequency tidal signal
2. **Long-period tide**: Low-frequency tidal signal
3. **Interpolation quality**: Quality flags for each point

Step 10: Process and Display Results
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Format and output the prediction results:

.. literalinclude:: ../../examples/prediction.cpp
   :language: c++
   :lines: 307-346

The results include:

* **Short_tide**: Short-period tidal elevation
* **LP_tide**: Long-period tidal elevation
* **Pure_Tide**: Combined ocean tide (Short + LP)
* **Geo_Tide**: Total geodetic tide (Pure + Load)
* **Rad_Tide**: Radial loading effect

Complete Example
----------------

Here's the complete working example:

.. literalinclude:: ../../examples/prediction.cpp
   :language: c++

Expected Output
---------------

When you run this example, you should see output similar to:

.. code-block:: text

    ==============================================================================================================
                            FES2022 TIDE PREDICTION RESULTS
    ==============================================================================================================
    Date        Hour      Latitude   Longitude  Short_tide     LP_tide   Pure_Tide    Geo_Tide    Rad_Tide
    --------------------------------------------------------------------------------------------------------------
    1983-10-10 0    h      59.195      -7.688     -92.468       0.925     -91.543     -88.976       2.568
    1983-10-20 0    h      59.195      -7.688     -44.481       0.185     -44.296     -42.692       1.604
    1983-10-30 0    h      59.195      -7.688       6.376      -0.629       5.746       6.167       0.420
    ...

The output columns represent:

* **Date**: Prediction date (YYYY-MM-DD format)
* **Hour**: Hour of the day (0-23)
* **Latitude/Longitude**: Location coordinates in degrees
* **Short_tide**: Short-period ocean tide in meters
* **LP_tide**: Long-period ocean tide in meters
* **Pure_Tide**: Total ocean tide (Short + LP) in meters
* **Geo_Tide**: Total geodetic tide including load effects in meters
* **Rad_Tide**: Radial load tide effect in meters

Compilation Instructions
------------------------

To compile and run this example, you need to link against the required libraries:

.. code-block:: bash

   # Compile the example
   g++ -O2 -Wall prediction.cpp --std=c++20 \
       -I ../include/ \
       -I /path/to/include \
       -I /path/to/include/eigen3 \
       -L/path/to/fes_library -lfes \
       -L/path/to/lib -lnetcdf-cxx4 -lnetcdf \
       -o tide_prediction

   # Run the example
   LD_LIBRARY_PATH=/path/to/lib ./tide_prediction

**Important Notes:**

* Replace ``/path/to/`` with your actual NetCDF installation path
* Ensure the FES library (``libfes.a`` or ``libfes.so``) is available in your library path
* Update the data file paths in the source code to match your FES2022 data location

Integration with CMake
-----------------------

For production use, integrate the library with CMake:

.. code-block:: cmake

   # CMakeLists.txt
   cmake_minimum_required(VERSION 3.15)
   project(TidePrediction)

   set(CMAKE_CXX_STANDARD 20)
   set(CMAKE_CXX_STANDARD_REQUIRED ON)

   # Find required packages
   find_package(Eigen3 REQUIRED)
   find_package(PkgConfig REQUIRED)
   pkg_check_modules(NETCDF_CXX REQUIRED netcdf-cxx4)

   # Find FES library
   find_library(FES_LIBRARY fes REQUIRED)
   find_path(FES_INCLUDE_DIR fes/tide.hpp REQUIRED)

   # Create executable
   add_executable(tide_prediction prediction.cpp)

   # Link libraries
   target_link_libraries(tide_prediction
       ${FES_LIBRARY}
       ${NETCDF_CXX_LIBRARIES}
       Eigen3::Eigen
   )

   target_include_directories(tide_prediction PRIVATE
       ${FES_INCLUDE_DIR}
       ${NETCDF_CXX_INCLUDE_DIRS}
   )
