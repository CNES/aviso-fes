.. currentmodule:: pyfes

Introduction
============

This module provides a Python interface to the tidal prediction software.
The FES tides database is available from the `AVISO website
<http://www.aviso.altimetry.fr/en/data/products/auxiliary-products/global-tide-fes.html>`_.

To predict tides, you need to instantiate a
:class:`pyfes.core.AbstractTidalModelComplex128` or
:class:`pyfes.core.AbstractTidalModelComplex64` object. The former uses double
precision floating point numbers, while the latter uses single precision
floating point numbers. The single precision version is faster and uses less
memory, but is less accurate. Whatever the precision, the internal computations
are done using double precision floating point numbers.

.. _confguration_file:

Configuration file
------------------

To instantiate a tidal model, you need to provide the YAML file describing the
model to use. The YAML file is a text file containing two sections: ``tide`` and
``radial``. The ``tide`` section contains the configuration of the tidal model
used to calculate the tide elevations. The ``radial`` section contains the
configuration of the model used to calculate the tide loading effect.

The ``tide`` or ``radial`` section contains one of the following keys:

* ``cartesian``
* ``lgp``

The ``cartesian`` key indicates that the numeric model used a Cartesian
grid to describe the ocean. The ``lgp`` key indicates that the numeric model
used a LGP discretization to describe the ocean.

.. note::

    The configuration could contain environment variables. The environment
    variables are replaced by their values before the configuration is parsed.
    The syntax for environment variables is ``${NAME}``, where ``NAME`` is the
    name of the environment variable.

Cartesian grid
~~~~~~~~~~~~~~

The section ``cartesian`` contains the following keys:

* ``lat``: the name of the netCDF variable containing the latitude of the grid
  points. Optional, default: ``lat``.
* ``lon``: the name of the netCDF variable containing the longitude of the grid
  points. Optional, default: ``lon``.
* ``amplitude``: the name of the netCDF variable containing the amplitude of the
  tidal constituents. Optional, default: ``amplitude``.
* ``phase``: the name of the netCDF variable containing the phase of the tidal
  constituents. Optional, default: ``phase``.
* ``epsilon``: the tolerance used to determine if the longitude axis is periodic
  or not. If the step size times the number of elements in the longitude axis
  is equal to 360 degrees within the given tolerance, then the longitude axis
  is considered periodic. Otherwise, it is considered non-periodic. Optional,
  default: ``1e-6``.
* ``paths``: a dictionary containing the paths to the netCDF files containing
  the tidal constituents. The keys are the names of the tidal constituents,
  and the values are the paths to the netCDF files containing the tidal
  constituents.

LGP discretization
~~~~~~~~~~~~~~~~~~

The section ``lgp`` contains the following keys:

* ``lat``: the name of the netCDF variable containing the latitude of the mesh
  points. Optional, default: ``lat``.
* ``lon``: the name of the netCDF variable containing the longitude of the mesh
  points. Optional, default: ``lon``.
* ``amplitude``: the pattern of the netCDF variable containing the amplitude of
  the tidal constituents. The pattern must contain the string ``{constituent}``,
  which is replaced by the name of the tidal constituent. Example:
  ``amp_{constituent}``. Optional, default: ``{constituent}_amplitude``.
* ``phase``: the pattern of the netCDF variable containing the phase of the
  tidal constituents. The pattern must contain the string ``{constituent}``,
  which is replaced by the name of the tidal constituent. Example:
  ``pha_{constituent}``. Optional, default: ``{constituent}_phase``.
* ``codes``: The name of the netCDF variable containing the LPG codes of the
  mesh points.
* ``max_distance``: The maximum distance between a mesh point and a requested
  point outside the mesh. If the distance is greater than this value, the
  estimation is undefined, otherwise the estimation is extrapolated. Optional,
  default: ``0.0`` (extrapolation is not allowed).
* ``path``: the path to the netCDF file containing the tidal constituents.
* ``triangles``: the name of the netCDF variable containing the vertices of the
  triangles of the mesh. Optional, default: ``triangle``
* ``type``: the type of LPG discretization. The possible values are ``lgp1`` and
  ``lpg2``. Optional, default: ``lgp1``.

Example
~~~~~~~

The following example shows how to instantiate a tidal model using the
configuration file ``fes2014b.yaml``.

The configuration file ``fes2014b.yaml``:

.. code-block:: yaml

    radial:
        cartesian:
            lat: lat
            lon: lon
            amplitude: amp
            phase: pha
            epsilon: 1e-6
            paths:
                M2: ${PATH}/M2.nc
                S2: ${PATH}/S2.nc
                K1: ${PATH}/K1.nc
                O1: ${PATH}/O1.nc
                P1: ${PATH}/P1.nc
                Q1: ${PATH}/Q1.nc
    tide:
        lgp:
            lat: lat
            lon: lon
            amplitude: amp_{constituent}
            phase: pha_{constituent}
            codes: codes
            max_distance: 0.0
            path: ${PATH}/fes2014b_elevations.nc
            triangles: triangle
            type: lgp1

The Python code to instantiate the tidal model and evaluate the tide elevation:

.. code-block:: python

    import pyfes

    cfg = pyfes.load_config('fes2014b.yaml')
    ...
    tide, lp, _ = pyfes.evaluate_tide(cfg['tide'], dates, lons, lats, num_threads=1)
    load = pyfes.evaluate_radial(cfg['radial'], dates, lons, lats, num_threads=1)[0]
    geocentric_tide = tide + load + lp

.. note::

  A full example of tide prediction is available in the `gallery
  <auto_examples/ex_prediction.html>`_.
