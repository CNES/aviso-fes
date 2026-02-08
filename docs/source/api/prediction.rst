.. _api_prediction:

====================
Prediction Functions
====================

These are the three entry points for computing tides. Each function accepts
arrays of epochs, longitudes, and latitudes and returns Eigen-style vectors
with the predicted heights.

``evaluate_tide`` interpolates harmonic constants from a gridded tidal model,
while ``evaluate_tide_from_constituents`` works directly from user-supplied
amplitudes and phase lags (e.g. from a tide-gauge harmonic analysis).
``evaluate_equilibrium_long_period`` provides the long-period equilibrium tide
from the Cartwright-Tayler-Edden tables.

.. autofunction:: pyfes.evaluate_tide

.. autofunction:: pyfes.evaluate_tide_from_constituents

.. autofunction:: pyfes.evaluate_equilibrium_long_period
