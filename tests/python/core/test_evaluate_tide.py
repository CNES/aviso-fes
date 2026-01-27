# Copyright (c) 2025 CNES
#
# All rights reserved. Use of this source code is governed by a
# BSD-style license that can be found in the LICENSE file.
import concurrent.futures
import pathlib
import sys
import time

import netCDF4
import numpy
from pyfes import core
import pytest

#: Path to the directory containing the test datasets.
DATASET = pathlib.Path(__file__).parent.parent / 'dataset'

#: Dictionary mapping tidal constituent codes to their corresponding netCDF
#: file paths.
TIDAL_WAVES = {
    '2N2': DATASET / '2N2_tide.nc',
    'K1': DATASET / 'K1_tide.nc',
    'K2': DATASET / 'K2_tide.nc',
    'M2': DATASET / 'M2_tide.nc',
    'M4': DATASET / 'M4_tide.nc',
    'Mf': DATASET / 'Mf_tide.nc',
    'Mm': DATASET / 'Mm_tide.nc',
    'MSqm': DATASET / 'Msqm_tide.nc',
    'Mtm': DATASET / 'Mtm_tide.nc',
    'N2': DATASET / 'N2_tide.nc',
    'O1': DATASET / 'O1_tide.nc',
    'P1': DATASET / 'P1_tide.nc',
    'Q1': DATASET / 'Q1_tide.nc',
    'S1': DATASET / 'S1_tide.nc',
    'S2': DATASET / 'S2_tide.nc',
}

#: Real Tide Gauge Constituents from TICON-3 for Brest (amplitude in cm, phase
#: in degrees).
BREST_TICON3_DATA = {
    'M2': (205.113, 109.006),
    'K1': (6.434, 75.067),
    'N2': (41.695, 90.633),
    'O1': (6.587, 327.857),
    'P1': (2.252, 63.658),
    'Q1': (2.040, 281.362),
    'K2': (21.361, 145.892),
    'S2': (74.876, 148.283),
    'S1': (0.797, 11.441),
    'SA': (4.905, 322.761),
    'T2': (4.171, 138.535),
    'MF': (1.031, 175.663),
    'MM': (0.425, 199.741),
    '2N2': (5.699, 72.786),
    'M4': (5.437, 105.940),
    'J1': (0.241, 123.005),
    'SSA': (2.047, 98.898),
    'MSF': (0.356, 24.980),
    'MSQM': (0.115, 254.934),  # Noted as MSQ in TICON-3
    'EPS2': (1.968, 89.471),  # Noted as EP2 in TICON-3
    'L2': (6.392, 102.910),
    'M3': (1.977, 15.860),
    'R2': (0.534, 158.066),
    'MU2': (8.566, 105.087),  # Noted as MI2 in TICON-3
    'MTM': (0.110, 142.031),
    'NU2': (7.780, 86.614),  # Noted as NI2 in TICON-3
    'LAMBDA2': (2.625, 75.845),  # Noted as LM2 in TICON-3
    'MN4': (1.937, 60.491),
    'MS4': (3.258, 181.835),
    'MKS2': (0.758, 173.969),  # Noted as MKS in TICON-3
    'N4': (0.291, 9.263),
    'M6': (3.153, 354.764),
    'M8': (0.231, 231.883),
    'S4': (0.217, 289.151),
    '2Q1': (0.376, 234.893),
    'OO1': (0.136, 213.353),
    'M1': (0.535, 83.038),
}
#: Location of the Brest tide gauge (longitude, latitude).
BREST_LOCATION = (-4.495, 48.383)


