.. currentmodule:: pyfes

API
===

Tide evaluation
---------------

This function evaluates the tidal height at a given time and location. The
location is specified by longitude and latitude in degrees. The time is
given as a numpy datetime64 array. The function returns a tuple of three numpy
arrays: the short-period tide heights, the long-period tide heights, and the
quality flags. The tidal heights are given in the same units as the tidal model.

.. autofunction:: pyfes.evaluate_tide

Tide evaluation from constituents
---------------------------------

This function evaluates the tidal height at a given time and location using
the provided tidal constituents. The location is given as latitude in degrees.
The time is given as a numpy datetime64 array. The tidal height is returned as
a tuple of two numpy arrays: the first array contains the short-period tide
heights, and the second array contains the long-period tide heights. Both
arrays have the same length as the time array. The tidal heights are given in
the same units as the tidal model.

.. autofunction:: pyfes.evaluate_tide_from_constituents

Equilibrium long period tide evaluation
---------------------------------------

This function calculates equilibrium ocean tides over long periods using the
Cartwright-Tayler-Edden spectral line summation method. It incorporates both 2nd
and 3rd order tidal potential components, with validation against Tamura's
potential model. The function accepts multiple key inputs: time as numpy
datetime64 array, latitude in degrees (positive northward), optional list of
constituents to exclude, and optional computation settings (which include thread
count configuration via ``with_num_threads()``).
When the thread count is set to 0, the function automatically determines the
optimal thread count. Returns a
numpy array of computed tide heights. This implementation is based on established
research by Cartwright & Tayler (1971), Cartwright & Edden (1973),
and Tamura (1987).

.. autofunction:: pyfes.evaluate_equilibrium_long_period

Configuration file
------------------

This function provides a way to load the tidal model from a configuration
file. For more information on the configuration file format, see the
:ref:`configuration file format <configuration_file>` section.

.. currentmodule:: pyfes

.. autofunction:: pyfes.config.load

Configuration classes
---------------------

The configuration classes are used to store the configuration of the tidal
model.

.. autoclass:: pyfes.config.Configuration
   :members:
   :show-inheritance:

.. autoclass:: pyfes.config.Common
   :members:
   :show-inheritance:

.. autoclass:: pyfes.config.Cartesian
   :members:
   :show-inheritance:

.. autoclass:: pyfes.config.LGP
   :members:
   :show-inheritance:


Settings
--------

The settings are used to control the behavior of the tidal evaluation.

.. currentmodule:: pyfes

.. autoclass:: pyfes.Settings
   :members:
   :undoc-members:
   :show-inheritance:

.. autoclass:: pyfes.FesRuntimeSettings
   :show-inheritance:

   .. automethod:: __init__

.. autoclass:: pyfes.PerthRuntimeSettings
   :members:
   :show-inheritance:

   .. automethod:: __init__

Astronomic angles
-----------------

.. currentmodule:: pyfes.astronomic_angle

.. autoclass:: pyfes.astronomic_angle.AstronomicAngle
   :members:
   :undoc-members:
   :show-inheritance:

Type hints
----------

.. currentmodule:: pyfes.type_hints

.. automodule:: pyfes.type_hints
