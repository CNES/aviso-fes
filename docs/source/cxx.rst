C++ API
=======

This library allows you to make tide predictions using C++ and Python. The
Python part is documented in the rest of this documentation. This section is
intended for readers who primarily want to use the C++ part of this library.

This documentation presents how to use the API to calculate tides in C++ using
either the FES 2022 model or GOT models. The FES 2022 model is used to make tide
predictions in SWOT KaRIn products.

Build system integration is not provided in this library, but we will illustrate it
with an example. In a more advanced use case, this library must be integrated
with CMake, but here we will focus on explaining basic usage using command line
commands to make it as simple as possible.

Overview
--------

The FES C++ library provides high-performance tide prediction capabilities
using two distinct prediction engines:

**FES/Darwin Engine**

* Uses Darwin's astronomical argument notation with Schureman nodal corrections
* 142 supported tidal constituents
* Compatible with: FES2014, FES2022 atlases
* Default formulae: ``SCHUREMAN_ORDER_1``
* Configurable inference modes (default: ``kSpline`` for FES)

**PERTH5/Doodson Engine**

* Uses Doodson number classification with group modulations
* 123 supported tidal constituents
* Compatible with: GOT4.10, GOT5.5, GOT5.6 atlases
* Default formulae: ``IERS``
* Configurable inference modes (zero, linear, spline, or Fourier)
* Developed by Dr. Richard Ray (NASA GSFC)

Both engines support two types of tidal models:

* **Ocean Tide Model (LGP1/LGP2)**: Uses unstructured triangular mesh with
  Lagrange P1 or P2 finite elements for ocean tide predictions
* **Radial Load Tide Model (Cartesian)**: Uses regular cartesian grids for
  computing radial loading effects

Choosing the Right Engine
-------------------------

The engine choice depends on your tidal atlas format:

**Use FES/Darwin Engine when:**

* Your tidal atlas is FES2014 or FES2022
* Following traditional oceanographic conventions
* Replicating established FES predictions
* Working with atlases using Darwin notation
* Need compatibility with historical FES software

**Use PERTH5/Doodson Engine when:**

* Your tidal atlas is GOT4.10, GOT5.5, GOT5.6
* Working with Richard Ray's tide models
* Need group modulation capabilities
* Require fine control over admittance interpolation
* Working with atlases using Doodson notation
* Need IERS astronomical conventions

Prerequisites
-------------

Before using the C++ library, ensure you have the following dependencies
installed:

* **C++14 compatible compiler** (GCC 10+, Clang 11+, or MSVC 2019+)
* **Eigen3** library for linear algebra operations
* **NetCDF-CXX4** library for reading tide model data files
* **Tidal model data files** (FES or GOT datasets depending on chosen engine)

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

2. **Constituent Namespaces**:

   * ``fes::darwin``: Darwin notation constituents (for FES engine)
   * ``fes::perth``: Doodson notation constituents (for PERTH5 engine)

3. **Core Functions**:

   * ``fes::evaluate_tide()``: Main function for tide prediction

4. **Configuration Classes**:

   * ``fes::FESSettings``: Default configuration for FES/Darwin engine
   * ``fes::PerthSettings``: Default configuration for PERTH5/Doodson engine
   * ``fes::Settings``: Base settings class (shared by both engines)
   * ``fes::angle::Formulae``: Enumeration of astronomical formulae options

Engine Configuration
--------------------

FES/Darwin Engine Settings
~~~~~~~~~~~~~~~~~~~~~~~~~~

The FES/Darwin engine uses ``fes::FESSettings`` (a subclass of
``fes::Settings``) for configuration:

.. code-block:: c++

   #include "fes/settings.hpp"

   // Create FES runtime settings (defaults: kSpline inference, kSchuremanOrder1)
   auto settings = fes::FESSettings()
       .with_num_threads(0)  // 0 = auto-detect
       .with_astronomic_formulae(fes::angle::Formulae::kSchuremanOrder1)
       .with_inference_type(fes::InferenceType::kSpline)
       .with_time_tolerance(3600.0);  // 1 hour angle caching

Configuration options (all defined on the base ``fes::Settings`` class):

* **inference_type**: Controls minor constituent inference:

  * ``kSpline``: Spline-based interpolation (default, recommended for FES)
  * ``kLinear``: Linear interpolation (recommended for GOT)
  * ``kZero``: No inference (fastest, requires all constituents)
  * ``kFourier``: Fourier-based (Munk-Cartwright)

* **astronomic_formulae**: Choose between ``kSchuremanOrder1`` (traditional),
  ``kSchuremanOrder3``, or ``kIERS`` (modern conventions)
* **time_tolerance**: Caching duration for astronomical angles (seconds). When
  set to zero, angles are recalculated for each date change.
* **num_threads**: Number of threads for parallel computation (0 = auto-detect)

PERTH5/Doodson Engine Settings
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

The PERTH5/Doodson engine uses ``fes::PerthSettings`` (a subclass of
``fes::Settings``) for configuration:

.. code-block:: c++

   #include "fes/settings.hpp"

   // Create PERTH5 runtime settings (defaults: kLinear inference, kIERS)
   auto settings = fes::PerthSettings()
       .with_num_threads(0)
       .with_group_modulations(true)
       .with_inference_type(fes::InferenceType::kLinear);

   // Or with Fourier interpolation
   auto precise_settings = fes::PerthSettings()
       .with_group_modulations(true)
       .with_inference_type(fes::InferenceType::kFourier);

Additional configuration options:

* **group_modulations**: Enable efficient grouped nodal corrections
* **astronomic_formulae**: Defaults to ``kIERS`` for modern conventions

.. note::

   Inference types are **generic** and defined on the base ``fes::Settings``
   class. All four modes (``kSpline``, ``kLinear``, ``kZero``, ``kFourier``)
   can be used with any engine.

Inference Types Comparison
~~~~~~~~~~~~~~~~~~~~~~~~~~

.. list-table:: Inference Types (``fes::InferenceType``)
   :header-rows: 1
   :widths: 20 20 15 45

   * - Inference Type
     - Accuracy
     - Speed
     - Use Case
   * - ``kZero``
     - Lowest (no inference)
     - Fastest
     - All constituents available in atlas
   * - ``kLinear``
     - Good (balanced)
     - Fast
     - Recommended for GOT atlases
   * - ``kSpline``
     - High
     - Fast
     - Recommended for FES atlases (default)
   * - ``kFourier``
     - Highest (best)
     - Slower
     - Munk-Cartwright Fourier series

Step-by-Step Usage Guide
------------------------

Using the FES C++ library involves the following main steps:

Step 1: Include Required Headers
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

.. literalinclude:: ../../examples/prediction.cpp
   :language: c++
   :lines: 1-17

Step 2: Configure Data Paths
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Define the paths to your model data files:

.. literalinclude:: ../../examples/prediction.cpp
   :language: c++
   :lines: 19-23

Step 3: Load Tidal Constituents
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Create functions to specify which tidal constituents to load. Both engines use
the unified ``fes::ConstituentId`` enum, but each engine namespace provides its
own set of supported constituent constants:

**For FES/Darwin Engine (fes::darwin namespace):**

.. literalinclude:: ../../examples/prediction.cpp
   :language: c++
   :lines: 28-82

**For PERTH5/Doodson Engine (fes::perth namespace):**

The PERTH5 namespace provides similar constituent constants under
``fes::perth``:

.. code-block:: c++

   #include "fes/perth/constituent.hpp"

   static auto ocean_tide_constituents() -> std::list<fes::ConstituentId> {
     return {
       fes::perth::kM2,   fes::perth::kS2,   fes::perth::kN2,
       fes::perth::kK2,   fes::perth::kK1,   fes::perth::kO1,
       fes::perth::kP1,   fes::perth::kQ1,   fes::perth::kMf,
       fes::perth::kMm,   fes::perth::kSsa,  fes::perth::kSa
       // ... additional constituents as needed
     };
   }

Step 4: Data Loading Functions
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Create utility functions for loading NetCDF data:

.. literalinclude:: ../../examples/prediction.cpp
   :language: c++
   :lines: 84-109

Step 5: Load Cartesian Model
~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Load the radial load tide model from individual NetCDF files:

.. literalinclude:: ../../examples/prediction.cpp
   :language: c++
   :lines: 111-195

Key points for Cartesian model loading:

* Each tidal constituent is stored in a separate NetCDF file
* The grid coordinates must be consistent across all constituents
* Amplitude and phase data are combined into complex waves
* The constituent enumeration type must match your chosen engine

Step 6: Load LGP2 Model
~~~~~~~~~~~~~~~~~~~~~~~