def load_model(
    configuration: dict[str, pathlib.Path],
    tide_type: core.TideType,
) -> core.tidal_model.CartesianComplex64:
    """Load a tidal model from netCDF files."""
    model = None
    constituent_map = core.darwin.constituents()
    for key, value in configuration.items():
        with netCDF4.Dataset(value) as ds:
            if model is None:
                lon = ds.variables['lon'][:]
                lat = ds.variables['lat'][:]
                x_axis = core.Axis(lon, is_circular=True)
                y_axis = core.Axis(lat)
                model = core.tidal_model.CartesianComplex64(
                    x_axis,
                    y_axis,
                    constituent_map=constituent_map,
                    tide_type=tide_type,
                    longitude_major=False,
                )
            amp = numpy.ma.filled(
                ds.variables['amplitude'][:],
                numpy.nan,
            )
            pha = numpy.radians(
                numpy.ma.filled(ds.variables['phase'][:], numpy.nan)
            )
            wave = amp * numpy.cos(pha) + 1j * amp * numpy.sin(pha)
            model.add_constituent(key, wave.ravel())
    assert model is not None
    return model


def check_tide(tide, radial) -> None:
    """Validate tide and radial outputs against a table of expected values."""
    # Table of expected (h, lp, load) for each hour 0..23
    EXPECTED = [
        (-100.9914560408143, 0.8962384327950185, 3.88117368364037),
        (-137.10501916452552, 0.8694202846939806, 4.328348741981256),
        (-138.48300705968734, 0.8419806943259117, 3.710706028150491),
        (-104.3457724500508, 0.8139371002569467, 2.134266036020157),
        (-42.51596183308732, 0.7853073299813511, -0.052040879755192906),
        (32.374242083878144, 0.7561095855618518, -2.3413995258830425),
        (102.16689747007992, 0.726362429024919, -4.194239034681996),
        (149.46864323824715, 0.6960847675405596, -5.1719730959138985),
        (162.10203064752824, 0.6652958383054876, -5.045680499426743),
        (136.5054127560599, 0.6340151932123841, -3.852400085012575),
        (78.8955846722141, 0.6022626833383417, -1.8849646441211187),
        (3.644727138436709, 0.5700584431878236, 0.3819118518486439),
        (-70.65877793134331, 0.5374228748036814, 2.41050843885501),
        (-126.15176059285257, 0.5043766316815259, 3.733863680480435),
        (-150.11458353803485, 0.4709406025333071, 4.070714605116643),
        (-137.77925970081364, 0.43713589495004734, 3.392774681238372),
        (-93.13207274339132, 0.4029838189159635, 1.9276794897123648),
        (-27.81961238661622, 0.3685058702253908, 0.09726404822892877),
        (41.54129769786677, 0.33372371382897054, -1.5928061547465937),
        (97.24994132240296, 0.2986591671120649, -2.6837902382004675),
        (124.93942256769164, 0.2633341831064725, -2.881719343109528),
        (117.46522048490488, 0.2277708336655093, -2.1324613415016462),
        (77.02639695142673, 0.1919912926311164, -0.635100251236484),
        (14.661322939332242, 0.15601781901998055, 1.2095550658262517),
    ]

    tol = 1e-5
    for hour in range(24):
        h = tide[0][hour]
        lp = tide[1][hour]
        load = radial[0][hour]

        exp_h, exp_lp, exp_load = EXPECTED[hour]

        assert h == pytest.approx(exp_h, tol)
        assert lp == pytest.approx(exp_lp, tol)
        assert load == pytest.approx(exp_load, tol)


