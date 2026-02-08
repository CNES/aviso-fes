.. _api_wave_system:

===========
Wave System
===========

The wave system is composed of two main objects:

* :class:`~pyfes.WaveInterface` represents a single tidal constituent, giving
  access to its Doodson number, Darwin name, angular speed, type, and the
  current nodal factors *f* and *u*.
* :class:`~pyfes.WaveTableInterface` holds the complete catalogue of
  constituents for a given engine and provides bulk operations: computing nodal
  corrections, running harmonic analysis, and selecting waves that meet
  Rayleigh separation criteria.

A wave table is created with :func:`pyfes.wave_table_factory`:

.. code-block:: python

   wt = pyfes.wave_table_factory(pyfes.EngineType.DARWIN)

WaveInterface
-------------

.. autoclass:: pyfes.WaveInterface
   :members:
   :undoc-members:

WaveTableInterface
------------------

.. autoclass:: pyfes.WaveTableInterface
   :members:
   :undoc-members:


Factory
-------

.. autofunction:: pyfes.wave_table_factory
