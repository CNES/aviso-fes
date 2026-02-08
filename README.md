![build](https://github.com/CNES/aviso-fes/actions/workflows/ci.yaml/badge.svg)
[![platforms](https://anaconda.org/conda-forge/pyfes/badges/platforms.svg?service=github)](https://anaconda.org/conda-forge/pyfes)
[![latest-release-date](https://anaconda.org/conda-forge/pyfes/badges/latest_release_date.svg?service=github)](https://github.com/CNES/aviso-fes/commits/main)
[![license](https://anaconda.org/conda-forge/pyfes/badges/license.svg?service=github)](https://opensource.org/licenses/BSD-3-Clause)
[![download](https://anaconda.org/conda-forge/pyfes/badges/downloads.svg?service=github)](https://anaconda.org/conda-forge/pyfes)
[![Binder](https://mybinder.org/badge_logo.svg)](https://mybinder.org/v2/gh/CNES/aviso-fes/main?urlpath=lab/tree/notebooks/auto_examples)

# PyFES - Ocean Tide Prediction Software

PyFES is a Python library for predicting ocean tides anywhere on Earth. Using
global tidal atlases such as FES2022 and GOT5.6, it evaluates tidal heights and
currents through harmonic analysis -- the same scientific method developed by
Lord Kelvin over 150 years ago.

## Features

- **Tidal height prediction** at any coastal or oceanic location from global
  atlases (FES2022, GOT5.6, etc.)
- **Tidal current computation** for marine navigation and oceanographic studies
- **Harmonic analysis** to extract tidal constituents from observed sea-level
  time series
- **Prediction from known constituents** without requiring a tidal atlas
- **Long-period equilibrium tide** computation from Cartwright-Tayler-Edden
  spectral tables
- **Multiple tidal model formats**: regular Cartesian grids and unstructured
  finite-element meshes (LGP1/LGP2)
- **Dual prediction engines** supporting both Darwin (FES) and Doodson (GOT)
  harmonic notation systems
- **Configurable inference modes** (zero, linear, spline, Fourier) for minor
  constituent estimation

## Try It -- No Installation Required

**[Launch interactive notebooks in Binder](https://mybinder.org/v2/gh/CNES/aviso-fes/main?urlpath=lab/tree/notebooks/auto_examples)**
to explore examples including tidal prediction, harmonic analysis,
interpolation techniques, and engine comparison. Everything runs in the cloud.

## Installation

### From conda-forge

```bash
conda install -c conda-forge pyfes
```

### From source

PyFES requires a C++14 compiler and CMake:

```bash
git clone https://github.com/CNES/aviso-fes.git
cd aviso-fes
pip install -e .
```

## Quick Start

### Predicting tides from a tidal atlas

Create a YAML configuration file describing your tidal model:

```yaml
engine: darwin
tide:
  cartesian:
    paths:
      M2: ${FES_DATA}/M2_tide.nc
      S2: ${FES_DATA}/S2_tide.nc
      K1: ${FES_DATA}/K1_tide.nc
      O1: ${FES_DATA}/O1_tide.nc
```

Then load and predict:

```python
import numpy as np
import pyfes

config = pyfes.config.load('ocean_tide.yaml')

dates = np.arange(
    np.datetime64('2024-01-01'),
    np.datetime64('2024-01-02'),
    np.timedelta64(1, 'h'),
)
lons = np.full(dates.shape, -7.688)
lats = np.full(dates.shape, 59.195)

tide, lp, flags = pyfes.evaluate_tide(
    config.models['tide'], dates, lons, lats,
    settings=config.settings,
)
total_tide = tide + lp  # in the same units as the tidal atlas
```

### Predicting from known constituents

```python
constituents = {
    'M2': (205.1, 109.0),  # (amplitude_cm, phase_deg)
    'S2': (74.9, 148.3),
    'K1': (6.4, 75.1),
    'O1': (6.6, 327.9),
}

tide, lp = pyfes.evaluate_tide_from_constituents(
    constituents, dates, latitude=48.38,
)
```

## Prediction Engines

PyFES provides two prediction engines to support different tidal atlas formats:

| Feature | FES/Darwin | PERTH/Doodson |
|---|---|---|
| **YAML key** | `engine: darwin` | `engine: perth` |
| **Notation** | Darwin notation | Doodson numbers |
| **Constituents** | 99 | 80 |
| **Nodal corrections** | Individual Schureman factors | Individual (group modulations optional) |
| **Default inference** | `SPLINE` | `LINEAR` |
| **Compatible atlases** | FES2014, FES2022 | GOT4.10, GOT5.5, GOT5.6 |

Both engines share the same high-level API and support the same set of
configurable inference types. The choice depends on your tidal atlas format:
**FES atlases use the Darwin engine**, while **GOT atlases use the PERTH
engine**.

See the [Prediction Engines documentation](https://cnes.github.io/aviso-fes/engines.html)
for a detailed comparison and usage guidance.

## Tidal Constituents

PyFES provides two catalogues of tidal constituents, one for each prediction
engine. Every constituent is characterised by its name, angular speed, and XDO
notation encoding.

- **Darwin catalogue** -- 99 constituents used by the FES/Darwin engine. See
  the [full list](https://cnes.github.io/aviso-fes/constituents/darwin.html).
- **Doodson catalogue** -- 80 constituents used by the PERTH/Doodson engine.
  See the [full list](https://cnes.github.io/aviso-fes/constituents/perth.html).

## About FES2022

This package is the fully revised version of the FES2022 distribution, including
both the PyFES prediction software and access to the
[FES2022 tides databases](data/fes2022b/README.md). FES2022 represents the
state-of-the-art in global tidal modeling, with improved accuracy especially for
satellite altimetry applications.

A full technical description is available in the
[FES2022 handbook](https://www.aviso.altimetry.fr/fileadmin/documents/data/tools/hdbk_FES2022.pdf)
and the scientific paper (Lyard et al. 2024).

## Documentation

The complete documentation is available on
[GitHub Pages](https://cnes.github.io/aviso-fes/), including:

- [Getting Started](https://cnes.github.io/aviso-fes/getting_started.html) --
  installation and quickstart examples
- [User Guide](https://cnes.github.io/aviso-fes/user_guide.html) --
  configuration, settings, inference modes, and prediction functions
- [Prediction Engines](https://cnes.github.io/aviso-fes/engines.html) --
  Darwin vs Doodson engine comparison
- [Tidal Constituents](https://cnes.github.io/aviso-fes/constituents/index.html) --
  complete constituent catalogues
- [Theoretical Background](https://cnes.github.io/aviso-fes/theory/index.html) --
  mathematical foundations of the harmonic method
- [API Reference](https://cnes.github.io/aviso-fes/api.html) --
  detailed function documentation
- [Examples](https://cnes.github.io/aviso-fes/auto_examples/index.html) --
  worked examples with plots

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
> databases -- you will need to implement your own reader to access them.*

## Credits

When using FES2022, please mention: *FES2022 was produced by LEGOS, NOVELTIS
and CLS Ocean and Climate Division; the project was funded by CNES. It is
distributed by AVISO, with support from CNES (http://www.aviso.altimetry.fr/)*

## References

* Lyard, F., Carrere, L., Fouchet, E., Cancet, M., Greenberg, D., Dibarboure,
  G., and Picot, N.: FES2022 a step towards a SWOT-compliant tidal correction,
  Submitted to J. Geophy. Res., in review, 2025

* Lyard, F. H., Allain, D. J., Cancet, M., Carrere, L., and Picot, N.: FES2014
  global ocean tide atlas: design and performance, Ocean Sci., 17, 615-649,
  https://doi.org/10.5194/os-17-615-2021, 2021.

* Carrere L., F. Lyard, M. Cancet, A. Guillot, N. Picot: *FES 2014, a new tidal
  model - Validation results and perspectives for improvements*, presentation
  to ESA Living Planet Conference, Prague 2016.

## Contact

Questions, suggestions, or need support? Reach out to the AVISO team:
[aviso@altimetry.fr](mailto:aviso@altimetry.fr)
