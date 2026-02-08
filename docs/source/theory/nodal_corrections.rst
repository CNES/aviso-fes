.. _nodal_corrections_theory:

==================================================
Nodal Corrections: Amplitude and Phase Modulations
==================================================

The standard harmonic prediction equation treats each :term:`tidal
constituent` as a pure sinusoid of fixed amplitude and phase. In reality,
the amplitude and phase of each constituent are slowly modulated by the
18.61-year precession of the :term:`lunar node`. Rather than include all the
closely spaced "satellite" frequencies that produce this modulation, the
classical approach absorbs it into time-dependent correction factors
:math:`f` (amplitude) and :math:`u` (phase), collectively known as **nodal
corrections**.

This section documents all the nodal correction formulae implemented in
PyFES, following Schureman's notation.

The Prediction Equation with Nodal Corrections
================================================

The full harmonic prediction equation at universal time :math:`t_0` is:

.. math::

    h(t_0) = H_0 + \sum_{k=1}^{N} f_k(t_0) \, H_k \,
    \cos\!\left[\omega_k t_0 + V_{k,0}(t_0) + u_k(t_0) - G_k\right]

where:

- :math:`H_0` is the mean water level above the chart datum,
- :math:`H_k` is the amplitude of constituent :math:`k` from the tidal atlas,
- :math:`G_k` is the :term:`phase lag` of constituent :math:`k`,
- :math:`\omega_k` is the angular speed of constituent :math:`k`,
- :math:`V_{k,0}` is the equilibrium :term:`astronomical argument` at
  Greenwich,
- :math:`f_k` is the **nodal amplitude factor** (dimensionless, close to 1),
- :math:`u_k` is the **nodal phase correction** (in degrees or radians).

The factors :math:`f_k` and :math:`u_k` vary slowly over the 18.61-year
nodal cycle and are recomputed at each time step (or, in some older
implementations, once per year).

Physical Origin
================

The dominant source of nodal modulation is the regression of the Moon's
ascending node :math:`N` with a period of 18.61 years. As the node regresses,
the inclination :math:`I` of the lunar orbit to the celestial equator varies
between approximately 18.3° and 28.6°, modulating the tidal potential
coefficients that depend on :math:`I`.

In the harmonic development, each major constituent is accompanied by
nearby "satellite" frequencies separated by the nodal rate
(:math:`\dot{N}' \approx 0.0022°/\text{h}`). Instead of resolving these
satellites individually (which would require very long observational records),
the classical method treats each group as a single constituent whose
amplitude and phase vary slowly with :math:`N`.

Nodal Amplitude Factor :math:`f`
==================================

The factor :math:`f_k` is defined as the ratio of the true obliquity factor
(which depends on :math:`I` and hence on :math:`N`) to its mean value. Since
:math:`I` varies over the 18.61-year cycle, :math:`f_k` oscillates around
unity. The mean obliquity values used in the denominators are the constants
:math:`k_{65}` through :math:`k_{235}` defined in PyFES (from Schureman's
formulae).

Long-Period Constituents
-------------------------

.. math::

    f(M_m) = \frac{\frac{2}{3} - \sin^2 I}{k_{65}}
    \qquad (k_{65} = 0.5021)
    \qquad \text{[Schureman formula 73]}

.. math::

    f(M_f) = \frac{\sin^2 I}{k_{66}}
    \qquad (k_{66} = 0.1578)
    \qquad \text{[Schureman formula 74]}

Diurnal Constituents
---------------------

.. math::

    f(O_1) = \frac{\sin I \cos^2(I/2)}{k_{67}}
    \qquad (k_{67} = 0.3800)
    \qquad \text{[Schureman formula 75]}

.. math::

    f(J_1) = \frac{\sin 2I}{k_{68}}
    \qquad (k_{68} = 0.7214)
    \qquad \text{[Schureman formula 76]}

.. math::

    f(OO_1) = \frac{\sin I \sin^2(I/2)}{k_{69}}
    \qquad (k_{69} = 0.0164)
    \qquad \text{[Schureman formula 77]}

.. math::

    f(K_1) = \sqrt{k_{227,1} \sin^2 2I + k_{227,2} \sin 2I \cos \nu
             + k_{227,3}}
    \qquad \text{[Schureman formula 227]}

where :math:`k_{227,1} = 0.8965`, :math:`k_{227,2} = 0.6001`,
:math:`k_{227,3} = 0.1006`. The :math:`K_1` formula is more complex because
this constituent combines lunar and solar terms (see
:ref:`astronomic_angles_theory`).

Semidiurnal Constituents
-------------------------

.. math::

    f(M_2) = \frac{\cos^4(I/2)}{k_{70}}
    \qquad (k_{70} = 0.9154)
    \qquad \text{[Schureman formula 78]}

.. math::

    f(K_2) = \sqrt{k_{235,1} \sin^4 I + k_{235,2} \sin^2 I \cos 2\nu
             + k_{235,3}}
    \qquad \text{[Schureman formula 235]}

where :math:`k_{235,1} = 19.0444`, :math:`k_{235,2} = 2.7702`,
:math:`k_{235,3} = 0.0981`.

