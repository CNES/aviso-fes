.. _compound_waves_theory:

=================================
Compound and Shallow-Water Waves
=================================

In addition to the astronomical constituents derived from the
:doc:`tide-generating potential <tide_generating_potential>`, ocean tide
records reveal frequencies that are not present in the potential development.
These arise from **non-linear hydrodynamic interactions** and are called
compound waves or shallow-water waves. Understanding their origin is important
because they can dominate the tidal signal in coastal and estuarine regions.

Non-Linear Origin
==================

In the deep open ocean, the tidal wave behaves as a nearly linear long wave
and remains sinusoidal. As the wave propagates over the continental shelf and
into shallow waters, several non-linear processes distort its shape:

1. **Depth-dependent propagation speed**: The speed of a gravity wave is
   :math:`\sqrt{g \, d}`, where :math:`d` is the water depth. High water
   propagates faster than low water because the total depth is greater at
   high tide, causing the wave to steepen.

2. **Quadratic bottom friction**: The frictional drag on the seabed is
   proportional to :math:`|u| \, u` (where :math:`u` is the current
   velocity), introducing harmonics of the fundamental frequency.

3. **Advective acceleration**: The :math:`u \cdot \nabla u` term in the
   momentum equation generates sum and difference frequencies from
   interacting constituents.

In the spectral domain, these distortions manifest as new frequencies that
are integer combinations of the original astronomical constituent
frequencies.

Frequency Combinations
=======================

The frequency :math:`\omega` of a compound wave is an algebraic combination
of its parent constituents' frequencies. Common examples include:

**Overtides** (self-interaction of a single constituent):

.. math::

    \begin{aligned}
    M_4 &: \quad \omega_{M_4} = 2\,\omega_{M_2} \\
    M_6 &: \quad \omega_{M_6} = 3\,\omega_{M_2} \\
    M_8 &: \quad \omega_{M_8} = 4\,\omega_{M_2} \\
    S_4 &: \quad \omega_{S_4} = 2\,\omega_{S_2}
    \end{aligned}

**Compound tides** (interaction of two or more constituents):

.. math::

    \begin{aligned}
    MS_4 &: \quad \omega_{MS_4} = \omega_{M_2} + \omega_{S_2} \\
    MN_4 &: \quad \omega_{MN_4} = \omega_{M_2} + \omega_{N_2} \\
    MK_3 &: \quad \omega_{MK_3} = \omega_{M_2} + \omega_{K_1} \\
    MO_3 &: \quad \omega_{MO_3} = \omega_{M_2} + \omega_{O_1} \\
    2MS_6 &: \quad \omega_{2MS_6} = 2\,\omega_{M_2} + \omega_{S_2} \\
    2MK_3 &: \quad \omega_{2MK_3} = 2\,\omega_{M_2} - \omega_{K_1}
    \end{aligned}

**Difference frequencies** can also appear:

.. math::

    2MS_2 : \quad \omega_{2MS_2} = 2\,\omega_{M_2} - \omega_{S_2}

The naming convention uses the parent constituent symbols with a subscript
indicating the :term:`tidal species` (number of cycles per day).

Species Classification
========================

Compound waves span a range of species:

.. list-table::
   :header-rows: 1
   :widths: 25 25 50

   * - Species
     - Period range
     - Example constituents
   * - Terdiurnal (3)
     - ~8 hours
     - :math:`MO_3`, :math:`MK_3`, :math:`2MK_3`, :math:`M_3`
   * - Quarter-diurnal (4)
     - ~6 hours
     - :math:`M_4`, :math:`MS_4`, :math:`MN_4`, :math:`S_4`
   * - Sixth-diurnal (6)
     - ~4 hours
     - :math:`M_6`, :math:`2MS_6`, :math:`2MN_6`
   * - Eighth-diurnal (8)
     - ~3 hours
     - :math:`M_8`, :math:`3MS_8`

Nodal Corrections for Compound Waves
======================================

The :term:`nodal corrections <nodal correction>` for compound waves are
derived directly from those of their parent constituents:

**Amplitude factor** :math:`f`:

.. math::

    f(\text{compound}) = \prod_{\text{parents}} f(\text{parent})^{|c|}

where :math:`c` is the integer multiplier of each parent frequency. For
example:

.. math::

    \begin{aligned}
    f(M_4) &= f(M_2)^2 \\
    f(M_6) &= f(M_2)^3 \\
    f(MS_4) &= f(M_2) \times f(S_2) = f(M_2) \quad \text{(since } f(S_2) = 1\text{)}
    \end{aligned}

**Phase correction** :math:`u`:

.. math::

    u(\text{compound}) = \sum_{\text{parents}} c \times u(\text{parent})

For example:

.. math::

    \begin{aligned}
    u(M_4) &= 2\,u(M_2) \\
    u(MK_3) &= u(M_2) + u(K_1) \\
    u(2MK_3) &= 2\,u(M_2) - u(K_1)
    \end{aligned}

The sign of the multiplier :math:`c` follows the sign in the frequency
combination: positive for sum frequencies, negative for difference
frequencies.

Significance in Practice
==========================

Compound waves are typically small in the open ocean but can become the
dominant signal in estuaries and shallow coastal regions. In extreme cases
(e.g., the Gironde estuary), the quarter-diurnal and sixth-diurnal
constituents cause visible asymmetry between flood and ebb tide durations,
with flood durations shortened to about 4.5 hours and ebb durations
extended to about 8.5 hours.

PyFES includes a comprehensive set of compound wave constituents in its wave
tables. These are listed in the constituent reference pages:

- :doc:`../constituents/darwin` (FES/Darwin engine)
- :doc:`../constituents/perth` (PERTH/Doodson engine)

References
==========

- Simon, B. (2013). *Marées Océaniques et Côtières* (943-MOC), Ch. V,
  pp. 120--123.
- Schureman, P. (1940). *Manual of Harmonic Analysis and Prediction of Tides*,
  SP 98, pp. 46--48.