Load the ocean tide model from the LGP2 format file:

.. literalinclude:: ../../examples/prediction.cpp
   :language: c++
   :lines: 197-276

Key points for LGP2 model loading:

* All constituents are stored in a single NetCDF file
* Uses unstructured triangular mesh with Lagrange P2 elements
* Requires mesh topology (triangles and LGP2 codes)
* Supports spatial indexing for efficient interpolation

Step 7: Model Initialization and Engine Selection
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Create model instances and configure settings based on your chosen engine:

**For FES/Darwin Engine:**

.. literalinclude:: ../../examples/prediction.cpp
   :language: c++
   :lines: 279-286

**For PERTH5/Doodson Engine:**

.. code-block:: c++

   // Load models using perth namespace constituents
   auto radial_handler = load_load_tide_model();  // Using fes::ConstituentId
   auto tide_handler = load_ocean_tide_model();   // Using fes::ConstituentId

   // Create PERTH5 runtime settings
   auto settings = fes::PerthSettings()
       .with_num_threads(0)
       .with_group_modulations(true)
       .with_inference_type(fes::InferenceType::kLinear);

Step 8: Prepare Input Data
~~~~~~~~~~~~~~~~~~~~~~~~~~

Set up your prediction parameters:

.. literalinclude:: ../../examples/prediction.cpp
   :language: c++
   :lines: 287-299

Input requirements:

* **Times**: Unix timestamps (seconds since 1970-01-01)
* **Coordinates**: Longitude and latitude in degrees
* **All vectors must have the same size**

Step 9: Perform Tide Evaluation
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Call the main prediction function. The API is identical for both engines:

.. literalinclude:: ../../examples/prediction.cpp
   :language: c++
   :lines: 306-312

The ``fes::evaluate_tide()`` function returns a tuple containing:

1. **Short-period tide**: High-frequency tidal signal
2. **Long-period tide**: Low-frequency tidal signal
3. **Interpolation quality**: Quality flags for each point

Step 10: Process and Display Results
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Format and output the prediction results:

.. literalinclude:: ../../examples/prediction.cpp
   :language: c++
   :lines: 314-353

The results include:

* **Short_tide**: Short-period tidal elevation
* **LP_tide**: Long-period tidal elevation
* **Pure_Tide**: Combined ocean tide (Short + LP)
* **Geo_Tide**: Total geodetic tide (Pure + Load)
* **Rad_Tide**: Radial loading effect

Complete Example (FES/Darwin Engine)
------------------------------------

Here's the complete working example using the FES/Darwin engine:

.. literalinclude:: ../../examples/prediction.cpp
   :language: c++

PERTH5/Doodson Engine Example
-----------------------------

Here's an example skeleton for using the PERTH5/Doodson engine with GOT models:

.. code-block:: c++

   #include <chrono>
   #include <iostream>
   #include <map>
   #include <vector>

   #include "fes/settings.hpp"
   #include "fes/tidal_model/cartesian.hpp"
   #include "fes/tide.hpp"
   #include "fes/perth/constituent.hpp"

   // Define GOT model paths
   static std::map<fes::ConstituentId, std::string> got_tide_paths() {
     return {
       {fes::perth::kM2, "/path/to/got5.6/m2.nc"},
       {fes::perth::kS2, "/path/to/got5.6/s2.nc"},
       {fes::perth::kN2, "/path/to/got5.6/n2.nc"},
       {fes::perth::kK1, "/path/to/got5.6/k1.nc"},
       {fes::perth::kO1, "/path/to/got5.6/o1.nc"},
       // ... additional constituents
     };
   }

   int main() {
     // Load GOT model (similar to FES loading, using fes::ConstituentId)
     auto tide_handler = load_got_model();  // Implementation similar to FES

     // Create PERTH5 settings
     auto settings = fes::PerthSettings()
         .with_num_threads(0)
         .with_group_modulations(true)
         .with_inference_type(fes::InferenceType::kLinear);

     // Prepare time series
     Eigen::VectorXd times(24);
     // ... populate times

     auto lon = Eigen::VectorXd::Constant(times.size(), -7.688);
     auto lat = Eigen::VectorXd::Constant(times.size(), 59.195);

     // Evaluate tide - same API as FES engine
     auto [tide, lp, quality] = fes::evaluate_tide(
         tide_handler.get(), times, lon, lat, settings);

     // Process results...
     return 0;
   }

