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
by a complex transfer function :math:`Z(\omega)` of the angular frequency
:math:`\omega`, called the :term:`admittance`, which relates the observed
tidal amplitude and phase to the equilibrium (theoretical) forcing amplitude:

.. math::

    Z_k = \frac{H_k^{\text{obs}}}{H_k^{\text{eq}}} \, e^{i G_k}

where :math:`H_k^{\text{obs}}` is the observed amplitude,
:math:`H_k^{\text{eq}}` is the equilibrium amplitude from the tidal potential
development, and :math:`G_k` is the phase lag.

The physical insight underlying inference is that **the admittance varies
smoothly with frequency within each tidal species**. Constituents close in
frequency share similar propagation and resonance characteristics, so their
admittances can be interpolated from a few well-determined reference
constituents. The inferred constituent then gets the complex amplitude
:math:`Z(\omega) \, H^{\text{eq}}`, that is:

.. math::

    H^{\text{inf}} = |Z(\omega)| \times H^{\text{eq}}, \qquad
    G^{\text{inf}} = \arg(Z(\omega))

Inference Modes
================

PyFES offers four inference modes, configurable via
:meth:`~pyfes.Settings.with_inference_type()`. ``ZERO``, ``LINEAR`` and
``FOURIER`` work with both prediction engines; ``SPLINE`` too, but with the
PERTH engine it does not infer :math:`M_1` (see below).

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

The interpolation is piecewise linear on two segments: for a minor
constituent at angular frequency :math:`\omega`, with reference frequencies
:math:`\omega_1 < \omega_2 < \omega_3`, the segment
:math:`[\omega_1, \omega_2]` is used when :math:`\omega \leq \omega_2` and
:math:`[\omega_2, \omega_3]` otherwise:

.. math::

    Z(\omega) = Z(\omega_a) + \frac{\omega - \omega_a}{\omega_b - \omega_a}
                \left[Z(\omega_b) - Z(\omega_a)\right]

Constituents outside the reference interval are extrapolated from the nearest
segment: for example :math:`2Q_1` and :math:`\sigma_1` below :math:`Q_1`,
:math:`OO_1` above :math:`K_1`, :math:`\varepsilon_2` below :math:`N_2` and
:math:`\eta_2` above :math:`S_2`.

This mode is **recommended for GOT atlases** (GOT4.10, GOT5.5, GOT5.6).

Spline Inference (``SPLINE``)
-------------------------------

Despite its name, this mode does not interpolate a spline through reference
admittances. Each minor constituent is a fixed linear combination of two or
three reference constituents (complex tides), with precomputed coefficients
taken from R. Ray's *perth2* and *perth3* programs and from the Grenoble
(FES) software:

.. math::

    \hat{H}_k = \sum_j \alpha_{kj} \, Z_j

- :math:`2Q_1`, :math:`\sigma_1`, :math:`\rho_1` from :math:`Q_1` and
  :math:`O_1`;
- :math:`M_{11}`, :math:`M_{12}`, :math:`\chi_1`, :math:`\pi_1`,
  :math:`\varphi_1`, :math:`\theta_1`, :math:`J_1`, :math:`OO_1` from
  :math:`O_1` and :math:`K_1`;
- :math:`2N_2` from :math:`N_2` and :math:`M_2`, then :math:`\varepsilon_2`
  from :math:`2N_2` and :math:`N_2` (:math:`2N_2` is thus both inferred and
  used as a reference);
- :math:`\eta_2` from :math:`M_2` and :math:`K_2`;
- :math:`\mu_2`, :math:`\nu_2`, :math:`\lambda_2`, :math:`L_2`, :math:`T_2`
  from :math:`N_2`, :math:`M_2` and :math:`K_2`, with the spline admittance
  coefficients published in *Geophysical Research Letters*, 18(5), 845--848
  (1991).

Long-period constituents are not inferred. Only the constituents defined in
the wave table are inferred: the PERTH wave table defines :math:`M_1` instead
of :math:`M_{11}` and :math:`M_{12}`, so :math:`M_1` is not inferred in this
mode with the PERTH engine (``LINEAR`` and ``FOURIER`` infer it).

This mode is **recommended for FES atlases** (FES2014, FES2022).

Fourier Inference (``FOURIER``)
---------------------------------

The admittance is represented by a low-order Fourier series in frequency
(Munk--Cartwright approach), with a lag :math:`\tau = 48` hours:

.. math::

    Z(\omega) = c_0 + c_1 \cos(\omega \tau) + c_2 \sin(\omega \tau)

The three complex coefficients are not fitted: they are obtained exactly from
the three reference admittances (:math:`Q_1`, :math:`O_1`, :math:`K_1` or
:math:`N_2`, :math:`M_2`, :math:`S_2`):

.. math::

    \mathbf{c} = \mathbf{A}^{-1} \, \mathbf{Z}_{\text{ref}}, \qquad
    \mathbf{A}_{j,:} = \left(1, \; \cos \omega_j \tau, \; \sin \omega_j \tau
    \right)

Long-period constituents still use the linear interpolation in this mode.

Long-Period Constituents
-------------------------

In the ``LINEAR`` and ``FOURIER`` modes, long-period constituents are also
inferred, by linear interpolation of the admittances of the 18.61-year node
tide, :math:`M_m` and :math:`M_f`. When the atlas does not provide the node
tide, its equilibrium value is used. The ``ZERO`` and ``SPLINE`` modes do not
infer long-period constituents.

Equilibrium Amplitudes and Love Numbers
=========================================

The inference calculation requires the equilibrium (theoretical) amplitude
:math:`H_k^{\text{eq}}` for each constituent. These values derive from the
tidal potential development (see :doc:`tide_generating_potential`) and are
stored as constants in PyFES.

For diurnal constituents, the equilibrium amplitudes are multiplied by the
diminishing factor

.. math::

    \gamma_2(\omega) = 1 + k_2(\omega) - h_2(\omega)

where :math:`k_2` and :math:`h_2` are the degree-2 body-tide Love numbers
of Mathews et al. (1995, PMM95B model). They vary with frequency in the
diurnal band because of the free-core-nutation resonance, close to
:math:`K_1`. This correction accounts for the elastic deformation of the
solid Earth under the tidal potential. The reference admittances are divided
by :math:`H^{\text{eq}} \gamma_2(\omega_{\text{ref}})`, and the inferred
constituents multiplied by :math:`\gamma_2(\omega)`.

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
- Mathews, P. M., Buffett, B. A. & Shapiro, I. I. (1995). Love numbers for
  a rotating spheroidal Earth: New definitions and numerical values.
  *Geophysical Research Letters*, 22(5), 579--582.
- Simon, B. (2013). *Marées Océaniques et Côtières* (943-MOC), Ch. VI.
