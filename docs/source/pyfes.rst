.. currentmodule:: pyfes

Introduction
============

To forecast tides, instantiate either a
:class:`pyfes.core.TidalModelInterfaceComplex128` for double precision floating
point calculations, or a :class:`pyfes.core.TidalModelInterfaceComplex64` for
single precision. While the latter is quicker and more memory-efficient, it
sacrifices some accuracy. Regardless of the chosen precision, all internal
computations are performed using double precision.

PyFES supports two prediction engines:

* **FES/Darwin** (``engine: darwin``): the historical engine using Schureman/Darwin
  nodal corrections and admittance handling.
* **PERTH5/Doodson** (``engine: perth``): a Doodson-number-based engine that
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
`YAML <https://yaml.org/>`_ format. This file specifies where to find the tidal
model data and how it is structured.

The YAML file begins with a global configuration header. The optional ``engine``
keyword selects the prediction engine: either ``darwin`` (default) or ``perth``.
If specified, this keyword must appear at the top level of the file.

The configuration then describes two tidal components:

* **tide**: The primary ocean tide model, used to compute oceanic elevations
  (the vertical rise and fall of the sea surface).
* **radial**: The ocean tide loading model, which accounts for elastic
  deformation of the seafloor under the weight of the overlying water column.

Each section must specify the grid type of its model data: ``cartesian`` for
regular grids or ``lgp`` for unstructured triangular meshes.

.. note::

    File paths support environment variable expansion. For example,
    ``${FES_DATA}/my_model.nc`` resolves using the ``FES_DATA`` environment
    variable.

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
   engine: darwin
   radial:
     cartesian:
       paths:
         2N2: ${FES_DATA}/fes2014b_load_tide/2n2.nc
         K1: ${FES_DATA}/fes2014b_load_tide/k1.nc
         # ... and so on for all loading constituents
         S2: ${FES_DATA}/fes2014b_load_tide/s2.nc
         M2: ${FES_DATA}/fes2014b_load_tide/m2.nc

   # Primary Ocean Tide model on an unstructured (LGP) grid
   tide:
     lgp:
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

Using the PERTH5 engine just requires switching the top-level ``engine`` key to
``perth`` and providing a PERTH-compatible atlas.

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

.. _runtime_settings:

Runtime Settings
----------------

Runtime settings control the prediction engine behavior and must match your
tidal atlas format. When you load a configuration file via
:func:`pyfes.config.load`, PyFES automatically instantiates the appropriate
settings class based on the ``engine`` key in your YAML file:

* ``engine: darwin`` → :class:`pyfes.FesRuntimeSettings`
* ``engine: perth`` → :class:`pyfes.PerthRuntimeSettings`

Each engine has specific configuration options tailored to its prediction
methodology. The settings are returned in ``cfg.settings`` and passed to
prediction functions.

FES/Darwin Runtime Settings
~~~~~~~~~~~~~~~~~~~~~~~~~~~~

For FES tidal atlases (FES2014, FES2022), use :class:`pyfes.FesRuntimeSettings`:

.. code-block:: python

    import pyfes

    # Create Darwin engine settings with recommended defaults
    settings = (
        pyfes.FesRuntimeSettings()
        .with_inference_type(pyfes.SPLINE)
        .with_astronomic_formulae(pyfes.Formulae.SCHUREMAN_ORDER_1)
        .with_time_tolerance(3600.0)
        .with_num_threads(0)
    )

    # Use with tide evaluation
    tide, lp, flags = pyfes.evaluate_tide(
        tide_model, dates, lons, lats, settings=settings
    )

**Available Options:**

