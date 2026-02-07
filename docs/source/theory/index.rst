.. _theory:

=======================
Theoretical Foundations
=======================

PyFES implements the **harmonic method** for ocean tide prediction, a technique
with roots in Lord Kelvin's work around 1867 and formalised through the
contributions of Darwin, Doodson, and Schureman over the following decades. The
method rests on the physical insight that the complex, quasi-periodic motion of
the tide at any location can be decomposed into a finite sum of sinusoidal
components --- the :term:`tidal constituents <tidal constituent>` --- each driven
by a distinct astronomical forcing.

This section provides the mathematical foundations underlying the PyFES
prediction engines. It complements the practical guidance in
:doc:`../engines` with rigorous derivations of the key equations
implemented in the library.

.. toctree::
   :maxdepth: 1
   :caption: Theory:
   :hidden:

   tide_generating_potential
   harmonic_development
   astronomic_angles
   nodal_corrections
   harmonic_analysis
   admittance_inference
   compound_waves

References
----------

The theoretical development presented here draws primarily from:

* Doodson, A. T. (1921). The Harmonic Development of the Tide-Generating
  Potential. *Proceedings of the Royal Society of London A*, 100(704),
  305--329.
* Cartwright, D. E. & Tayler, R. J. (1971). New Computations of the
  Tide-generating Potential. *Geophysical Journal of the Royal Astronomical
  Society*, 23, 45--74.
* Cartwright, D. E. & Edden, A. C. (1973). Corrected Tables of Tidal
  Harmonics. *Geophysical Journal of the Royal Astronomical Society*, 33,
  253--264.
* Meeus, J. (1998). *Astronomical Algorithms*, 2nd ed. Willmann-Bell, Inc.
* Petit, G. & Luzum, B. (2010). IERS Conventions (2010). IERS Technical
  Note 36.
* Ray, R. D. (1999). A Global Ocean Tide Model From TOPEX/POSEIDON Altimetry:
  GOT99.2. NASA Technical Memorandum 209478.
* Schureman, P. (1940). *Manual of Harmonic Analysis and Prediction of Tides*.
  U.S. Coast and Geodetic Survey, Special Publication No. 98.
* Simon, B. (2013). *Marées Océaniques et Côtières* (Coll. Synthèses,
  943-MOC). Institut Océanographique / SHOM, Paris.
