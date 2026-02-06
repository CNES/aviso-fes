.. currentmodule:: pyfes

Core API
========

The core API is the main API for the Python language. This modules is built
on the top of the C++ API and provides a more Pythonic interface.

Enumerators
-----------

These classes are used to map the C++ enumerators to Python.

.. toctree::
   :maxdepth: 1

   core/engine_type
   core/formulae
   core/inference_type
   core/tide_type

Tidal Constituents
------------------

These objects provides a Python interface to the harmonic constituents and
astronomic angles.

.. toctree::
   :maxdepth: 1

   core/astronomic_angle
   core/darwin/index
   core/perth/index

Tidal Models
------------

These objects provides a Python interface to interpolate the tidal models
at the requested spatio-temporal coordinates.

.. toctree::
   :maxdepth: 2

   core/abstract
   core/index
   core/tidal_model/cartesian
   core/tidal_model/lgp1
   core/tidal_model/lgp2

Settings
--------

This class provides a Python interface to the settings used for the tidal
computation.

.. toctree::
   :maxdepth: 1

   core/settings

Tidal Prediction
----------------

These objects provides a Python interface to predict the tidal elevations
at the requested spatio-temporal coordinates.

.. toctree::
   :maxdepth: 1

   core/tide
