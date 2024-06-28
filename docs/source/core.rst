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

   core/constituent
   core/tide_type
   core/quality

Tidal Constituents
------------------

These objects provides a Python interface to the harmonic constituents and
astronomic angles.

.. toctree::
   :maxdepth: 1


   core/astronomic_angle
   core/wave
   core/wave_table

Tidal Models
------------

These objects provides a Python interface to interpolate the tidal models
at the requested spatio-temporal coordinates.

.. toctree::
   :maxdepth: 1

   core/abstract
   core/cartesian
   core/lgp1
   core/lgp2

Tidal Prediction
----------------

These objects provides a Python interface to predict the tidal elevations
at the requested spatio-temporal coordinates.

.. toctree::
   :maxdepth: 1

   core/tide
