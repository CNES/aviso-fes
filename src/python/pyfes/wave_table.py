# Copyright (c) 2025 CNES
#
# All rights reserved. Use of this source code is governed by a
# BSD-style license that can be found in the LICENSE file.
"""Properties of tidal constituents."""
from __future__ import annotations

from typing import TYPE_CHECKING
import datetime

import numpy

from . import core
from .core import Formulae

if TYPE_CHECKING:
    from .type_hints import (
        VectorComplex128,
        VectorDateTime64,
        VectorFloat64,
        VectorUInt16,
    )

#: Maximum number of tidal constituents to display in the representation
MAX_CONSTITUENTS = 9


class WaveTable(core.WaveTable):
    """Properties of tidal constituents.
    
    The WaveTable manages tidal wave constituents, allows computation of nodal
    corrections, performs tidal predictions, and supports harmonic analysis.

    .. warning::

        The order of wave constituents in the resulting WaveTable does **NOT**
        match the order specified in the constructor's ``waves`` argument.
        Constituents are internally ordered according to the declaration order
        of the enumerated `Constituent
        <https://github.com/CNES/aviso-fes/blob/main/include/fes/constituent.hpp>`_
        fields.

        Example:
            >>> from pyfes import WaveTable, Constituent
            >>> wt = WaveTable (["M2", "S2", "N2", "K1"])
            >>> wt.keys()
            ['K1', 'N2', 'M2', 'S2']

        To determine the actual ordering of constituents, use the 
        :py:func:`~pyfes.constituents.known()` function.
    """

    def __repr__(self) -> str:
        constituents: list[str] = self.keys()
        if len(constituents) > MAX_CONSTITUENTS:
            constituents = constituents[:4] + ['...'] + constituents[-4:]

        return '{}.{}({})'.format(self.__class__.__module__,
                                  self.__class__.__name__,
                                  ', '.join(constituents))

    def compute_nodal_modulations(
        self,
        dates: list[datetime.datetime] | VectorDateTime64,
        leap_seconds: VectorUInt16,
        formulae: Formulae = Formulae.kSchuremanOrder1,
    ) -> tuple[VectorFloat64, VectorFloat64]:
        """Compute nodal modulations for amplitude and phase.

        Args:
            dates: Dates for which the nodal modulations are computed.
            leap_seconds: Leap seconds for the dates provided.
            formulae: Astronomic formulae used to evaluate the astronomic
                arguments for a given date. Default is
                :py:attr:`pyfes.Formulae.kSchuremanOrder1
                <pyfes.core.Formulae.kSchuremanOrder1>`.
        Returns:
            The nodal modulations for amplitude and phase.
        """
        if isinstance(dates, list) and all(
                isinstance(item, datetime.datetime) for item in dates):
            datetime64: VectorDateTime64 = numpy.array(
                [core.datemanip.as_int64(item) for item in dates],
                dtype='datetime64[us]')
            return super().compute_nodal_modulations(datetime64, leap_seconds)
        # The method throws an error if the dates are not datetime64
        return super().compute_nodal_modulations(
            dates,  # type: ignore[arg-type]
            leap_seconds,
            formulae)

    def freq(self) -> VectorFloat64:
        """Gets the waves frequencies in radians per seconds."""
        return numpy.array([wave.freq for wave in self], dtype=numpy.float64)

    @staticmethod
    def harmonic_analysis(
        h: VectorFloat64,
        f: VectorFloat64,
        vu: VectorFloat64,
    ) -> VectorComplex128:
        """Harmonic Analysis.
        The harmonic analysis method consists in expressing the ocean tidal
        variations as a sum of independent constituents accordingly to the
        tidal potential spectrum. Then the sea surface elevation at a point
        :math:`(x, y)` and time :math:`t` can be expressed as a linear sum as
        follow:

        .. math::

            S_{ap} = S_{0}(x, y) + \\sum_{k=0}^n f_{k}(t)S_{k}(x, y)
            \\times cos [\\omega_{k}t + {v}_{k}(t) + u_{k}(t) - G_{k}(x,y)]

        where:

        * :math:`n` is the number of constituents,
        * :math:`S_{0}(x, y)` is the mean sea level,
        * :math:`S_{k}(x, y)` is the amplitude of the constituent of index
          :math:`k`,
        * :math:`G_{k}(x, y)` is the phase lag relative to Greenwich time,
        * :math:`w_{k}` is the angular frequency of the constituent of
          index :math:`k`,
        * :math:`v_{k}` is the astronomical argument at time :math:`t`,
        * :math:`f_{k}(t)` is the nodal correction coefficient applied to
          the amplitude of the constituent of index :math:`k`,
        * :math:`u_{k}(t)` is the nodal correction coefficient applied to
          the phase of the constituent of index :math:`k`.

        The a priori analysis spectrum includes the most important astronomical
        constituents in the Darwin development, completed by Schureman in 1958,
        and many non-linear waves. Tidal constants and astronomical arguments
        are now derived from this software, incorporating legacy data from
        FES2014. A comprehensive definition of waves can also be found in
        Schureman (1958). This spectrum is the most commonly used for harmonic
        analysis due to the simplification given by the nodal correction concept
        (:math:`f` and :math:`u` coefficients above) which allows dealing with
        slow motions of the lunar ascending node and reducing the number of
        constituents in the tidal spectrum. More details about this harmonic
        analysis method can be found in Ponchaut et al. 1999.

        Args:
            h: Sea level.
            f: Nodal correction coefficient applied to the
                amplitude of the constituents analyzed.
            vu: Astronomical argument at time :math:`t` + the nodal correction
                coefficient applied to the phase of the constituents analyzed

        Returns:
            The complex number representing the different reconstructed waves.
        """
        return core.WaveTable.harmonic_analysis(h, f, vu)