.. list-table::
   :header-rows: 1
   :widths: 35 65

   * - Option
     - Description
   * - ``with_astronomic_formulae``
     - Astronomical angle formulation:

       * ``SCHUREMAN_ORDER_1`` (default): First-order Schureman formulations
       * ``SCHUREMAN_ORDER_3``: Third-order Schureman (higher accuracy)
       * ``IERS``: IERS conventions (modern ephemerides)

       Default is ``SCHUREMAN_ORDER_1`` for consistency with historical FES
       conventions.
   * - ``with_time_tolerance``
     - Time span in seconds for angle reuse optimization. Astronomical angles
       are expensive to compute; this setting allows reusing angles within the
       specified tolerance. Default: 3600.0 (1 hour). Set to 0.0 to disable
       caching.
   * - ``with_inference_type``
     - Admittance interpolation strategy for minor constituents:

       * ``SPLINE`` (recommended for FES): Spline-based interpolation
       * ``LINEAR``: Linear interpolation
       * ``FOURIER``: Fourier-based interpolation
       * ``ZERO``: No inference; use only provided constituents

       Inference types are generic and can be used with any engine.
   * - ``with_num_threads``
     - Number of parallel threads for computation. Set to 0 (default) to let
       PyFES automatically determine the optimal thread count based on
       available CPU cores.

**Example with Third-Order Schureman:**

.. code-block:: python

    # Higher accuracy with third-order formulations
    settings = (
        pyfes.FesRuntimeSettings()
        .with_astronomic_formulae(pyfes.Formulae.SCHUREMAN_ORDER_3)
        .with_time_tolerance(1800.0)  # 30-minute tolerance
        .with_num_threads(4)  # Explicit thread count
    )

PERTH5/Doodson Runtime Settings
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

For GOT models and Doodson-based atlases, use :class:`pyfes.PerthRuntimeSettings`:

.. code-block:: python

    import pyfes

    # Create PERTH5 engine settings with recommended defaults
    settings = (
        pyfes.PerthRuntimeSettings()
        .with_group_modulations(True)
        .with_inference_type(pyfes.LINEAR)
        .with_astronomic_formulae(pyfes.Formulae.IERS)
        .with_time_tolerance(3600.0)
        .with_num_threads(0)
    )

    # Use with tide evaluation
    tide, lp, flags = pyfes.evaluate_tide(
        tide_model, dates, lons, lats, settings=settings
    )

**Available Options:**

.. list-table::
   :header-rows: 1
   :widths: 35 65

   * - Option
     - Description
   * - ``with_group_modulations``
     - Enable group-based nodal corrections. When ``True`` (recommended),
       related constituents are modulated together, providing computational
       efficiency. When ``False``, individual nodal corrections are applied.
       Default: ``True``.
   * - ``with_inference_type``
     - Admittance interpolation strategy for minor constituents:

       * ``LINEAR`` (recommended for GOT): Linear interpolation
       * ``SPLINE``: Spline-based interpolation
       * ``FOURIER``: Fourier-based interpolation
       * ``ZERO``: No inference; use only provided constituents

       Inference types are generic and can be used with any engine.
   * - ``with_astronomic_formulae``
     - Astronomical angle formulation:

       * ``IERS`` (default): IERS conventions (recommended for PERTH5)
       * ``SCHUREMAN_ORDER_1``: First-order Schureman
       * ``SCHUREMAN_ORDER_3``: Third-order Schureman

       Default is ``IERS`` for consistency with modern ephemerides.
   * - ``with_time_tolerance``
     - Time span in seconds for angle reuse optimization. Default: 3600.0
       (1 hour).
   * - ``with_num_threads``
     - Number of parallel threads. Default: 0 (auto-detect).

**Example with Maximum Accuracy:**

.. code-block:: python

    # High-precision configuration for critical applications
    settings = (
        pyfes.PerthRuntimeSettings()
        .with_group_modulations(True)
        .with_inference_type(pyfes.FOURIER)
        .with_astronomic_formulae(pyfes.Formulae.IERS)
        .with_time_tolerance(0.0)  # No caching, recompute every time
        .with_num_threads(8)
    )

**Example with Minimal Inference:**

.. code-block:: python

    # Use only explicitly provided constituents (no inference)
    settings = (
        pyfes.PerthRuntimeSettings()
        .with_group_modulations(True)
        .with_inference_type(pyfes.ZERO)
        .with_astronomic_formulae(pyfes.Formulae.IERS)
    )

