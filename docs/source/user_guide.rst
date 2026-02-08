.. _user_guide:

==========
User Guide
==========

This guide covers the practical aspects of using PyFES for tidal prediction:
configuration, runtime settings, prediction functions, and harmonic analysis.
For installation and a minimal example, see :doc:`getting_started`. For the
mathematical foundations, see :doc:`theory/index`.

.. _configuration_file:

Configuration
=============

PyFES loads tidal models from YAML configuration files using
:func:`pyfes.config.load`. The configuration describes the grid type, the
constituent files, and the prediction engine.

YAML File Structure
-------------------

A configuration file has the following structure:

.. code-block:: yaml

    engine: darwin          # or 'perth'
    tide:
      cartesian:
        paths:
          M2: /path/to/M2_tide.nc
          S2: /path/to/S2_tide.nc
          K1: /path/to/K1_tide.nc
          # ...
    radial:                 # optional
      cartesian:
        paths:
          M2: /path/to/M2_radial.nc
          # ...

Top-Level Keys
~~~~~~~~~~~~~~

``engine``
    Selects the prediction engine: ``darwin`` (FES/Darwin, default) or
    ``perth`` (PERTH/Doodson). See :doc:`engines` for a comparison.

``tide``
    The ocean tide elevation model.

``radial``
    The radial (loading) tide model. Optional.

Each model section contains either a ``cartesian`` or ``lgp`` block describing
the grid type.

Cartesian Grids
~~~~~~~~~~~~~~~

For regular latitude/longitude grids, each constituent is stored in a separate
NetCDF file:

.. code-block:: yaml

    tide:
      cartesian:
        latitude: lat           # variable name (default: 'lat')
        longitude: lon          # variable name (default: 'lon')
        amplitude: amplitude    # variable name (default: 'amplitude')
        phase: phase            # variable name (default: 'phase')
        paths:
          M2: /path/to/M2.nc
          S2: /path/to/S2.nc

LGP Unstructured Meshes
~~~~~~~~~~~~~~~~~~~~~~~~

For finite-element meshes (LGP1 or LGP2 discretization), all constituents are
stored in a single NetCDF file with variable name patterns:

.. code-block:: yaml

    tide:
      lgp:
        type: lgp2              # 'lgp1' or 'lgp2'
        path: /path/to/model.nc
        triangle: triangle      # triangle connectivity variable
        codes: codes            # land/sea mask variable
        amplitude: '{constituent}_amplitude'
        phase: '{constituent}_phase'
        constituents:
          - M2
          - S2
          - K1

Environment Variable Interpolation
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Paths can reference environment variables using ``${VAR}`` syntax:

.. code-block:: yaml

    paths:
      M2: ${FES_DATA}/M2_tide.nc

Dynamic Constituents
~~~~~~~~~~~~~~~~~~~~

The ``dynamic`` key lists constituents that are considered part of the atlas
but not stored as grid files. These are excluded from the long-period
equilibrium calculation and admittance inference:

.. code-block:: yaml

    tide:
      cartesian:
        dynamic:
          - A5
        paths:
          M2: /path/to/M2.nc

Loading a Configuration
-----------------------

.. code-block:: python

    import pyfes

    config = pyfes.config.load('ocean_tide.yaml')

    # Access the models and settings
    tide_model = config.models['tide']
    settings = config.settings

    # Optionally load only a regional subset
    config = pyfes.config.load('ocean_tide.yaml', bbox=(-10, 40, 10, 60))

The returned :class:`~pyfes.config.Configuration` contains:

- ``models``: dictionary mapping ``'tide'`` and optionally ``'radial'`` to
  tidal model objects.
- ``settings``: runtime settings (:class:`~pyfes.FesRuntimeSettings` or
  :class:`~pyfes.PerthRuntimeSettings`) appropriate for the selected engine.

Runtime Settings
================

