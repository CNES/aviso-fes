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

from . import config as config, core
from .core import (
    DARWIN as DARWIN,
    DOODSON as DOODSON,
    FOURIER as FOURIER,
    LINEAR as LINEAR,
    RADIAL as RADIAL,
    SPLINE as SPLINE,
    TIDE as TIDE,
    ZERO as ZERO,
    AstronomicAngle as AstronomicAngle,
    Axis as Axis,
    EngineType as EngineType,
    FesRuntimeSettings as FesRuntimeSettings,
    Formulae as Formulae,
    FrequencyUnit as FrequencyUnit,
    InferenceType as InferenceType,
    PerthRuntimeSettings as PerthRuntimeSettings,
    Settings as Settings,
    TidalModelInterfaceComplex64,
    TidalModelInterfaceComplex128,
    TideType as TideType,
    WaveInterface as WaveInterface,
    WaveTableInterface as WaveTableInterface,
    WaveType as WaveType,
    darwin as darwin,
    generate_markdown_table as generate_markdown_table,
    known_constituents as known_constituents,
    parse_constituent as parse_constituent,
    perth as perth,
    tidal_model as tidal_model,
    wave_table_factory as wave_table_factory,
)
from .type_hints import VectorDateTime64, VectorFloat64, VectorInt8
from .version import __version__  # noqa: F401


__all__ = [
    'DARWIN',
    'DOODSON',
    'FOURIER',
    'LINEAR',
    'RADIAL',
    'SPLINE',
    'TIDE',
    'ZERO',
    'AstronomicAngle',
    'Axis',
    'EngineType',
    'FesRuntimeSettings',
    'Formulae',
    'FrequencyUnit',
    'InferenceType',
    'PerthRuntimeSettings',
    'Settings',
    'TideType',
    'WaveInterface',
    'WaveTableInterface',
    'WaveType',
    'config',
    'darwin',
    'evaluate_equilibrium_long_period',
    'evaluate_tide',
    'evaluate_tide_from_constituents',
    'generate_markdown_table',
    'known_constituents',
    'parse_constituent',
    'perth',
    'tidal_model',
    'wave_table_factory',
]


def evaluate_tide(
    tidal_model: TidalModelInterfaceComplex128 | TidalModelInterfaceComplex64,
    date: VectorDateTime64,
    longitude: VectorFloat64,
    latitude: VectorFloat64,
    *,
    settings: Settings | None = None,
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
          :py:class:`FesRuntimeSettings` runs the Darwin prediction engine
          and :py:class:`PerthRuntimeSettings` runs the PERTH prediction engine.

    Returns:
        * The height of the diurnal and semi-diurnal constituents of the
          tidal spectrum (same unit as the tidal model, typically cm)
        * The height of the long period wave constituents of the tidal
          spectrum (same unit as the tidal model, typically cm)
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

    Example:
        >>> import pyfes
        >>> import numpy as np
        >>> # 1. Load the model (requires a config file)
        >>> model = pyfes.config.load('path/to/config.yaml')
        >>> # 2. Define space and time
        >>> dates = np.array(['2025-01-01T12:00:00'], dtype='datetime64[us]')
        >>> lons = np.array([0.0])
        >>> lats = np.array([45.0])
        >>> # 3. Compute tide
        >>> tide, lp, flags = pyfes.evaluate_tide(model, dates, lons, lats)
        >>> total_height = tide + lp

    Note:
      Computed height of the diurnal and semi-diurnal constituents is set
      to nan if no data is available at the given position. The long period wave
      constituents is always computed because this value does not depend on
      model data.

    """
    return core.evaluate_tide(
        tidal_model,
        date,
        longitude,
        latitude,
        settings,
    )


def evaluate_tide_from_constituents(
    constituents: Mapping[str, tuple[float, float]],
    date: VectorDateTime64,
    latitude: float,
    *,
    settings: Settings | None = None,
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
            (amplitude, phase) properties. Amplitude is in metric units and
            phase is in degrees.
        date: Date of the tide calculation.
        latitude: Latitude in degrees for the position.
        settings: Settings used for the tide calculation. Using
          :py:class:`FesRuntimeSettings` runs the Darwin prediction engine, and
          :py:class:`PerthRuntimeSettings` runs the PERTH prediction
          engine. The default value is None, which corresponds to the
          default settings for FES models.

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
        ...     constituents, dates, 48.0
        ... )

    Note:
        The constituents dictionary should map each
        :py:class:`pyfes.Constituent` to a tuple of (amplitude, phase). The
        amplitude and phase values are typically obtained from prior harmonic
        analysis of observed tide gauge data.

    """
    return core.evaluate_tide_from_constituents(
        constituents,
        date,
        latitude,
        settings,
    )


def evaluate_equilibrium_long_period(
    date: VectorDateTime64,
    latitude: VectorFloat64,
    *,
    constituents: list[str] | None = None,
    settings: FesRuntimeSettings | None = None,
) -> VectorFloat64:
    """Compute the long-period equilibrium ocean tides.

    This calculates the geometric tidal potential (the "theoretical" tide)
    created by the attraction of the Moon and Sun over long periods
    (fortnightly, monthly, semi-annual, etc.). It sums the spectral lines from
    the Cartwright-Tayler-Edden tables.

    Use this function if you need the purely astronomic long-period component
    independent of a specific tidal atlas.

    Args:
        date: Date of the tide calculation.
        latitude: Latitude in degrees for the position at which the tide is
            calculated.
        constituents: List of constituents to remove from the inferred table
            of long period waves. If None, all constituents are included in the
            calculation.
        settings: Settings used for the tide calculation. See
            :py:class:`Settings` for more details.

    Returns:
        The height of the long period wave constituents of the tidal spectrum
        (cm).

    Example:
        >>> import pyfes
        >>> import numpy as np
        >>> dates = np.array(['2025-01-01'], dtype='datetime64[us]')
        >>> lats = np.array([45.0])
        >>> # Calculate full LP tide
        >>> lp_tide = pyfes.evaluate_equilibrium_long_period(dates, lats)

    References:
        - Cartwright & Tayler, Geophys. J. R.A.S., 23, 45, 1971.
        - Cartwright & Edden, Geophys. J. R.A.S., 33, 253, 1973.
        - Tamura Y., Bull. d'information des marees terrestres, Vol. 99, 1987.

    """
    return core.evaluate_equilibrium_long_period(
        date,
        latitude,
        constituents or [],
        settings,
    )
