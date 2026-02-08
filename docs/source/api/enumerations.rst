.. _api_enumerations:

============
Enumerations
============

Enumeration types used throughout the library to select engines, formulae,
inference methods, and to classify tidal waves.

.. class:: pyfes.EngineType

   Selects which prediction engine to use.

   .. attribute:: DARWIN

      Uses Darwin notation with Schureman's nodal corrections. This is the
      classical approach developed for FES tidal atlases.

   .. attribute:: DOODSON

      Uses Doodson number classification with group modulations. Developed by
      Dr. Richard Ray at NASA Goddard Space Flight Center, this engine is
      designed for GOT (Goddard Ocean Tide) models.

   .. property:: name

      Returns the name of the engine type.

   .. property:: value

      Returns the value associated with the engine type.

.. class:: pyfes.Formulae

   Selects the polynomial expressions used to compute astronomical angles.

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

   Method used to estimate minor constituents not explicitly provided in the
   tidal atlas.

   .. attribute:: FOURIER

      Munk-Cartwright Fourier series interpolation of admittances.

   .. attribute:: LINEAR

      Piecewise linear interpolation of admittances, default method for the
      Doodson engine.

   .. attribute:: SPLINE

      Spline interpolation of admittance, default method for the Darwin
      engine.

   .. attribute:: ZERO

      No inference performed; use only explicitly provided constituents.

   .. property:: name

      Returns the name of the inference type.

   .. property:: value

      Returns the value associated with the inference type.

.. class:: pyfes.FrequencyUnit

   Unit for expressing tidal constituent frequencies.

   .. attribute:: DEGREE_PER_HOUR

      Frequency in degrees per hour (°/h).

   .. attribute:: RADIAN_PER_HOUR

      Frequency in radians per second (rad/s).

   .. property:: name

      Returns the name of the frequency unit.

   .. property:: value

      Returns the value associated with the frequency unit.

.. class:: pyfes.TideType

   Distinguishes between the different physical quantities that can be
   predicted.

   .. attribute:: TIDE

      Ocean tide.

   .. attribute:: RADIAL

      Radial (loading) tide.

   .. property:: name

      Returns the name of the tide type.

   .. property:: value

      Returns the value of the tide type.

.. class:: pyfes.WaveType

   Spectral band of a tidal constituent.

   .. attribute:: SHORT_PERIOD

      Short-period waves (diurnal and semidiurnal species).

   .. attribute:: LONG_PERIOD

      Long-period waves (long-period species).

   .. property:: name

      Returns the name of the wave type.

   .. property:: value

      Returns the value associated with the wave type.