def test_tide() -> None:
    """Test tide evaluation against expected values."""
    wave_files = {
        '2N2': DATASET / '2N2_radial.nc',
        'K1': DATASET / 'K1_radial.nc',
        'K2': DATASET / 'K2_radial.nc',
        'M2': DATASET / 'M2_radial.nc',
        'N2': DATASET / 'N2_radial.nc',
        'O1': DATASET / 'O1_radial.nc',
        'P1': DATASET / 'P1_radial.nc',
        'Q1': DATASET / 'Q1_radial.nc',
        'S2': DATASET / 'S2_radial.nc',
    }
    radial_model = load_model(wave_files, core.RADIAL)
    tidal_model = load_model(TIDAL_WAVES, core.TIDE)
    dates = numpy.empty((24,), dtype='M8[ms]')
    lons = numpy.empty((24,), dtype=numpy.float64)
    lats = numpy.empty((24,), dtype=numpy.float64)

    for hour in range(24):
        dates[hour] = numpy.datetime64(f'1983-01-01T{hour:02d}:00:00')
        lons[hour] = -7.688
        lats[hour] = 59.195
    tide = core.evaluate_tide(
        tidal_model,
        dates,
        lons,
        lats,
        core.FesRuntimeSettings().with_num_threads(1),
    )
    radial_waves = core.evaluate_tide(
        radial_model,
        dates,
        lons,
        lats,
        core.FesRuntimeSettings().with_num_threads(1),
    )
    check_tide(tide, radial_waves)


def cpu_intensive_task(
    tidal_model: core.tidal_model.CartesianComplex64,
    dates: numpy.ndarray,
    lon: numpy.ndarray,
    lat: numpy.ndarray,
    thread_id: int,
) -> dict[str, float | int | None]:
    """CPU-intensive task that should benefit from free-threading."""
    start_time = time.time()

    # Simulate heavy computation
    for _ in range(10):  # Multiple iterations to stress test
        result = core.evaluate_tide(
            tidal_model,
            dates,
            lon,
            lat,
            core.FesRuntimeSettings().with_num_threads(
                1
            ),  # Force single thread per call for this test
        )

    end_time = time.time()
    return {
        'thread_id': thread_id,
        'duration': end_time - start_time,
        'result_shape': result[0].shape if result else None,
    }


@pytest.mark.skipif(
    sys.platform.startswith('win'), reason='Not relevant on Windows'
)
def test_parallel_tide_evaluation() -> None:
    """Test parallel tide evaluation to verify GIL release."""
    # Setup test data
    lon = numpy.linspace(0, 10, 100)
    lat = numpy.linspace(45, 55, 100)
    dates = numpy.full(lon.shape, numpy.datetime64('2024-01-01T00:00:00'))

    # Create a simple tidal model (you'll need to adapt this to your actual
    # model)
    tidal_model = load_model(TIDAL_WAVES, core.TIDE)

    num_threads = 4

    # Sequential execution
    start_sequential = time.time()
    for i in range(num_threads):
        _ = cpu_intensive_task(tidal_model, dates, lon, lat, i)
    sequential_time = time.time() - start_sequential

    # Parallel execution
    start_parallel = time.time()
    with concurrent.futures.ThreadPoolExecutor(
        max_workers=num_threads
    ) as executor:
        futures = [
            executor.submit(
                cpu_intensive_task,
                tidal_model,
                dates,
                lon,
                lat,
                i,
            )
            for i in range(num_threads)
        ]
        _ = [
            future.result()
            for future in concurrent.futures.as_completed(futures)
        ]
    parallel_time = time.time() - start_parallel

    speedup = sequential_time / parallel_time
    assert speedup > 1


def test_evaluate_tide_from_constituents() -> None:
    """Test evaluate_tide_from_constituents function."""
    start_date = numpy.datetime64('2024-01-01T00:00:00')
    end_date = numpy.datetime64('2024-01-02T00:00:00')
    dates = numpy.arange(start_date, end_date, numpy.timedelta64(1, 'h'))

    constituents = dict(BREST_TICON3_DATA.items())

    tide, long_period = core.evaluate_tide_from_constituents(
        constituents,
        dates,
        BREST_LOCATION[1],
        core.FesRuntimeSettings().with_num_threads(1),
    )
    assert len(tide) == len(dates)
    assert len(long_period) == len(dates)
    # Simple checks on the output ranges
    assert -250 < tide.min() < 0
    assert 0 < tide.max() < 250
    assert -10 < long_period.min() < 10
    assert -10 < long_period.max() < 10
