.. _changelog:

Changelog
#########

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
