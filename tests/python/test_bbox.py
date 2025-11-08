# Copyright (c) 2025 CNES
#
# All rights reserved. Use of this source code is governed by a
# BSD-style license that can be found in the LICENSE file.
import pathlib

import numpy
import pyfes
import pyfes.config as config_handler
import pytest

DATASET = pathlib.Path(__file__).parent / 'dataset'


def generate_coordinates():
    lons = numpy.arange(-180, 180, 1)
    lats = numpy.arange(-90, 90, 1)
    lons, lats = numpy.meshgrid(lons, lats)
    shape = lons.shape
    dates = numpy.full(shape, 'now', dtype='datetime64[us]')
    return lons, lats, shape, dates


@pytest.mark.parametrize('bbox', [(-10, -10, 10, 10), (-20, -20, -10, -10)])
def test_bbox_cartesian(tmp_path, bbox) -> None:
    """Test the configuration of the cartesian tide."""
    lons, lats, shape, dates = generate_coordinates()

    settings = f"""
tide:
    cartesian:
        paths:
            2N2: {DATASET / '2N2_tide.nc'}
            K1: {DATASET / 'K1_tide.nc'}
            K2: {DATASET / 'K2_tide.nc'}
            M2: {DATASET / 'M2_tide.nc'}
            M4: {DATASET / 'M4_tide.nc'}
            Mf: {DATASET / 'Mf_tide.nc'}
            Mm: {DATASET / 'Mm_tide.nc'}
            MSqm: {DATASET / 'Msqm_tide.nc'}
            Mtm: {DATASET / 'Mtm_tide.nc'}
            N2: {DATASET / 'N2_tide.nc'}
            O1: {DATASET / 'O1_tide.nc'}
            P1: {DATASET / 'P1_tide.nc'}
            Q1: {DATASET / 'Q1_tide.nc'}
            S1: {DATASET / 'S1_tide.nc'}
            S2: {DATASET / 'S2_tide.nc'}
"""
    settings_path = str(tmp_path / 'config.yaml')
    with open(settings_path, 'w', encoding='utf-8') as stream:
        stream.write(settings)
    config = config_handler.load(settings_path)
    tide1, lp1, m1 = pyfes.evaluate_tide(
        config['tide'], dates.ravel(), lons.ravel(), lats.ravel(), num_threads=1
    )
    tide1 = tide1.reshape(shape)
    lp1 = lp1.reshape(shape)
    m1 = (m1 > 0).reshape(shape)

    config = config_handler.load(settings_path, bbox)
    tide2, lp2, m2 = pyfes.evaluate_tide(
        config['tide'], dates.ravel(), lons.ravel(), lats.ravel(), num_threads=1
    )
    tide2 = tide2.reshape(shape)
    lp2 = lp2.reshape(shape)
    m2 = (m2 > 0).reshape(shape)

    assert numpy.sum(m1) > numpy.sum(m2)
    assert numpy.sum(numpy.isnan(tide2)) > numpy.sum(numpy.isnan(tide1))

    # Only correctly interpolated values are identical.
    numpy.testing.assert_array_equal(tide1[m2], tide2[m2])
    numpy.testing.assert_equal(lp1[m2], lp2[m2])


@pytest.mark.parametrize('bbox', [(-10, -10, 10, 10), (170, -10, 190, 10)])
def test_bbox_lgp2(tmp_path, bbox) -> None:
    """Test the configuration of the lgp2 tide."""
    lon, lat, shape, dates = generate_coordinates()

    settings = f"""
tide:
    lgp:
        path: {DATASET / 'fes_2014.nc'}
        codes: lgp2
        amplitude: "{{constituent}}_amp"
        phase: "{{constituent}}_phase"
        type: lgp2
        constituents:
            - M2
            - K1
            - O1
            - P1
            - Q1
            - S1
"""
    settings_path = str(tmp_path / 'config.yaml')
    with open(settings_path, 'w', encoding='utf-8') as stream:
        stream.write(settings)

    config = config_handler.load(settings_path)
    tide1, lp1, _ = pyfes.evaluate_tide(
        config['tide'], dates.ravel(), lon.ravel(), lat.ravel(), num_threads=0
    )
    tide1 = tide1.reshape(shape)
    lp1 = lp1.reshape(shape)
    config = config_handler.load(settings_path, bbox)
    tide2, lp2, _ = pyfes.evaluate_tide(
        config['tide'], dates.ravel(), lon.ravel(), lat.ravel(), num_threads=0
    )
    tide2 = tide2.reshape(shape)
    lp2 = lp2.reshape(shape)

    assert numpy.nanmax(tide1 - tide2) == 0
    assert numpy.sum(numpy.isnan(tide2)) > numpy.sum(numpy.isnan(tide1))
    numpy.testing.assert_equal(lp1, lp2)
