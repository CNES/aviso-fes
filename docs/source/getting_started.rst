.. _getting_started:

===============
Getting Started
===============

Installation
============

From conda-forge
-----------------

.. code-block:: bash

    conda install -c conda-forge pyfes

Building from Source
--------------------

PyFES requires a C++14 compiler and CMake:

.. code-block:: bash

    git clone https://github.com/CNES/aviso-fes.git
    cd aviso-fes
    pip install -e .

.. _quickstart:

Quickstart: Predicting Tides from a Model
==========================================

The most common use case is predicting ocean tides at given locations and times
using a tidal atlas (FES2022, GOT5.6, etc.).

Step 1: Create a YAML Configuration File
-----------------------------------------

The configuration file describes your tidal model and selects the prediction
engine. Here is a minimal example for a FES atlas with Cartesian grids:

.. code-block:: yaml

    engine: darwin
    tide:
      cartesian:
        paths:
          M2: /path/to/M2_tide.nc
          S2: /path/to/S2_tide.nc
          K1: /path/to/K1_tide.nc
          O1: /path/to/O1_tide.nc
          # ... additional constituents

The top-level ``engine`` key selects the prediction engine: ``darwin`` for
FES atlases or ``perth`` for GOT atlases. See :doc:`engines` for details on
each engine.

Environment variables are supported using ``${VAR}`` syntax:

.. code-block:: yaml

    engine: darwin
    tide:
      cartesian:
        paths:
          M2: ${FES_DATA}/M2_tide.nc

Step 2: Load the Model and Predict
------------------------------------

.. code-block:: python

    import numpy as np
    import pyfes

    # Load the tidal model and settings from the YAML file
    config = pyfes.config.load('ocean_tide.yaml')

    # Define the prediction coordinates and times
    dates = np.arange(
        np.datetime64('2024-01-01'),
        np.datetime64('2024-01-02'),
        np.timedelta64(1, 'h'),
    )
    lons = np.full(dates.shape, -7.688)   # longitude in degrees
    lats = np.full(dates.shape, 59.195)   # latitude in degrees

    # Compute the tide
    tide, lp, flags = pyfes.evaluate_tide(
        config.models['tide'], dates, lons, lats,
        settings=config.settings,
    )

    # tide: short-period constituent heights (cm)
    # lp: long-period equilibrium tide (cm)
    # flags: quality flags (>0: interpolated, <0: extrapolated, 0: undefined)
    total_tide = tide + lp

The :func:`~pyfes.config.load` function returns a
:class:`~pyfes.config.Configuration` named tuple containing:

- ``models``: a dictionary mapping ``'tide'`` (and optionally ``'radial'``)
  to tidal model objects.
- ``settings``: the runtime settings appropriate for the chosen engine
  (:class:`~pyfes.FESSettings` or :class:`~pyfes.PerthSettings`).

The actual prediction is performed by :func:`~pyfes.evaluate_tide`, which
evaluates the configured model at the requested times and locations.

Quickstart: Prediction from Known Constituents
===============================================

If you already know the tidal constituents at a location (e.g., from harmonic
analysis of tide gauge data), you can predict tides directly:

.. code-block:: python

    import numpy as np
    import pyfes

    # Constituents from harmonic analysis: {name: (amplitude_cm, phase_deg)}
    constituents = {
        'M2': (205.1, 109.0),
        'S2': (74.9, 148.3),
        'K1': (6.4, 75.1),
        'O1': (6.6, 327.9),
    }

    dates = np.arange(
        np.datetime64('2024-01-01'),
        np.datetime64('2024-01-02'),
        np.timedelta64(1, 'h'),
    )

    tide, lp = pyfes.evaluate_tide_from_constituents(
        constituents, dates, latitude=48.38,
    )

See :func:`~pyfes.evaluate_tide_from_constituents` for the full API.

.. _quickstart_bounding_box:

Limiting the Loaded Region
===========================

For large global atlases, you can load only a regional subset using the
``bbox`` parameter:

.. code-block:: python

    config = pyfes.config.load(
        'ocean_tide.yaml',
        bbox=(-10, 40, 10, 60),  # (min_lon, min_lat, max_lon, max_lat)
    )

Next Steps
==========

- :doc:`user_guide` — configuration, settings, inference modes, and all
  prediction functions.
- :doc:`engines` — choosing between the FES/Darwin and PERTH/Doodson engines.
- :doc:`auto_examples/index` — worked examples with plots.
- :doc:`theory/index` — mathematical foundations of the harmonic method.
