.. _changelog:

Changelog
#########

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
