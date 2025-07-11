.. _changelog:

Changelog
#########

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
