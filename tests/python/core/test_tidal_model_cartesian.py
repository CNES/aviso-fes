# Copyright (c) 2026 CNES
#
# All rights reserved. Use of this source code is governed by a
# BSD-style license that can be found in the LICENSE file.
import pathlib

import netCDF4
import numpy
import pytest

from pyfes import core


S2 = pathlib.Path(__file__).parent.parent / 'dataset' / 'S2_tide.nc'


def _expected_value(lon: numpy.ndarray, lat: numpy.ndarray) -> numpy.ndarray:
    """A simple function to generate expected values for testing."""
    return (lon + 2.0 * lat) + 1j * (lon - 2.0 * lat)


def _make_wave(lon: numpy.ndarray, lat: numpy.ndarray) -> numpy.ndarray:
    """Generate a wave based on the expected value function."""
    mx, my = numpy.meshgrid(lon, lat, indexing='ij')
    return _expected_value(mx, my).ravel()


def test_interpolate() -> None:
    """Test interpolation of tidal model."""
    with netCDF4.Dataset(S2) as nc:
        lon = nc.variables['lon'][:]
        lat = nc.variables['lat'][:]
        amp = numpy.ma.filled(
            numpy.radians(nc.variables['amplitude'][:]), numpy.nan
        )
        pha = numpy.ma.filled(
            numpy.radians(nc.variables['phase'][:]), numpy.nan
        )

    # Create a tidal model
    x_axis = core.Axis(lon, is_periodic=True)
    y_axis = core.Axis(lat)

    amp = amp.astype(numpy.float64)
    pha = pha.astype(numpy.float64)

    wave = amp * numpy.cos(pha) + 1j * amp * numpy.sin(pha)

    model = core.tidal_model.CartesianComplex128(
        x_axis, y_axis, longitude_major=False
    )
    assert len(model) == 0
    assert bool(model) is False
    model.add_constituent('S2', wave.ravel())
    assert len(model) == 1
    assert bool(model) is True
    assert model.identifiers() == ['S2']

    x = numpy.arange(-180, 180, 1.0, dtype=numpy.float64)
    y = numpy.arange(-89.5, 90, 1.0, dtype=numpy.float64)

    mx, my = numpy.meshgrid(x, y, indexing='ij')

    result, quality = model.interpolate(mx.ravel(), my.ravel())

    interpolated = result['S2']
    interpolated[quality[quality <= 0]] = numpy.nan
    interpolated = interpolated.reshape(mx.shape).T
    interpolated = numpy.roll(interpolated, 180, axis=1)

    # Compare with the original data
    assert numpy.nanmean(
        numpy.absolute(wave) - numpy.absolute(interpolated)
    ) == pytest.approx(0, rel=1e-6)

    # Interpolation test without adding a waveform
    model.clear()
    assert len(model) == 0
    result, quality = model.interpolate(mx.ravel(), my.ravel())
    assert result == {}
    assert numpy.all(quality == 0)


def test_resample_more_points() -> None:
    """Test resampling of tidal model to a finer grid."""
    origin_lon = numpy.arange(0.0, 2.01, 1.0)
    origin_lat = numpy.arange(0.0, 2.01, 1.0)
    target_lon = numpy.arange(0.0, 2.01, 0.5)
    target_lat = numpy.arange(0.0, 2.01, 0.5)

    model = core.tidal_model.CartesianComplex128(
        core.Axis(target_lon), core.Axis(target_lat)
    )

    wave = _make_wave(origin_lon, origin_lat)
    resampled = model.resample(
        core.Axis(origin_lon), core.Axis(origin_lat), wave, True, 1
    )

    expected = _make_wave(target_lon, target_lat)
    numpy.testing.assert_allclose(resampled, expected, rtol=1e-12, atol=1e-12)


def test_resample_less_points() -> None:
    """Test resampling of tidal model to a coarser grid."""
    origin_lon = numpy.arange(0.0, 2.01, 0.5)
    origin_lat = numpy.arange(0.0, 2.01, 0.5)
    target_lon = numpy.arange(0.0, 2.01, 1.0)
    target_lat = numpy.arange(0.0, 2.01, 1.0)

    model = core.tidal_model.CartesianComplex128(
        core.Axis(target_lon), core.Axis(target_lat)
    )

    wave = _make_wave(origin_lon, origin_lat)
    resampled = model.resample(
        core.Axis(origin_lon), core.Axis(origin_lat), wave, True, 1
    )

    expected = _make_wave(target_lon, target_lat)
    numpy.testing.assert_allclose(resampled, expected, rtol=1e-12, atol=1e-12)
