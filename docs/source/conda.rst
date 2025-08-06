Setup with conda or mamba
=========================

The easiest way to get started with the library is to install it using conda or
mamba.

`Conda <https://conda.io/projects/conda/en/latest/index.html>`_ is an
open-source package management system and environment management system that
runs on Windows, macOS, and Linux. Conda quickly installs, runs, and updates
packages and their dependencies. Conda easily creates, saves, loads, and
switches between environments on your local computer. It was created for Python
programs, but it can package and distribute software for any language.

`Mamba <https://mamba.readthedocs.io/en/latest/index.html>`_ is a
re-implementation of the conda package manager in C++. It is significantly
faster than conda and is fully compatible with all conda packages.

The first step is to install a package manager. The preferred way to do this is to
install **Miniforge**, which is a minimal installer for conda and also includes
mamba. Miniforge is a distribution of conda that includes only conda, Python,
and a small number of other packages. You can find more information about
Miniforge on `GitHub <https://github.com/conda-forge/miniforge>`_.

Then you can install the library using either conda or mamba. The library is
available on the conda-forge channel. To install the library, you can run one of
the following commands:

.. code-block:: bash

    # Using conda
    conda install -c conda-forge pyfes

    # Using mamba
    mamba install -c conda-forge pyfes
