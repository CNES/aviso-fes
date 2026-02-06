.. currentmodule:: pyfes.core.darwin

.. _darwin_engine_reference:

Darwin Engine
=============

The Darwin engine implements the classical FES/Darwin harmonic prediction
approach. It uses Schureman/Darwin nodal corrections and admittance calculations
following established oceanographic conventions.

Overview
--------

The FES/Darwin engine is the historical prediction engine for FES (Finite
Element Solution) tidal atlases. It employs Darwin's notation system for tidal
constituents, where each constituent is expressed through combinations of
fundamental astronomical variables.

This engine is specifically designed for use with FES2014 and FES2022 tidal
atlases and follows the traditional oceanographic conventions established by
Darwin and Schureman.

Darwin Harmonic Notation
-------------------------

The Darwin notation system represents tidal constituents through their
astronomical arguments expressed as combinations of five fundamental
astronomical variables:

**Fundamental Astronomical Arguments:**

* **s**: Mean longitude of the moon
* **h**: Mean longitude of the sun
* **p**: Longitude of the moon's perigee
* **N**: Negative longitude of the moon's ascending node
* **p₁**: Longitude of the sun's perihelion (solar perigee)

Each constituent is defined by its angular speed (degrees per hour) and its
astronomical argument expressed as a linear combination of these variables.

**Example - M₂ Constituent:**

The principal lunar semidiurnal tide M₂ has an astronomical argument:

.. math::

    V(M_2) = 2(\tau + h - s + p)

where :math:`\tau` is the Greenwich hour angle, representing Earth's rotation.

Nodal Corrections
-----------------

The Darwin engine applies **Schureman's nodal modulation** to account for the
18.6-year lunar nodal cycle. This cycle arises from the precession of the
moon's orbital plane.

For each constituent, two correction factors are applied:

* **f (nodal factor)**: Amplitude modulation factor
* **u (nodal angle)**: Phase correction angle

These corrections vary slowly over the 18.6-year cycle and are computed using
Schureman's formulations. The predicted height for a constituent is:

.. math::

    h(t) = f \cdot H \cos(V(t) + \kappa - u)

where:

* :math:`H` is the constituent amplitude from the tidal atlas
* :math:`\kappa` is the phase lag from the tidal atlas
* :math:`V(t)` is the astronomical argument at time *t*

Admittance and Inference
-------------------------

The Darwin engine handles minor constituents not explicitly provided in the
tidal atlas through **traditional admittance relationships**. The admittance is
the complex ratio between a minor and major constituent:

.. math::

    Z = \frac{H_{\text{minor}}}{H_{\text{major}}} e^{i(\kappa_{\text{minor}} - \kappa_{\text{major}})}

This allows PyFES to estimate amplitudes and phases for many additional
constituents beyond those in the atlas, improving prediction accuracy.

.. note::

   Inference types are now **generic** and can be used with any engine. For
   FES atlases, the recommended inference type is ``SPLINE``. See
   :ref:`inference_types` for all available modes.

Supported Constituents
----------------------

The Darwin engine supports **142 tidal constituents**, including:

* **Long-period tides**: Sa, Ssa, Mm, Mf, etc.
* **Diurnal tides**: K₁, O₁, P₁, Q₁, etc.
* **Semidiurnal tides**: M₂, S₂, N₂, K₂, etc.
* **Terdiurnal tides**: M₃, MK₃, etc.
* **Compound tides**: M₄, M₆, M₈, etc.

For the complete list with speeds and astronomical arguments, see
:doc:`constituents`.

Compatible Tidal Models
-----------------------

The Darwin engine is designed for use with FES tidal atlases:

* **FES2014**: 34 constituents provided in the atlas
* **FES2022**: 34 constituents provided in the atlas

These models provide amplitude and phase for major constituents. PyFES uses
admittance to infer the remaining constituents for complete tidal predictions.

Configuration
-------------

To use the Darwin engine, set ``engine: darwin`` at the top level of your YAML
configuration file:

.. code-block:: yaml

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

Runtime settings for the Darwin engine:

.. code-block:: python

    import pyfes

    settings = (
        pyfes.FesRuntimeSettings()
        .with_inference_type(pyfes.SPLINE)
        .with_astronomic_formulae(pyfes.Formulae.SCHUREMAN_ORDER_1)
        .with_time_tolerance(3600.0)
        .with_num_threads(0)
    )

Astronomic Formulae Options
----------------------------

The Darwin engine supports multiple formulations for computing astronomical
angles:

* **SCHUREMAN_ORDER_1** (default): Schureman's first-order formulations
* **SCHUREMAN_ORDER_3**: Schureman's third-order formulations (higher accuracy)
* **IERS**: IERS conventions (for consistency with modern ephemerides)

The default ``SCHUREMAN_ORDER_1`` provides a good balance of accuracy and
compatibility with historical FES predictions.

References
----------

The Darwin engine implements formulations documented in:

* **Schureman, P. (1940)**. *Manual of Harmonic Analysis and Prediction of
  Tides*. U.S. Coast and Geodetic Survey, Special Publication No. 98.

* **Darwin, G. H. (1907)**. *The Harmonic Analysis of Tidal Observations*.
  Scientific Papers, Vol. 1.

For detailed constituent formulations following Schureman's reference, see
:doc:`reference`.

See Also
--------

* :ref:`prediction_engines` - Comparison with PERTH5/Doodson engine
* :doc:`constituents` - Complete list of 142 supported constituents
* :doc:`reference` - Schureman reference formulas for each constituent
* :ref:`runtime_settings` - Detailed runtime configuration options

.. toctree::
   :hidden:

   constituents
   reference
