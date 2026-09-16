# Copyright (c) 2026 CNES
#
# All rights reserved. Use of this source code is governed by a
# BSD-style license that can be found in the LICENSE file.
import pathlib
import shutil

import pytest

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


def test_config_path_holding_a_dollar(tmp_path, monkeypatch) -> None:
    """Test a model path expanded from a variable holding a dollar sign."""
    directory = tmp_path / 'fes$2022'
    directory.mkdir()
    shutil.copy(DATASET / 'fes_2014.nc', directory)
    monkeypatch.setenv('FES_TEST_DIR', str(directory))
    settings = """
tide:
    lgp:
        path: ${FES_TEST_DIR}/fes_2014.nc
        codes: lgp2
        amplitude: "{constituent}_amp"
        phase: "{constituent}_phase"
        type: lgp2
        constituents:
            - M2
        dynamic:
            - A5
"""
    settings_path = str(tmp_path / 'config.yaml')
    with open(settings_path, 'w', encoding='utf-8') as stream:
        stream.write(settings)
    config = pyfes.config.load(settings_path)
    assert 'tide' in config.models


def test_config_expand_more_variables_than_the_depth(monkeypatch) -> None:
    """Test a value holding more variables than the interpolation depth."""
    indices = range(pyfes.config.MAX_INTERPOLATION_DEPTH + 1)
    for index in indices:
        monkeypatch.setenv(f'FES_TEST_{index}', str(index))
    rawval = '/'.join(f'${{FES_TEST_{index}}}' for index in indices)
    assert pyfes.config._expand(rawval) == '/'.join(
        str(index) for index in indices
    )


def test_config_expand_nested_variables(monkeypatch) -> None:
    """Test a variable referencing another variable."""
    monkeypatch.setenv('FES_TEST_ROOT', '/data')
    monkeypatch.setenv('FES_TEST_MODEL', '${FES_TEST_ROOT}/fes2022')
    assert (
        pyfes.config._expand('${FES_TEST_MODEL}/M2_tide.nc')
        == '/data/fes2022/M2_tide.nc'
    )


def test_config_expand_nesting_up_to_the_depth(monkeypatch) -> None:
    """Test variables nested as deep as, then deeper than, the depth."""
    depth = pyfes.config.MAX_INTERPOLATION_DEPTH
    for index in range(depth):
        monkeypatch.setenv(f'FES_TEST_{index}', f'${{FES_TEST_{index + 1}}}')
    monkeypatch.setenv(f'FES_TEST_{depth}', 'leaf')
    assert pyfes.config._expand('${FES_TEST_1}') == 'leaf'
    with pytest.raises(pyfes.config.InterpolationDepthError):
        pyfes.config._expand('${FES_TEST_0}')


def test_config_expand_self_referencing_variable(monkeypatch) -> None:
    """Test a variable defining itself."""
    monkeypatch.setenv('FES_TEST_LOOP', '${FES_TEST_LOOP}')
    with pytest.raises(pyfes.config.InterpolationDepthError):
        pyfes.config._expand('${FES_TEST_LOOP}')


def test_config_expand_undefined_variable(monkeypatch) -> None:
    """Test a variable missing from the environment."""
    monkeypatch.delenv('FES_TEST_UNDEFINED', raising=False)
    with pytest.raises(pyfes.config.InterpolationError):
        pyfes.config._expand('${FES_TEST_UNDEFINED}')
