# Copyright (c) 2025 CNES
#
# All rights reserved. Use of this source code is governed by a
# BSD-style license that can be found in the LICENSE file.
import concurrent.futures
import datetime
import pathlib
import random
import threading
import time

import netCDF4
import numpy
from pyfes import core
from pyfes.leap_seconds import get_leap_seconds
import pytest

from . import is_free_threaded

FES_TIDE_TIME_SERIES = pathlib.Path(
    __file__).parent.parent / 'dataset' / 'fes_tide_time_series.nc'


def test_constructor():
    wt = core.WaveTable()
    assert len(wt) == 99
    assert len([item for item in wt]) == 99
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
        'Mm', 'Mf', 'Mtm', 'MSqm', '2Q1', 'Sigma1', 'Q1', 'Rho1', 'O1', 'MP1',
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


def benchmark_wave_table_operations():
    """Benchmark operations that should be GIL-free."""

    def compute_nodal_modulations(dates, leap_seconds):
        wave_table = core.WaveTable()
        return wave_table.compute_nodal_modulations(dates, leap_seconds)

    # Setup test data
    n_dates = 1000
    dates = numpy.array([
        numpy.datetime64('2024-01-01T00:00:00') + numpy.timedelta64(i, 'h')
        for i in range(n_dates)
    ])
    leap_seconds = numpy.full((n_dates, 1), 37, dtype=numpy.uint16)

    # Single-threaded benchmark
    start = time.time()
    _ = compute_nodal_modulations(dates, leap_seconds)
    single_time = time.time() - start

    # Multi-threaded benchmark
    start = time.time()
    with concurrent.futures.ThreadPoolExecutor(max_workers=4) as executor:
        futures = [
            executor.submit(compute_nodal_modulations, dates[i::4],
                            leap_seconds[i::4]) for i in range(4)
        ]
        _ = [f.result() for f in futures]
    multi_time = time.time() - start

    speedup = single_time / multi_time
    print(f'Single-threaded: {single_time:.3f}s')
    print(f'Multi-threaded: {multi_time:.3f}s')
    print(f'Speedup: {speedup:.2f}x')

    return speedup


def test_performance_improvement():
    """Test that free-threading provides performance benefits."""
    if not is_free_threaded():
        pytest.skip('Free-threading not available')

    speedup = benchmark_wave_table_operations()
    assert speedup > 1


def test_concurrent_wave_table_access():
    """Stress test concurrent access to wave tables."""

    errors = []
    results = []

    def worker(worker_id):
        try:
            # Create independent wave table
            wave_table = core.WaveTable(['S1', 'S2', 'M2', 'K1', 'O1'])

            # Perform various operations
            for _ in range(50):
                # Random operations
                operation = random.choice(
                    ['keys', 'values', 'harmonic_analysis'])

                if operation == 'keys':
                    keys = wave_table.keys()
                    assert len(keys) > 0

                elif operation == 'values':
                    values = wave_table.values()
                    assert len(values) > 0

                elif operation == 'harmonic_analysis':
                    dates = numpy.random.randint(0, int(1e6),
                                                 10).astype('datetime64[us]')
                    leap_seconds = numpy.empty((10, ), dtype=numpy.uint16)
                    f, vu = wave_table.compute_nodal_modulations(
                        dates, leap_seconds)
                    h = numpy.random.random((10, ))
                    result = wave_table.harmonic_analysis(h, f, vu)
                    assert result.shape == (5, )

                # Small delay to increase chance of race conditions
                time.sleep(0.001)

            results.append(f'Worker {worker_id} completed successfully')

        except Exception as e:
            errors.append(f'Worker {worker_id} failed: {str(e)}')

    # Run many concurrent workers
    num_workers = 16
    threads = []

    for i in range(num_workers):
        thread = threading.Thread(target=worker, args=(i, ))
        threads.append(thread)
        thread.start()

    for thread in threads:
        thread.join()

    # Check results
    assert len(errors) == 0, f'Errors occurred: {errors}'
    assert len(
        results
    ) == num_workers, f'Expected {num_workers} results, got {len(results)}'
