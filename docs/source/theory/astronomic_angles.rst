.. _astronomic_angles_theory:

===============================
Astronomical Angle Computation
===============================

The harmonic prediction equation requires knowing the values of the six
:term:`fundamental variables` (:math:`\tau, s, h, p, N', p_1`) at any given
time. These angles are computed from polynomial expressions in the Julian
century :math:`T` measured from a reference epoch. PyFES provides four sets
of formulae, selectable via the :class:`~pyfes.Formulae` enumeration.

This section documents the mathematical expressions implemented in each
formula set and the auxiliary angles derived from them.

General Form
=============

All formula sets express each fundamental variable as a polynomial in the
Julian century :math:`T`:

.. math::

    \theta(T) = a_0 + a_1 T + a_2 T^2 + a_3 T^3 + \ldots

where the coefficients :math:`a_i` are given in arc-degrees (or, for the
IERS formulae, in arc-seconds with subsequent conversion). The Julian
century is defined as:

.. math::

    T = \frac{t - t_{\text{epoch}}}{36\,525 \times 86\,400}

where :math:`t` is in seconds and :math:`t_{\text{epoch}}` is the reference
epoch of the formula set.

Schureman Order 1
==================

:Enumeration value: ``Formulae.SCHUREMAN_ORDER_1``
:Reference epoch: J1900.0 (January 0.5, 1900 Greenwich Civil Time)
:Time scale: UTC
:Polynomial order: 1 (linear)
:Source: Schureman (1940), Table 1, p. 162

This is the simplest formula set, using first-order polynomials that give
the mean rate of change of each element over a century:

.. math::

    \begin{aligned}
    N &= 259°\,10'\,57.12'' - (5 \times 360° + 482\,912.63'') \, T \\
    h &= 279°\,41'\,48.04'' + 129\,602\,768.13'' \, T \\
    s &= 270°\,26'\,14.72'' + (1336 \times 360° + 1\,108\,411.20'') \, T \\
    p_1 &= 281°\,13'\,15.0'' + 6\,189.03'' \, T \\
    p &= 334°\,19'\,40.87'' + (11 \times 360° + 392\,515.94'') \, T
    \end{aligned}

The multiples of :math:`360°` represent complete revolutions per century
that are removed modulo :math:`360°` in practice.

Schureman Order 3
==================

:Enumeration value: ``Formulae.SCHUREMAN_ORDER_3``
:Reference epoch: J1900.0
:Time scale: UTC
:Polynomial order: 3 (cubic)
:Source: Schureman (1940), Table 1, p. 162

This extends the Order 1 formulae to cubic polynomials, providing improved
accuracy over multi-century intervals. The additional quadratic and cubic
coefficients capture slow secular variations in the orbital elements. For
example:

.. math::

    N = 259°\,10'\,57.12'' - (5 \times 360° + 482\,912.63'') \, T
        + 7.58'' \, T^2 + 0.008'' \, T^3

The other variables follow the same pattern with their respective
higher-order coefficients.

Meeus
======

:Enumeration value: ``Formulae.MEEUS``
:Reference epoch: J2000.0 (January 1.5, 2000 TDT)
:Time scale: Terrestrial Dynamical Time (TDT)
:Polynomial order: 4 (quartic)
:Source: Meeus (1998), *Astronomical Algorithms*, Ch. 47

The Meeus formulae use the J2000.0 epoch and Terrestrial Dynamical Time
rather than UTC. The fundamental variables are computed via intermediate
quantities:

.. math::

    \begin{aligned}
    s &= 218.3164477° + 481\,267.88123421° \, T - 0.0015786° \, T^2 + \ldots \\
    d &= 297.8501921° + 445\,267.1114034° \, T - 0.0018819° \, T^2 + \ldots \\
    h &= s - d \\
    g &= 357.5291092° + 35\,999.0502909° \, T - 0.0001536° \, T^2 + \ldots \\
    p_1 &= h - g \\
    N &= 125.0445479° - 1\,934.1362891° \, T + 0.0020754° \, T^2 + \ldots \\
    p &= 83.3532465° + 4\,069.0137287° \, T - 0.0103200° \, T^2 + \ldots
    \end{aligned}

