.. _api_tidal_models:

============
Tidal Models
============

Tidal models store the harmonic constants (amplitude and phase) on a
computational grid. A model is loaded once at startup and then passed to
:func:`~pyfes.evaluate_tide` for interpolation at arbitrary coordinates.

Three grid types are available, each templated on the value precision:

* **Cartesian** - regular latitude/longitude grids (e.g. individual FES
  constituent NetCDF files).
* **LGP1** - unstructured triangular meshes with first-order (P1) elements.
* **LGP2** - unstructured triangular meshes with second-order (P2) elements,
  the native format of the FES2022 ocean-tide model.

The ``Complex128`` variants use double-precision complex numbers
(``complex128``); ``Complex64`` variants use single-precision (``complex64``).

Axis
----

.. autoclass:: pyfes.core.Axis
   :members:
   :undoc-members:

Cartesian
---------

.. autoclass:: pyfes.core.tidal_model.CartesianComplex128
   :members:
   :undoc-members:

.. autoclass:: pyfes.core.tidal_model.CartesianComplex64
   :members:
   :undoc-members:

LGP1
-----

.. autoclass:: pyfes.core.tidal_model.LGP1Complex128
   :members:
   :undoc-members:

.. autoclass:: pyfes.core.tidal_model.LGP1Complex64
   :members:
   :undoc-members:

LGP2
-----

.. autoclass:: pyfes.core.tidal_model.LGP2Complex128
   :members:
   :undoc-members:

.. autoclass:: pyfes.core.tidal_model.LGP2Complex64
   :members:
   :undoc-members:
