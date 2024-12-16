# Copyright (c) 2024 CNES
#
# All rights reserved. Use of this source code is governed by a
# BSD-style license that can be found in the LICENSE file.
"""Tidal model prediction library."""
from __future__ import annotations

from . import core
from .astronomic_angle import AstronomicAngle
from .config import load as load_config
from .core import Constituent, Formulae, Quality, constituents
from .leap_seconds import get_leap_seconds
from .typing import VectorDateTime64, VectorFloat64, VectorInt8
from .version import __version__
from .wave_table import WaveDict, WaveTable

__all__ = [
    '__version__',
    'constituents',
    'AstronomicAngle',
    'Constituent',
    'Formulae',
    'Quality',
    'load_config',
    'WaveDict',
    'WaveTable',
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
        excluded: The list of tidal constituents to be excluded from the model.
            Constituents included in this list will be processed through
            admittance calculations and in the long-period equilibrium wave
            calculation routine (``lpe_minus_n_waves``).

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
                 time_tolerance: float = 0.0,
                 excluded: list[str] | None = None) -> None:
        excluded = excluded or []
        super().__init__(
            astronomic_formulae,
            time_tolerance,
            [core.constituents.parse(constituent) for constituent in excluded],
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
        * A flag indicating if the tide is correctly estimated or not. Possible
          values are

          .. list-table:: Flag values
              :header-rows: 1

              * - Flag
                - Description
              * - ``0``
                - The tide is undefined because the position is outside the
                  domain of the tidal model or the numerical model is
                  undefined for the requested spatial position. This value
                  corresponds to the :py:attr:`pyfes.Quality.kUndefined`
                  quality flag.
              * - ``1``
                - The tide is extrapolated with the nearest mesh point. This
                  value corresponds to the
                  :py:attr:`pyfes.Quality.kExtrapolated1` quality flag.
              * - ``2``
                - The tide is extrapolated using a linear interpolation with
                  two surrounding grid points. This value corresponds to the
                  :py:attr:`pyfes.Quality.kExtrapolated2` quality flag.
              * - ``3``
                - The tide is extrapolated using a linear interpolation with
                  three surrounding grid points. This value corresponds to the
                  :py:attr:`pyfes.Quality.kExtrapolated3` quality flag.
              * - ``4``
                - The tide is correctly estimated. This value corresponds to
                  the :py:attr:`pyfes.Quality.kInterpolated` quality flag.

        .. note::

            The flag value ``2`` or ``3`` are only possible if the
            tidal model used is a Cartesian grid.
    """
    return core.evaluate_tide(
        tidal_model,  # type: ignore
        date,
        get_leap_seconds(date),
        longitude,
        latitude,
        settings,
        num_threads,
    )
