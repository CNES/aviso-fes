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

The full harmonic prediction equation at universal time :math:`t` is:

.. math::

    h(t) = H_0 + \sum_{k=1}^{N} f_k(t) \, H_k \,
    \cos\!\left[V_k(t) + u_k(t) - G_k\right]

where:

- :math:`H_0` is the mean water level above the chart datum,
- :math:`H_k` is the amplitude of constituent :math:`k` from the tidal atlas,
- :math:`G_k` is the :term:`phase lag` of constituent :math:`k`,
- :math:`V_k(t)` is the equilibrium :term:`astronomical argument` at
  Greenwich at time :math:`t`; it advances at the angular speed
  :math:`\omega_k` of the constituent,
  :math:`V_k(t) = V_k(t_0) + \omega_k (t - t_0)` for a reference epoch
  :math:`t_0`,
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

The factor :math:`f_k` is the ratio of the obliquity factor :math:`g(I)`
(which depends on :math:`I` and hence on :math:`N`) to a mean value
:math:`k` of this factor. Schureman's constants :math:`k_{65}` to
:math:`k_{70}` (see :ref:`mean_obliquity_constants`) are not the mean of
:math:`g(I)` itself, but the mean over the nodal cycle of the complex
coefficient :math:`g(I) \, e^{iu}`, i.e. the amplitude of the central line of
the constituent. It is :math:`f_k e^{iu_k}`, not :math:`f_k` alone, that
averages to 1 over a nodal cycle. The coefficients :math:`k_{227}` and
:math:`k_{235}` of the :math:`K_1` and :math:`K_2` formulas below are of a
different kind: they are coefficients under a square root.

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

    f(L_2) = f(M_2) \times \frac{1}{R_a}
    \qquad \text{[Schureman formulas 213, 215]}

where :math:`1/R_a` is the ``x1ra`` factor documented in
:ref:`astronomic_angles_theory`. Unlike the other factors, it also depends
on the longitude of the lunar perigee, through :math:`p - \xi`
(8.85-year period); so does the phase factor :math:`R` of :math:`u(L_2)`.

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
    u(M_{tm}) &= -2\xi \\
    u(MS_{qm}) &= -2\xi
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

.. _mean_obliquity_constants:

Mean Obliquity Constants
=========================

The denominators :math:`k` in the :math:`f`-factor formulae are the means,
over the 18.61-year nodal cycle, of the complex coefficients
:math:`g(I) \, e^{iu}`:

.. math::

    k = \left| \frac{1}{2\pi} \int_0^{2\pi} g(I) \, e^{iu(N)} \, dN \right|

They differ from the mean of :math:`g(I)` alone: for :math:`M_f`, the mean
of :math:`\sin^2 I` is 0.1645, while :math:`k_{66} = 0.1578`. The values
below are those of the default build; when PyFES is built with
``FES_USE_IERS_CONSTANTS``, they are recomputed from the IERS 2010
constants:

.. list-table::
   :header-rows: 1
   :widths: 15 55 15 15

   * - Constant
     - :math:`g(I)`, averaged with :math:`e^{iu}`
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

When group modulations are disabled, the PERTH engine applies R. Ray's
individual nodal corrections, not the Schureman expressions documented
above. Each one is written as a sum of satellite lines:

.. math::

    f \, e^{iu} = 1 + \sum_j \alpha_j \, e^{i \phi_j}

where the satellite ratios :math:`\alpha_j` come from the
Cartwright--Tayler--Edden development of the potential and the phases
:math:`\phi_j` are combinations of :math:`N` and :math:`p`. For example,
:math:`f e^{iu}(O_1) = 1 + 0.1886 e^{iN} - 0.0058 e^{2iN} - 0.0065 e^{2ip}`.
For the main constituents, the two engines agree closely: over 1990--2030,
:math:`O_1` differs by less than 0.9 % in :math:`f` and 0.5° in :math:`u`,
and :math:`M_2` by less than 0.01 %.

References
==========

- Schureman, P. (1940). *Manual of Harmonic Analysis and Prediction of Tides*,
  SP 98, pp. 24--26 (formulas 65--80), pp. 44--46 (formulas 213--235).
- Simon, B. (2013). *Marées Océaniques et Côtières* (943-MOC), Ch. VI,
  pp. 159--163.
