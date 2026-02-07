.. _harmonic_development:

===================================================
Harmonic Development and the Doodson Classification
===================================================

The positions of the Moon and Sun vary quasi-periodically in time, causing
the :doc:`tide-generating potential <tide_generating_potential>` to oscillate
with a rich spectrum of discrete frequencies. The **harmonic development**
consists of expanding this potential into a sum of cosine terms --- the
:term:`tidal constituents <tidal constituent>` --- each characterised by a
unique frequency derived from the rates of change of six astronomical angles.
This section presents the derivation of the Doodson classification system that
systematically encodes these frequencies.

.. _fundamental_variables_theory:

The Six Fundamental Variables
==============================

All tidal frequencies can be expressed as integer linear combinations of the
rates of change of six independent astronomical angles, ranked by decreasing
angular velocity:

.. list-table:: Fundamental astronomical variables
   :header-rows: 1
   :widths: 10 45 20 25

   * - Symbol
     - Physical meaning
     - Angular velocity (°/h)
     - Period
   * - :math:`\tau`
     - Local mean lunar time (hour angle of the mean Moon)
     - 14.492052
     - 1.035 days
   * - :math:`s`
     - Mean longitude of the Moon
     - 0.549017
     - 27.322 days
   * - :math:`h`
     - Mean longitude of the Sun
     - 0.041069
     - 365.242 days
   * - :math:`p`
     - Mean longitude of the lunar perigee
     - 0.004642
     - 8.847 years
   * - :math:`N'`
     - Negative longitude of the Moon's ascending node (:math:`-N`)
     - 0.002206
     - 18.613 years
   * - :math:`p_1`
     - Mean longitude of the solar perihelion
     - 0.000002
     - 20,940 years

The local mean lunar time :math:`\tau` is related to the civil time
:math:`T` (in hours) and the mean longitudes by:

.. math::

    \tau = 15° \cdot T + h - s

This relation arises because the hour angle of the mean Moon equals the hour
angle of the mean Sun (:math:`15° \cdot T`) plus the difference in mean
longitudes (:math:`h - s`). These six variables are mutually independent
--- no exact rational relationship exists between any pair of their periods ---
which is why the tide is technically not periodic but merely
quasi-periodic.

.. _general_tidal_argument:

General Form of a Tidal Argument
==================================

Each tidal constituent :math:`k` has an :term:`astronomical argument` of the
form:

.. math::

    \theta_k(t) = n_1 \tau + n_2 s + n_3 h + n_4 p + n_5 N' + n_6 p_1

where the :math:`n_i` are small integers (typically in the range
:math:`-5` to :math:`+6` for potential-derived constituents). The constituent's
angular frequency is:

.. math::

    \omega_k = n_1 \dot{\tau} + n_2 \dot{s} + n_3 \dot{h}
             + n_4 \dot{p} + n_5 \dot{N}' + n_6 \dot{p}_1

where the dots denote the constant angular velocities listed in the table
above.

The coefficient :math:`n_1` determines the :term:`tidal species`: constituents
with :math:`n_1 = 0` are long-period, :math:`n_1 = 1` diurnal, :math:`n_1 = 2`
semidiurnal, and so on.

.. _doodson_encoding:

Doodson Number Encoding
========================

To obtain a compact notation avoiding negative digits, Doodson (1921) proposed
adding 5 to each coefficient :math:`n_i` (for :math:`i \geq 2`), while leaving
the species coefficient :math:`n_1` unchanged. The resulting six-digit integer
is the :term:`Doodson number`:

.. math::

    N_D = n_1 \; (n_2{+}5) \; (n_3{+}5) \; .\; (n_4{+}5) \; (n_5{+}5) \; (n_6{+}5)

where a dot conventionally separates the first three digits from the last
three. For digits exceeding 9, special symbols are used: ``X`` for 10,
``E`` for 11, ``T`` for 12.

**Example --- M₂ (principal lunar semidiurnal)**:

The :math:`M_2` constituent has the argument :math:`\theta = 2\tau`, meaning
:math:`(n_1, n_2, n_3, n_4, n_5, n_6) = (2, 0, 0, 0, 0, 0)`. Adding 5 to
each coefficient except the first:

.. math::

    N_D(M_2) = 2\;5\;5\;.\;5\;5\;5 = 255.555

**Example --- S₂ (principal solar semidiurnal)**:

The :math:`S_2` argument is :math:`\theta = 2\tau + 2s - 2h = 2T` (since
:math:`\tau = T + h - s` in hourly notation), giving
:math:`(n_1, n_2, n_3, n_4, n_5, n_6) = (2, 2, -2, 0, 0, 0)`:

.. math::

    N_D(S_2) = 2\;7\;3\;.\;5\;5\;5 = 273.555

**Example --- K₁ (lunisolar diurnal)**:

:math:`(n_1, n_2, n_3, n_4, n_5, n_6) = (1, 1, 1, 0, 0, 0)`:

.. math::

    N_D(K_1) = 1\;6\;5\;.\;5\;5\;5 = 165.555

