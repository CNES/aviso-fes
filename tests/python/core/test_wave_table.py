# Copyright (c) 2025 CNES
#
# All rights reserved. Use of this source code is governed by a
# BSD-style license that can be found in the LICENSE file.
import datetime
import pathlib

import netCDF4
import numpy
from pyfes import core
from pyfes.leap_seconds import get_leap_seconds
import pytest

FES_TIDE_TIME_SERIES = pathlib.Path(
    __file__).parent.parent / 'dataset' / 'fes_tide_time_series.nc'


def test_constructor():
    wt = core.WaveTable()
    assert len(wt) == 75
    assert len([item for item in wt]) == 75
    assert wt.find('M2') == wt[core.kM2]
    assert wt.find('m2') == wt[core.kM2]
    assert wt.find('M2') != wt[core.kK1]
    with pytest.raises(ValueError):
        wt.find('__M2__')
    assert sorted(core.constituents.known()) == sorted(
        [item.name() for item in wt])
    for item in wt:
        assert item.ident == getattr(core, 'k' + item.name())

    wt = core.WaveTable(['Q1', 'O1', 'P1', 'S1', 'K1', 'M2'])
    assert len(wt) == 6
    assert tuple(map(lambda x: x.name(),
                     wt)) == ('Q1', 'O1', 'P1', 'S1', 'K1', 'M2')
    assert wt.keys() == ['Q1', 'O1', 'P1', 'S1', 'K1', 'M2']
    assert wt.values() == [
        wt.find('Q1'),
        wt.find('O1'),
        wt.find('P1'),
        wt.find('S1'),
        wt.find('K1'),
        wt.find('M2'),
    ]


def test_wave():
    aa = core.AstronomicAngle()
    aa.update(datetime.datetime(2000, 1, 1), 32)
    wt = core.WaveTable(['M2'])
    wave = wt.find('M2')
    assert wave.freq * 24 == pytest.approx(12.140833182614747, 1e-6)
    assert wave.type == wave.TidalType.kShortPeriod


def test_harmonic_analysis():
    with netCDF4.Dataset(FES_TIDE_TIME_SERIES) as dataset:
        time = dataset['time'][:].astype('datetime64[us]')
        h = dataset['ocean'][:] * 1e-2

    wt = core.WaveTable([
        'Mm', 'Mf', 'Mtm', 'Msqm', '2Q1', 'Sigma1', 'Q1', 'Rho1', 'O1', 'MP1',
        'M11', 'M12', 'M13', 'Chi1', 'Pi1', 'P1', 'S1', 'K1', 'Psi1', 'Phi1',
        'Theta1', 'J1', 'OO1', 'MNS2', 'Eps2', '2N2', 'Mu2', '2MS2', 'N2',
        'Nu2', 'M2', 'MKS2', 'Lambda2', 'L2', '2MN2', 'T2', 'S2', 'R2', 'K2',
        'MSN2', 'Eta2', '2SM2', 'MO3', '2MK3', 'M3', 'MK3', 'N4', 'MN4', 'M4',
        'SN4', 'MS4', 'MK4', 'S4', 'SK4', 'R4', '2MN6', 'M6', 'MSN6', '2MS6',
        '2MK6', '2SM6', 'MSK6', 'S6', 'M8', 'MSf', 'Ssa', 'Sa'
    ], )
    leap_seconds = get_leap_seconds(time)
    f, vu = wt.compute_nodal_modulations(time, leap_seconds)
    w = wt.harmonic_analysis(h, f, vu)
    delta = h - wt.tide_from_tide_series(time, leap_seconds, w)

    assert delta.mean(), pytest.approx(0, rel=1e-16)
    assert delta.std(), pytest.approx(0, rel=1e-12)


def test_harmonic_analysis_with_empty_table():
    time = numpy.arange(numpy.datetime64('2018-01-01'),
                        numpy.datetime64('2018-03-01'),
                        3600,
                        dtype='datetime64[s]')
    h = numpy.random.rand(time.shape[0])
    leap_seconds = get_leap_seconds(time)

    wt = core.WaveTable(['M2', 'S2', 'N2', 'K1', 'O1', 'Q1'])
    w = wt.harmonic_analysis(h,
                             *wt.compute_nodal_modulations(time, leap_seconds))
    assert numpy.all(
        ~numpy.isnan(wt.tide_from_tide_series(time, leap_seconds, w)))

    wt = core.WaveTable()
    w = wt.harmonic_analysis(h,
                             *wt.compute_nodal_modulations(time, leap_seconds))
    assert numpy.all(
        ~numpy.isnan(wt.tide_from_tide_series(time, leap_seconds, w)))
