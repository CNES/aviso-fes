# Copyright (c) 2026 CNES
#
# All rights reserved. Use of this source code is governed by a
# BSD-style license that can be found in the LICENSE file.
import concurrent.futures
import pathlib
import sys
import time

import netCDF4
import numpy
import pytest

from pyfes import core


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
    for key, value in configuration.items():
        with netCDF4.Dataset(value) as ds:
            if model is None:
                lon = ds.variables['lon'][:]
                lat = ds.variables['lat'][:]
                x_axis = core.Axis(lon, is_periodic=True)
                y_axis = core.Axis(lat)
                model = core.tidal_model.CartesianComplex64(
                    x_axis,
                    y_axis,
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


# Reference table of (h, lp, load) per hour 0..23, keyed by build flavor.
# When the C++ library is rebuilt with ``FES_USE_IERS_CONSTANTS`` the constants
# in numbers.hpp shift, the predicted tide drifts a few millimetres, and the
# ``iers2010`` table below must be refreshed from a one-off run on that build.
EXPECTED_TIDE: dict[str, list[tuple[float, float, float]]] = {
    'schureman': [
        (-100.9914560408143, 0.9225987436536937, 3.88117368364037),
        (-137.10501916452552, 0.8958797954886075, 4.328348741981256),
        (-138.48300705968734, 0.8685435249266603, 3.710706028150491),
        (-104.3457724500508, 0.8406075713383988, 2.134266036020157),
        (-42.51596183308732, 0.8120899664988928, -0.052040879755192906),
        (32.374242083878144, 0.7830091198625782, -2.3413995258830425),
        (102.16689747007992, 0.7533838035856185, -4.194239034681996),
        (149.46864323824715, 0.723233137325911, -5.1719730959138985),
        (162.10203064752824, 0.6925765727407006, -5.045680499426743),
        (136.5054127560599, 0.6614338777644806, -3.852400085012575),
        (78.8955846722141, 0.6298251207010751, -1.8849646441211187),
        (3.644727138436709, 0.5977706540660582, 0.3819118518486439),
        (-70.65877793134331, 0.5652910982930023, 2.41050843885501),
        (-126.15176059285257, 0.5324073252401842, 3.733863680480435),
        (-150.11458353803485, 0.4991404415437375, 4.070714605116643),
        (-137.77925970081364, 0.46551177186779913, 3.392774681238372),
        (-93.13207274339132, 0.4315428420047164, 1.9276794897123648),
        (-27.81961238661622, 0.39725536187729554, 0.09726404822892877),
        (41.54129769786677, 0.362671208469935, -1.5928061547465937),
        (97.24994132240296, 0.3278124086924018, -2.6837902382004675),
        (124.93942256769164, 0.2927011221780059, -2.881719343109528),
        (117.46522048490488, 0.2573596240467227, -2.1324613415016462),
        (77.02639695142673, 0.2218102876625862, -0.635100251236484),
        (14.661322939332242, 0.18607556741268932, 1.2095550658262517),
    ],
    'iers2010': [
        (-100.98822480037529, 0.9237511218495038, 3.8811901650618332),
        (-137.097048232869, 0.8970316728779556, 4.328365559396372),
        (-138.4725278722844, 0.869694421102863, 3.7107243370979286),
        (-104.33586791034888, 0.8417570071239117, 2.134294589755064),
        (-42.509721558587145, 0.8132374641990737, -0.05198892359994449),
        (32.37456064958061, 0.7841542035177789, -2.3413113756408),
        (102.16049099929785, 0.7545259992214051, -4.194107304275129),
        (149.45641521657816, 0.7243719732012738, -5.171799717932914),
        (162.0864079050915, 0.6937115795940626, -5.045478388394841),
        (136.489760726482, 0.6625645890573508, -3.852191990824),
        (78.88336100403725, 0.6309510728592033, -1.884779346841636),
        (3.6385947875833557, 0.5988913867179047, 0.382045174548401),
        (-70.65763217712114, 0.5664061545053924, 2.410566363619619),
        (-126.1439086392463, 0.5335162517509788, 3.733833746924149),
        (-150.10216464146595, 0.5002427889913814, 4.07059832616364),
        (-137.7653822199834, 0.46660709501760267, 3.3925876170316385),
        (-93.11998829211153, 0.4326306999717139, 1.9274483449921018),
        (-27.81188981896157, 0.3983353183455662, 0.09702145908465071),
        (41.54338992490613, 0.36374283190822154, -1.59302793896453),
        (97.24670102026342, 0.3288752725659298, -2.6839653293641628),
        (124.93257165814332, 0.2937548051563388, -2.881832508129235),
        (117.45739229339519, 0.258403710207555, -2.1325097115343103),
        (77.02038081023828, 0.22284436669135568, -0.6350921864183299),
        (14.659264675927863, 0.18709923479786694, 1.209603523443338),
    ],
}


def check_tide(tide, radial, expected) -> None:
    """Validate tide and radial outputs against a table of expected values."""
    tol = 1e-5
    for hour in range(24):
        h = tide[0][hour]
        lp = tide[1][hour]
        load = radial[0][hour]

        exp_h, exp_lp, exp_load = expected[hour]

        assert h == pytest.approx(exp_h, tol)
        assert lp == pytest.approx(exp_lp, tol)
        assert load == pytest.approx(exp_load, tol)


def test_tide(constants_flavor: str) -> None:
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
        core.FESSettings().with_num_threads(1),
    )
    radial_waves = core.evaluate_tide(
        radial_model,
        dates,
        lons,
        lats,
        core.FESSettings().with_num_threads(1),
    )
    check_tide(tide, radial_waves, EXPECTED_TIDE[constants_flavor])


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
            core.FESSettings().with_num_threads(
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
        core.FESSettings().with_num_threads(1),
    )
    assert len(tide) == len(dates)
    assert len(long_period) == len(dates)

    # Simple checks on the output ranges
    assert -250 < tide.min() < 0
    assert 0 < tide.max() < 250
    assert -10 < long_period.min() < 10
    assert -10 < long_period.max() < 10
