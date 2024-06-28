import pathlib
import tempfile

import numpy

from .. import config as config_handler, evaluate_tide

DATASET = pathlib.Path(__file__).parent / 'dataset'


def test_evalute_tide_lgp2(tmpdir) -> None:
    """Test the evaluation of the tide using LPG2 mesh."""
    config = f"""
tide:
    lgp:
        path: {DATASET / "fes_2014.nc"}
        codes: lgp2
        amplitude: "{{constituent}}_amp"
        phase: "{{constituent}}_phase"
        type: lgp2
        constituents:
            - 2N2
            - K1
            - K2
            - M2
            - N2
            - O1
            - P1
            - Q1
            - S1
            - S2
"""
    config_path = str(tmpdir / 'config.yaml')
    with open(config_path, 'w', encoding='utf-8') as stream:
        stream.write(config)
    handler = config_handler.load(config_path)

    dates = numpy.empty((24, ), dtype='M8[ms]')
    lons = numpy.empty((24, ), dtype=numpy.float64)
    lats = numpy.empty((24, ), dtype=numpy.float64)

    for hour in range(24):
        dates[hour] = numpy.datetime64(f'1983-01-01T{hour:02d}:00:00')
        lons[hour] = -7.688
        lats[hour] = 59.195
    tide, _, _ = evaluate_tide(handler['tide'],
                               dates,
                               lons,
                               lats,
                               num_threads=1)
    assert tide.shape == (24, )
