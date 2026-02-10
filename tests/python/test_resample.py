# Copyright (c) 2026 CNES
#
# All rights reserved. Use of this source code is governed by a
# BSD-style license that can be found in the LICENSE file.
"""Tests for the resampling of Cartesian tidal models.

This module tests:
  - GridProperties.__ne__: correct detection of grid mismatches.
  - Cartesian.load(): automatic resampling when constituents live on different
    grids, verified through the YAML configuration path.
"""

import pathlib
import warnings

import netCDF4
import numpy
import pytest

import pyfes
from pyfes.config import GridProperties


DATASET = pathlib.Path(__file__).parent / 'dataset'


# ---------------------------------------------------------------------------
# Helpers
# ---------------------------------------------------------------------------


def _create_tide_nc(
    path: str,
    lon: numpy.ndarray,
    lat: numpy.ndarray,
    amplitude: numpy.ndarray,
    phase: numpy.ndarray,
) -> None:
    """Write a minimal tide NetCDF file compatible with the config loader."""
    with netCDF4.Dataset(path, 'w') as ds:
        ds.createDimension('lon', len(lon))
        ds.createDimension('lat', len(lat))

        v = ds.createVariable('lon', 'f4', ('lon',))
        v.units = 'degrees_east'
        v[:] = lon

        v = ds.createVariable('lat', 'f4', ('lat',))
        v.units = 'degrees_north'
        v[:] = lat

        v = ds.createVariable('amplitude', 'f4', ('lat', 'lon'))
        v.units = 'cm'
        v[:] = amplitude

        v = ds.createVariable('phase', 'f4', ('lat', 'lon'))
        v.units = 'degrees'
        v[:] = phase


def _make_amplitude(lon: numpy.ndarray, lat: numpy.ndarray) -> numpy.ndarray:
    """Bilinear amplitude field: exact under bilinear interpolation."""
    mx, my = numpy.meshgrid(lat, lon, indexing='ij')
    return 1.0 + 0.01 * mx + 0.005 * my


def _make_phase(lon: numpy.ndarray, lat: numpy.ndarray) -> numpy.ndarray:
    """Constant phase field so that resampling is exact."""
    return numpy.full((len(lat), len(lon)), 45.0, dtype=numpy.float32)


# ---------------------------------------------------------------------------
# GridProperties.__ne__
# ---------------------------------------------------------------------------


class TestGridProperties:
    """Tests for the GridProperties inequality operator."""

    def setup_method(self) -> None:
        self.base = GridProperties(
            dtype=numpy.dtype(numpy.complex64),
            lon=numpy.arange(0.0, 10.0, 1.0),
            lat=numpy.arange(-5.0, 5.0, 1.0),
            shape=(10, 10),
            longitude_major=True,
        )

    def test_equal(self) -> None:
        """Identical properties must compare as equal."""
        other = GridProperties(
            dtype=numpy.dtype(numpy.complex64),
            lon=numpy.arange(0.0, 10.0, 1.0),
            lat=numpy.arange(-5.0, 5.0, 1.0),
            shape=(10, 10),
            longitude_major=True,
        )
        assert not (self.base != other)

    def test_different_shape(self) -> None:
        """Differing shape must be detected."""
        other = self.base._replace(shape=(20, 20))
        assert self.base != other

    def test_different_longitude_major(self) -> None:
        """Differing storage order must be detected."""
        other = self.base._replace(longitude_major=False)
        assert self.base != other

    def test_different_lon(self) -> None:
        """Differing longitude values must be detected."""
        other = self.base._replace(
            lon=numpy.arange(1.0, 11.0, 1.0),
        )
        assert self.base != other

    def test_different_lat(self) -> None:
        """Differing latitude values must be detected."""
        other = self.base._replace(
            lat=numpy.arange(0.0, 10.0, 1.0),
        )
        assert self.base != other

    def test_different_lon_size(self) -> None:
        """Differing longitude size triggers shape mismatch first."""
        other = self.base._replace(
            lon=numpy.arange(0.0, 10.0, 0.5),
            shape=(20, 10),
        )
        assert self.base != other

    def test_different_dtype_only(self) -> None:
        """Dtype alone does not trigger inequality (excluded by design)."""
        other = self.base._replace(dtype=numpy.complex128)
        assert not (self.base != other)

    def test_not_implemented(self) -> None:
        """Comparing with a non-GridProperties object returns NotImplemented."""
        assert self.base.__ne__('not a GridProperties') is NotImplemented

    def test_repr(self) -> None:
        """__repr__ must contain the key information."""
        r = repr(self.base)
        assert 'GridProperties' in r
        assert 'shape=(10, 10)' in r


# ---------------------------------------------------------------------------
# Config-level resampling
# ---------------------------------------------------------------------------


def test_load_same_grid_no_warning(tmp_path) -> None:
    """Loading constituents on the same grid must not emit a warning."""
    settings = f"""\
tide:
    cartesian:
        paths:
            M2: {DATASET / 'M2_tide.nc'}
            S2: {DATASET / 'S2_tide.nc'}
"""
    settings_path = str(tmp_path / 'config.yaml')
    with open(settings_path, 'w', encoding='utf-8') as stream:
        stream.write(settings)

    with warnings.catch_warnings():
        warnings.simplefilter('error')
        pyfes.config.load(settings_path)


