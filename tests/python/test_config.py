# Copyright (c) 2026 CNES
#
# All rights reserved. Use of this source code is governed by a
# BSD-style license that can be found in the LICENSE file.
import pathlib

import pyfes


DATASET = pathlib.Path(__file__).parent / 'dataset'


def test_config_cartesian(tmp_path) -> None:
    """Test the configuration of the cartesian tide."""
    settings = f"""
radial:
    cartesian:
        paths:
            M2: {DATASET / 'M2_tide.nc'}
            K1: {DATASET / 'K1_tide.nc'}
            O1: {DATASET / 'O1_tide.nc'}
            P1: {DATASET / 'P1_tide.nc'}
            Q1: {DATASET / 'Q1_tide.nc'}
            S1: {DATASET / 'S1_tide.nc'}
        dynamic:
            - A5
"""
    settings_path = str(tmp_path / 'config.yaml')
    with open(settings_path, 'w', encoding='utf-8') as stream:
        stream.write(settings)
    config = pyfes.config.load(settings_path)
    assert 'radial' in config.models
    assert isinstance(config.settings, pyfes.FESSettings)


def test_config_lgp2(tmp_path) -> None:
    """Test the configuration of the lgp2 tide."""
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
        dynamic:
            - A5
"""
    settings_path = str(tmp_path / 'config.yaml')
    with open(settings_path, 'w', encoding='utf-8') as stream:
        stream.write(settings)
    config = pyfes.config.load(settings_path)
    assert 'tide' in config.models
    assert isinstance(config.settings, pyfes.FESSettings)
