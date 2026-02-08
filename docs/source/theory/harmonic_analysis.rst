.. _harmonic_analysis_theory:

====================================================
Harmonic Analysis: From Observations to Constituents
====================================================

Harmonic analysis is the inverse of tidal prediction: given a time series
of observed sea levels, the goal is to recover the amplitudes and
:term:`phase lags <phase lag>` of the :term:`tidal constituents <tidal
constituent>`. This section presents the mathematical formulation
implemented in PyFES.

The Observation Equation
=========================

At discrete observation times :math:`t_j` (:math:`j = 1, \ldots, M`), the
observed sea level can be modelled as a sum of :math:`N` tidal constituents
plus a residual:

.. math::

    h(t_j) = S_0 + \sum_{k=1}^{N} f_k(t_j) \left[
        a_k \cos\!\left(\omega_k t_j + V_k(t_j) + u_k(t_j)\right)
      + b_k \sin\!\left(\omega_k t_j + V_k(t_j) + u_k(t_j)\right)
    \right] + \varepsilon_j

where:

- :math:`S_0` is the mean sea level,
- :math:`a_k` and :math:`b_k` are the in-phase and quadrature components of
  constituent :math:`k`,
- :math:`f_k` and :math:`V_k + u_k` are the :term:`nodal corrections <nodal
  correction>` (see :doc:`nodal_corrections`),
- :math:`\omega_k` is the angular speed of constituent :math:`k`,
- :math:`\varepsilon_j` is the residual (noise, meteorological effects,
  etc.).

The unknown parameters are :math:`S_0`, :math:`a_k`, and :math:`b_k` for
each constituent.

Least-Squares Formulation
==========================

Defining the **modulated** phase of constituent :math:`k` at time :math:`t_j`
as:

.. math::

    \phi_{k,j} = \omega_k t_j + V_k(t_j) + u_k(t_j)

and collecting all unknowns into a vector
:math:`\mathbf{x} = (S_0, a_1, b_1, a_2, b_2, \ldots, a_N, b_N)^T`
of dimension :math:`2N + 1`, the observation equation becomes the linear
system:

.. math::

    \mathbf{h} = \mathbf{A} \mathbf{x} + \boldsymbol{\varepsilon}

where the **design matrix** :math:`\mathbf{A}` has :math:`M` rows (one per
observation) and :math:`2N + 1` columns:

.. math::

    \mathbf{A}_{j,:} = \left(1, \;
        f_1 \cos \phi_{1,j}, \; f_1 \sin \phi_{1,j}, \;
        f_2 \cos \phi_{2,j}, \; f_2 \sin \phi_{2,j}, \;
        \ldots, \;
        f_N \cos \phi_{N,j}, \; f_N \sin \phi_{N,j}
    \right)

The least-squares solution minimises :math:`\|\boldsymbol{\varepsilon}\|^2`
and is given by the **normal equations**:

.. math::

    \mathbf{A}^T \mathbf{A} \, \mathbf{x} = \mathbf{A}^T \mathbf{h}

This is the formulation implemented in PyFES's harmonic analysis routines.

Complex Amplitude Representation
===================================

The harmonic constants are conventionally expressed as an amplitude
:math:`H_k` and phase lag :math:`G_k`:

.. math::

    H_k = \sqrt{a_k^2 + b_k^2}, \qquad
    G_k = \arctan\!\left(\frac{b_k}{a_k}\right)

PyFES stores these as complex numbers:

.. math::

    Z_k = a_k + i \, b_k = H_k \, e^{i G_k}

This complex representation is used throughout the tidal model data
structures (e.g., ``CartesianComplex128``, ``LGP1Complex128``).

.. _rayleigh_criterion_theory:

The Rayleigh Criterion
======================

Two constituents of angular speeds :math:`\omega_1` and :math:`\omega_2` can
be independently resolved from a record of duration :math:`T` only if:

.. math::

    T > \frac{2\pi}{|\omega_1 - \omega_2|}
    \quad \Leftrightarrow \quad
    T > \frac{1}{|f_1 - f_2|}

where :math:`f_i = \omega_i / 2\pi` are the cyclic frequencies. This is the
:term:`Rayleigh criterion`.

In practice, a Rayleigh factor :math:`C_R \geq 1` (typically
:math:`C_R = 1`) is used, requiring :math:`T > C_R / |f_1 - f_2|`. As the
record length increases, more constituent pairs become separable. For example:

- A **15-day** record can separate species (diurnal from semidiurnal) but
  cannot resolve :math:`M_2` from :math:`S_2` (which requires about 14.8 days
  --- barely).
- A **30-day** record separates most constituents within a species except
  closely spaced pairs like :math:`K_1` and :math:`P_1`.
- A **year-long** record resolves the vast majority of significant
  constituents.

The ``select_waves_for_analysis()`` method on
:class:`~pyfes.core.WaveTableInterface` automatically applies this criterion
to select the constituents resolvable for a given record duration.

Role of Nodal Corrections in Analysis
=======================================

The :term:`nodal corrections <nodal correction>` :math:`f_k` and :math:`u_k`
appear explicitly in the design matrix :math:`\mathbf{A}`. If they are
omitted or computed with insufficient accuracy, the recovered amplitudes and
phases will be biased. For records shorter than a year, the nodal corrections
are essentially constant and can be evaluated at the midpoint of the record.
For multi-year records, time-varying corrections should be applied at each
observation time.

References
==========

- Simon, B. (2013). *Marées Océaniques et Côtières* (943-MOC), Ch. VI,
  pp. 126--158.
- Schureman, P. (1940). *Manual of Harmonic Analysis and Prediction of Tides*,
  SP 98, pp. 49--100.
