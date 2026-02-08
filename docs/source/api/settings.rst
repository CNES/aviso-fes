.. _api_settings:

========
Settings
========

Every prediction call accepts a :class:`~pyfes.Settings` object that controls
the engine type, astronomical formulae, inference method, nodal-correction
options, and parallelism.

Two convenience subclasses provide ready-to-use defaults:

* :class:`~pyfes.FESSettings` - Darwin engine defaults (Schureman formulae,
  spline inference).
* :class:`~pyfes.PerthSettings` - Doodson engine defaults (IERS formulae,
  linear inference, long-period equilibrium disabled).

All settings classes expose a fluent builder interface, so calls can be chained:

.. code-block:: python

   settings = pyfes.FESSettings() \
       .with_num_threads(4) \
       .with_time_tolerance(3600)

.. autoclass:: pyfes.Settings
   :members:
   :undoc-members:

.. autoclass:: pyfes.FESSettings
   :members:
   :undoc-members:
   :show-inheritance:

.. autoclass:: pyfes.PerthSettings
   :members:
   :undoc-members:
   :show-inheritance:
