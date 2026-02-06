# Copyright (c) 2026 CNES
#
# All rights reserved. Use of this source code is governed by a
# BSD-style license that can be found in the LICENSE file.
"""Astronomic angle computation."""

from __future__ import annotations

import datetime

from . import core
from .core import Formulae


class AstronomicAngle(core.AstronomicAngle):
    """Initialize some astronomic data useful for nodal corrections.

    In tidal work the only celestial bodies that need to be considered are the
    moon and sun. Although every other celestial body whose gravitational
    influence reaches the Earth create a theoretical tide producing force, the
    greater distance or smaller size of such body renders negligible any effect
    of this force upon the tides of the Earth. In deriving mathematical
    expressions for the tide-producing forces of the moon and sun, the principal
    factors to be taken into consideration are the rotation of the earth, the
    volution of the moon around the earth, the revolution of the earth around
    the sun, the inclination of the moon's orbit to the earth's equator, and the
    obliquity of the ecliptic.

    Args:
        formulae: Astronomic formulae used to evaluate the astronomic arguments
            for a given date. Default is
            :py:attr:`pyfes.Formulae.SCHUREMAN_ORDER_1
            <pyfes.core.Formulae.SCHUREMAN_ORDER_1>`.

    """

    def __init__(self, formulae: Formulae = Formulae.SCHUREMAN_ORDER_1) -> None:
        """Initialize the AstronomicAngle."""
        super().__init__(formulae)
        self.date: datetime.datetime | None = None

    def update(  # type: ignore[override]
        self,
        date: datetime.datetime | None = None,
    ) -> None:
        """Update the astronomic angles.

        Args:
            date: Date for which the astronomic angles are computed. If None,
                the current date is used.

        """
        if date is None:
            date = datetime.datetime.utcnow()
        super().update(date)
        self.date = date

    def __repr__(self) -> str:
        """Return the string representation of the AstronomicAngle."""
        return f'{self.__class__.__name__}({self.date})'
