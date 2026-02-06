.. _prediction_engines:

==========================================
Prediction Engines and Harmonic Notations
==========================================

PyFES supports two distinct prediction engines, each using different harmonic
constituent notation systems and calculation methodologies. This page explains
the differences between the engines and helps you choose the right one for your
application.

Overview
========

.. list-table:: Engine Comparison
   :header-rows: 1
   :widths: 30 35 35

   * - Feature
     - FES/Darwin Engine
     - PERTH5/Doodson Engine
   * - Engine code
     - ``darwin``
     - ``perth``
   * - Notation system
     - Darwin
     - Doodson
   * - Nodal corrections
     - Schureman/Darwin
     - Group modulations
   * - Constituents supported
     - 142
     - 123
   * - Primary developer
     - FES team
     - Dr. Richard Ray (NASA GSFC)
   * - Compatible models
     - FES2014, FES2022
     - GOT4.10, GOT5.5, GOT5.6
   * - Default formulae
     - ``SCHUREMAN_ORDER_1``
     - ``IERS``
   * - Admittance handling
     - Configurable inference modes
     - Configurable inference modes
   * - Recommended inference
     - ``SPLINE``
     - ``LINEAR``

Both engines implement the fundamental harmonic method for tidal prediction but
differ in their mathematical formulation, constituent representation, and
approach to nodal corrections.

.. _darwin_notation:

Darwin Harmonic Notation
=========================

The Darwin notation system, used by the FES/Darwin engine, represents tidal
constituents through their astronomical arguments expressed as combinations of
fundamental astronomical variables. This classical approach was developed by
Sir George Darwin and refined by Paul Schureman in his seminal 1940 manual
*Manual of Harmonic Analysis and Prediction of Tides*.

Fundamental Astronomical Arguments
-----------------------------------

Each tidal constituent is defined by its speed and astronomical argument
composed of five fundamental variables:

* **s**: Mean longitude of the moon
* **h**: Mean longitude of the sun
* **p**: Longitude of the moon's perigee
* **N**: Negative longitude of the moon's ascending node
* **p₁**: Longitude of the sun's perihelion

For example, the principal lunar semidiurnal tide M₂ has an astronomical
argument:

.. math::

    V(M_2) = 2(\tau + h - s + p)

where :math:`\tau` is the Greenwich hour angle.

Nodal Corrections
-----------------

The Darwin engine applies **Schureman's nodal modulation** to account for the
18.6-year lunar nodal cycle. Each constituent's amplitude and phase are
corrected using:

* **f**: Nodal modulation factor (amplitude correction)
* **u**: Nodal phase correction

These corrections are computed individually for each constituent based on
established oceanographic conventions documented in Schureman (1940).

The predicted tidal height for a constituent is:

.. math::

    h(t) = f \cdot H \cos(V(t) + \kappa - u)

where :math:`H` is the constituent amplitude and :math:`\kappa` is the phase
lag from the tidal atlas.

Admittance and Inference
-------------------------

Minor constituents not explicitly provided in the tidal atlas are inferred
using admittance relationships. The admittance is the complex ratio between a
minor and major constituent, allowing PyFES to estimate amplitudes and phases
for constituents not in the atlas.

.. note::

   Inference types are now **generic** and can be used with any engine. To
   reproduce the results of a given tidal model, the appropriate inference
   type should be selected: ``SPLINE`` for FES atlases (FES2014, FES2022)
   and ``LINEAR`` for GOT atlases (GOT4.10, GOT5.5, GOT5.6).

References
----------

* Schureman, P. (1940). *Manual of Harmonic Analysis and Prediction of Tides*.
  U.S. Coast and Geodetic Survey, Special Publication No. 98.
* Darwin, G. H. (1907). *The Harmonic Analysis of Tidal Observations*.
  Scientific Papers, Vol. 1.

.. _doodson_notation:

Doodson Harmonic Notation
==========================

The Doodson notation system, used by the PERTH5 engine, provides a systematic
numerical classification of tidal constituents based on the tide-generating
potential. Developed by Arthur Thomas Doodson in 1921, this system assigns
each constituent a unique six-digit number encoding the coefficients of
fundamental astronomical arguments.

Doodson Numbers
---------------

