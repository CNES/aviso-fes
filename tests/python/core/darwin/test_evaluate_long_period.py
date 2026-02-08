# Copyright (c) 2026 CNES
#
# All rights reserved. Use of this source code is governed by a
# BSD-style license that can be found in the LICENSE file.
import pathlib

import netCDF4
import numpy

from pyfes import core


DATASET_DIR = pathlib.Path(__file__).parent.parent.parent / 'dataset'
LONG_PERIOD = DATASET_DIR / 'equilibrium_long_period.nc'


def test_evaluate_equilibrium_long_period() -> None:
    """Test the evaluation of the long period tide."""
    with netCDF4.Dataset(LONG_PERIOD) as dataset:
        time_utc = dataset.variables['time'][:]
        latitude = dataset.variables['latitude'][:]
        ocean_tide_eq = dataset.variables['ocean_tide_eq'][:]

        time_utc = numpy.datetime64('2000-01-01') + numpy.round(
            time_utc * 1e6
        ).astype(numpy.int64).astype('m8[us]')

    computed = core.evaluate_equilibrium_long_period(time_utc, latitude, [])

    # In GDR product, equilibrium_long_period is set to 0 when the position is
    # on land
    mask = ocean_tide_eq == 0
    computed[mask] = numpy.nan
    ocean_tide_eq[mask] = numpy.nan

    numpy.testing.assert_allclose(
        computed * 1e-2, ocean_tide_eq, atol=0.0001, equal_nan=True
    )