Here, :math:`d` is the mean elongation of the Moon from the Sun and
:math:`g` is the mean anomaly of the Sun. The higher-order terms provide
improved precision for modern epochs.

.. note::

    When using the Meeus or IERS formulae, PyFES automatically converts
    UTC to TDT using the ``utc_2_tdt()`` function, applying the appropriate
    :math:`\Delta T` correction.

IERS
=====

:Enumeration value: ``Formulae.IERS``
:Reference epoch: J2000.0
:Time scale: Terrestrial Dynamical Time (TDT)
:Polynomial order: 4 (quartic)
:Source: IERS Conventions (2010), based on Simon et al. (1994)

The IERS formulae use the five **lunisolar fundamental arguments** of
the nutation theory:

.. math::

    \begin{aligned}
    l &= 485\,868.249036'' + 1\,717\,915\,923.2178'' \, T
         + 31.8792'' \, T^2 + \ldots \quad \text{(mean anomaly of the Moon)} \\
    l' &= 1\,287\,104.79305'' + 129\,596\,581.0481'' \, T
          - 0.5532'' \, T^2 + \ldots \quad \text{(mean anomaly of the Sun)} \\
    F &= 335\,779.526232'' + 1\,739\,527\,262.8478'' \, T
         - 12.7512'' \, T^2 + \ldots \quad \text{(argument of latitude)} \\
    D &= 1\,072\,260.70369'' + 1\,602\,961\,601.2090'' \, T
         - 6.3706'' \, T^2 + \ldots \quad \text{(mean elongation)} \\
    \Omega &= 450\,160.398036'' - 6\,962\,890.5431'' \, T
              + 7.4722'' \, T^2 + \ldots \quad \text{(longitude of ascending node)}
    \end{aligned}

These are then converted to the traditional Doodson fundamental variables:

.. math::

    \begin{aligned}
    s &= F + \Omega & \text{(mean longitude of the Moon)} \\
    h &= F + \Omega - D & \text{(mean longitude of the Sun)} \\
    p &= F + \Omega - l & \text{(longitude of lunar perigee)} \\
    p_1 &= -l' + F - D + \Omega & \text{(longitude of solar perihelion)} \\
    N &= \Omega & \text{(longitude of the ascending node)}
    \end{aligned}

Comparison of Formula Sets
===========================

.. list-table::
   :header-rows: 1
   :widths: 25 18 18 18 18

   * - Property
     - Schureman O1
     - Schureman O3
     - Meeus
     - IERS
   * - Epoch
     - J1900.0
     - J1900.0
     - J2000.0
     - J2000.0
   * - Time scale
     - UTC
     - UTC
     - TDT
     - TDT
   * - Poly. order
     - 1
     - 3
     - 4
     - 4
   * - Default engine
     - Darwin
     - Darwin
     - Either
     - PERTH

Derived Auxiliary Angles
=========================

After computing the six fundamental variables, PyFES derives several auxiliary
angles required for the :doc:`nodal corrections <nodal_corrections>`. These are
computed once per time step in the :meth:`~pyfes.AstronomicAngle.update()`
method.

Obliquity of the Lunar Orbit (:math:`I`)
------------------------------------------

The inclination :math:`I` of the lunar orbit with respect to the celestial
equator varies between approximately 18.3° and 28.6° over the 18.61-year
nodal cycle. From Schureman (p. 156):

.. math::

    \cos I = \cos i \cos \omega - \sin i \sin \omega \cos N

where :math:`i = 5.145°` is the inclination of the lunar orbit to the
ecliptic and :math:`\omega = 23.452°` is the obliquity of the ecliptic. The
numerical constants are:

