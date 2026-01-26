# Copyright (c) 2026 CNES
#
# All rights reserved. Use of this source code is governed by a
# BSD-style license that can be found in the LICENSE file.
"""PyFES: Python interface to FES tidal models.

PyFES is a Python library that provides an interface to the FES (Finite
Element Solution) tidal models developed by CNES (Centre National d'Études
Spatiales) and PERTH5 tidal model developed by Richard Ray. These models are
widely used in oceanography and geophysics for predicting tidal variations in
sea level.
"""

from collections.abc import Mapping
from .core.tidal_model import (
    AbstractTidalModelComplex128,
    AbstractTidalModelComplex64,
)
from .core import (
    Formulae as Formulae,
    AstronomicAngle as AstronomicAngle,
    FesRuntimeSettings,
    PerthRuntimeSettings,
    TidalType as TidalType,
    InterpolationType as InterpolationType,
)
from . import core
from .type_hints import (
    VectorDateTime64,
    VectorFloat64,
    VectorInt8,
)
from .core import darwin, perth

__all__ = [
    'AstronomicAngle',
    'FesRuntimeSettings',
    'Formulae',
    'InterpolationType',
    'PerthRuntimeSettings',
    'TidalType',
    'darwin',
    'evaluate_equilibrium_long_period',
    'evaluate_tide',
    'evaluate_tide_from_constituents',
    'perth',
]


def evaluate_tide(
    tidal_model: AbstractTidalModelComplex128 | AbstractTidalModelComplex64,
    date: VectorDateTime64,
    longitude: VectorFloat64,
    latitude: VectorFloat64,
    *,
    settings: FesRuntimeSettings | PerthRuntimeSettings | None = None,
) -> tuple[VectorFloat64, VectorFloat64, VectorInt8]:
    """Compute the tide at the given location and time.

    Args:
        tidal_model: Tidal models used to interpolate the modeled waves.
        date: Date of the tide calculation.
        longitude: Longitude in degrees for the position at which the tide is
            calculated.
        latitude: Latitude in degrees for the position at which the tide is
            calculated.
        settings: Settings used for the tide calculation. Using
          :py:class:`FesRuntimeSettings` runs the FES prediction engine, and
          :py:class:`PerthRuntimeSettings` runs the PERTH5 prediction
          engine. The default value is None, which corresponds to the
          default settings of the FES model.

    Returns:
        * The height of the diurnal and semi-diurnal constituents of the
          tidal spectrum (cm)
        * The height of the long period wave constituents of the tidal
          spectrum (cm)
        * The quality flag indicating the reliability of the tide
          calculation at the given position:

          - **0**: the tide is undefined (no model data available at the
            given position).
          - **Positive values**: the tide is interpolated at the given
            position using ``N`` data points (where ``N`` is the
            number of data points used for the interpolation).
          - **Negative values**: the tide is extrapolated at the given
            position using ``-N`` data points (where ``N`` is the
            number of data points used for the extrapolation).

    Note:
      Computed height of the diurnal and semi-diurnal constituents is set
      to nan if no data is available at the given position. The long period wave
      constituents is always computed because this value does not depend on
      model data.

    """
    return core.evaluate_tide(
        tidal_model,  # type: ignore[arg-type]
        date,
        longitude,
        latitude,
        settings,  # type: ignore[arg-type]
    )


def evaluate_tide_from_constituents(
    constituents: Mapping[str, tuple[float, float]],
    date: VectorDateTime64,
    latitude: float,
    *,
    settings: FesRuntimeSettings | PerthRuntimeSettings | None = None,
) -> tuple[VectorFloat64, VectorFloat64]:
    """Compute the tide from known tidal constituents.

    Unlike :py:func:`evaluate_tide` which interpolates constituents from a
    tidal model, this function computes the tidal prediction directly from a
    list of tidal constituents whose properties (amplitude and phase) are known.
    This is typically used for tide gauge analysis and prediction, where the
    constituents have been previously determined from harmonic analysis of
    observed sea level data.

    Args:
        constituents: Dictionary mapping tidal constituents to their
            (amplitude, phase) properties. Amplitude is in centimeters and
            phase is in degrees.
        date: Date of the tide calculation.
        latitude: Latitude in degrees for the position.
        settings: Settings used for the tide calculation. Using
          :py:class:`FesRuntimeSettings` runs the FES prediction engine, and
          :py:class:`PerthRuntimeSettings` runs the PERTH5 prediction
          engine. The default value is None, which corresponds to the
          default settings of the FES model.

    Returns:
        * The height of the diurnal and semi-diurnal constituents of the
          tidal spectrum (cm)
        * The height of the long period wave constituents of the tidal
          spectrum (cm)

    Example:
        >>> import pyfes
        >>> import numpy as np
        >>> # Define constituents from harmonic analysis
        >>> constituents = {
        ...     'M2': (50.0, 120.0),  # 50cm amplitude, 120° phase
        ...     'S2': (20.0, 90.0),  # 20cm amplitude, 90° phase
        ...     'K1': (15.0, 45.0),  # 15cm amplitude, 45° phase
        ... }
        >>> # Calculate tide at a specific location over time
        >>> dates = np.arange(
        ...     np.datetime64('2024-01-01'),
        ...     np.datetime64('2024-01-02'),
        ...     np.timedelta64(1, 'h'),
        ... )
        >>> tide, lp = pyfes.evaluate_tide_from_constituents(
        ...     constituents, dates, -5.0, 48.0
        ... )

    Note:
        The constituents dictionary should map each
        :py:class:`pyfes.Constituent` to a tuple of (amplitude, phase). The
        amplitude and phase values are typically obtained from prior harmonic
        analysis of observed tide gauge data.

    """
    return core.evaluate_tide_from_constituents(
        constituents,  # type: ignore[arg-type]
        date,
        latitude,
        settings,  # type: ignore[arg-type]
    )


def evaluate_equilibrium_long_period(
    date: VectorDateTime64,
    latitude: VectorFloat64,
    *,
    settings: FesRuntimeSettings | None = None,
) -> VectorFloat64:
    """Compute the long period ocean tides.

    The complete tidal spectral lines from the Cartwright-Tayler-Edden
    tables are summed over to compute the long-period tide.

    Order 2 and order 3 of the tidal potential for the long period waves is
    now taken into account.

    The decomposition was validated compared to the potential proposed by
    Tamura.

    Technical references:
    - Cartwright & Tayler, Geophys. J. R.A.S., 23, 45, 1971.
    - Cartwright & Edden, Geophys. J. R.A.S., 33, 253, 1973.
    - Tamura Y., Bull. d'information des marees terrestres, Vol. 99, 1987.

    Args:
        date: Date of the tide calculation.
        latitude: Latitude in degrees for the position at which the tide is
            calculated.
        settings: Settings used for the tide calculation. See
            :py:class:`Settings` for more details.

    Returns:
        The height of the long period wave constituents of the tidal spectrum
        (cm).

    """
    return core.darwin.evaluate_equilibrium_long_period(
        date,
        latitude,
        settings,
    )
