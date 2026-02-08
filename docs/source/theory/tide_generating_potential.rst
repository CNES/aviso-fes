.. _tide_generating_potential:

=============================
The Tide-Generating Potential
=============================

The physical origin of ocean tides lies in the differential gravitational
attraction exerted by the Moon and Sun on the Earth. At the Earth's centre,
the gravitational pull of a celestial body is exactly balanced by the orbital
acceleration of the Earth--Moon or Earth--Sun system. At any other point on the
Earth's surface, a residual force remains: the **tide-generating force**. This
section derives the mathematical form of the corresponding potential, which
underpins the entire harmonic prediction method implemented in PyFES.

Tide-Generating Force
=====================

Consider a celestial body :math:`A` of mass :math:`M_A` at distance :math:`R`
from the Earth's centre :math:`T`. At a point :math:`P` on the Earth's
surface, at geocentric distance :math:`a` (the Earth's mean radius) and
geocentric zenith distance :math:`z` from the sub-body point, the
tide-generating force per unit mass is the *difference* between the
gravitational attraction at :math:`P` and the uniform attraction at :math:`T`.

The distance from :math:`A` to :math:`P` is:

.. math::

    r = \sqrt{R^2 + a^2 - 2 a R \cos z}

The tide-generating force is directed along the line :math:`AP` (the
*tractive* component) and can be decomposed into a radial (vertical) and two
horizontal components. For tidal prediction, it is more convenient to work
with the **potential** of this force rather than the force itself.

Potential Expansion in Legendre Polynomials
============================================

The gravitational potential of body :math:`A` at point :math:`P`, minus the
uniform potential at the Earth's centre, can be expanded in a series of
Legendre polynomials:

.. math::

    W = \frac{G M_A}{R} \sum_{n=2}^{\infty}
        \left(\frac{a}{R}\right)^n P_n(\cos z)

where:

- :math:`G` is the gravitational constant,
- :math:`a/R` is the ratio of the Earth's radius to the distance of the
  celestial body (the lunar parallax :math:`a/R \approx 1/60` for the Moon),
- :math:`P_n(\cos z)` is the Legendre polynomial of degree :math:`n`,
- the :math:`n=1` term vanishes because it represents a uniform translation
  of the entire Earth.

Since :math:`(a/R)^n` decreases rapidly with :math:`n`, only the first few
terms are significant. For the Moon, the :math:`n=2` term dominates, and the
:math:`n=3` term contributes at the level of a few percent. For the Sun,
the much larger distance makes only the :math:`n=2` term non-negligible.
In Doodson's original development, terms with amplitudes below :math:`10^{-5}`
of the dominant :math:`M_2` constituent were discarded.

The Basic Tidal Potential
=========================

Retaining only the dominant :math:`n=2` term, the tidal potential at
colatitude :math:`\varphi` and geocentric zenith distance :math:`z` is:

.. math::

    W_2 = \frac{3}{2} \frac{G M_A}{R}
          \left(\frac{a}{R}\right)^2
          \left(\cos^2 z - \frac{1}{3}\right)

This can be rewritten using the basic factor :math:`U`:

.. math::

    U = \frac{3}{2} \frac{M_A}{M_T} \left(\frac{a}{R_0}\right)^3 g a

where :math:`M_T` is the Earth's mass, :math:`R_0` is the mean distance of
the celestial body, and :math:`g` is the gravitational acceleration at the
Earth's surface. The ratio :math:`U_{\text{Moon}} / U_{\text{Sun}} \approx 2.2`
quantifies the Moon's dominant role in tide generation.

Decomposition into Laplace Species
====================================

The key step linking the potential to observable tidal frequencies is the
decomposition of :math:`\cos^2 z` into functions of the observer's latitude
:math:`\varphi` and the celestial body's coordinates. Using the spherical
harmonic addition theorem, the second-degree potential separates into three
terms, known as the :term:`Laplace species`:

.. math::

    W_2 = W_{2,0} + W_{2,1} + W_{2,2}

Each species corresponds to a different dependence on the hour angle
:math:`\tau` (the angular position of the body relative to the observer's
meridian):

**Long-period species** (:math:`m = 0`):

.. math::

    W_{2,0} \propto \left(\frac{1}{2} - \frac{3}{2} \sin^2 \varphi\right)
    \left(\frac{2}{3} - \cos^2 \delta \right)

This term depends only on the body's declination :math:`\delta`, not on the
hour angle. It produces oscillations with periods from a fortnight to years
(e.g., :math:`M_f`, :math:`M_m`, :math:`S_{sa}`). The latitude factor
vanishes at :math:`\varphi = 35.26°` and is maximal at the poles.

**Diurnal species** (:math:`m = 1`):

.. math::

    W_{2,1} \propto \sin 2\varphi \cdot \sin 2\delta \cdot \cos \tau

This term oscillates once per day (:math:`\cos \tau`) and produces
the diurnal constituents (:math:`O_1`, :math:`K_1`, :math:`P_1`,
:math:`Q_1`). The latitude factor :math:`\sin 2\varphi` is maximal at
:math:`\varphi = 45°` and vanishes at the equator and poles.

**Semidiurnal species** (:math:`m = 2`):

.. math::

    W_{2,2} \propto \cos^2 \varphi \cdot \cos^2 \delta \cdot \cos 2\tau

This term oscillates twice per day (:math:`\cos 2\tau`) and produces
the dominant semidiurnal constituents (:math:`M_2`, :math:`S_2`, :math:`N_2`,
:math:`K_2`). The latitude factor :math:`\cos^2 \varphi` is maximal at the
equator and vanishes at the poles.

Connection to PyFES
====================

The tidal constituents stored in :class:`pyfes.darwin.WaveTable` and
:class:`pyfes.perth.WaveTable` are the individual spectral lines obtained by
further expanding each species into its harmonic components (see
:doc:`harmonic_development`). Each constituent's frequency is determined by
the rates of change of the astronomical angles appearing in the expansion,
and its theoretical amplitude derives from the coefficients of the potential
development.

The :class:`~pyfes.WaveInterface` property :attr:`~pyfes.WaveInterface.type`
distinguishes between :attr:`~pyfes.WaveType.SHORT_PERIOD` (diurnal and
semidiurnal species) and :attr:`~pyfes.WaveType.LONG_PERIOD` (long-period
species), directly reflecting the Laplace species classification.

References
==========

- Simon, B. (2013). *Marées Océaniques et Côtières* (943-MOC), Ch. III.
- Schureman, P. (1940). *Manual of Harmonic Analysis and Prediction of Tides*,
  SP 98, pp. 10--28.
- Doodson, A. T. (1921). The Harmonic Development of the Tide-Generating
  Potential. *Proc. Roy. Soc. London A*, 100(704), 305--329.
