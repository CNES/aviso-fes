Python wrapper
##############

This folder contains the necessary script to build a Python wrapper to the
FES library.

The setup script uses the Python Distribution Utilities ("Distutils") to build
and install the library. Please see the associated documentation on how to
install a `Python package <https://docs.python.org/3/installing/index.html#installing-index>`_.

Build and installing
********************

Build requirements
==================

Because programs are written in Python, libraries in C you must obviously have
Python and a C compiler installed on your system to use these programs.
The compiling C requires the `NetCDF <http://www.unidata.ucar.edu/software/netcdf/>`_
development library and the `uthash <http://troydhanson.github.io/uthash/index.html>`_  C Header.

Configure
=========

Grab the latest archive release file from the
`download <https://bitbucket.org/cnes_aviso/fes/downloads>`_ page. Once you
have satisfied the requirements detailed above, you can configure the build
process with :

.. code-block:: bash

    python3 setup.py setup

The configuration process allows you to set options to specify, for example,
the paths to the NetCDF required libraries. The available options are
displayed using the option ``python setup.py setup --help``.

For example, to compile the library under Mac OS X enter the following command :

.. code-block:: bash

    python3 setup.py setup --netcdf-include=/opt/local/include \
        --uthash-includes=/opt/local/include/uthash \
        --netcdf-libraries=/opt/local/lib

Building
========

To build the library type the command ``python3 setup.py build`` at the root of
the Python wrapper folder.

Install
=======

To install just type the command ``python setup.py``. You can specify an
alternate installation path, with:

.. code-block:: bash

    python setup.py install --prefix=/usr/local

Usage
*****

Examples of creation of instances for calculating the tide.

.. code-block:: python

    import fes
    import os
    import datetime
    import numpy as np

    # Changing these values to the directory containing your data
    os.environ['FES_DATA'] = '../test/data'
    configuration_file = "../test/fes.ini"

    # Create handler
    short_tide = fes.Handler("ocean", "io", configuration_file)
    radial_tide = fes.Handler("radial", "io", configuration_file)

    # Wanted position
    lon = -7.688
    lat = 59.195

    # Start time
    date = datetime.datetime(1983, 1, 1)

    # Calculation step
    step = datetime.timedelta(seconds=3600)

    print("%-27s %9s %9s %9s %7s %7s %8s %8s" % (
        "Date",
        "Latitude",
        "Longitude",
        "Short_tide",
        "LP_tide",
        "Pure_tide",
        "Geo_tide",
        "Rad_tide"))

    # Compute the tide for the wanted location for 24 hours
    while date < datetime.datetime(1983, 1, 2):
        tide, lp = short_tide.scalar(lat, lon, date)
        if tide is not None:
            load, load_lp = radial_tide.scalar(lat, lon, date)
            print("%-27s %9.3f %9.3f %9.3f %8.3f %9.3f %9.3f %7.3f" % (
                date,
                lat,
                lon,
                tide,
                lp,
                tide + lp,
                tide + lp + load,
                load))
        date += step

    # Do the same computation but using with the vectorized version
    lats = np.array([59.195] * 24)
    lons = np.array([-7.688] * 24)
    times = np.empty((24,), dtype='datetime64[us]')
    for hour in range(0,24):
        times[hour] = datetime.datetime(1983, 1, 1, hour)

    tide, lp = short_tide.vector(lats, lons, times)
    load, load_lp = radial_tide.vector(lats, lons, times)

    print("\n%-27s %9s %9s %9s %7s %7s %8s %8s" % (
        "Date",
        "Latitude",
        "Longitude",
        "Short_tide",
        "LP_tide",
        "Pure_tide",
        "Geo_tide",
        "Rad_tide"))
    for ix in range(times.size):
        print("%-27s %9.3f %9.3f %9.3f %8.3f %9.3f %9.3f %7.3f" % (
            times[ix],
            lats[ix],
            lons[ix],
            tide[ix],
            lp[ix],
            tide[ix] + lp[ix],
            tide[ix] + lp[ix] + load[ix],
            load[ix]))


API for Python
**************

The Python library provides an interface to control the C API library.

``fes`` objects
===============

*class* **fes.Handler** (*tide, mode, path*)

    Creates a new FES handler.

        * ``tide`` specifies the tide computation mode : **tide** to compute
          the short tide or ``radial`` to compute the radial tide.
        * ``mode`` drives the memory management of the library :
          ``io`` no data are loaded into memory (data for the calculation will
          be read from the netCDF grids if necessary) or ``memory`` to load
          all NetCDF grids into memory.
        * ``path`` defines the path to the configuration file to use.

        .. note::

            if you want to use the library in a multi-threaded environment it
            is necessary to create as many ``Handler`` that you have threads.

Instance methods:

**fes.Handler.scalar** (*lat, lon, date*)

    Tidal computation for scalar values

    * ``lat`` is the latitude in degrees (positive north) for the position
      at which tide is computed.
    * ``lon`` is the longitude in degrees for the position at which tide is
      computed.
    * ``date`` date at which tide is computed

    Returns a tuple that contains:

        * Computed height of the diurnal and
          semi-diurnal constituents of the tidal spectrum
          (in centimeters) or None if no data is available
          at the given position.
        * Computed height of the long period wave constituents of
          the tidal spectrum (in centimeters). This value is always
          computed because this value does not depend on input grids.

**fes.Handler.scalar** (*lats, lons, dates*)

    Tidal computation for vector values

    This method is identical to the previous method, except that the arguments
    provided and the values returned are numpy arrays.

    .. note::

        Calling the method ``min_number`` is meaningless if you use this
        function because you will get the number of points used for the last
        evaluated value.

**fes.Handler.fes_min_number** ()

    Retrieves the minimum number of points used during the interpolation.

**fes.Handler.set_buffer_size** (size)

    Fixed size, in MB, of read buffer in case read mode selected is "io".
