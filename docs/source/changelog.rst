.. _changelog:

Changelog
#########

2025.8.0
========

.. important::

   **Git History Rewritten**: The git history has been rewritten to remove git 
   LFS files. The test dataset is now hosted on osf.io and downloaded 
   automatically during testing. **Existing clones of the repository must be 
   deleted and cloned again** to avoid conflicts and ensure proper operation.

New Features
------------
* **Improved LGP extrapolation algorithm**: Previously, extrapolation for points
  outside the LGP mesh only used the single closest vertex. The new algorithm
  now considers multiple vertices within the maximum allowed distance, applying
  inverse distance weighting based on all nearby vertices. This provides
  smoother transitions in extrapolated regions and more accurate results near
  mesh boundaries. The quality flag reflects this change by returning a negative
  value equal to the number of vertices used in the extrapolation (limited to
  -127).
* **Enhanced quality flag reporting**: The quality flag now provides more
  detailed information for extrapolated values. A negative flag (e.g., -3)
  indicates that the value was extrapolated using that number of vertices (e.g.,
  3). This allows for a more precise assessment of data quality in extrapolated
  regions, particularly for LGP meshes. See the Breaking Changes section for
  details on the updated quality flag semantics.
* **Updated documentation**: The API documentation has been updated to reflect
  the new quality flag semantics and the improved LGP extrapolation algorithm.

Breaking Changes
----------------

* **Removed the Quality enum**: The ``pyfes.Quality`` enum has been removed
  from the public API. Quality flags are now returned as integer values instead
  of enum constants.

  * ``pyfes.Quality.kUndefined`` (0) → ``0``
  * ``pyfes.Quality.kExtrapolated1`` (1) → ``-1`` (negative values indicate
    extrapolation)
  * ``pyfes.Quality.kExtrapolated2`` (2) → ``-2``
  * ``pyfes.Quality.kExtrapolated3`` (3) → ``-3``
  * ``pyfes.Quality.kInterpolated`` (4) → positive values (indicate
    interpolation using N data points)

* **Updated quality flag semantics**: Quality flags returned by interpolation
  functions now use a more intuitive system:

  * ``0``: Value is undefined (no data available)
  * **Positive values**: Value is interpolated using :math:`N` data points
    (where :math:`N` equals the quality flag value)
  * **Negative values**: Value is extrapolated using :math:`\lvert N\rvert`
    data points (where :math:`\lvert N\rvert` equals the absolute value)

* **API signature changes**: Functions that previously returned ``Quality``
  enum values now return ``int8_t`` or ``VectorInt8`` types. This affects:

  * ``evaluate_tide()`` return type changed from ``VectorQuality`` to
    ``VectorInt8``
  * ``AbstractTidalModel.interpolate()`` methods now return ``int`` instead of
    ``Quality``

* **Removed documentation**: The ``pyfes.core.Quality`` class documentation has
  been removed from the API reference.

Migration Guide
~~~~~~~~~~~~~~~

**For quality flag checking in user code:**

.. code-block:: python

   # Old code
   if quality == pyfes.Quality.kUndefined:
       # handle undefined
   elif quality == pyfes.Quality.kInterpolated:
       # handle interpolated
   elif quality in [pyfes.Quality.kExtrapolated1, pyfes.Quality.kExtrapolated2, pyfes.Quality.kExtrapolated3]:
       # handle extrapolated

   # New code
   if quality == 0:
       # handle undefined
   elif quality > 0:
       # handle interpolated (quality = number of data points used)
   elif quality < 0:
       # handle extrapolated (abs(quality) = number of data points used)

**For filtering interpolated vs extrapolated values:**

.. code-block:: python

   # Old code
   interpolated_mask = (quality == pyfes.Quality.kInterpolated)

   # New code
   interpolated_mask = (quality > 0)


2025.7.0
========

New Features
------------

* Added new tidal constituents: M1, M11, M12, M13, N2P, and L2P, with
  corresponding formulas, classes, and updates to constituent and wave handling.
* Enhanced Wave class to support initialization with Darwin parameters using a
  new Darwin builder pattern.
* Implemented IERS formula for astronomical angle calculations and added related
  tests.
* Added ``arcseconds`` function to convert angles from arcseconds to radians.

Bug Fixes
---------

* Corrected mean_c20 and mean_c30 calculations for mass conservation in long
  period equilibrium.
