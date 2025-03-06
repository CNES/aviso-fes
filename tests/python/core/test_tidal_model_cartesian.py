# Copyright (c) 2025 CNES
#
# All rights reserved. Use of this source code is governed by a
# BSD-style license that can be found in the LICENSE file.
import pathlib

import netCDF4
import numpy
from pyfes import core
import pytest

S2 = pathlib.Path(__file__).parent.parent / 'dataset' / 'S2_tide.nc'


def test_interpolate():
    """Test interpolation of tidal model."""
    with netCDF4.Dataset(S2) as nc:
        lon = nc.variables['lon'][:]
        lat = nc.variables['lat'][:]
        amp = numpy.ma.filled(numpy.radians(nc.variables['amplitude'][:]),
                              numpy.nan)
        pha = numpy.ma.filled(numpy.radians(nc.variables['phase'][:]),
                              numpy.nan)

    # Create a tidal model
    x_axis = core.Axis(lon, is_circular=True)
    y_axis = core.Axis(lat)

    amp = amp.astype(numpy.float64)
    pha = pha.astype(numpy.float64)

    wave = amp * numpy.cos(pha) + 1j * amp * numpy.sin(pha)

    model = core.tidal_model.CartesianComplex128(x_axis,
                                                 y_axis,
                                                 longitude_major=False)
    assert len(model) == 0
    assert bool(model) is False
    model.add_constituent('S2', wave.ravel())
    assert len(model) == 1
    assert bool(model) is True
    assert model.identifiers() == [core.kS2]

    x = numpy.arange(-180, 180, 1.0, dtype=numpy.float64)
    y = numpy.arange(-89.5, 90, 1.0, dtype=numpy.float64)

    mx, my = numpy.meshgrid(x, y, indexing='ij')

    result, quality = model.interpolate(mx.ravel(), my.ravel())

    interpolated = result[core.kS2]
    interpolated[quality[quality != core.kInterpolated]] = numpy.nan
    interpolated = interpolated.reshape(mx.shape).T
    interpolated = numpy.roll(interpolated, 180, axis=1)

    # Compare with the original data
    assert numpy.nanmean(numpy.absolute(wave) -
                         numpy.absolute(interpolated)) == pytest.approx(
                             0, rel=1e-6)

    # Interpolation test without adding a waveform
    model.clear()
    assert len(model) == 0
    result, quality = model.interpolate(mx.ravel(), my.ravel())
    assert result == {}
    assert numpy.all(quality == core.kUndefined)
