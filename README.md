![build](https://github.com/CNES/aviso-fes/actions/workflows/ci.yaml/badge.svg)
[![platforms](https://anaconda.org/conda-forge/pyfes/badges/platforms.svg?service=github)](https://anaconda.org/conda-forge/pyfes)
[![latest-release-date](https://anaconda.org/conda-forge/pyfes/badges/latest_release_date.svg?service=github)](https://github.com/CNES/aviso-fes/commits/main)
[![license](https://anaconda.org/conda-forge/pyfes/badges/license.svg?service=github)](https://opensource.org/licenses/BSD-3-Clause)
[![download](https://anaconda.org/conda-forge/pyfes/badges/downloads.svg?service=github)](https://anaconda.org/conda-forge/pyfes)
[![Binder](https://mybinder.org/badge_logo.svg)](https://mybinder.org/v2/gh/CNES/aviso-fes/main?urlpath=lab/tree/notebooks/auto_examples)

# PyFES - Ocean Tide Prediction Software

PyFES is a powerful Python library for predicting ocean tides anywhere on Earth.
Using sophisticated global tidal atlases like FES2022, it can accurately
forecast tidal heights and currents based on harmonic analysis - the same
scientific method that has been used for over 150 years to understand and
predict tidal behavior.

## What can PyFES do?

- **Predict tidal heights** at any coastal or oceanic location
- **Calculate tidal currents** for marine navigation and oceanographic
  studies
- **Perform harmonic analysis** to extract tidal constituents from observational
data
- **Support multiple tidal models** including the latest FES2022 and legacy
  FES2014
- **Handle large-scale data processing** for research and operational
  applications

## Try it now - No installation required!

Want to see PyFES in action?
**[Launch interactive notebooks in Binder](https://mybinder.org/v2/gh/CNES/aviso-fes/main?urlpath=lab/tree/notebooks/auto_examples)** to explore example applications including:

- **Tidal prediction** at specific locations
- **Harmonic analysis** of tidal observations
- **Interpolation techniques** for spatial tidal mapping
- **Astronomical constants** and tidal constituent analysis

The examples run completely in the cloud - no software installation needed!

## About FES2022

This package is the fully revised version of the FES2022 distribution, including
both the PyFES prediction software and access to the
[FES2022 tides databases](data/fes2022b/README.md). FES2022 represents the
state-of-the-art in global tidal modeling, with improved accuracy especially for
satellite altimetry applications.

A full technical description is available in the
[FES2022 handbook](https://www.aviso.altimetry.fr/fileadmin/documents/data/tools/hdbk_FES2022.pdf)
and the scientific paper (Lyard et al. 2024).

## Credits

When using FES2022, please mention: *FES2022 was produced by LEGOS, NOVELTIS
and CLS Ocean and Climate Division; the project was funded by CNES. It is
distributed by AVISO, with support from CNES (http://www.aviso.altimetry.fr/)*

## Documentation

The complete documentation for PyFES is available on
[GitHub](https://cnes.github.io/aviso-fes/), including:

- **Installation guides** for different platforms
- **API reference** with detailed function documentation
- **Tutorial notebooks** with step-by-step examples
- **Theoretical background** on tidal prediction methods

## Tidal Constituents

PyFES supports a comprehensive set of tidal harmonic constituents for accurate
predictions. The complete list of constituents used in this software is
available in the [CONSTITUENTS.md](CONSTITUENTS.md) file.

## Legacy C Version

For users requiring the original C implementation, the legacy version remains
available on its
[own branch](https://github.com/CNES/aviso-fes/tree/legacy-c-version-archive).
The reference version used to compute the FES2022b tidal solution is
[2.9.7](https://github.com/CNES/aviso-fes/releases/tag/2.9.7).

> **Note:** The legacy C version is archived and will not receive further
> updates. For new projects, we recommend using the current Python version or
> the C++ library.<br/>
*Please note: The C++ library does not include a reader for the tidal
> databases—you will need to implement your own reader to access them.*

## References

### Key Publications

* Lyard, F., Carrere, L., Fouchet, E., Cancet, M., Greenberg, D., Dibarboure, G., and Picot, N.: FES2022 a step towards a SWOTcompliant tidal correction, Submitted to J. Geophy. Res., in review, 2025

* Lyard, F. H., Allain, D. J., Cancet, M., Carrère, L., and Picot, N.: FES2014 global ocean tide atlas: design and performance, Ocean Sci., 17, 615–649, https://doi.org/10.5194/os-17-615-2021, 2021.

* Carrere L., F. Lyard, M. Cancet, A. Guillot, N. Picot: *FES 2014, a new tidal model - Validation results and perspectives for improvements*, presentation to ESA Living Planet Conference, Prague 2016.

## Contact

Questions, suggestions, or need support? Reach out to the AVISO team:

📧 [aviso@altimetry.fr](mailto:aviso@altimetry.fr)