.. math::

    f(L_2) = f(M_2) \times \frac{1}{P_a}
    \qquad \text{[Schureman formula 215]}

where :math:`1/P_a` is the ``x1ra`` factor documented in
:ref:`astronomic_angles_theory`.

Compound Constituents
----------------------

For :term:`compound waves <compound wave>` formed by the interaction of parent
constituents, the nodal factor is the product of the parents' factors:

.. math::

    \begin{aligned}
    f(M_4) &= f(M_2)^2 \\
    f(M_6) &= f(M_2)^3 \\
    f(MS_4) &= f(M_2) \times f(S_2) = f(M_2) \qquad \text{since } f(S_2) = 1 \\
    f(MN_4) &= f(M_2) \times f(N_2) = f(M_2)^2
    \end{aligned}

Solar constituents (:math:`S_2`, :math:`S_1`, etc.) have :math:`f = 1` since
the Sun has no nodal modulation.

Nodal Phase Correction :math:`u`
==================================

The phase correction :math:`u_k` adjusts the phase of each constituent to
account for the slowly varying position of the lunar node. The corrections
follow systematic patterns depending on the constituent class.

Long-Period Constituents
-------------------------

.. math::

    \begin{aligned}
    u(M_m) &= 0 \\
    u(M_f) &= -2\xi \\
    u(M_{tf}) &= -2\xi \\
    u(M_{stm}) &= 0
    \end{aligned}

Diurnal Constituents
---------------------

.. math::

    \begin{aligned}
    u(O_1) &= +2\xi - \nu \\
    u(Q_1) &= +2\xi - \nu \\
    u(J_1) &= -\nu \\
    u(OO_1) &= -2\xi - \nu \\
    u(K_1) &= -\nu'
    \end{aligned}

where :math:`\xi` and :math:`\nu` are the longitude and right ascension of
the lunar intersection, and :math:`\nu'` is the combined lunar-solar
correction angle (see :ref:`astronomic_angles_theory`).

Semidiurnal Constituents
-------------------------

.. math::

    \begin{aligned}
    u(M_2) &= +2\xi - 2\nu \\
    u(N_2) &= +2\xi - 2\nu \\
    u(L_2) &= +2\xi - 2\nu - R \\
    u(K_2) &= -2\nu''
    \end{aligned}

where :math:`R` is the phase factor for :math:`L_2` and :math:`\nu''` is the
combined correction for :math:`K_2`.

Compound Constituents
----------------------

For compound waves, the phase correction is the algebraic sum of the
parents' corrections. For example:

.. math::

    \begin{aligned}
    u(M_4) &= 2 \, u(M_2) = 4\xi - 4\nu \\
    u(M_6) &= 3 \, u(M_2) = 6\xi - 6\nu \\
    u(MS_4) &= u(M_2) + u(S_2) = 2\xi - 2\nu
    \end{aligned}

Mean Obliquity Constants
=========================

The denominators in the :math:`f`-factor formulae are the mean values of the
obliquity-dependent expressions, computed with :math:`I` at its mean value.
These constants come from Schureman's evaluation of the expressions at the
mean inclination:

.. list-table::
   :header-rows: 1
   :widths: 15 55 15 15

   * - Constant
     - Expression (mean value of)
     - Schureman
     - PyFES
   * - :math:`k_{65}`
     - :math:`\frac{2}{3} - \sin^2 I`
     - 0.5021
     - 0.5021
   * - :math:`k_{66}`
     - :math:`\sin^2 I`
     - 0.1578
     - 0.1578
   * - :math:`k_{67}`
     - :math:`\sin I \cos^2(I/2)`
     - 0.3800
     - 0.3800
   * - :math:`k_{68}`
     - :math:`\sin 2I`
     - 0.7214
     - 0.7214
   * - :math:`k_{69}`
     - :math:`\sin I \sin^2(I/2)`
     - 0.0164
     - 0.0164
   * - :math:`k_{70}`
     - :math:`\cos^4(I/2)`
     - 0.9154
     - 0.9154

PERTH Group Modulations
=========================

The PERTH/Doodson engine offers an alternative approach to nodal corrections.
Instead of applying individual :math:`f` and :math:`u` factors to each
constituent, related constituents within the same tidal group (sharing the
same :math:`n_1` and :math:`n_2` coefficients) are **modulated together**.
The group modulation sums over the sideline frequencies within each group,
providing a more physically consistent treatment of the nodal cycle.

Group modulations are enabled via:

.. code-block:: python

    settings = pyfes.PerthSettings().with_group_modulations(True)

When group modulations are disabled, the PERTH engine falls back to
individual nodal corrections computed from the same Schureman-derived
expressions documented above.

References
==========

- Schureman, P. (1940). *Manual of Harmonic Analysis and Prediction of Tides*,
  SP 98, pp. 24--26 (formulas 65--80), pp. 44--46 (formulas 213--235).
- Simon, B. (2013). *Marées Océaniques et Côtières* (943-MOC), Ch. VI,
  pp. 159--163.