Choosing Between Engines
~~~~~~~~~~~~~~~~~~~~~~~~~

The choice of engine depends on your tidal atlas format. Both engines share
the same high-level API, but each is optimized for different atlas types.

**Use FES/Darwin (FesRuntimeSettings) when:**

* Your tidal atlas is FES2014 or FES2022
* Following traditional oceanographic conventions
* Replicating established FES-based predictions
* Working with atlases using Darwin notation

**Use PERTH5/Doodson (PerthRuntimeSettings) when:**

* Your tidal atlas is GOT4.10, GOT5.5, GOT5.6, or similar
* Working with Richard Ray's tide models
* Need group modulation capabilities
* Require fine control over admittance interpolation
* Working with atlases using Doodson notation

.. important::

    Both sections (``tide`` and ``radial``) in your configuration file must
    use the same engine. Mixing engines is not supported.

For a detailed comparison of the two engines, see :ref:`prediction_engines`.

Performance Tuning
~~~~~~~~~~~~~~~~~~

**Time Tolerance:**

The ``with_time_tolerance`` setting controls how long astronomical angles are
cached and reused. This is a key performance optimization:

* **Larger tolerance** (e.g., 3600 seconds): Faster computation, slightly
  reduced precision
* **Smaller tolerance** (e.g., 60 seconds): More accurate, more computation
* **Zero tolerance**: Maximum accuracy, no caching, slowest

For most applications, the default 3600 seconds (1 hour) provides excellent
accuracy with good performance.

**Thread Count:**

Setting ``with_num_threads(0)`` (default) lets PyFES automatically choose the
optimal thread count. For production systems, you may want to set an explicit
count based on available CPU cores and concurrent workloads.

**Inference Type:**

The inference type affects both accuracy and performance. Inference types are
generic and can be used with any engine:

* ``ZERO``: Fastest, no additional constituents
* ``LINEAR``: Good balance (recommended for GOT atlases)
* ``SPLINE``: Spline-based interpolation (recommended for FES atlases)
* ``FOURIER``: Fourier-based interpolation

Examples
~~~~~~~~

**Complete Prediction Workflow:**

.. code-block:: python

    import pyfes
    import numpy as np

    # Load configuration (automatically creates appropriate settings)
    config = pyfes.config.load('fes2022b.yaml')

    # Define prediction coordinates and times
    dates = np.arange('2025-01-01', '2025-01-02', dtype='datetime64[h]')
    lons = np.array([-7.688, 0.0, 10.5])
    lats = np.array([59.195, 45.0, 30.0])

    # Evaluate tide components
    ocean_tide, lp_tide, flags = pyfes.evaluate_tide(
        config.models['tide'], dates, lons, lats, settings=config.settings
    )
    radial_tide, _, _ = pyfes.evaluate_tide(
        config.models['radial'], dates, lons, lats, settings=config.settings
    )

    # Calculate total geocentric tide
    total_tide = ocean_tide + radial_tide + lp_tide

**Custom Settings Override:**

.. code-block:: python

    # Load configuration but override settings
    config = pyfes.config.load('fes2022b.yaml')

    # Create custom settings
    custom_settings = (
        pyfes.FesRuntimeSettings()
        .with_astronomic_formulae(pyfes.Formulae.SCHUREMAN_ORDER_3)
        .with_time_tolerance(0.0)  # Maximum accuracy
    )

    # Use custom settings instead of config.settings
    tide, lp, flags = pyfes.evaluate_tide(
        config.models['tide'], dates, lons, lats, settings=custom_settings
    )

.. note::

    Complete examples of tide prediction are available in the `example gallery
    <auto_examples/index.html>`_, including:

    * :doc:`auto_examples/ex_prediction` - Basic prediction example
    * :doc:`auto_examples/ex_engine_comparison` - Engine comparison
    * :doc:`auto_examples/ex_constituents_prediction` - Constituent-based prediction
