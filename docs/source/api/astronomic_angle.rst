.. _api_astronomic_angle:

===================
Astronomical Angles
===================

The :class:`~pyfes.AstronomicAngle` class encapsulates the slowly-varying
astronomical arguments that drive the tidal potential: the mean longitudes and
anomalies of the Moon and Sun, the longitude of the lunar node, and derived
quantities such as the Doodson variables.

These angles are computed internally for every call to :func:`~pyfes.evaluate_tide`
or :func:`~pyfes.evaluate_tide_from_constituents`, but the class is also
exposed publicly so that users can inspect or reuse the values for nodal
corrections and harmonic analysis.

.. autoclass:: pyfes.AstronomicAngle
   :members:
   :undoc-members:
