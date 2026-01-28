.. currentmodule:: pyfes.core.perth

.. _perth_engine_reference:

Perth5 Engine
=============

The PERTH5 engine implements the Doodson-number-based harmonic prediction
approach developed by Dr. Richard Ray at NASA Goddard Space Flight Center. It
supports group modulations and multiple inference interpolation modes for
enhanced tidal analysis capabilities.

Overview
--------

The PERTH5 (Program for the Estimation and Reconstitution of Tides, version 5)
engine uses the Doodson number system to classify and compute tidal
constituents. This approach provides a systematic framework for handling all
periodic terms in the tide-generating potential.

This engine is specifically designed for use with GOT (Goddard Ocean Tide)
series tidal models and other atlases following Doodson notation conventions.

Doodson Harmonic Notation
--------------------------

The Doodson notation system, developed by Arthur Thomas Doodson in 1921,
provides a systematic numerical classification of tidal constituents. Each
constituent is assigned a unique identifier encoding the coefficients of
fundamental astronomical arguments.

**Doodson Numbers:**

Each constituent is represented by a six-digit number (plus optional sign)
format: :math:`D_1D_2D_3.D_4D_5D_6`

The Doodson number encodes the astronomical argument:

.. math::

    \theta = D_1\tau + D_2s + D_3h + D_4p + D_5N' + D_6p_1

where each :math:`D_i` is obtained by adding 5 to the actual coefficient
(allowing representation of negative coefficients), and:

* :math:`\tau`: Greenwich hour angle
* **s**: Mean longitude of the moon
* **h**: Mean longitude of the sun
* **p**: Longitude of the moon's perigee
* **N'**: Negative longitude of the moon's ascending node
* **p₁**: Longitude of the sun's perihelion

**Examples:**

.. list-table:: Doodson Number Examples
   :header-rows: 1
   :widths: 20 20 60

   * - Constituent
     - Doodson Number
     - Description
   * - M₂
     - 255.555
     - Principal lunar semidiurnal
   * - S₂
     - 273.555
     - Principal solar semidiurnal
   * - K₁
     - 165.555
     - Lunisolar diurnal
   * - O₁
     - 145.555
     - Lunar diurnal
   * - N₂
     - 245.655
     - Larger lunar elliptic semidiurnal
   * - K₂
     - 275.555
     - Lunisolar semidiurnal

This systematic encoding allows for complete classification of all tidal
constituents and facilitates computational organization.

Group Modulations
-----------------

Instead of applying individual nodal corrections to each constituent, the
PERTH5 engine can use **group modulations**. Related constituents are grouped
together and modulated as a set.

**Advantages of Group Modulations:**

* **Computational efficiency**: Fewer correction calculations needed
* **Consistent treatment**: Related constituents handled together
* **Reduced complexity**: Simplified nodal correction framework

Group modulations are particularly beneficial when working with large numbers
of constituents. Enable them via:

.. code-block:: python

    settings = pyfes.PerthRuntimeSettings().with_group_modulations(True)

.. note::

    Group modulations are recommended for most applications and are based on
    the theoretical framework developed by Richard Ray.

Inference Types
---------------

The PERTH5 engine offers three distinct modes for handling minor constituents
not explicitly provided in the tidal atlas:

.. list-table:: Inference Interpolation Modes
   :header-rows: 1
   :widths: 30 70

   * - Mode
     - Description
   * - ``ZERO_ADMITTANCE``
     - No inference performed; use only explicitly provided constituents. Best
       when atlas contains all needed constituents or minimal inference is
       desired.
   * - ``LINEAR_ADMITTANCE``
     - Linear interpolation between constituents (default). Provides good
       balance between accuracy and computational cost.
   * - ``FOURIER_ADMITTANCE``
     - Fourier-based admittance interpolation. Most accurate but
       computationally more expensive. Recommended for high-precision
       applications.

**Configuration Examples:**

.. code-block:: python

    # No inference - explicit constituents only
    settings = (
        pyfes.PerthRuntimeSettings()
        .with_inference_type(pyfes.InterpolationType.ZERO_ADMITTANCE)
    )

    # Linear admittance (default, recommended for most uses)
    settings = (
        pyfes.PerthRuntimeSettings()
        .with_inference_type(pyfes.InterpolationType.LINEAR_ADMITTANCE)
    )

    # Maximum accuracy with Fourier interpolation
    settings = (
        pyfes.PerthRuntimeSettings()
        .with_inference_type(pyfes.InterpolationType.FOURIER_ADMITTANCE)
    )