* Updated expected values in tide tests for improved accuracy.
* Fixed handling of NaN values in Cartesian interpolation by resetting to
  undefined.
* Updated frequency documentation to reflect correct units in ``wave.hpp`` and
  ``wave_table.py``.
* Updated copyright year in LICENSE and corrected license type in ``setup.cfg``.

Documentation
-------------

* Improved documentation for formulae, wave constructors, and frequency units
  for clarity and consistency.

Build System
------------

* Updated pre-commit hook versions for pyupgrade and clang-format.

Testing
-------

* Added tests for new wave constituents and additional assertions for IERS
  astronomical angle calculations.

2025.4.1
========

Bug Fixes
---------
* Fixed bilinear interpolation function to return NaN instead of zero when no
  valid points are found (corrected issue where
  std::numeric_limits<std::complex<double>>::quiet_NaN() was incorrectly
  returning zero).

2025.4.0
========

Documentation
-------------
* Enhanced documentation for WaveTable class in wave_table.py to clarify usage
  and constituent ordering (#45).
* Updated module references in api.rst for type hints.
* Improved documentation comments in box.hpp and triangle.hpp for clarity.
* Updated latest release date link in README to point to the correct repository.

Compatibility
-------------
* Corrected compatibility with older versions of Eigen.

Build System
------------
* Updated flake8 and clang-format versions in pre-commit configuration.

2025.3.0
========

New Features
------------
* Add the keyword argument ``bbox`` to the function ``load_config`` to specify
  the bounding box of the region to load from the tidal model files.

Documentation
-------------
* Add examples for the new ``bbox`` keyword argument in the documentation.

2025.2.0
========

Bug Fixes
---------

* Fix the calculation of the quality flag from LGP models

New Features
------------

* Add evaluate_equilibrium_long_period function to the API
* Remove excluded constituents parameter from Settings class and related
  functions
* Allow dates before January 1st, 1972 to determine the leap seconds between
  TAI/UTC. In this case, a user warning is thrown and the leap seconds are set
  to 0.

Documentation
-------------

* enhance documentation and add examples for the LGP discretization

Miscellaneous
-------------

* Refactor type hinting and improve code clarity with TYPE_CHECKING imports

2025.1.0
========
* enhance Axis class to support circular properties and add circle degrees constant
* fix formatting and update mathematical notation in constituent.rst
* update leap-seconds.txt from https://hpiers.obspm.fr/iers/bul/bulc/ntp/leap-seconds.list
* Fix typo in the docstrings
* bug fix in the definition of the MK4 coefficients: nu' assigned instead of nu''

2024.12.0
=========

Bug Fixes
---------
* Fixed incorrect mapping between waves and their indices in sparse wave tables.

2024.11.1
=========

Build System
------------
* Add custom build backend and update documentation to use build option with
  pip.

Bug Fixes
---------
* Fix incorrect wave identifier mapping in harmonic analysis for sparse tables
  of constituents (`#36 <https://github.com/CNES/aviso-fes/issues/36>`_).

2024.11.0
=========

New Features
------------
* Added GitHub issue templates for bug reports and feature requests to improve
  user support and issue tracking.

Build System
------------
* Updated the pre-commit configuration to use the latest versions of hooks:
    * pre-commit updated from v4.6.0 to v5.0.0.
    * pyupgrade updated from v3.16.0 to v3.19.0.
    * flake8 updated from 7.1.0 to 7.1.1.
    * clang-format updated from v18.1.8 to v19.1.2.
    * mypy updated from v1.10.1 to v1.13.0.
* Disabled docformatter in pre-commit hooks.
* Set CMake policy CMP0167 to NEW for improved compatibility.

Documentation
-------------
* Added detailed introduction and data sections to the documentation.
* Included bibliography and credits in the documentation.
* Added configuration classes to the API reference:
    * pyfes.config.Common
    * pyfes.config.Cartesian
    * pyfes.config.LGP
* Updated examples with additional explanations and warnings regarding the use
  of tidal models.

Miscellaneous
-------------

* Minor updates and improvements to code comments and docstrings.

2024.6.0
========

New Features
------------

* Added support for the LGP discretization in the tidal model.
* Rewrote the library in C++ to enhance performance and maintainability.
* Rewrote the Python bindings to the C++ library using pybind11.
* Rewrote the documentation to reflect the changes in the library.
* Added support for the latest tide atlas files from AVISO and the latest
  tidal model FES 2022.
