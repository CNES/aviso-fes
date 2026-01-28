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
import pytest

from .. import is_free_threaded

DATASET_DIR = pathlib.Path(__file__).parent.parent.parent / 'dataset'
FES_TIDE_TIME_SERIES = DATASET_DIR / 'fes_tide_time_series.nc'


def test_constructor() -> None:
    wt = core.darwin.WaveTable()
    assert len(wt) == 99
    assert len(list(wt)) == 99
    with pytest.raises(
        ValueError,
        match='unknown constituent name: __M2__',
    ):
        wt['__M2__']

    wt = core.darwin.WaveTable(['Q1', 'O1', 'P1', 'S1', 'K1', 'M2'])
    assert len(wt) == 6
    assert sorted(x.name() for x in wt) == sorted(
        ('Q1', 'O1', 'P1', 'S1', 'K1', 'M2')
    )
    assert sorted(wt.keys()) == sorted(['Q1', 'O1', 'P1', 'S1', 'K1', 'M2'])
    assert sorted(item.name() for item in wt.values()) == sorted(
        [
            wt['Q1'].name(),
            wt['O1'].name(),
            wt['P1'].name(),
            wt['S1'].name(),
            wt['K1'].name(),
            wt['M2'].name(),
        ]
    )
    assert 'Q1' in wt
    assert 'XX' not in wt


def test_wave() -> None:
    aa = core.AstronomicAngle()
    aa.update(datetime.datetime(2000, 1, 1))
    wt = core.darwin.WaveTable(['M2'])
    wave = wt['M2']
    assert wave.freq * 24 == pytest.approx(12.140833182614747, 1e-6)
    assert wave.type == core.SHORT_PERIOD


def test_harmonic_analysis() -> None:
    with netCDF4.Dataset(FES_TIDE_TIME_SERIES) as dataset:
        time = dataset['time'][:].astype('datetime64[us]')
        h = dataset['ocean'][:] * 1e-2

    wt = core.darwin.WaveTable(
        [
            'Mm',
            'Mf',
            'Mtm',
            'MSqm',
            '2Q1',
            'Sigma1',
            'Q1',
            'Rho1',
            'O1',
            'MP1',
            'M11',
            'M12',
            'M13',
            'Chi1',
            'Pi1',
            'P1',
            'S1',
            'K1',
            'Psi1',
            'Phi1',
            'Theta1',
            'J1',
            'OO1',
            'MNS2',
            'Eps2',
            '2N2',
            'Mu2',
            '2MS2',
            'N2',
            'Nu2',
            'M2',
            'MKS2',
            'Lambda2',
            'L2',
            '2MN2',
            'T2',
            'S2',
            'R2',
            'K2',
            'MSN2',
            'Eta2',
            '2SM2',
            'MO3',
            '2MK3',
            'M3',
            'MK3',
            'N4',
            'MN4',
            'M4',
            'SN4',
            'MS4',
            'MK4',
            'S4',
            'SK4',
            'R4',
            '2MN6',
            'M6',
            'MSN6',
            '2MS6',
            '2MK6',
            '2SM6',
            'MSK6',
            'S6',
            'M8',
            'MSf',
            'Ssa',
            'Sa',
        ],
    )
    f, vu = wt.compute_nodal_modulations(time)
    w = wt.harmonic_analysis(h, f, vu)
    delta = h - wt.tide_from_tide_series(time, w)

    assert delta.mean(), pytest.approx(0, rel=1e-16)
    assert delta.std(), pytest.approx(0, rel=1e-12)


def test_harmonic_analysis_with_empty_table() -> None:
    time = numpy.arange(
        numpy.datetime64('2018-01-01'),
        numpy.datetime64('2018-03-01'),
        3600,
        dtype='datetime64[s]',
    )
    h = numpy.random.default_rng().random(time.shape[0])

    wt = core.darwin.WaveTable(['M2', 'S2', 'N2', 'K1', 'O1', 'Q1'])
    w = wt.harmonic_analysis(h, *wt.compute_nodal_modulations(time))
    assert numpy.all(~numpy.isnan(wt.tide_from_tide_series(time, w)))

    wt = core.darwin.WaveTable()
    w = wt.harmonic_analysis(h, *wt.compute_nodal_modulations(time))
    assert numpy.all(~numpy.isnan(wt.tide_from_tide_series(time, w)))


def benchmark_wave_table_operations():
    """Benchmark operations that should be GIL-free."""

    def compute_nodal_modulations(dates):
        wave_table = core.darwin.WaveTable()
        return wave_table.compute_nodal_modulations(dates)

    # Setup test data
    n_dates = 1000
    dates = numpy.array(
        [
            numpy.datetime64('2024-01-01T00:00:00') + numpy.timedelta64(i, 'h')
            for i in range(n_dates)
        ]
    )
    # Single-threaded benchmark
    start = time.time()
    _ = compute_nodal_modulations(dates)
    single_time = time.time() - start

    # Multi-threaded benchmark
    start = time.time()
    with concurrent.futures.ThreadPoolExecutor(max_workers=4) as executor:
        futures = [
            executor.submit(compute_nodal_modulations, dates[i::4])
            for i in range(4)
        ]
        _ = [f.result() for f in futures]
    multi_time = time.time() - start

    speedup = single_time / multi_time
    print(f'Single-threaded: {single_time:.3f}s')
    print(f'Multi-threaded: {multi_time:.3f}s')
    print(f'Speedup: {speedup:.2f}x')

    return speedup


def test_performance_improvement() -> None:
    """Test that free-threading provides performance benefits."""
    if not is_free_threaded():
        pytest.skip('Free-threading not available')

    speedup = benchmark_wave_table_operations()
    assert speedup > 1


def test_concurrent_wave_table_access() -> None:
    """Stress test concurrent access to wave tables."""
    errors = []
    results = []

    def worker(worker_id) -> None:
        rng = numpy.random.default_rng(seed=worker_id)
        try:
            # Create independent wave table
            wave_table = core.darwin.WaveTable(['S1', 'S2', 'M2', 'K1', 'O1'])

            # Perform various operations
            for _ in range(50):
                # Random operations
                operation = random.choice(
                    ['keys', 'values', 'harmonic_analysis']
                )

                if operation == 'keys':
                    keys = wave_table.keys()
                    assert len(keys) > 0

                elif operation == 'values':
                    values = wave_table.values()
                    assert len(values) > 0

                elif operation == 'harmonic_analysis':
                    dates = rng.integers(0, int(1e6), 10).astype(
                        'datetime64[us]'
                    )
                    f, vu = wave_table.compute_nodal_modulations(dates)
                    h = rng.random((10,))
                    result = wave_table.harmonic_analysis(h, f, vu)
                    assert result.shape == (5,)

                # Small delay to increase chance of race conditions
                time.sleep(0.001)

            results.append(f'Worker {worker_id} completed successfully')

        except Exception as e:
            errors.append(f'Worker {worker_id} failed: {e!s}')

    # Run many concurrent workers
    num_workers = 16
    threads = []

    for i in range(num_workers):
        thread = threading.Thread(target=worker, args=(i,))
        threads.append(thread)
        thread.start()

    for thread in threads:
        thread.join()

    # Check results
    assert len(errors) == 0, f'Errors occurred: {errors}'
    assert len(results) == num_workers, (
        f'Expected {num_workers} results, got {len(results)}'
    )
