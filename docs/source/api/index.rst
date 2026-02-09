.. _api_reference:

=============
API Reference
=============

This section documents the complete public API of the ``pyfes`` package. The
library is organized around a small set of high-level prediction functions that
operate on tidal models and settings objects. Everything else - wave tables,
astronomical angles, constituent enumerations - supports or configures those
core functions.

A typical workflow involves three steps:

1. **Load a tidal model** (:doc:`tidal_models`) from NetCDF atlas files using
   a :doc:`configuration <configuration>` file.
2. **Choose an engine** by instantiating the appropriate :doc:`settings`
   object (``FESSettings`` for FES atlases, ``PerthSettings`` for GOT atlases).
3. **Call a prediction function** (:doc:`prediction`) with arrays of epochs,
   longitudes, and latitudes.

The remaining modules let you inspect and manipulate the underlying tidal
machinery: :doc:`enumerations` define the engine types, formulae, and inference
methods; :doc:`astronomic_angle` exposes the lunar and solar arguments;
:doc:`wave_system` gives access to the constituent catalogue and nodal
corrections; and the :doc:`engine_modules` document the Darwin- and
Doodson-specific internals.

.. toctree::
   :maxdepth: 1
   :caption: API Sections:
   :hidden:

   prediction
   settings
   enumerations
   astronomic_angle
   wave_system
   tidal_models
   engine_modules
   configuration
   utilities
   exceptions
