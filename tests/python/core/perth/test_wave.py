# Copyright (c) 2026 CNES
#
# All rights reserved. Use of this source code is governed by a
# BSD-style license that can be found in the LICENSE file.
"""Comprehensive tests for the pyfes.core.perth.Wave class."""

import numpy as np
import pytest

from pyfes import core


class TestPerthWave:
    """Test suite for perth.Wave class properties."""

    def setup_method(self) -> None:
        """Setup test fixtures."""
        self.wave_table = core.perth.WaveTable(['M2', 'S2', 'N2', 'K1', 'O1'])

    def test_wave_name(self) -> None:
        """Test wave name property."""
        wave = self.wave_table['M2']
        name = wave.name

        assert isinstance(name, str)
        assert name == 'M2'

    def test_wave_doodson_number(self) -> None:
        """Test Doodson number method."""
        wave = self.wave_table['M2']
        doodson = wave.doodson_numbers()

        # Should be a vector of integers with 7 elements
        assert len(doodson) == 7

    def test_wave_tide_property(self) -> None:
        """Test tide (complex amplitude) property."""
        wave = self.wave_table['M2']
        tide = wave.tide

        # Should be a complex number
        assert isinstance(tide, (complex, np.complexfloating))

    def test_wave_vu_property(self) -> None:
        """Test vu (sum of Greenwich argument and nodal phase) property."""
        wave = self.wave_table['M2']
        vu = wave.vu

        # Should be a float
        assert isinstance(vu, float)
        # Should be a reasonable angle
        assert np.isfinite(vu)

    def test_wave_type(self) -> None:
        """Test wave type property."""
        wave = self.wave_table['M2']
        wave_type = wave.type

        # Type should be a valid WaveType
        assert wave_type is not None

    def test_wave_is_modeled(self) -> None:
        """Test is_modeled property."""
        wave = self.wave_table['M2']
        is_modeled = wave.is_modeled

        # Should be a boolean
        assert isinstance(is_modeled, (bool, np.bool_))

    def test_xdo_alphabetical(self) -> None:
        """Test alphabetical Doodson representation."""
        wave = self.wave_table['M2']
        xdo_alpha = wave.xdo_alphabetical()

        assert isinstance(xdo_alpha, str)
        assert len(xdo_alpha) > 0

    def test_xdo_numerical(self) -> None:
        """Test numerical Doodson representation."""
        wave = self.wave_table['M2']
        xdo_num = wave.xdo_numerical()

        assert isinstance(xdo_num, str)
        assert len(xdo_num) > 0

    def test_wave_properties_multiple_constituents(self) -> None:
        """Test wave properties across multiple constituents."""
        constituents = ['M2', 'S2', 'N2', 'K1', 'O1']

        for name in constituents:
            wave = self.wave_table[name]
            assert wave.name == name
            assert np.isfinite(wave.vu)
            assert len(wave.doodson_numbers()) == 7

    def test_wave_doodson_uniqueness(self) -> None:
        """Test that different constituents have different Doodson numbers."""
        m2 = self.wave_table['M2']
        s2 = self.wave_table['S2']

        # Different constituents should have different Doodson numbers
        assert not np.array_equal(m2.doodson_numbers(), s2.doodson_numbers())

    def test_wave_tide_amplitude(self) -> None:
        """Test tide property contains amplitude information."""
        wave = self.wave_table['M2']
        tide = wave.tide

        # Complex tide should have magnitude
        magnitude = abs(tide)
        assert isinstance(magnitude, float)
        assert np.isfinite(magnitude)

    def test_wave_type_consistency(self) -> None:
        """Test that types are consistent across constituents."""
        for wave in self.wave_table.waves():
            wave_type = wave.type
            assert wave_type is not None

    def test_wave_access_by_index(self) -> None:
        """Test accessing waves by constituent name."""
        names = ['M2', 'S2', 'N2']

        for name in names:
            wave = self.wave_table[name]
            assert wave.name == name

    def test_wave_iteration(self) -> None:
        """Test iterating over waves in table."""
        count = 0
        known = set(self.wave_table.constituents)
        for wave in self.wave_table.waves():
            assert wave.name in known
            assert len(wave.doodson_numbers()) == 7
            count += 1

        assert count == len(known)

    def test_wave_invalid_access(self) -> None:
        """Test that invalid wave access raises error."""
        with pytest.raises((KeyError, ValueError)):
            _ = self.wave_table['__INVALID__']

    def test_wave_comparison_semidiurnal_vs_diurnal(self) -> None:
        """Test comparing semidiurnal and diurnal waves."""
        m2 = self.wave_table['M2']
        k1 = self.wave_table['K1']

        # Both should be valid waves
        assert m2.name == 'M2'
        assert k1.name == 'K1'

    def test_wave_properties_are_read_only(self) -> None:
        """Test that wave properties are properly accessible."""
        wave = self.wave_table['M2']
        # These should all be accessible
        _ = wave.name
        _ = wave.doodson_numbers()
        _ = wave.tide
        _ = wave.vu
        _ = wave.type
        _ = wave.is_modeled
        _ = wave.xdo_alphabetical()
        _ = wave.xdo_numerical()

    def test_wave_frequency(self) -> None:
        """Test wave frequency method."""
        wave = self.wave_table['M2']
        freq = wave.frequency()

        assert isinstance(freq, float)
        assert freq > 0

    def test_wave_period(self) -> None:
        """Test wave period property."""
        wave = self.wave_table['M2']
        period = wave.period

        assert isinstance(period, float)
        assert period > 0