.. math::

    \cos I = 0.9137 - 0.0357 \cos N

Longitude of the Lunar Intersection (:math:`\xi`)
---------------------------------------------------

The longitude in the Moon's orbit of the intersection with the celestial
equator is:

.. math::

    \xi = -\arctan\!\left(1.0188 \, \tan \tfrac{N}{2}\right)
          -\arctan\!\left(0.6441 \, \tan \tfrac{N}{2}\right) + N

Right Ascension of the Lunar Intersection (:math:`\nu`)
--------------------------------------------------------

.. math::

    \nu = \arctan\!\left(1.0188 \, \tan \tfrac{N}{2}\right)
         -\arctan\!\left(0.6441 \, \tan \tfrac{N}{2}\right)

These two auxiliary angles appear in the phase corrections :math:`u` of most
tidal constituents (see :doc:`nodal_corrections`).

Factors for K₁ and K₂
-----------------------

The lunisolar constituents :math:`K_1` and :math:`K_2` combine lunar and solar
terms, requiring dedicated correction angles:

.. math::

    \nu' = \arctan\!\left(
        \frac{\sin 2I \sin \nu}{\sin 2I \cos \nu + 0.3347}
    \right)

.. math::

    \nu'' = \frac{1}{2}\arctan\!\left(
        \frac{\sin^2 I \sin 2\nu}{\sin^2 I \cos 2\nu + 0.0727}
    \right)

where the numerical constants 0.3347 and 0.0727 are the ratios of the solar to
lunar coefficients (Schureman formulas 224 and 232).

Factors for L₂
----------------

The constituent :math:`L_2` requires two additional quantities:

.. math::

    x_{1ra} = \sqrt{1 + 36\,\tan^2\!\left(\tfrac{I}{2}\right)^2
              - 12\,\tan^2\!\left(\tfrac{I}{2}\right)
              \cos 2(p - \xi)}

.. math::

    R = \arctan\!\left(
        \frac{\sin 2(p - \xi)}{(6\,\tan^2(I/2))^{-1} - \cos 2(p - \xi)}
    \right)

These are derived from Schureman formulas 213--214 (p. 44).

Connection to the AstronomicAngle Class
=========================================

The :class:`~pyfes.AstronomicAngle` Python class exposes all
the angles described above:

.. list-table::
   :header-rows: 1
   :widths: 15 85

   * - Property
     - Description
   * - ``t``
     - Mean solar angle relative to Greenwich (hour angle of the mean Sun)
   * - ``s``
     - Mean longitude of the Moon
   * - ``h``
     - Mean longitude of the Sun
   * - ``p``
     - Longitude of the lunar perigee
   * - ``p1``
     - Longitude of the solar perihelion
   * - ``n``
     - Longitude of the Moon's ascending node
   * - ``i``
     - Obliquity of the lunar orbit (:math:`I`)
   * - ``xi``
     - Longitude in Moon's orbit of the lunar intersection (:math:`\xi`)
   * - ``nu``
     - Right ascension of the lunar intersection (:math:`\nu`)
   * - ``nuprim``
     - Correction angle for :math:`K_1` (:math:`\nu'`)
   * - ``nusec``
     - Correction angle for :math:`K_2` (:math:`\nu''`)
   * - ``x1ra``
     - Amplitude factor for :math:`L_2` (:math:`x_{1ra}`)
   * - ``r``
     - Phase factor for :math:`L_2` (:math:`R`)

References
==========

- Schureman, P. (1940). *Manual of Harmonic Analysis and Prediction of Tides*,
  SP 98, Table 1 (p. 162), p. 156, formulas 213--214, 224, 232.
- Meeus, J. (1998). *Astronomical Algorithms*, 2nd ed., Ch. 47.
- Petit, G. & Luzum, B. (2010). IERS Conventions (2010), IERS Technical
  Note 36.
- Simon, B. (2013). *Marées Océaniques et Côtières* (943-MOC), Appendix A.