class WaveDict(WaveTable):
    """Manages the tidal wave table as a dictionary."""

    def freq(self) -> dict[str, float]:
        """Gets the waves frequencies in radians per seconds."""
        return {wave.name(): wave.freq for wave in self}

    def harmonic_analysis(  # type: ignore[override]
        self,
        h: VectorFloat64,
        f: VectorFloat64,
        vu: VectorFloat64,
    ) -> dict[str, VectorComplex128]:
        """Harmonic Analysis.

        Args:
            h: Sea level.
            f: Nodal correction coefficient applied to the amplitude of the
                constituents analyzed.
            vu: Astronomical argument at time :math:`t` + the nodal correction
                coefficient applied to the phase of the constituents analyzed

        Returns:
            A mapping between the wave name and its complex number representing
            it.
        .. seealso::
            :py:meth:`WaveTable.harmonic_analysis`
        """
        analysis: VectorComplex128 = super().harmonic_analysis(h, f, vu)
        return dict(zip(self.keys(), analysis))

    def tide_from_tide_series(
            self,
            dates: VectorDateTime64,
            leap_seconds: VectorUInt16,
            wave: dict[str, VectorComplex128],
            formulae: Formulae = Formulae.kSchuremanOrder3) -> VectorFloat64:
        """Calculates the tide of a given time series.

        Args:
            dates: time series data
            leap_seconds: Leap seconds for the dates provided.
            formulae: Astronomic formulae used to evaluate the astronomic
                arguments for a given date. Default is
                :py:attr:`Formulae.kSchuremanOrder3`.
            wave: Tidal wave properties.

        Returns:
            The tide calculated for the time series provided.
        """
        if len(wave) != len(self):
            raise ValueError('wave must contain as many items as tidal '
                             'constituents loaded')
        wave_properties: VectorComplex128 = numpy.array(
            [wave[item] for item in self])
        return super().tide_from_tide_series(dates, leap_seconds,
                                             wave_properties, formulae)