def test_load_mixed_resolution_triggers_resampling(tmp_path) -> None:
    """Constituents with different grids must trigger a resampling warning."""
    # Use the existing 1° M2 file as the first constituent (sets the target
    # grid), then create a coarser 2° S2 file that will need resampling.
    lon_coarse = numpy.arange(0.0, 360.0, 2.0, dtype=numpy.float32)
    lat_coarse = numpy.arange(-89.5, 90.0, 2.0, dtype=numpy.float32)
    amp = _make_amplitude(lon_coarse, lat_coarse)
    pha = _make_phase(lon_coarse, lat_coarse)

    coarse_path = str(tmp_path / 'S2_coarse.nc')
    _create_tide_nc(coarse_path, lon_coarse, lat_coarse, amp, pha)

    settings = f"""\
tide:
    cartesian:
        paths:
            M2: {DATASET / 'M2_tide.nc'}
            S2: {coarse_path}
"""
    settings_path = str(tmp_path / 'config.yaml')
    with open(settings_path, 'w', encoding='utf-8') as stream:
        stream.write(settings)

    with pytest.warns(UserWarning, match='resampled'):
        config = pyfes.config.load(settings_path)

    model = config.models['tide']
    assert len(model) == 2
    assert sorted(model.identifiers()) == ['M2', 'S2']


def test_resampled_model_produces_correct_tides(tmp_path) -> None:
    """A resampled model must produce the same tides as a uniform-grid model.

    Strategy:
      1. Create two synthetic NetCDF files on a fine grid (1°) with a simple
         bilinear amplitude and constant phase.
      2. Create a third file on a coarser grid (2°) with the same analytical
         field.
      3. Load (fine M2 + coarse K1) → coarse K1 gets resampled onto the fine
         grid.
      4. Load (fine M2 + fine K1) → reference model, no resampling needed.
      5. Evaluate both models at the same points and compare.
    """
    lon_fine = numpy.arange(0.0, 360.0, 1.0, dtype=numpy.float32)
    lat_fine = numpy.arange(-89.5, 90.0, 1.0, dtype=numpy.float32)
    lon_coarse = numpy.arange(0.0, 360.0, 2.0, dtype=numpy.float32)
    lat_coarse = numpy.arange(-89.5, 90.0, 2.0, dtype=numpy.float32)

    # Fine M2
    m2_fine_path = str(tmp_path / 'M2_fine.nc')
    _create_tide_nc(
        m2_fine_path,
        lon_fine,
        lat_fine,
        _make_amplitude(lon_fine, lat_fine),
        _make_phase(lon_fine, lat_fine),
    )

    # Fine K1 (reference)
    k1_fine_path = str(tmp_path / 'K1_fine.nc')
    _create_tide_nc(
        k1_fine_path,
        lon_fine,
        lat_fine,
        _make_amplitude(lon_fine, lat_fine),
        _make_phase(lon_fine, lat_fine),
    )

    # Coarse K1 (will be resampled)
    k1_coarse_path = str(tmp_path / 'K1_coarse.nc')
    _create_tide_nc(
        k1_coarse_path,
        lon_coarse,
        lat_coarse,
        _make_amplitude(lon_coarse, lat_coarse),
        _make_phase(lon_coarse, lat_coarse),
    )

    # --- Reference model: both on the fine grid --------------------------
    ref_yaml = f"""\
tide:
    cartesian:
        paths:
            M2: {m2_fine_path}
            K1: {k1_fine_path}
"""
    ref_path = str(tmp_path / 'ref.yaml')
    with open(ref_path, 'w', encoding='utf-8') as f:
        f.write(ref_yaml)
    ref_config = pyfes.config.load(ref_path)

    # --- Resampled model: fine M2 + coarse K1 ----------------------------
    mixed_yaml = f"""\
tide:
    cartesian:
        paths:
            M2: {m2_fine_path}
            K1: {k1_coarse_path}
"""
    mixed_path = str(tmp_path / 'mixed.yaml')
    with open(mixed_path, 'w', encoding='utf-8') as f:
        f.write(mixed_yaml)
    with pytest.warns(UserWarning, match='resampled'):
        mixed_config = pyfes.config.load(mixed_path)

    # --- Evaluate at a set of test points --------------------------------
    test_lon = numpy.arange(10.0, 20.0, 1.0, dtype=numpy.float64)
    test_lat = numpy.arange(-5.0, 5.0, 1.0, dtype=numpy.float64)
    mx, my = numpy.meshgrid(test_lon, test_lat)
    dates = numpy.full(mx.shape, 'now', dtype='datetime64[us]')

    ref_tide, ref_lp, _ = pyfes.evaluate_tide(
        ref_config.models['tide'],
        dates.ravel(),
        mx.ravel(),
        my.ravel(),
        settings=ref_config.settings,
    )

    mixed_tide, mixed_lp, _ = pyfes.evaluate_tide(
        mixed_config.models['tide'],
        dates.ravel(),
        mx.ravel(),
        my.ravel(),
        settings=mixed_config.settings,
    )

    # The bilinear field + constant phase is exactly reproducible, so the
    # resampled and reference models should give identical results at
    # positions that lie on both grids.
    numpy.testing.assert_allclose(mixed_tide, ref_tide, rtol=1e-4, atol=1e-6)
    numpy.testing.assert_allclose(mixed_lp, ref_lp, rtol=1e-4, atol=1e-6)