Expected Output
---------------

When you run the FES example, you should see output similar to:

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

API Compatibility
-----------------

Despite different internal formulations, both engines share the same high-level
API. The only differences are:

1. **Constituent helpers**: ``fes::darwin::constituents`` vs ``fes::perth::constituents``
2. **Settings class**: ``FESSettings`` vs ``PerthSettings`` (both inherit ``Settings``)

Both use the unified ``fes::ConstituentId`` enum. This means you can switch
engines with minimal code changes:

.. code-block:: c++

   // Engine-specific: settings only
   #ifdef USE_FES_ENGINE
   auto settings = fes::FESSettings()
       .with_inference_type(fes::InferenceType::kSpline);
   #else
   auto settings = fes::PerthSettings()
       .with_group_modulations(true)
       .with_inference_type(fes::InferenceType::kLinear);
   #endif

   // Engine-agnostic: tide evaluation
   auto [tide, lp, quality] = fes::evaluate_tide(
       model.get(), times, lon, lat, settings);

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
* Update the data file paths in the source code to match your tidal atlas location

Integration with CMake
-----------------------

For production use, integrate the library with CMake:

.. code-block:: cmake

   # CMakeLists.txt
   cmake_minimum_required(VERSION 3.30)
   project(TidePrediction LANGUAGES CXX)

   set(CMAKE_CXX_STANDARD 20)
   set(CMAKE_CXX_STANDARD_REQUIRED ON)

   cmake_policy(SET CMP0167 NEW)

   # Find required packages
   find_package(Eigen3 REQUIRED)

   # Find Boost (used by FES library)
   find_package(Boost 1.79 REQUIRED)

   # find_package(BLAS)
   find_package(BLAS)

   # Find netcdf-cxx4 (handles conda installation)
   find_package(PkgConfig)
   if(PkgConfig_FOUND)
      pkg_check_modules(NETCDF_CXX netcdf-cxx4)
   endif()

   # If pkg-config didn't find it, try manual search (for conda environments)
   if(NOT NETCDF_CXX_FOUND)
      if(DEFINED ENV{CONDA_PREFIX})
         set(NETCDF_CXX_INCLUDE_DIRS "$ENV{CONDA_PREFIX}/include")
         find_library(NETCDF_CXX_LIBRARY
               NAMES netcdf-cxx4 netcdf_c++4
               HINTS "$ENV{CONDA_PREFIX}/lib"
               REQUIRED
         )
         set(NETCDF_CXX_LIBRARIES ${NETCDF_CXX_LIBRARY})
         message(STATUS "Found netcdf-cxx4 in conda environment: ${NETCDF_CXX_LIBRARY}")
      else()
         message(FATAL_ERROR "netcdf-cxx4 not found. Please install it or set CONDA_PREFIX.")
      endif()
   endif()

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
      Boost::boost
   )
   if(BLAS_FOUND)
      target_link_libraries(tide_prediction ${BLAS_LIBRARIES})
   endif()

   target_include_directories(tide_prediction PRIVATE
      ${FES_INCLUDE_DIR}
      ${NETCDF_CXX_INCLUDE_DIRS}
   )

Summary
-------

Key takeaways for using the C++ API:

1. **Two prediction engines** with different notations:

   * FES/Darwin: Traditional oceanographic conventions (142 constituents)
   * PERTH5/Doodson: Modern Doodson classification (123 constituents)

2. **Engine choice depends on tidal atlas format**:

   * FES atlases → FES/Darwin engine with ``fes::darwin`` namespace
   * GOT atlases → PERTH5/Doodson engine with ``fes::perth`` namespace

3. **Both engines share the same high-level API**:

   * Switch engines by changing constituent namespace and settings class
   * No changes needed in the tide evaluation code

4. **Generic inference types** (``fes::InferenceType``):

   * ``kSpline`` (recommended for FES), ``kLinear`` (recommended for GOT)
   * ``kZero`` (no inference), ``kFourier`` (Munk-Cartwright)
   * All modes available on any engine

5. **Runtime settings provide fine-tuning**:

   * ``fes::FESSettings`` for Darwin engine (defaults: ``kSpline``, ``kSchuremanOrder1``)
   * ``fes::PerthSettings`` for PERTH5 engine (defaults: ``kLinear``, ``kIERS``)