All prediction parameters are configured through the :class:`~pyfes.Settings`
class using a builder pattern. The two engine-specific subclasses differ only
in their default values:

.. list-table::
   :header-rows: 1
   :widths: 40 30 30

   * - Setting
     - :class:`~pyfes.FesRuntimeSettings`
     - :class:`~pyfes.PerthRuntimeSettings`
   * - ``inference_type``
     - ``SPLINE``
     - ``LINEAR``
   * - ``astronomic_formulae``
     - ``SCHUREMAN_ORDER_1``
     - ``MEEUS``
   * - ``group_modulations``
     - ``False``
     - ``True``
   * - ``compute_long_period_equilibrium``
     - ``True``
     - ``True``

All settings can be overridden on either engine using the builder methods:

.. code-block:: python

    import pyfes

    # Start from engine defaults
    settings = pyfes.FesRuntimeSettings()

    # Customise individual settings
    settings = (
        pyfes.FesRuntimeSettings()
        .with_inference_type(pyfes.SPLINE)
        .with_astronomic_formulae(pyfes.Formulae.MEEUS)
        .with_num_threads(4)
    )

    # Or with the PERTH engine
    settings = (
        pyfes.PerthRuntimeSettings()
        .with_inference_type(pyfes.LINEAR)
        .with_group_modulations(True)
    )

.. _inference_types:

Inference Modes
===============

PyFES infers minor tidal constituents not present in the atlas from nearby
major constituents using :term:`admittance` relationships. The inference mode
is **engine-generic**: all four modes work identically with both the Darwin
and PERTH engines.

.. list-table::
   :header-rows: 1
   :widths: 15 50 35

   * - Mode
     - Description
     - Recommended for
   * - ``ZERO``
     - No inference. Only atlas constituents are used.
     - Diagnostics
   * - ``LINEAR``
     - Linear admittance interpolation between reference constituents.
     - GOT atlases (GOT4.10, GOT5.5, GOT5.6)
   * - ``SPLINE``
     - Spline interpolation through reference constituents, providing
       smoother admittance curves.
     - FES atlases (FES2014, FES2022)
   * - ``FOURIER``
     - Low-order Fourier series (Munk--Cartwright approach) fitted to
       reference admittances.
     - Research / comparison

.. code-block:: python

    settings = pyfes.FesRuntimeSettings().with_inference_type(pyfes.SPLINE)
    settings = pyfes.PerthRuntimeSettings().with_inference_type(pyfes.LINEAR)

    # Any mode can be used with any engine
    settings = pyfes.PerthRuntimeSettings().with_inference_type(pyfes.SPLINE)

For the mathematical details of each inference method, see
:doc:`theory/admittance_inference`.

.. _astronomic_formulae:

Astronomic Formulae
===================

PyFES supports four sets of polynomial expressions for computing the
:term:`fundamental variables` (the six time-varying astronomical angles that
define tidal frequencies):

.. list-table::
   :header-rows: 1
   :widths: 25 15 60

   * - Formulae
     - Epoch
     - Description
   * - ``SCHUREMAN_ORDER_1``
     - J1900.0
     - First-order Schureman polynomials. Fastest; sufficient for most
       predictions near the present epoch.
   * - ``SCHUREMAN_ORDER_3``
     - J1900.0
     - Third-order Schureman extensions. Better accuracy over longer
       time spans.
   * - ``MEEUS``
     - J2000.0
     - Jean Meeus (1998) expressions using Terrestrial Dynamical Time.
       High precision over several centuries.
   * - ``IERS``
     - J2000.0
     - IERS 2010 conventions based on Simon et al. (1994) lunisolar
       fundamental arguments. Highest precision.

.. code-block:: python

    settings = pyfes.FesRuntimeSettings().with_astronomic_formulae(
        pyfes.Formulae.MEEUS
    )

For the derivation of each formula set, see :doc:`theory/astronomic_angles`.