class TestPerthWaveTable:
    """Test suite for perth.WaveTable class."""

    def test_wave_table_constructor(self) -> None:
        """Test WaveTable constructor."""
        wt = core.perth.WaveTable()
        assert len(wt) > 0

    def test_wave_table_with_specific_constituents(self) -> None:
        """Test WaveTable with specific constituents."""
        constituents = ['M2', 'S2', 'K1']
        wt = core.perth.WaveTable(constituents)

        assert len(wt) > 0
        for name in constituents:
            assert name in wt

    def test_wave_table_constituents(self) -> None:
        """Test constituents property."""
        wt = core.perth.WaveTable(['M2', 'S2', 'K1'])
        names = wt.constituents

        assert isinstance(names, list)
        assert len(names) > 0

    def test_wave_table_waves(self) -> None:
        """Test waves() method."""
        wt = core.perth.WaveTable(['M2', 'S2'])
        waves = wt.waves()

        assert isinstance(waves, list)
        assert len(waves) > 0

    def test_wave_table_len(self) -> None:
        """Test __len__ method."""
        wt = core.perth.WaveTable()

        assert len(wt) == len(wt.constituents)

    def test_wave_table_contains(self) -> None:
        """Test __contains__ method."""
        wt = core.perth.WaveTable(['M2', 'S2'])

        assert 'M2' in wt
        assert 'S2' in wt
        assert '__INVALID__' not in wt

    def test_wave_table_getitem(self) -> None:
        """Test __getitem__ method."""
        wt = core.perth.WaveTable(['M2'])
        wave = wt['M2']

        assert wave.name == 'M2'

    def test_wave_table_iteration(self) -> None:
        """Test iteration via waves() method."""
        constituents = ['M2', 'S2', 'K1']
        wt = core.perth.WaveTable(constituents)

        names = [wave.name for wave in wt.waves()]

        for constituent in constituents:
            assert constituent in names

    def test_wave_table_empty_vs_full(self) -> None:
        """Test difference between empty and full table."""
        empty_wt = core.perth.WaveTable([])
        full_wt = core.perth.WaveTable()

        assert len(full_wt) >= len(empty_wt)

    def test_wave_table_duplicate_constituents(self) -> None:
        """Test WaveTable with duplicate constituents."""
        wt = core.perth.WaveTable(['M2', 'M2', 'S2'])

        assert 'M2' in wt
        assert 'S2' in wt


class TestPerthWaveTableIntegration:
    """Integration tests for PERTH wave table operations."""

    def test_wave_table_consistency(self) -> None:
        """Test consistency of wave table operations."""
        wt1 = core.perth.WaveTable(['M2', 'S2'])
        wt2 = core.perth.WaveTable(['M2', 'S2'])

        assert len(wt1) == len(wt2)

        m2_1 = wt1['M2']
        m2_2 = wt2['M2']

        assert m2_1.name == m2_2.name

    def test_wave_table_subset_operations(self) -> None:
        """Test wave table operations with different constituents."""
        full_wt = core.perth.WaveTable()
        partial_wt = core.perth.WaveTable(['M2', 'S2', 'K1'])

        # All waves in partial_wt should be in full_wt
        for wave in partial_wt.waves():
            assert wave.name in full_wt

    def test_wave_frequency_comparison(self) -> None:
        """Test comparing wave frequencies."""
        wt = core.perth.WaveTable(['M2', 'K1'])

        m2 = wt['M2']
        k1 = wt['K1']

        # Both should have valid frequencies
        assert np.isfinite(m2.frequency())
        assert np.isfinite(k1.frequency())

    def test_wave_doodson_consistency(self) -> None:
        """Test Doodson number consistency."""
        wt = core.perth.WaveTable(['M2'])

        wave1 = wt['M2']
        doodson1 = np.array(wave1.doodson_numbers())

        wave2 = wt['M2']
        doodson2 = np.array(wave2.doodson_numbers())

        # Doodson numbers should be identical
        np.testing.assert_array_equal(doodson1, doodson2)
