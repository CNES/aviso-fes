.. _api_engine_modules:

========================
Engine-Specific Modules
========================

Each prediction engine provides its own :class:`Wave` and :class:`WaveTable`
subclasses that carry the engine-specific constituent catalogue and nodal
correction logic.

In normal usage you do not need to instantiate these directly -
:func:`pyfes.wave_table_factory` returns the appropriate
:class:`~pyfes.WaveTableInterface` for the requested engine. The classes
documented here are useful when you need engine-specific behaviour or want to
inspect the constituent definitions.

Darwin
------

The :mod:`pyfes.darwin` module provides the 99 constituents expressed in Darwin
notation with Schureman's nodal corrections.

.. autoclass:: pyfes.darwin.WaveTable
   :members:
   :undoc-members:
   :show-inheritance:

.. autoclass:: pyfes.darwin.Wave
   :members:
   :undoc-members:
   :show-inheritance:

PERTH / Doodson
---------------

The :mod:`pyfes.perth` module provides the 80 constituents expressed in Doodson
number notation. Group modulations are available but disabled by default.

.. autoclass:: pyfes.perth.WaveTable
   :members:
   :undoc-members:
   :show-inheritance:

.. autoclass:: pyfes.perth.Wave
   :members:
   :undoc-members:
   :show-inheritance:
