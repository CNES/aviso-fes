Setup with the existing python
===============================

This section explains how to compile the code, install, and run the various
scripts using `setuptools <https://setuptools.readthedocs.io/en/latest/>`_.

Requirements
############

Since the programs are written in Python and C++, you must have Python 3 (at
least version 3.10), a C++ compiler, and `cmake <https://cmake.org/>`_
installed on your system to build the library.

.. note::

   The C++ compiler must support the ISO C++ 2014 standard

Compiling the C++ code requires the following development libraries:

* `Boost <https://www.boost.org/>`_
* `Eigen3 <http://eigen.tuxfamily.org/>`_

Eigen3 can utilize optimized libraries such as `Intel Math Kernel Library (MKL)
<https://software.intel.com/en-us/mkl>`_, `OpenBlas
<https://www.openblas.net/>`_, or `Accelerate
<https://developer.apple.com/documentation/accelerate>`_ on MacOSX to enhance
computation speed. These libraries replace the default BLAS implementation
provided by Eigen3.

You can install these packages on Ubuntu by typing the following command:

.. code-block:: bash

    sudo apt-add-repository multiverse && sudo apt-get update # MKL is in the multiverse repo.
    sudo apt-get install g++ cmake libeigen3-dev libmkl-dev libboost-dev

You also need to install Python libraries before configuring and installing
this software:

* `numpy <https://www.numpy.org/>`_
* `netCDF4 <https://unidata.github.io/netcdf4-python/>`_

You can install these packages on Ubuntu by typing the following command:

.. code-block:: bash

    sudo apt-get install python3-numpy python3-netcdf4

Build
#####

Once you have satisfied the requirements detailed above, to build the library,
type the command ``python3 setup.py build`` at the root of the project.

You can specify, among other things, the following options to `build-ext`
command to customize the build:

.. list-table:: Build options
    :header-rows: 1

    * - Option
      - Description
    * - ``--cmake-args``
      - Additional arguments for CMake
    * - ``--cxx-compiler``
      - Preferred C++ compiler
    * - ``--iers``
      - Use IERS 2010 constants for tidal potential. By default, the
        library uses the constants from Paul Schureman's
        "Manual of Harmonic Analysis and Prediction of Tides" (Special
        Publication No. 98, 1940 Edition).
    * - ``--generator``
      - Selected CMake generator
    * - ``--mkl``
      - Use MKL as the BLAS library. The MKL library is searched in the
        Python prefix path. Alternatively, you can set the environment variable
        ``MKLROOT`` to the MKL library path to help the build system locate it.
    * - ``--reconfigure``
      - Forces CMake to reconfigure this project

Run the ``python setup.py build-ext --help`` command to view all the options
available for building the library.

Alternatively, using pip:

.. code-block:: bash

    pip install numpy netCDF4 git+https://github.com/CNES/aviso-fes.git


Test
####

Requirements
------------

Running tests require the following Python libraries:

* `pytest <https://docs.pytest.org/en/latest/>`_

Running tests
-------------

The distribution contains a set of test cases that can be processed with the
standard Python test framework. To run the full test suite,
use the following at the root of the project:

.. code-block:: bash

    pytest -v

Automatic Documentation
#######################

The source code of this documentation is managed by
`sphinx <http://www.sphinx-doc.org/en/master/>`_. It is possible to
generate it in order to produce a local mini WEB site to read and navigate it.
To do this, type the following command: ::

    sphinx-build -b html docs/source docs/build

Install
#######

To install just type the command `pip install .` at the root of the project.
