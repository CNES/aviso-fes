# Copyright (c) 2024 CNES
#
# All rights reserved. Use of this source code is governed by a
# BSD-style license that can be found in the LICENSE file.
import pathlib
import pickle

from .. import config as config_handler

DATASET = pathlib.Path(__file__).parent / 'dataset'


def test_config_cartesian(tmpdir):
    """Test the configuration of the cartesian tide."""
    config = f"""
radial:
    cartesian:
        paths:
            M2: {DATASET / "M2_tide.nc"}
            K1: {DATASET / "K1_tide.nc"}
            O1: {DATASET / "O1_tide.nc"}
            P1: {DATASET / "P1_tide.nc"}
            Q1: {DATASET / "Q1_tide.nc"}
            S1: {DATASET / "S1_tide.nc"}
        dynamic:
            - A5
"""
    config_path = str(tmpdir / 'config.yaml')
    with open(config_path, 'w', encoding='utf-8') as stream:
        stream.write(config)
    config = config_handler.load(config_path)

    other = pickle.loads(pickle.dumps(config))
    assert config.keys() == other.keys()
    assert config != other


def test_config_lgp2(tmpdir):
    """Test the configuration of the lgp2 tide."""
    config = f"""
tide:
    lgp:
        path: {DATASET / "fes_2014.nc"}
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
        dynamic:
            - A5
"""
    config_path = str(tmpdir / 'config.yaml')
    with open(config_path, 'w', encoding='utf-8') as stream:
        stream.write(config)
    config = config_handler.load(config_path)

    other = pickle.loads(pickle.dumps(config))
    assert config.keys() == other.keys()
    assert config != other
