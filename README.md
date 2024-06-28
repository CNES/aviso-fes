# DESCRIPTION

This package is the fully revised version of the FES2022 distribution.
This distribution includes the FES2022 tidal prediction software managed on this
page and the [FES2022 tides databases](data/fes2022/README.md)
A full description of the FES2022 tidal solution is given in the handbook
(https://www.aviso.altimetry.fr/fileadmin/documents/data/tools/hdbk_FES2022.pdf)
and in the paper (Lyard et al. 2024).

# CREDITS

When using FES2022, please mention: *FES2022 was produced by LEGOS, NOVELTIS and CLS
Ocean and Climate Division; the project was funded by CNES. It is distributed by AVISO,
with support from CNES (http://www.aviso.altimetry.fr/)*

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

# Coming Library Update 

This version of the library will no longer be updated. The new code under
development is designed to handle the new finite element grids of FES2022. The
new code can be accessed in the
[develop](https://github.com/CNES/aviso-fes/tree/develop) branch. It will become
official at the end of this year. For now, we do not provide support for this
new version.

# BIBLIOGRAPHY

* Lyard, F. H., Carrere, L., Fouchet, E., Cancet, M., Greenberg, D.,
  Dibarboure, G. and Picot, N.: "FES2022 a step towards a SWOT-compliant tidal
  correction", in preparation, to be submitted to Ocean Sciences in 2024.

* Lyard, F. H., Allain, D. J., Cancet, M., Carrère, L., and Picot, N.: FES2014
  global ocean tide atlas: design and performance, Ocean Sci., 17, 615–649,
  https://doi.org/10.5194/os-17-615-2021, 2021.

* Carrere L., F. Lyard, M. Cancet, A. Guillot, N. Picot: *FES 2014, a new tidal
  model - Validation results and perspectives for improvements*, presentation to
  ESA Living Planet Conference, Prague 2016.

# CONTACT

[aviso@altimetry.fr](mailto:aviso@altimetry.fr)
