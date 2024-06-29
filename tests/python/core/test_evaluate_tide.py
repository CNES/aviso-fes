# Copyright (c) 2024 CNES
#
# All rights reserved. Use of this source code is governed by a
# BSD-style license that can be found in the LICENSE file.
import pathlib

import netCDF4
import numpy
from pyfes import core
from pyfes.leap_seconds import get_leap_seconds
import pytest

DATASET = pathlib.Path(__file__).parent.parent / 'dataset'


def load_model(configuration, tide_type):
    model = None
    for key, value in configuration.items():
        with netCDF4.Dataset(value) as ds:
            if model is None:
                lon = ds.variables['lon'][:]
                lat = ds.variables['lat'][:]
                x_axis = core.Axis(lon, is_circular=True)
                y_axis = core.Axis(lat)
                model = core.tidal_model.CartesianComplex64(
                    x_axis, y_axis, tide_type=tide_type, longitude_major=False)
            amp = numpy.ma.filled(ds.variables['amplitude'][:], numpy.nan)
            pha = numpy.radians(
                numpy.ma.filled(ds.variables['phase'][:], numpy.nan))
            wave = amp * numpy.cos(pha) + 1j * amp * numpy.sin(pha)
            model.add_constituent(key, wave.ravel())
    return model


def check_tide(tide, radial):
    for hour in range(24):
        h = tide[0][hour]
        lp = tide[1][hour]
        load = radial[0][hour]

        if hour == 0:
            assert h == pytest.approx(-100.9914560408143, 1e-5)
            assert lp == pytest.approx(0.9166038030400234, 1e-5)
            assert load == pytest.approx(3.88117368364037, 1e-5)
        elif hour == 1:
            assert h == pytest.approx(-137.10501916452552, 1e-5)
            assert lp == pytest.approx(0.8898752532275898, 1e-5)
            assert load == pytest.approx(4.328348741981256, 1e-5)
        elif hour == 2:
            assert h == pytest.approx(-138.48300705968734, 1e-5)
            assert lp == pytest.approx(0.8625289296811098, 1e-5)
            assert load == pytest.approx(3.710706028150491, 1e-5)
        elif hour == 3:
            assert h == pytest.approx(-104.3457724500508, 1e-5)
            assert lp == pytest.approx(0.8345824426096758, 1e-5)
            assert load == pytest.approx(2.134266036020157, 1e-5)
        elif hour == 4:
            assert h == pytest.approx(-42.51596183308732, 1e-5)
            assert lp == pytest.approx(0.8060537940850989, 1e-5)
            assert load == pytest.approx(-0.052040879755192906, 1e-5)
        elif hour == 5:
            assert h == pytest.approx(32.374242083878144, 1e-5)
            assert lp == pytest.approx(0.7769613634213312, 1e-5)
            assert load == pytest.approx(-2.3413995258830425, 1e-5)
        elif hour == 6:
            assert h == pytest.approx(102.16689747007992, 1e-5)
            assert lp == pytest.approx(0.7473238922215271, 1e-5)
            assert load == pytest.approx(-4.194239034681996, 1e-5)
        elif hour == 7:
            assert h == pytest.approx(149.46864323824715, 1e-5)
            assert lp == pytest.approx(0.7171604692200954, 1e-5)
            assert load == pytest.approx(-5.1719730959138985, 1e-5)
        elif hour == 8:
            assert h == pytest.approx(162.10203064752824, 1e-5)
            assert lp == pytest.approx(0.6864905148590934, 1e-5)
            assert load == pytest.approx(-5.045680499426743, 1e-5)
        elif hour == 9:
            assert h == pytest.approx(136.5054127560599, 1e-5)
            assert lp == pytest.approx(0.6553337656241297, 1e-5)
            assert load == pytest.approx(-3.852400085012575, 1e-5)
        elif hour == 10:
            assert h == pytest.approx(78.8955846722141, 1e-5)
            assert lp == pytest.approx(0.6237102581602095, 1e-5)
            assert load == pytest.approx(-1.8849646441211187, 1e-5)
        elif hour == 11:
            assert h == pytest.approx(3.644727138436709, 1e-5)
            assert lp == pytest.approx(0.5916403132203619, 1e-5)
            assert load == pytest.approx(0.3819118518486439, 1e-5)
        elif hour == 12:
            assert h == pytest.approx(-70.65877793134331, 1e-5)
            assert lp == pytest.approx(0.5591445193810965, 1e-5)
            assert load == pytest.approx(2.41050843885501, 1e-5)
        elif hour == 13:
            assert h == pytest.approx(-126.15176059285257, 1e-5)
            assert lp == pytest.approx(0.5262437166402825, 1e-5)
            assert load == pytest.approx(3.733863680480435, 1e-5)
        elif hour == 14:
            assert h == pytest.approx(-150.11458353803485, 1e-5)
            assert lp == pytest.approx(0.4929589798337336, 1e-5)
            assert load == pytest.approx(4.070714605116643, 1e-5)
        elif hour == 15:
            assert h == pytest.approx(-137.77925970081364, 1e-5)
            assert lp == pytest.approx(0.4593116018954215, 1e-5)
            assert load == pytest.approx(3.392774681238372, 1e-5)
        elif hour == 16:
            assert h == pytest.approx(-93.13207274339132, 1e-5)
            assert lp == pytest.approx(0.4253230770838105, 1e-5)
            assert load == pytest.approx(1.9276794897123648, 1e-5)
        elif hour == 17:
            assert h == pytest.approx(-27.81961238661622, 1e-5)
            assert lp == pytest.approx(0.39101508400872387, 1e-5)
            assert load == pytest.approx(0.09726404822892877, 1e-5)
        elif hour == 18:
            assert h == pytest.approx(41.54129769786677, 1e-5)
            assert lp == pytest.approx(0.35640946860741807, 1e-5)
            assert load == pytest.approx(-1.5928061547465937, 1e-5)
        elif hour == 19:
            assert h == pytest.approx(97.24994132240296, 1e-5)
            assert lp == pytest.approx(0.3215282271004507, 1e-5)
            assert load == pytest.approx(-2.6837902382004675, 1e-5)
        elif hour == 20:
            assert h == pytest.approx(124.93942256769164, 1e-5)
            assert lp == pytest.approx(0.28639348883143334, 1e-5)
            assert load == pytest.approx(-2.881719343109528, 1e-5)
        elif hour == 21:
            assert h == pytest.approx(117.46522048490488, 1e-5)
            assert lp == pytest.approx(0.251027499089178, 1e-5)
            assert load == pytest.approx(-2.1324613415016462, 1e-5)
        elif hour == 22:
            assert h == pytest.approx(77.02639695142673, 1e-5)
            assert lp == pytest.approx(0.21545260194634613, 1e-5)
            assert load == pytest.approx(-0.635100251236484, 1e-5)
        elif hour == 23:
            assert h == pytest.approx(14.661322939332242, 1e-5)
            assert lp == pytest.approx(0.17969122304114335, 1e-5)
            assert load == pytest.approx(1.2095550658262517, 1e-5)