Each constituent is identified by a six-digit number (plus optional sign)
representing coefficients in the astronomical argument. For example:

* **M₂**: 255.555 (principal lunar semidiurnal)
* **S₂**: 273.555 (principal solar semidiurnal)
* **K₁**: 165.555 (lunisolar diurnal)
* **O₁**: 145.555 (lunar diurnal)

The Doodson number :math:`D_1D_2D_3.D_4D_5D_6` encodes the argument:

.. math::

    \theta = D_1\tau + D_2s + D_3h + D_4p + D_5N' + D_6p_1

where each :math:`D_i` is obtained by adding 5 to the actual coefficient, and
:math:`N' = -N` is the negative of the ascending node longitude.

This encoding provides a systematic way to classify all possible periodic terms
in the tide-generating potential.

Group Modulations
-----------------

Instead of applying individual nodal corrections to each constituent, the
PERTH5 engine can use **group modulations**. Related constituents are grouped
together and modulated as a set, providing:

* Computational efficiency
* Consistent treatment of related constituents
* Reduced complexity in nodal correction calculations

Group modulations are enabled via:

.. code-block:: python

    settings = pyfes.PerthRuntimeSettings().with_group_modulations(True)

.. _inference_types:

Inference Types
---------------

PyFES offers four inference modes for handling constituents not explicitly
provided in the tidal atlas. These modes are **generic** and can be used with
any engine, although the recommended default depends on the tidal atlas:

.. list-table:: Inference Types
   :header-rows: 1
   :widths: 30 70

   * - Type
     - Description
   * - ``ZERO``
     - No inference; use only explicitly provided constituents
   * - ``LINEAR``
     - Linear interpolation between constituents (recommended for GOT)
   * - ``SPLINE``
     - Spline-based admittance interpolation (recommended for FES)
   * - ``FOURIER``
     - Fourier-based admittance interpolation

Select the inference type based on your tidal atlas and accuracy requirements:

.. code-block:: python

    # Recommended for FES atlases (spline interpolation)
    settings = (
        pyfes.FesRuntimeSettings()
        .with_inference_type(pyfes.SPLINE)
    )

    # Recommended for GOT atlases (linear interpolation)
    settings = (
        pyfes.PerthRuntimeSettings()
        .with_inference_type(pyfes.LINEAR)
    )

    # No inference, explicit constituents only
    settings = (
        pyfes.FesRuntimeSettings()
        .with_inference_type(pyfes.ZERO)
    )

IERS Astronomical Formulae
---------------------------

The PERTH5 engine defaults to IERS (International Earth Rotation and Reference
Systems Service) conventions for computing astronomical angles, providing
consistency with modern ephemerides and reference systems.

Developer Background
--------------------

The PERTH5 approach was developed by Dr. Richard Ray at NASA Goddard Space
Flight Center. It forms the theoretical basis for the GOT (Goddard Ocean Tide)
series of global tide models, which are widely used in satellite altimetry and
oceanographic research.

References
----------

* Doodson, A. T. (1921). The Harmonic Development of the Tide-Generating
  Potential. *Proceedings of the Royal Society of London A*, 100(704),
  305-329.
* Ray, R. D. (1999). A Global Ocean Tide Model From TOPEX/POSEIDON Altimetry:
  GOT99.2. NASA Technical Memorandum 209478.

.. _mathematical_comparison:

Mathematical Comparison
=======================

While both engines implement the fundamental harmonic method, they differ in
their mathematical formulation:

Astronomical Arguments
----------------------

**Darwin Engine**:

.. math::

    V(t) = \text{linear combination of } (\tau, s, h, p, N, p_1)

Computed using Schureman's expressions for each constituent.

**PERTH5 Engine**:

.. math::

    \theta(t) = \sum_{i=1}^{6} c_i \cdot \text{arg}_i(t)

where :math:`c_i` are the Doodson number coefficients.

Nodal Corrections
-----------------

**Darwin Engine**:

Individual nodal factors :math:`f` and phase corrections :math:`u` for each
constituent:

.. math::

    h_{\text{constituent}}(t) = f \cdot H \cos(V(t) + \kappa - u)

**PERTH5 Engine**:

Group-based modulations applied to related constituents:

.. math::

    h_{\text{group}}(t) = \sum_{\text{constituents in group}} H_i \cos(\theta_i(t) + \kappa_i) \cdot f_{\text{group}}

Admittance Calculation
----------------------

**Darwin Engine**:

Traditional admittance relationships based on oceanographic conventions:

.. math::

    H_{\text{minor}} = |Z| \cdot H_{\text{major}}, \quad
    \kappa_{\text{minor}} = \kappa_{\text{major}} + \arg(Z)

**PERTH5 Engine**:

Configurable inference with four modes (zero, linear, spline, Fourier),
allowing fine-tuned control over minor constituent estimation.

.. note::

   Although the admittance calculation approaches differ historically between
   the engines, the inference types are now **generic** and can be configured
   on either engine via ``with_inference_type()``.

.. _choosing_engine:

Choosing the Right Engine
==========================

The choice of prediction engine depends primarily on your tidal atlas format
and your application requirements.

Decision Guide
--------------

.. list-table::
   :header-rows: 1
   :widths: 50 50

   * - Use FES/Darwin Engine
     - Use PERTH5/Doodson Engine
   * - ✓ Tidal atlas is FES2014 or FES2022
     - ✓ Tidal atlas is GOT4.10, GOT5.5, GOT5.6
   * - ✓ Following traditional oceanographic conventions
     - ✓ Working with Richard Ray's tide models
   * - ✓ Replicating established FES predictions
     - ✓ Need group modulation capabilities
   * - ✓ Application requires Darwin notation
     - ✓ Require fine control over admittance
   * - ✓ Using Schureman reference formulations
     - ✓ Application uses Doodson classification

Compatibility Requirements
--------------------------

.. warning::

   **Both sections of your configuration file must use the same engine.**
   Mixing engines (e.g., ``darwin`` for tide and ``perth`` for radial) is not
   supported and will result in an error.

The engine must match your tidal atlas format:

* **FES atlases** (FES2014, FES2022) → Use ``engine: darwin``
* **GOT atlases** (GOT4.10, GOT5.5, GOT5.6) → Use ``engine: perth``

Configuration Examples
----------------------

**FES2022 with Darwin Engine**:

.. code-block:: yaml

    # FES2022 Configuration
    engine: darwin
    tide:
      lgp:
        path: ${FES_DATA}/fes2022b/ocean_tide.nc
        codes: codes
        constituents: [2N2, K1, K2, M2, N2, O1, P1, Q1, S1, S2]

    radial:
      cartesian:
        paths:
          M2: ${FES_DATA}/fes2022b_load/m2.nc
          S2: ${FES_DATA}/fes2022b_load/s2.nc
          # ... other constituents

**GOT5.6 with PERTH5 Engine**:

.. code-block:: yaml

    # GOT5.6 Configuration
    engine: perth
    tide:
      cartesian:
        paths:
          M2: ${GOT_DATA}/got5.6_m2.nc
          S2: ${GOT_DATA}/got5.6_s2.nc
          K1: ${GOT_DATA}/got5.6_k1.nc
          O1: ${GOT_DATA}/got5.6_o1.nc
          # ... other constituents

    radial:
      cartesian:
        paths:
          M2: ${GOT_DATA}/got5.6_load_m2.nc
          # ... other constituents

Runtime Settings
----------------

Each engine has its own runtime settings class. Both share common options
(inference type, thread count, time tolerance) defined in
:class:`pyfes.Settings`, with engine-specific defaults.

**FES/Darwin Settings**:

.. code-block:: python

    import pyfes

    settings = (
        pyfes.FesRuntimeSettings()
        .with_inference_type(pyfes.SPLINE)
        .with_astronomic_formulae(pyfes.Formulae.SCHUREMAN_ORDER_1)
        .with_time_tolerance(3600.0)
        .with_num_threads(0)
    )

**PERTH5/Doodson Settings**:

.. code-block:: python

    import pyfes

    settings = (
        pyfes.PerthRuntimeSettings()
        .with_group_modulations(True)
        .with_inference_type(pyfes.LINEAR)
        .with_astronomic_formulae(pyfes.Formulae.IERS)
        .with_time_tolerance(3600.0)
        .with_num_threads(0)
    )