**Example --- O₁ (principal lunar diurnal)**:

:math:`(n_1, n_2, n_3, n_4, n_5, n_6) = (1, -1, 0, 0, 0, 0)`:

.. math::

    N_D(O_1) = 1\;4\;5\;.\;5\;5\;5 = 145.555

Extended Doodson Number
-----------------------

In practice, an additional seventh digit is appended to encode a phase
quadrant offset :math:`n_7 \times 90°` that ensures all potential terms
appear as positive cosines. The coefficient :math:`n_7` is 0 or 1
for cosine terms and :math:`\pm 1` for sine terms. With the same offset
convention, the seventh digit is :math:`n_7 + 5`.

Species, Groups, and Subgroups
-------------------------------

The Doodson classification introduces a natural hierarchy:

- **Species**: constituents sharing the same :math:`n_1` (number of cycles
  per lunar day).
- **Group**: constituents within a species sharing the same :math:`n_2`
  (same :math:`n_1` and :math:`n_2`).
- **Subgroup**: constituents within a group sharing the same :math:`n_3`.

Since constituents are sorted by ascending Doodson number, they are
automatically ordered by increasing frequency within each species.

.. _xdo_theory:

XDO Notation
=============

Because :term:`compound waves <compound wave>` arising from non-linear
hydrodynamic interactions can have coefficients outside the :math:`[-5, +6]`
range of the original Doodson encoding, an extended alphabetical notation was
developed. In this **XDO** (Extended Doodson Ordering) system, each
coefficient :math:`n_i` (including :math:`n_1`) is mapped to a letter:

.. list-table:: XDO letter mapping
   :header-rows: 1
   :widths: 60 40

   * - Coefficient value
     - Letter
   * - :math:`\ldots, -3, -2, -1, 0, 1, 2, 3, 4, 5, 6, 7, \ldots`
     - :math:`\ldots,` W, X, Y, Z, A, B, C, D, E, F, G, :math:`\ldots`

The mapping uses the sequence ``R S T U V W X Y Z A B C D E F G H I J ...``
with ``Z`` at coefficient 0. For example:

- :math:`M_2` with coefficients (2, 0, 0, 0, 0, 0, 0) becomes ``BZZZZZ``
- :math:`S_2` with coefficients (2, 2, -2, 0, 0, 0, 0) becomes ``BCXZZZZ``

In PyFES, the ``xdo_numerical()`` and ``xdo_alphabetical()`` methods on
:class:`~pyfes.core.WaveInterface` return these representations.

Darwin versus Doodson Notation
================================

The two notation systems are mathematically equivalent but differ in
philosophy:

- **Darwin notation** assigns mnemonic names to individual constituents
  (:math:`M_2`, :math:`S_2`, :math:`K_1`, etc.) and defines each
  constituent's argument, speed, and nodal corrections explicitly. This is
  the approach used by the FES/Darwin engine (``engine: darwin``).

- **Doodson notation** uses the systematic six-digit code to identify
  constituents by their astronomical argument coefficients, enabling
  mechanical enumeration of all possible tidal frequencies. This is the
  approach used by the PERTH/Doodson engine (``engine: perth``).

PyFES supports both via its dual-engine architecture: the ``darwin.WaveTable``
stores constituents with their Darwin-style argument definitions, while the
``perth.WaveTable`` uses the Doodson coefficient representation internally.

The Harmonic Prediction Equation
===================================

Combining the potential development with the ocean's response at a given
location yields the **harmonic tidal equation**. The tidal height at a
point with longitude :math:`G` at universal time :math:`t_0` is:

.. math::

    h(t_0) = H_0 + \sum_{k=1}^{N} f_k \, H_k \,
    \cos\!\left(V_{k,0}(t_0) + \omega_k \, t_0 - G_k + u_k\right)

where:

- :math:`H_0` is the mean water level above the datum,
- :math:`H_k` and :math:`G_k` are the amplitude and :term:`phase lag`
  (harmonic constants) of constituent :math:`k` at the given location,
- :math:`V_{k,0}(t_0)` is the :term:`astronomical argument` at the Greenwich
  meridian at time :math:`t_0 = 0`,
- :math:`\omega_k` is the angular speed of constituent :math:`k`,
- :math:`f_k` and :math:`u_k` are the :term:`nodal corrections <nodal
  correction>` (see :doc:`nodal_corrections`).

The harmonic constants :math:`(H_k, G_k)` are provided by tidal atlases
(FES2022, GOT5.6, etc.) or determined from tide gauge observations via
:doc:`harmonic analysis <harmonic_analysis>`.

References
==========

- Simon, B. (2013). *Marées Océaniques et Côtières* (943-MOC), Ch. V,
  pp. 102--125.
- Schureman, P. (1940). *Manual of Harmonic Analysis and Prediction of Tides*,
  SP 98, pp. 3--9.
- Doodson, A. T. (1921). The Harmonic Development of the Tide-Generating
  Potential. *Proc. Roy. Soc. London A*, 100(704), 305--329.
