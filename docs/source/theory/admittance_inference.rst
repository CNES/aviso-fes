.. _admittance_inference_theory:

==========================================
Admittance and Minor Constituent Inference
==========================================

Tidal atlases (FES2022, GOT5.6) typically provide harmonic constants for only
a subset of significant constituents. Many minor constituents, while small
individually, contribute meaningfully to the total tide. Rather than requiring
every constituent to be explicitly mapped, PyFES **infers** the amplitudes and
phases of minor constituents from those of nearby major constituents using the
concept of **admittance**.

The Admittance Concept
=======================

The ocean's response to tidal forcing at a given location can be characterised
by a complex transfer function :math:`Z(f)`, called the :term:`admittance`,
which relates the observed tidal amplitude and phase to the equilibrium
(theoretical) forcing amplitude:

.. math::

    Z_k = \frac{H_k^{\text{obs}}}{H_k^{\text{eq}}} \, e^{i G_k}

where :math:`H_k^{\text{obs}}` is the observed amplitude,
:math:`H_k^{\text{eq}}` is the equilibrium amplitude from the tidal potential
development, and :math:`G_k` is the phase lag.

The physical insight underlying inference is that **the admittance varies
smoothly with frequency within each tidal species**. Constituents close in
frequency share similar propagation and resonance characteristics, so their
admittances can be interpolated from a few well-determined reference
constituents.

Inference Modes
================

PyFES offers four inference modes, configurable via
:meth:`~pyfes.Settings.with_inference_type()`. These are generic and can be
used with either prediction engine.

Zero Inference (``ZERO``)
--------------------------

No inference is performed. Only constituents explicitly provided in the tidal
atlas are used for prediction. Minor constituents not in the atlas contribute
nothing to the predicted tide. This mode is useful for diagnostic purposes or
when the atlas already contains a comprehensive set of constituents.

Linear Inference (``LINEAR``)
------------------------------

The admittance is linearly interpolated between reference constituents within
each species. For each species, three reference frequencies are used:

- **Diurnal**: :math:`Q_1`, :math:`O_1`, :math:`K_1`
- **Semidiurnal**: :math:`N_2`, :math:`M_2`, :math:`S_2`

For a minor constituent at frequency :math:`f` between two reference
frequencies :math:`f_a` and :math:`f_b`:

.. math::

    Z(f) = Z(f_a) + \frac{f - f_a}{f_b - f_a} \left[Z(f_b) - Z(f_a)\right]

The inferred amplitude and phase are then:

.. math::

    H^{\text{inf}} = |Z(f)| \times H^{\text{eq}}, \qquad
    G^{\text{inf}} = \arg(Z(f))

This mode is **recommended for GOT atlases** (GOT4.10, GOT5.5, GOT5.6).

Spline Inference (``SPLINE``)
-------------------------------

The admittance is interpolated using a smooth spline through the reference
constituents. The spline uses additional reference points compared to linear
inference:

- **Diurnal**: :math:`Q_1`, :math:`O_1`, :math:`K_1`
- **Semidiurnal**: :math:`2N_2`, :math:`N_2`, :math:`M_2`, :math:`K_2`

The spline provides a smoother admittance curve, which typically yields
more accurate inference for minor constituents far from the reference
frequencies.

This mode is **recommended for FES atlases** (FES2014, FES2022).

Fourier Inference (``FOURIER``)
---------------------------------

The admittance is represented as a low-order Fourier series
(Munk--Cartwright approach) fitted to the reference constituents:

.. math::

    Z(f) = \sum_{n=0}^{N} c_n \, e^{i n \alpha(f)}

where :math:`\alpha(f)` is a normalised frequency coordinate and the
coefficients :math:`c_n` are determined by fitting to the reference
admittances. This approach is particularly effective when the admittance has a
systematic frequency dependence across the species.

Equilibrium Amplitudes and Love Numbers
=========================================

The inference calculation requires the equilibrium (theoretical) amplitude
:math:`H_k^{\text{eq}}` for each constituent. These values derive from the
tidal potential development (see :doc:`tide_generating_potential`) and are
stored as constants in PyFES.

For diurnal constituents, the equilibrium amplitudes are corrected by the
**frequency-dependent Love number** :math:`(1 + k_l - h_l)`, where
:math:`k_l` and :math:`h_l` are the load Love numbers at frequency
:math:`l`. This correction accounts for the elastic deformation of the
solid Earth in response to the tidal load.

Selecting an Inference Mode
============================

The choice of inference mode should match the tidal atlas:

.. code-block:: python

    # Recommended for FES atlases
    settings = pyfes.FESSettings().with_inference_type(pyfes.SPLINE)

    # Recommended for GOT atlases
    settings = pyfes.PerthSettings().with_inference_type(pyfes.LINEAR)

    # No inference (diagnostic)
    settings = pyfes.FESSettings().with_inference_type(pyfes.ZERO)

    # Fourier inference
    settings = pyfes.FESSettings().with_inference_type(pyfes.FOURIER)

References
==========

- Ray, R. D. (1999). A Global Ocean Tide Model From TOPEX/POSEIDON Altimetry:
  GOT99.2. NASA Technical Memorandum 209478.
- Munk, W. H. & Cartwright, D. E. (1966). Tidal Spectroscopy and Prediction.
  *Philosophical Transactions of the Royal Society of London A*, 259(1105),
  533--581.
- Simon, B. (2013). *Marées Océaniques et Côtières* (943-MOC), Ch. VI.