def test_tide():
    radial = {
        '2N2': DATASET / '2N2_radial.nc',
        'K1': DATASET / 'K1_radial.nc',
        'K2': DATASET / 'K2_radial.nc',
        'M2': DATASET / 'M2_radial.nc',
        'N2': DATASET / 'N2_radial.nc',
        'O1': DATASET / 'O1_radial.nc',
        'P1': DATASET / 'P1_radial.nc',
        'Q1': DATASET / 'Q1_radial.nc',
        'S2': DATASET / 'S2_radial.nc'
    }
    tide = {
        '2N2': DATASET / '2N2_tide.nc',
        'K1': DATASET / 'K1_tide.nc',
        'K2': DATASET / 'K2_tide.nc',
        'M2': DATASET / 'M2_tide.nc',
        'M4': DATASET / 'M4_tide.nc',
        'Mf': DATASET / 'Mf_tide.nc',
        'Mm': DATASET / 'Mm_tide.nc',
        'Msqm': DATASET / 'Msqm_tide.nc',
        'Mtm': DATASET / 'Mtm_tide.nc',
        'N2': DATASET / 'N2_tide.nc',
        'O1': DATASET / 'O1_tide.nc',
        'P1': DATASET / 'P1_tide.nc',
        'Q1': DATASET / 'Q1_tide.nc',
        'S1': DATASET / 'S1_tide.nc',
        'S2': DATASET / 'S2_tide.nc'
    }
    model_radial = load_model(radial, core.kRadial)
    model_tide = load_model(tide, core.kTide)
    dates = numpy.empty((24, ), dtype='M8[ms]')
    lons = numpy.empty((24, ), dtype=numpy.float64)
    lats = numpy.empty((24, ), dtype=numpy.float64)

    for hour in range(24):
        dates[hour] = numpy.datetime64(f'1983-01-01T{hour:02d}:00:00')
        lons[hour] = -7.688
        lats[hour] = 59.195
    leap_seconds = get_leap_seconds(dates)
    tide = core.evaluate_tide(model_tide,
                              dates,
                              leap_seconds,
                              lons,
                              lats,
                              num_threads=1)
    radial = core.evaluate_tide(model_radial,
                                dates,
                                leap_seconds,
                                lons,
                                lats,
                                num_threads=1)
    check_tide(tide, radial)