See :ref:`runtime_settings` for detailed documentation on all available
options.

.. _constituent_support:

Constituent Support
===================

The two engines support different sets of tidal constituents due to their
different formulations.

Constituent Counts
------------------

* **FES/Darwin Engine**: 142 constituents
* **PERTH5/Doodson Engine**: 123 constituents

Many major constituents are common to both engines, but each has unique
constituents specific to its formulation.

Listing Constituents
--------------------

You can query which constituents are supported by each engine:

.. code-block:: python

    import pyfes

    # List Darwin engine constituents
    darwin_wt = pyfes.darwin.WaveTable()
    print(f"Darwin: {len(darwin_wt)} constituents")

    # List PERTH5 engine constituents
    perth_wt = pyfes.perth.WaveTable()
    print(f"PERTH5: {len(perth_wt)} constituents")

    # Find common and unique constituents
    darwin_names = {w.name for w in darwin_wt}
    perth_names = {w.name for w in perth_wt}
    common = darwin_names & perth_names
    darwin_only = darwin_names - perth_names
    perth_only = perth_names - darwin_names

    print(f"Common: {len(common)}")
    print(f"Darwin-only: {len(darwin_only)}")
    print(f"PERTH5-only: {len(perth_only)}")

For complete constituent lists with speeds and Doodson numbers, see:

* :doc:`core/darwin/constituents` - FES/Darwin constituents
* :doc:`core/perth/constituents` - PERTH5/Doodson constituents

.. _api_compatibility:

API Compatibility
=================

Despite their different internal formulations, both engines share the same
high-level API. This means you can switch between engines by simply changing
your configuration file—no code changes are required in your prediction
scripts.

Common API Functions
--------------------

These functions work identically with both engines:

.. code-block:: python

    import pyfes
    import numpy as np

    # Load configuration (works with both engines)
    config = pyfes.config.load('my_config.yaml')

    # Define prediction coordinates and times
    dates = np.arange('2025-01-01', '2025-01-02', dtype='datetime64[h]')
    lons = np.array([-7.688])
    lats = np.array([59.195])

    # Evaluate tide (works with both engines)
    tide, lp, flags = pyfes.evaluate_tide(
        config.models['tide'], dates, lons, lats, settings=config.settings
    )

    # The only difference is in the configuration file!

Engine-Specific Features
-------------------------

While the core API is identical, each engine has specific recommended defaults:

* **FES/Darwin**: Schureman nodal corrections, ``SPLINE`` inference recommended
* **PERTH5**: Group modulations, ``LINEAR`` inference recommended

Both engines support the same set of inference types (``ZERO``, ``LINEAR``,
``SPLINE``, ``FOURIER``) and share the common settings defined in
:class:`pyfes.Settings`. Engine-specific subclasses
(:class:`pyfes.FesRuntimeSettings` and :class:`pyfes.PerthRuntimeSettings`)
provide the appropriate defaults.

Performance Considerations
==========================

Both engines are highly optimized for production use, but they have different
performance characteristics.

Computational Efficiency
------------------------

* **FES/Darwin**: Optimized for FES atlases with traditional admittance
* **PERTH5**: Group modulations can provide efficiency gains with many constituents

Memory Usage
------------

Memory usage depends primarily on the tidal atlas size and grid resolution,
not on the prediction engine. Both engines have similar memory footprints for
comparable atlases.

Accuracy
--------

Both engines provide comparable accuracy when used with their respective
atlases. The choice of engine should be driven by atlas compatibility, not
accuracy concerns.

Further Reading
===============

* :ref:`darwin_engine_reference` - Detailed Darwin engine documentation
* :ref:`perth_engine_reference` - Detailed PERTH5 engine documentation
* :ref:`runtime_settings` - Runtime settings configuration
* :doc:`auto_examples/ex_engine_comparison` - Complete comparison example
* :doc:`glossary` - Terminology definitions

.. seealso::

   For practical examples comparing the two engines, see:

   * :doc:`auto_examples/ex_prediction` - Basic prediction example
   * :doc:`auto_examples/ex_engine_comparison` - Engine comparison example
   * :doc:`auto_examples/ex_constituents_prediction` - Constituent-based prediction