Tidal Model Types
=================

PyFES supports three grid discretisations, each available in double
(``Complex128``) and single (``Complex64``) precision:

* **Cartesian grids** — regular latitude/longitude grids. Each constituent is
  stored in a separate NetCDF file containing amplitude and phase arrays.
* **LGP1** — linear Galerkin--Petrov discretisation on unstructured triangular
  meshes. All constituents are stored in a single NetCDF file.
* **LGP2** — quadratic Galerkin--Petrov discretisation on unstructured triangular
  meshes, providing higher accuracy than LGP1 on the same mesh.

The grid type is determined by the YAML configuration (``cartesian`` or ``lgp``
section).

.. _prediction_functions:

Prediction Functions
====================

PyFES provides three prediction functions.

``evaluate_tide`` — From a Tidal Atlas
---------------------------------------

Predicts tides by interpolating constituent amplitudes and phases from a tidal
model at the requested locations:

.. code-block:: python

    tide, lp, flags = pyfes.evaluate_tide(
        config.models['tide'],
        dates,       # numpy datetime64 array
        longitudes,  # numpy float64 array (degrees)
        latitudes,   # numpy float64 array (degrees)
        settings=config.settings,
    )

Returns:

- ``tide``: short-period constituent heights (cm). ``NaN`` where no data is
  available.
- ``lp``: long-period equilibrium tide (cm). Always computed (does not depend
  on the model).
- ``flags``: quality indicators. Positive = number of interpolation points
  used; negative = extrapolated; zero = undefined.

``evaluate_tide_from_constituents`` — From Known Harmonics
-----------------------------------------------------------

Predicts tides from a dictionary of known constituents (e.g., from harmonic
analysis of tide gauge data):

.. code-block:: python

    tide, lp = pyfes.evaluate_tide_from_constituents(
        constituents,  # {'M2': (amplitude, phase), ...}
        dates,
        latitude=48.38,
        settings=settings,
    )

Amplitudes are in the same units as the model (typically cm); phases are in
degrees.

``evaluate_equilibrium_long_period`` — Equilibrium LPE
-------------------------------------------------------

Computes the long-period equilibrium tide from the Cartwright--Tayler--Edden
spectral tables:

.. code-block:: python

    lpe = pyfes.evaluate_equilibrium_long_period(
        dates,
        latitudes,
        constituents=['Mm', 'Mf'],  # optional: exclude these
    )

The ``constituents`` parameter lists long-period constituents already present
in your atlas that should be excluded to avoid double-counting.

Harmonic Analysis
=================

PyFES also supports the inverse problem: extracting tidal constituents from
observed sea-level time series.

Selecting Resolvable Constituents
---------------------------------

The :term:`Rayleigh criterion` determines which constituents can be resolved
from a given record length:

.. code-block:: python

    wt = pyfes.darwin.WaveTable()  # or pyfes.perth.WaveTable()

    # Select constituents resolvable from a 30-day record
    names = wt.select_waves_for_analysis(duration=30 * 24 * 3600)

Computing Nodal Modulations
----------------------------

Build the matrices of nodal amplitude factors (:math:`f`) and phase
corrections (:math:`V+u`) needed for the design matrix:

.. code-block:: python

    f, vu = wt.compute_nodal_modulations(dates)

Solving the Least-Squares Problem
-----------------------------------

The static method :meth:`~pyfes.WaveTableInterface.harmonic_analysis` solves
the observation equation using least squares:

.. code-block:: python

    # h: observed sea level [n_times, n_positions]
    # f: nodal factors [n_times, n_constituents]
    # vu: V+u phases [n_times, n_constituents]
    amplitudes = pyfes.WaveTableInterface.harmonic_analysis(h, f, vu)

The result is a vector of complex amplitudes :math:`H_k e^{i G_k}` for each
constituent. For the mathematical formulation, see
:doc:`theory/harmonic_analysis`.
