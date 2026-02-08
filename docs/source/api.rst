.. _api_reference:

=============
API Reference
=============

This page provides the complete API reference for the ``pyfes`` package,
auto-generated from the source code docstrings.

Prediction Functions
====================

.. autofunction:: pyfes.evaluate_tide

.. autofunction:: pyfes.evaluate_tide_from_constituents

.. autofunction:: pyfes.evaluate_equilibrium_long_period

Settings
========

.. autoclass:: pyfes.Settings
   :members:
   :undoc-members:

.. autoclass:: pyfes.FESSettings
   :members:
   :undoc-members:
   :show-inheritance:

.. autoclass:: pyfes.PerthSettings
   :members:
   :undoc-members:
   :show-inheritance:

Enumerations
============

.. class:: pyfes.EngineType

   Enum class for tidal prediction engines.

   .. attribute:: DOODSON

      Uses Darwin notation with Schureman's nodal corrections. This is the
      classical approach developed for FES tidal atlases.

   .. attribute:: DARWIN

      Uses Doodson number classification with group modulations. Developed by
      Dr. Richard Ray at NASA Goddard Space Flight Center, this engine is
      designed for GOT (Goddard Ocean Tide) models.

   .. property:: name

      Returns the name of the engine type.

   .. property:: value

      Returns the value associated with the engine type.

.. class:: pyfes.FrequencyUnit

   Enum class for frequency units.

   .. attribute:: DEGREE_PER_HOUR

      Frequency in degrees per hour (°/h).

   .. attribute:: RADIAN_PER_HOUR

      Frequency in radians per second (rad/s).

   .. property:: name

      Returns the name of the frequency unit.

   .. property:: value

      Returns the value associated with the frequency unit.

.. class:: pyfes.Formulae

   This Enum class encapsulates the various astronomical formulae required for
   computing astronomical angles.

   .. attribute:: SCHUREMAN_ORDER_1

      First-order polynomial expressions from Schureman (1940), Table 1, p.
      162. Uses the J1900.0 epoch and UTC time scale. This is the simplest and
      most traditional formula set, suitable for predictions within a few
      decades of the reference epoch.

   .. attribute:: SCHUREMAN_ORDER_3

      Third-order polynomial expressions extending Schureman's formulae with
      quadratic and cubic correction terms. Same J1900.0 epoch and UTC time
      scale, but with improved accuracy for dates further from the reference
      epoch.

   .. attribute:: MEEUS

      Expressions from Jean Meeus, *Astronomical Algorithms* (2nd ed., 1998).
      Uses the J2000.0 epoch and Terrestrial Dynamical Time (TDT), with
      fourth-order polynomials derived from modern ephemerides. PyFES
      automatically applies the UTC-to-TDT correction when this formula set is
      selected.

   .. attribute:: IERS

      Expressions from the IERS Conventions (2010), based on Simon et al.
      (1994). Uses the J2000.0 epoch and TDT, computing the five lunisolar
      fundamental arguments (:math:`l, l', F, D, \Omega`) and converting them
      to the traditional Doodson variables.

   .. property:: name

      Returns the name of the formula.

   .. property:: value

      Returns the value of the formula.

.. class:: pyfes.InferenceType

   This Enum class represents the different inference types available in PyFES
   for handling minor tidal constituents not explicitly provided in the tidal
   atlas. Each inference type corresponds to a specific method of estimating
   the contributions of these minor constituents based on the provided data
   and settings.

   .. attribute:: FOURIER

      Munk-Cartwright Fourier series interpolation of admittances.

   .. attribute:: LINEAR

      Piecewise linear interpolation of admittances, default method for the
      DOODSON engine.

   .. attribute:: SPLINE

      Spline interpolation of admittance, default method for the DARWIN
      engine.

   .. attribute:: ZERO

      No inference performed; use only explicitly provided constituents.

   .. property:: name

      Returns the name of the inference type.

   .. property:: value

      Returns the value associated with the inference type.


.. class:: pyfes.TideType

   Enum class for tide types.

   .. attribute:: TIDE

      Ocean tide

   .. attribute:: RADIAL

      Radial tide

   .. property:: name

      Returns the name of the tide type.

   .. property:: value

      Returns the value of the tide type.

.. class:: pyfes.WaveType

   Enum class for wave types.

   .. attribute:: SHORT_PERIOD

      Short-period waves (diurnal and semidiurnal species)

   .. attribute:: LONG_PERIOD

      Long-period waves (long-period species)

   .. property:: name

      Returns the name of the wave type.

   .. property:: value

      Returns the value associated with the wave type.

Astronomical Angles
===================

.. autoclass:: pyfes.AstronomicAngle
   :members:
   :undoc-members:

Wave System
===========

.. autoclass:: pyfes.WaveInterface
   :members:
   :undoc-members:

.. autoclass:: pyfes.WaveTableInterface
   :members:
   :undoc-members:

Tidal Models
============

.. autoclass:: pyfes.core.Axis
   :members:
   :undoc-members:

.. autoclass:: pyfes.core.tidal_model.CartesianComplex128
   :members:
   :undoc-members:

.. autoclass:: pyfes.core.tidal_model.CartesianComplex64
   :members:
   :undoc-members:

.. autoclass:: pyfes.core.tidal_model.LGP1Complex128
   :members:
   :undoc-members:

.. autoclass:: pyfes.core.tidal_model.LGP1Complex64
   :members:
   :undoc-members:

.. autoclass:: pyfes.core.tidal_model.LGP2Complex128
   :members:
   :undoc-members:

.. autoclass:: pyfes.core.tidal_model.LGP2Complex64
   :members:
   :undoc-members:

Engine-Specific Modules
========================

Darwin
------

.. autoclass:: pyfes.darwin.WaveTable
   :members:
   :undoc-members:
   :show-inheritance:

.. autoclass:: pyfes.darwin.Wave
   :members:
   :undoc-members:
   :show-inheritance:

PERTH/Doodson
-------------

.. autoclass:: pyfes.perth.WaveTable
   :members:
   :undoc-members:
   :show-inheritance:

.. autoclass:: pyfes.perth.Wave
   :members:
   :undoc-members:
   :show-inheritance:

Configuration
=============

.. autofunction:: pyfes.config.load

.. autoclass:: pyfes.config.Configuration
   :members:
   :undoc-members:

.. autoclass:: pyfes.config.Cartesian
   :members:
   :undoc-members:

.. autoclass:: pyfes.config.LGP
   :members:
   :undoc-members:

Utility Functions
=================

.. autofunction:: pyfes.known_constituents

.. autofunction:: pyfes.parse_constituent

.. autofunction:: pyfes.generate_markdown_table