Choose the inference type based on your accuracy requirements and available
computational resources.

Supported Constituents
----------------------

The PERTH5 engine supports **123 tidal constituents**, including:

* **Long-period tides**: Sa, Ssa, Mm, Mf, Node, etc.
* **Diurnal tides**: K₁, O₁, P₁, Q₁, etc.
* **Semidiurnal tides**: M₂, S₂, N₂, K₂, etc.
* **Terdiurnal and compound tides**: Various higher harmonics

The constituent set includes the "Node" constituent, which is unique to the
PERTH5 formulation.

For the complete list with Doodson numbers and speeds, see :doc:`constituents`.

Compatible Tidal Models
-----------------------

The PERTH5 engine is designed for use with GOT-series and other Doodson-based
tidal models:

* **GOT4.10**: Earlier GOT model
* **GOT5.5**: Improved accuracy GOT model
* **GOT5.6**: Latest GOT model with enhanced resolution
* Other models following Doodson notation conventions

These models are developed at NASA Goddard Space Flight Center and are widely
used in satellite altimetry and oceanographic applications.

Configuration
-------------

To use the PERTH5 engine, set ``engine: perth5`` in your YAML configuration
file:

.. code-block:: yaml

    tide:
      cartesian:
        engine: perth5
        paths:
          M2: ${GOT_DATA}/got5.6_m2.nc
          S2: ${GOT_DATA}/got5.6_s2.nc
          K1: ${GOT_DATA}/got5.6_k1.nc
          O1: ${GOT_DATA}/got5.6_o1.nc
          N2: ${GOT_DATA}/got5.6_n2.nc
          # ... additional constituents

    radial:
      cartesian:
        engine: perth5
        paths:
          M2: ${GOT_DATA}/got5.6_load_m2.nc
          S2: ${GOT_DATA}/got5.6_load_s2.nc
          # ... additional constituents

Runtime settings for the PERTH5 engine:

.. code-block:: python

    import pyfes

    settings = (
        pyfes.PerthRuntimeSettings()
        .with_group_modulations(True)
        .with_inference_type(pyfes.InterpolationType.LINEAR_ADMITTANCE)
        .with_astronomic_formulae(pyfes.Formulae.IERS)
        .with_time_tolerance(3600.0)
        .with_num_threads(0)
    )

Astronomic Formulae
-------------------

The PERTH5 engine defaults to **IERS** (International Earth Rotation and
Reference Systems Service) conventions for computing astronomical angles. This
provides consistency with modern ephemerides and reference systems used in
satellite geodesy and oceanography.

Alternative formulae options:

* **IERS** (default): Modern IERS conventions
* **SCHUREMAN_ORDER_1**: Schureman first-order (for historical comparison)
* **SCHUREMAN_ORDER_3**: Schureman third-order (higher accuracy)

The IERS formulation is recommended for most PERTH5 applications.

Developer Background
--------------------

The PERTH5 approach and the GOT tide models were developed by **Dr. Richard D.
Ray** at NASA Goddard Space Flight Center. Dr. Ray's work on tidal modeling
has been fundamental to satellite altimetry missions including TOPEX/Poseidon,
Jason series, and SWOT.

The Doodson-based framework provides advantages for systematic constituent
classification and is particularly well-suited for satellite altimetry
applications.

References
----------

Key publications on the PERTH5 approach and GOT models:

* **Doodson, A. T. (1921)**. The Harmonic Development of the Tide-Generating
  Potential. *Proceedings of the Royal Society of London A*, 100(704),
  305-329.

* **Ray, R. D. (1999)**. A Global Ocean Tide Model From TOPEX/POSEIDON
  Altimetry: GOT99.2. NASA Technical Memorandum 209478.

* **Ray, R. D. (2013)**. Precise comparisons of bottom-pressure and
  altimetric ocean tides. *Journal of Geophysical Research: Oceans*, 118,
  4570-4584.

See Also
--------

* :ref:`prediction_engines` - Comparison with FES/Darwin engine
* :doc:`constituents` - Complete list of 123 supported constituents
* :ref:`runtime_settings` - Detailed runtime configuration options
* :ref:`choosing_engine` - Guide to selecting the appropriate engine

.. toctree::
   :hidden:

   constituents
