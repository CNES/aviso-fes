Setup with the existing python
===============================

We will present how to compile the code, install and run the various scripts
with `setuptools <https://setuptools.readthedocs.io/en/latest/>`_.

Requirements
############

Because the programs are written in Python, and some parts of the library in
C++, you must have Python 3, at least Python version 3.10, a C++ compiler and
`cmake <https://cmake.org/>`_ installed on your system to build the library.

.. note::

   The C++ compiler must support the ISO C++ 2014 standard

The compiling C++ requires the following development library:


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

You need, also, to install Python libraries before configuring and installing
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

    * ``--boost-root`` to specify the Boost include directory,
    * ``--cxx-compiler`` to select the C++ compiler to use,
    * ``--debug`` to compile the C++ library in Debug mode,
    * ``--eigen-root`` to specify the Eigen3 include directory,
    * ``--generator`` to specify the generator used by CMake,
    * ``--mkl-root`` to specify the MKL root.

Run the ``python setup.py build-ext --help`` command to view all the options
available for building the library.

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
