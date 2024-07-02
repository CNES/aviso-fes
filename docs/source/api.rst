.. currentmodule:: pyfes

API
===

Tide evaluation
---------------

This function evaluates the tidal height at a given time and location. The
location is given as a tuple of latitude and longitude in degrees. The time is
given as numpy datetime64 array. The tidal height is returned as a numpy array
of the same length as the time array. The tidal height is given in the same
units as the tidal model.

.. autofunction:: pyfes.evaluate_tide

Configuration file
------------------

This function provides a way to load the tidal model from a configuration
file. For more information on the configuration file format, see the
:ref:`configuration file format <confguration_file>` section.

.. currentmodule:: pyfes.config

.. autofunction:: pyfes.config.load

Configuration classes
---------------------

The configuration classes are used to store the configuration of the tidal
model.

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

Astronomic angles
-----------------

.. currentmodule:: pyfes.astronomic_angle

.. autoclass:: pyfes.astronomic_angle.AstronomicAngle
   :members:
   :undoc-members:
   :show-inheritance:

Tidal constituents
------------------

.. currentmodule:: pyfes.constituents

.. autofunction:: pyfes.constituents.known

.. autofunction:: pyfes.constituents.name

.. autofunction:: pyfes.constituents.parse

Constituents Analysis
---------------------

.. currentmodule:: pyfes.wave_table

.. autoclass:: pyfes.wave_table.WaveTable
   :members:
   :undoc-members:
   :show-inheritance:

.. autoclass:: pyfes.wave_table.WaveDict
   :members:
   :undoc-members:
   :show-inheritance:

Leap Seconds
------------

.. currentmodule:: pyfes

.. autofunction:: pyfes.get_leap_seconds

Type hints
----------

.. currentmodule:: pyfes.typing

.. automodule:: pyfes.typing
