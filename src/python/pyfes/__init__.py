# Copyright (c) 2025 CNES
#
# All rights reserved. Use of this source code is governed by a
# BSD-style license that can be found in the LICENSE file.
"""Tidal model prediction library."""
from __future__ import annotations

from typing import TYPE_CHECKING

from . import core
from .astronomic_angle import AstronomicAngle
from .config import load as load_config
from .core import Constituent, Formulae, constituents
from .leap_seconds import get_leap_seconds
from .version import __version__
from .wave_table import WaveDict, WaveTable

if TYPE_CHECKING:
    from .type_hints import VectorDateTime64, VectorFloat64, VectorInt8

__all__ = [
    'AstronomicAngle',
    'Constituent',
    'Formulae',
    'WaveDict',
    'WaveTable',
    '__version__',
    'constituents',
    'load_config',
]


class Settings(core.Settings):
    """Tide prediction settings.

    Args:
        astronomic_formulae: Astronomic formulae used to evaluate the astronomic
            arguments for a given date. Default is
            :py:attr:`pyfes.Formulae.kSchuremanOrder1
            <pyfes.core.Formulae.kSchuremanOrder1>`.
        time_tolerance: time_tolerance The time in seconds during which
            astronomical angles are considered constant. The default value is
            0 seconds, indicating that astronomical angles do not remain
            constant with time.

    .. note::

        The parameter ``time_tolerance`` allows for the adjustment of
        astronomical angle calculations. When its value is set to zero, the
        angles will be recalculated each time the date changes. Otherwise, they
        will be considered valid as long as the time difference between the last
        evaluation and the current date is within the specified tolerance.

        In this case, it's important to consider that the tidal evaluation may
        vary slightly, within an epsilon range, for the same period when
        calculated using a different number of threads. This is because the
        period will be divided differently based on the number of threads,
        resulting in variations in the way angles are updated depending on the
        number of threads used.
    """

    def __init__(self,
                 *,
                 astronomic_formulae: Formulae = Formulae.kSchuremanOrder1,
                 time_tolerance: float = 0.0) -> None:
        super().__init__(
            astronomic_formulae,
            time_tolerance,
        )


def evaluate_tide(
    tidal_model: core.AbstractTidalModelComplex128
    | core.AbstractTidalModelComplex64,
    date: VectorDateTime64,
    longitude: VectorFloat64,
    latitude: VectorFloat64,
    *,
    settings: Settings | None = None,
    num_threads: int = 0,
) -> tuple[VectorFloat64, VectorFloat64, VectorInt8]:
    """Compute the tide at the given location and time.

    Args:
        tidal_model: Tidal models used to interpolate the modeled waves.
        date: Date of the tide calculation.
        longitude: Longitude in degrees for the position at which the tide is
            calculated.
        latitude: Latitude in degrees for the position at which the tide is
            calculated.
        settings: Settings used for the tide calculation. See
            :py:class:`Settings` for more details.
        num_threads: Number of threads to use for the calculation. If 0, all
            available threads are used.

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

    .. note::

      Computed height of the diurnal and semi-diurnal constituents is set
      to nan if no data is available at the given position. The long period wave
      constituents is always computed because this value does not depend on
      model data.
    """
    return core.evaluate_tide(
        tidal_model,  # type: ignore[arg-type]
        date,
        get_leap_seconds(date),
        longitude,
        latitude,
        settings,
        num_threads,
    )


def evaluate_equilibrium_long_period(
    date: VectorDateTime64,
    latitude: VectorFloat64,
    *,
    settings: Settings | None = None,
    num_threads: int = 0,
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
        num_threads: Number of threads to use for the calculation. If 0, all
            available threads are used.

    Returns:
        The height of the long period wave constituents of the tidal spectrum
        (cm).
    """
    return core.evaluate_equilibrium_long_period(
        date,
        get_leap_seconds(date),
        latitude,
        settings,
        num_threads,
    )
