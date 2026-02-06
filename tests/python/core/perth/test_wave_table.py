# Copyright (c) 2026 CNES
#
# All rights reserved. Use of this source code is governed by a
# BSD-style license that can be found in the LICENSE file.
"""Comprehensive tests for the pyfes.core.perth.WaveTable class."""

import numpy as np
from pyfes import core
import pytest


class TestPerthWaveTableExtended:
    """Extended test suite for perth.WaveTable class."""

    def test_wave_table_full_initialization(self) -> None:
        """Test WaveTable full initialization."""
        wt = core.perth.WaveTable()

        # Should have significant number of constituents
        assert len(wt) > 50

    def test_wave_table_access_all_constituents(self) -> None:
        """Test accessing all constituents in table."""
        wt = core.perth.WaveTable()

        for name in wt.constituents:
            wave = wt[name]
            assert wave is not None
            assert wave.name == name

    def test_wave_table_iteration_order(self) -> None:
        """Test order of iteration over wave table."""
        wt = core.perth.WaveTable(['M2', 'S2', 'K1'])

        waves_from_iteration = [w.name for w in wt.waves()]
        names = wt.constituents

        # All waves should be in constituents
        for wave_name in waves_from_iteration:
            assert wave_name in names

    def test_wave_table_access_invalid_constituent(self) -> None:
        """Test accessing invalid constituent raises error."""
        wt = core.perth.WaveTable()

        with pytest.raises((KeyError, ValueError)):
            _ = wt['INVALID']

    def test_wave_table_len_consistency(self) -> None:
        """Test that len() is consistent with constituents/waves."""
        wt = core.perth.WaveTable()

        assert len(wt) == len(wt.constituents)
        assert len(wt) == len(wt.waves())

    def test_wave_table_vu_are_valid(self) -> None:
        """Test that all vu (V+u) values are valid."""
        wt = core.perth.WaveTable()

        for wave in wt.waves():
            vu = wave.vu
            assert np.isfinite(vu), f'Invalid vu for {wave.name}'

    def test_wave_table_tide_amplitudes_are_valid(self) -> None:
        """Test that all tide amplitudes are valid."""
        wt = core.perth.WaveTable()

        for wave in wt.waves():
            tide = wave.tide
            magnitude = abs(tide)
            assert np.isfinite(magnitude), f'Invalid tide for {wave.name}'

    def test_wave_table_multiple_accesses(self) -> None:
        """Test multiple accesses to same constituent."""
        wt = core.perth.WaveTable()

        wave1 = wt['M2']
        wave2 = wt['M2']

        # Both should have same name and properties
        assert wave1.name == wave2.name
        np.testing.assert_array_equal(
            wave1.doodson_numbers(), wave2.doodson_numbers()
        )

    def test_wave_table_types_are_valid(self) -> None:
        """Test that all wave types are valid."""
        wt = core.perth.WaveTable()

        for wave in wt.waves():
            wave_type = wave.type
            assert wave_type is not None

    def test_wave_table_is_modeled_property(self) -> None:
        """Test is_modeled property for all waves."""
        wt = core.perth.WaveTable()

        for wave in wt.waves():
            is_modeled = wave.is_modeled
            assert isinstance(is_modeled, (bool, np.bool_))

    def test_wave_table_xdo_representations_valid(self) -> None:
        """Test that Doodson representations are valid."""
        wt = core.perth.WaveTable()

        for wave in wt.waves():
            xdo_alpha = wave.xdo_alphabetical()
            xdo_num = wave.xdo_numerical()

            assert isinstance(xdo_alpha, str)
            assert isinstance(xdo_num, str)
            assert len(xdo_alpha) > 0
            assert len(xdo_num) > 0

    def test_wave_table_disabled_constituents(self) -> None:
        """Test WaveTable with constituents disabled from admittance
        calculations."""
        # These constituents will be disabled from admittance calculations
        disabled_constituents = [
            'M2',
            'S2',
            'N2',
            'K2',
            'K1',
            'O1',
            'P1',
            'Q1',
            'Mf',
            'Mm',
            'Ssa',
            'Sa',
            'M4',
            'MS4',
            'MN4',
        ]
        wt = core.perth.WaveTable(disabled_constituents)

        # The wave table should still contain all constituents (not a sparse
        # table)
        assert len(wt) >= len(disabled_constituents)

        # All disabled constituents should be accessible
        for const in disabled_constituents:
            assert const in wt
