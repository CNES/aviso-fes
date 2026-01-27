.. currentmodule:: pyfes

Introduction
============

To forecast tides, instantiate either a
:class:`pyfes.core.AbstractTidalModelComplex128` for double precision floating
point calculations, or a :class:`pyfes.core.AbstractTidalModelComplex64` for
single precision. While the latter is quicker and more memory-efficient, it
sacrifices some accuracy. Regardless of the chosen precision, all internal
computations are performed using double precision.

PyFES can now run two prediction engines:

* **FES/Darwin** (``engine: fes``): the historical engine using Schureman/Darwin
  nodal corrections and admittance handling.
* **PERTH5/Doodson** (``engine: perth5``): a Doodson-number-based engine that
  supports group modulations and multiple inference interpolation modes.

Both engines share the same high-level API; you select the engine in the YAML
configuration and PyFES automatically instantiates the matching runtime
settings.

Tidal models are configured via a YAML file detailing the configurations for
tide elevation calculations and the radial model for tide loading effects. These
models can be set up to utilize either a Cartesian grid or LGP discretization.
Additionally, the library's API allows for further configuration. For more
details, refer to the classes :class:`pyfes.config.Common`,
:class:`pyfes.config.Cartesian`, and :class:`pyfes.config.LGP`.

Data
----

The source distribution features a ``data`` directory, which is located
alongside this project and exclusively contains configuration files. It's
important to note that the tide atlas files are not part of the source
distribution. To obtain the tide atlas files, they must be downloaded from the
`AVISO website
<http://www.aviso.altimetry.fr/en/data/products/auxiliary-products/global-tide-fes.html>`_.

.. _configuration_file:

Configuration File
------------------

To perform a tide prediction, PyFES requires a configuration file in
`YAML <https://yaml.org/>`_ format. This file tells the software where to find
the tidal model data and how it's structured.

At its core, the YAML file describes the models for two different tidal effects:

* **tide**: This section defines the primary ocean tide model used to calculate
  the tide's vertical rise and fall (oceanic elevations).
* **radial**: This section defines the model for the **ocean tide loading**
  effect. This is a secondary effect where the weight of the ocean tide causes
  the flexible sea floor to deform, which also needs to be accounted for in
  precise measurements.

Each of these sections must specify the grid type of the model data, which can
be either a ``cartesian`` grid or an ``lgp`` (unstructured triangular) grid.
Every model section also accepts an ``engine`` key set to ``fes`` (default) or
``perth5``. All sections within a single configuration file must use the same
engine.

.. note::

    You can use environment variables in your YAML file paths. For example,
    ``${FES_DATA}/my_model.nc`` will be automatically expanded if you have an
    environment variable named ``FES_DATA``.

.. _cartesian_grid:

Cartesian Grid
~~~~~~~~~~~~~~

A Cartesian grid is a regular, rectangular grid of points (like pixels in an
image), where data for each tidal constituent is typically stored in a
separate NetCDF file. This format is often used for tide loading models.

**Required Keys:**

* ``paths``: A dictionary mapping constituent names to their corresponding
  NetCDF file paths.

**Optional Keys:**

* ``latitude``: Name of the latitude variable in the NetCDF files. Default:
  ``lat``.
* ``longitude``: Name of the longitude variable. Default: ``lon``.
* ``amplitude``: Name of the tidal amplitude variable. Default: ``amplitude``.
* ``phase``: Name of the tidal phase variable. Default: ``phase``.
* ``dynamic``: The list of the waves to be considered as part of the given
  altlas (evaluated dynamically from the model). The wave declared in this list
  will be considered as part of the model components and will be disabled from
  the admittance calculation and/or in the long-period equilibrium wave
  calculation routine (``lpe_minus_n_waves``). Optional, default: ``[]``.
* ``engine``: Prediction engine to use (``fes`` or ``perth5``). Default:
  ``fes``.
* ``epsilon``: A small tolerance value to check if the longitude axis wraps
  around 360 degrees. Default: ``1e-6``.

**Example (``radial`` section):**

.. code-block:: yaml

    radial:
      cartesian:
        paths:
          M2: ${FES_DATA}/m2_load.nc
          S2: ${FES_DATA}/s2_load.nc
          K1: ${FES_DATA}/k1_load.nc
          O1: ${FES_DATA}/o1_load.nc
          # ... other constituents ...

.. _lgp_grid:

LGP Discretization (Unstructured Grid)
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

An LGP (Le Provost et al.) grid is an unstructured mesh made of interconnected
triangles of varying sizes. This allows the model to have higher resolution in
complex coastal areas and lower resolution in the deep ocean. FES ocean tide
models (like FES2022) commonly use this format, with all constituent data
stored in a single NetCDF file.

**Required Keys:**

* ``path``: The path to the single NetCDF file containing the unstructured grid
  and all tidal constituent data.
* ``codes``: The name of the NetCDF variable containing the LGP codes for the
  mesh points.
* ``constituents``: A list of the tidal constituents you want to use from this
  model for the prediction.

**Optional Keys:**

* ``latitude``: Name of the latitude variable for the mesh points. Default:
  ``lat``.
* ``longitude``: Name of the longitude variable. Default: ``lon``.
* ``amplitude``: A pattern for the amplitude variable names. The string
  ``{constituent}`` will be replaced by the constituent's name (e.g., ``M2``,
  ``S2``). Default: ``{constituent}_amplitude``.
* ``phase``: A pattern for the phase variable names. Default:
  ``{constituent}_phase``.
