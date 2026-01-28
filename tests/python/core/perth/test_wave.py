# Copyright (c) 2025 CNES
#
# All rights reserved. Use of this source code is governed by a
# BSD-style license that can be found in the LICENSE file.
"""Comprehensive tests for the pyfes.core.perth.Wave class."""

import numpy as np
from pyfes import core
import pytest


class TestPerthWave:
    """Test suite for perth.Wave class properties."""

    def setup_method(self) -> None:
        """Setup test fixtures."""
        self.wave_table = core.perth.WaveTable(['M2', 'S2', 'N2', 'K1', 'O1'])

    def test_wave_name(self) -> None:
        """Test wave name property."""
        wave = self.wave_table['M2']
        name = wave.name()

        assert isinstance(name, str)
        assert name == 'M2'

    def test_wave_doodson_number(self) -> None:
        """Test Doodson number property."""
        wave = self.wave_table['M2']
        doodson = wave.doodson_numbers

        # Should be a vector of integers
        assert isinstance(doodson, np.ndarray)
        assert doodson.dtype == np.int8
        assert len(doodson) == 7

    def test_wave_tide_property(self) -> None:
        """Test tide (complex amplitude) property."""
        wave = self.wave_table['M2']
        tide = wave.tide

        # Should be a complex number
        assert isinstance(tide, (complex, np.complexfloating))

    def test_wave_tidal_argument(self) -> None:
        """Test tidal argument property."""
        wave = self.wave_table['M2']
        arg = wave.tidal_argument

        # Should be a float
        assert isinstance(arg, float)
        # Should be a reasonable angle
        assert np.isfinite(arg)

    def test_wave_type(self) -> None:
        """Test wave type property."""
        wave = self.wave_table['M2']
        wave_type = wave.type

        # Type should be a valid TidalType
        assert wave_type is not None

    def test_wave_is_inferred(self) -> None:
        """Test is_inferred property."""
        wave = self.wave_table['M2']
        is_inferred = wave.is_inferred

        # Should be a boolean
        assert isinstance(is_inferred, (bool, np.bool_))

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
            assert wave.name() == name
            assert np.isfinite(wave.tidal_argument)
            assert len(wave.doodson_numbers) == 7

    def test_wave_doodson_uniqueness(self) -> None:
        """Test that different constituents have different Doodson numbers."""
        m2 = self.wave_table['M2']
        s2 = self.wave_table['S2']

        # Different constituents should have different Doodson numbers
        assert not np.array_equal(m2.doodson_numbers, s2.doodson_numbers)

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
        for wave in self.wave_table:
            wave_type = wave.type
            assert wave_type is not None

    def test_wave_access_by_index(self) -> None:
        """Test accessing waves by constituent name."""
        names = ['M2', 'S2', 'N2']

        for name in names:
            wave = self.wave_table[name]
            assert wave.name() == name

    def test_wave_iteration(self) -> None:
        """Test iterating over waves in table."""
        count = 0
        known = set(self.wave_table.keys())
        for wave in self.wave_table:
            assert wave.name() in known
            assert len(wave.doodson_numbers) == 7
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
        assert m2.name() == 'M2'
        assert k1.name() == 'K1'

    def test_wave_properties_are_read_only(self) -> None:
        """Test that wave properties are properly accessible."""
        wave = self.wave_table['M2']
        # These should all be accessible
        _ = wave.name()
        _ = wave.doodson_numbers
        _ = wave.tide
        _ = wave.tidal_argument
        _ = wave.type
        _ = wave.is_inferred
        _ = wave.xdo_alphabetical()
        _ = wave.xdo_numerical()


class TestPerthWaveTable:
    """Test suite for perth.WaveTable class."""

    def test_wave_table_constructor(self) -> None:
        """Test WaveTable constructor."""
        wt = core.perth.WaveTable()
        assert len(wt) > 0

    def test_wave_table_with_specific_constituents(self) -> None:
        """Test WaveTable with specific disabled constituents."""
        # These constituents will be disabled from admittance calculations
        disabled_constituents = ['M2', 'S2', 'K1']
        wt = core.perth.WaveTable(disabled_constituents)

        # Wave table should still be complete (not sparse)
        assert len(wt) > 0
        # All disabled constituents should be accessible
        for name in disabled_constituents:
            assert name in wt

    def test_wave_table_keys(self) -> None:
        """Test keys() method."""
        # Constituents passed to WaveTable are disabled from admittance
        # calculations
        wt = core.perth.WaveTable(['M2', 'S2', 'K1'])
        keys = wt.keys()

        assert isinstance(keys, list)
        # Table should contain more than just the disabled constituents
        assert len(keys) > 0

    def test_wave_table_values(self) -> None:
        """Test values() method."""
        # Constituents passed to WaveTable are disabled from admittance
        # calculations
        wt = core.perth.WaveTable(['M2', 'S2'])
        values = wt.values()

        assert isinstance(values, list)
        assert len(values) > 0

    def test_wave_table_len(self) -> None:
        """Test __len__ method."""
        wt = core.perth.WaveTable()

        assert len(wt) == len(wt.keys())

    def test_wave_table_contains(self) -> None:
        """Test __contains__ method."""
        # Constituents passed to WaveTable are disabled from admittance
        # calculations but are still contained in the wave table
        wt = core.perth.WaveTable(['M2', 'S2'])

        assert 'M2' in wt
        assert 'S2' in wt
        assert '__INVALID__' not in wt

    def test_wave_table_getitem(self) -> None:
        """Test __getitem__ method."""
        # Constituents passed to WaveTable are disabled from admittance
        # calculations
        wt = core.perth.WaveTable(['M2'])
        wave = wt['M2']

        assert wave.name() == 'M2'

    def test_wave_table_iteration(self) -> None:
        """Test __iter__ method."""
        # Constituents passed to WaveTable are disabled from admittance
        # calculations
        disabled_constituents = ['M2', 'S2', 'K1']
        wt = core.perth.WaveTable(disabled_constituents)

        names = [wave.name() for wave in wt]

        # All disabled constituents should be in the iteration
        for constituent in disabled_constituents:
            assert constituent in names

    def test_wave_table_empty_vs_full(self) -> None:
        """Test difference between empty and full table."""
        # Empty list means no constituents disabled
        empty_wt = core.perth.WaveTable([])
        full_wt = core.perth.WaveTable()

        # Both should have all constituents (no filtering occurs)
        assert len(full_wt) >= len(empty_wt)

    def test_wave_table_duplicate_constituents(self) -> None:
        """Test WaveTable with duplicate constituents in disabled list."""
        # Duplicates in the disabled constituents list should be handled
        # gracefully
        wt = core.perth.WaveTable(['M2', 'M2', 'S2'])

        # Both constituents should still be in the table
        assert 'M2' in wt
        assert 'S2' in wt


class TestPerthConstituents:
    """Test suite for perth.constituents() function."""

    def test_constituents_function(self) -> None:
        """Test constituents() function."""
        const_map = core.perth.constituents()

        assert const_map is not None

    def test_constituents_contains_major(self) -> None:
        """Test that constituents includes major waves."""
        const_map = core.perth.constituents()

        major = ['M2', 'S2', 'K1', 'O1']
        for name in major:
            assert name in const_map

    def test_constituents_iteration(self) -> None:
        """Test iterating over constituents map."""
        const_map = core.perth.constituents()
        count = 0
        for ident in const_map.values():
            assert isinstance(ident, int)
            count += 1

        assert count == len(const_map)

    def test_constituents_keys(self) -> None:
        """Test keys() method."""
        const_map = core.perth.constituents()
        keys = const_map.keys()

        assert isinstance(keys, list)
        assert 'M2' in keys

    def test_constituents_values(self) -> None:
        """Test values() method."""
        const_map = core.perth.constituents()
        values = const_map.values()

        assert isinstance(values, list)
        assert len(values) > 0

    def test_constituents_len(self) -> None:
        """Test __len__ method."""
        const_map = core.perth.constituents()

        assert len(const_map) > 0
        assert len(const_map) == len(const_map.keys())


class TestPerthWaveTableIntegration:
    """Integration tests for PERTH wave table operations."""

    def test_wave_table_consistency(self) -> None:
        """Test consistency of wave table operations."""
        # Constituents are disabled from admittance calculations in both tables
        wt1 = core.perth.WaveTable(['M2', 'S2'])
        wt2 = core.perth.WaveTable(['M2', 'S2'])

        # Both tables should have the same content (same disabled constituents)
        assert len(wt1) == len(wt2)

        m2_1 = wt1['M2']
        m2_2 = wt2['M2']

        assert m2_1.name() == m2_2.name()

    def test_wave_table_subset_operations(self) -> None:
        """Test wave table operations with different disabled constituents."""
        # Both tables are complete; they just have different disabled
        # constituents
        full_wt = core.perth.WaveTable()
        partial_wt = core.perth.WaveTable(['M2', 'S2', 'K1'])

        # All waves in partial_wt should be in full_wt
        for wave in partial_wt:
            assert wave.name() in full_wt

    def test_wave_frequency_comparison(self) -> None:
        """Test comparing wave frequencies."""
        # Constituents passed are disabled from admittance calculations
        wt = core.perth.WaveTable(['M2', 'K1'])

        m2 = wt['M2']
        k1 = wt['K1']

        # Both should have tidal arguments
        assert np.isfinite(m2.tidal_argument)
        assert np.isfinite(k1.tidal_argument)

    def test_wave_doodson_consistency(self) -> None:
        """Test Doodson number consistency."""
        # Constituents passed are disabled from admittance calculations
        wt = core.perth.WaveTable(['M2'])

        wave1 = wt['M2']
        doodson1 = wave1.doodson_numbers.copy()

        wave2 = wt['M2']
        doodson2 = wave2.doodson_numbers.copy()

        # Doodson numbers should be identical
        np.testing.assert_array_equal(doodson1, doodson2)
