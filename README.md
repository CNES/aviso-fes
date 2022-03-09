# DESCRIPTION

This package is the fully revised version of the FES2014 distribution.
This distribution includes the FES2014 tidal prediction software managed on this
page and the [FES2014 tides databases](data/fes2014/README.md)

# CREDITS

When using FES2014, please mention: *FES2014 was produced by NOVELTIS, LEGOS,
CLS Space Oceanography Division and CNES. It is distributed by AVISO, with
support from CNES (http://www.aviso.altimetry.fr/)*

# HOW TO INSTALL THE SOFTWARE ?

The complete description of the build method is described in the
[INSTALL](INSTALL.md) file located in the root of this software.

# HOW TO USE THE SOFTWARE ?

This distribution contains a C/Python API to be used to calculate the tide. The
[C API](C_API.md) documentation describes the functions to be used to
perform the calculation with C/C++. The directory [examples](examples) contains an
example of using this API.

This distribution contains a C/Python API to be used to calculate the tide.

The [C API](C_API.md) documentation describes the functions to be used to
perform the calculation with C/C++. The source code
[fes_slev.c](examples/fes_slev.c) contains an example of how to use this
API with the "C" language.

The [Python API](PYTHON_API.md) documentation describes the functions to be
used to perform the calculation with Python. The [examples](examples) directory
contains several examples of using this API:
[tide_gauge.py](examples/tide_gauge.py) illustrates the basic use of the API
(the program performs the same calculation as the C program) and the programs
[fes_slev.py](examples/fes_slev.py) and [fes_uv.py](examples/fes_uv.py)
illustrates the calculation of the global tide map and tidal velocities.

# BIBLIOGRAPHY

* Lyard F., L. Carrere, M. Cancet, A. Guillot, N. Picot: *FES2014, a new finite
  elements tidal model for global ocean*, in preparation, to be submitted to
  Ocean Dynamics in 2016.

* Carrere L., F. Lyard, M. Cancet, A. Guillot, N. Picot: *FES 2014, a new tidal
  model - Validation results and perspectives for improvements*, presentation to
  ESA Living Planet Conference, Prague 2016.

# CONTACT

[aviso@altimetry.fr](mailto:aviso@altimetry.fr)