* ``dynamic``: The list of the waves to be considered as part of the given
  altlas (evaluated dynamically from the model). The wave declared in this list
  will be considered as part of the model components and will be disabled from
  the admittance calculation and/or in the long-period equilibrium wave
  calculation routine (``lpe_minus_n_waves``). Optional, default: ``[]``.
* ``engine``: Prediction engine to use (``fes`` or ``perth5``). Default:
  ``fes``.
* ``triangle``: Name of the variable defining the mesh's triangles.
  Default: ``triangle``.
* ``max_distance``: The maximum distance (in grid units) to extrapolate a value
  if the requested point is outside the mesh. Default: ``0.0`` (no
  extrapolation).
* ``type``: The type of LGP discretization. Can be ``lgp1`` or ``lpg2``.
  Default: ``lgp1``.

.. _config_example:

Example Configuration
~~~~~~~~~~~~~~~~~~~~~

Here is a complete example of a ``fes2014b.yaml`` file, defining a model for
the **radial** tide loading (Cartesian grid) and another for the primary
**tide** (LGP grid).

.. code-block:: yaml
   :caption: fes2014b.yaml

   # Ocean Tide Loading model on a Cartesian grid
   radial:
     cartesian:
       engine: fes
       paths:
         2N2: ${FES_DATA}/fes2014b_load_tide/2n2.nc
         K1: ${FES_DATA}/fes2014b_load_tide/k1.nc
         # ... and so on for all loading constituents
         S2: ${FES_DATA}/fes2014b_load_tide/s2.nc
         M2: ${FES_DATA}/fes2014b_load_tide/m2.nc

   # Primary Ocean Tide model on an unstructured (LGP) grid
   tide:
     lgp:
       engine: fes
       path: ${FES_DATA}/fes2014b_elevations/fes2014b_elevations.nc
       codes: codes
       dynamic:
         - A5
       constituents:
         - 2N2
         - K1
         - K2
         - M2
         - N2
         - O1
         - P1
         - Q1
         - S1
         - S2
       # The variables in the NetCDF file follow the pattern amp_M2, pha_S2, etc.
       amplitude: amp_{constituent}
       phase: pha_{constituent}

Using the PERTH5 engine just requires switching the ``engine`` keys to
``perth5`` (both ``tide`` and ``radial`` sections must match) and providing a
PERTH-compatible atlas.

.. _using_the_config:

Using the Configuration File
----------------------------

Once your YAML file is ready, you can load it and run a prediction. The total
tide, which accounts for the primary ocean tide, loading effects, and
long-period tides, is calculated by summing the outputs.

.. code-block:: python

    import pyfes
    import numpy as np

    # Load the configuration from the YAML file
    cfg = pyfes.config.load("fes2014b.yaml")

    tide_model = cfg.models["tide"]
    radial_model = cfg.models["radial"]

    # Define coordinates and dates for the prediction
    lons = np.arange(-10, 10, 1.0)
    lats = np.arange(40, 60, 1.0)
    dates = np.arange('2021-01-01T00', '2021-01-01T10', dtype='datetime64[h]')

    # Evaluate the different tidal components
    ocean_tide, lp_tide, _ = pyfes.evaluate_tide(
        tide_model, dates, lons, lats, settings=cfg.settings)
    radial_tide, _, _ = pyfes.evaluate_tide(
        radial_model, dates, lons, lats, settings=cfg.settings)

    # Calculate the total geocentric tide
    geocentric_tide = ocean_tide + radial_tide + lp_tide

.. hint::

    Loading a global high-resolution grid can consume a lot of memory. To
    improve performance, you can load only a specific region of the grid by
    providing a ``bbox`` (bounding box) argument when loading the configuration.

    .. code-block:: python

        # Bounding box: [min_lon, min_lat, max_lon, max_lat]
        bbox = (-10, 40, 10, 60)
        cfg = pyfes.config.load('fes2014b.yaml', bbox=bbox)

Runtime Settings
----------------

Runtime settings pick the engine and tune the prediction. When you load a
configuration file, PyFES returns ``cfg.settings`` as either
:class:`pyfes.FesRuntimeSettings` or :class:`pyfes.PerthRuntimeSettings`
depending on the ``engine`` key. You can also build settings manually:

.. code-block:: python

    # PERTH5 engine with group modulations and Fourier admittance
    settings = (
        pyfes.PerthRuntimeSettings()
        .with_group_modulations(True)
        .with_inference_type(pyfes.InterpolationType.FOURIER_ADMITTANCE)
        .with_astronomic_formulae(pyfes.Formulae.IERS)
        .with_num_threads(0)
    )

    tide, lp, flag = pyfes.evaluate_tide(
        tide_model, dates, lons, lats, settings=settings)

Key options:

* ``with_astronomic_formulae``: Astronomic angles formulation. FES defaults to
  ``SCHUREMAN_ORDER_1``; PERTH5 defaults to ``IERS``.
* ``with_time_tolerance``: Time span (seconds) over which astronomical angles
  are reused.
* ``with_num_threads``: Parallel threads (``0`` lets PyFES choose).
* ``with_group_modulations`` (PERTH5 only): Enable group-modulation nodal
  corrections.
* ``with_inference_type`` (PERTH5 only): Admittance interpolation strategy
  (``ZERO_ADMITTANCE``, ``LINEAR_ADMITTANCE``, or ``FOURIER_ADMITTANCE``).

.. note::

    A full example of tide prediction is available in the `gallery
    <auto_examples/ex_prediction.html>`_.
