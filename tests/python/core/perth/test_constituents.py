# Copyright (c) 2025 CNES
#
# All rights reserved. Use of this source code is governed by a
# BSD-style license that can be found in the LICENSE file.
"""Comprehensive tests for the pyfes.core.perth.constituents() function."""

from pyfes import core
import pytest


class TestConstituents:
    """Test suite for darwin.constituents() function."""

    def test_constituents_returns_map(self) -> None:
        """Test that constituents() returns a ConstituentMap."""
        const_map = core.perth.constituents()

        assert const_map is not None
        # ConstituentMap maps constituent names to integer IDs
        assert hasattr(const_map, '__contains__')
        assert hasattr(const_map, '__getitem__')
        assert hasattr(const_map, 'keys')
        assert hasattr(const_map, 'values')

    def test_constituents_contains_major_waves(self) -> None:
        """Test that constituent map contains major tidal waves."""
        const_map = core.perth.constituents()

        major_constituents = ['M2', 'S2', 'K1', 'O1', 'N2', 'K2']

        for constituent in major_constituents:
            assert constituent in const_map, (
                f'{constituent} not found in constituents map'
            )

    def test_constituents_map_keys(self) -> None:
        """Test that constituent map has expected keys."""
        const_map = core.perth.constituents()

        # Map should have multiple constituents
        keys = const_map.keys()
        assert len(keys) > 0
        assert len(keys) >= 6  # At least the major constituents

    def test_constituents_map_values(self) -> None:
        """Test that constituent map has valid values."""
        const_map = core.perth.constituents()

        # Get values from map (should be integers)
        values = const_map.values()
        assert len(values) > 0

        # All values should be integers (constituent IDs)
        for constituent_id in values:
            assert isinstance(constituent_id, int)

    def test_constituents_map_iteration(self) -> None:
        """Test iteration over constituent map items."""
        const_map = core.perth.constituents()

        # Iterate over items (name, ID pairs)
        items = const_map.items()
        assert len(items) > 0

        count = 0
        for const_id, name in items:
            assert isinstance(const_id, int)
            assert isinstance(name, str)
            count += 1

        assert count > 0

    def test_constituents_map_contains(self) -> None:
        """Test membership testing in constituent map."""
        const_map = core.perth.constituents()

        # Test known constituents
        assert 'M2' in const_map
        assert 'K1' in const_map

        # Test unknown constituent
        assert '__UNKNOWN__' not in const_map

    def test_constituents_map_getitem(self) -> None:
        """Test accessing constituent IDs by name."""
        const_map = core.perth.constituents()

        # Should return integer IDs
        m2_id = const_map['M2']
        assert isinstance(m2_id, int)
        assert m2_id >= 0

        k1_id = const_map['K1']
        assert isinstance(k1_id, int)
        assert k1_id >= 0

    def test_constituents_map_invalid_access(self) -> None:
        """Test that invalid constituent access raises error."""
        const_map = core.perth.constituents()

        with pytest.raises((KeyError, ValueError)):
            _ = const_map['__INVALID__']

    def test_constituents_map_size(self) -> None:
        """Test size of constituent map."""
        const_map = core.perth.constituents()

        # Should have at least 80 constituents in Darwin
        assert len(const_map) >= 80

    def test_constituents_wave_properties_via_table(self) -> None:
        """Test accessing wave properties via WaveTable."""
        # Constituents passed are disabled from admittance calculations
        wt = core.perth.WaveTable(['M2', 'K1', 'S2'])

        m2 = wt['M2']

        # Should have frequency
        assert m2.freq > 0

        # Should have type
        assert m2.type is not None

        # Should have other properties
        assert m2.name() == 'M2'

    def test_constituents_map_consistency(self) -> None:
        """Test that multiple calls return consistent data."""
        const_map1 = core.perth.constituents()
        const_map2 = core.perth.constituents()

        # Both maps should have same size
        assert len(const_map1) == len(const_map2)

        # Same constituent should have same ID
        m2_id_1 = const_map1['M2']
        m2_id_2 = const_map2['M2']

        assert m2_id_1 == m2_id_2

    def test_constituents_all_have_names(self) -> None:
        """Test that all constituents have valid names."""
        const_map = core.perth.constituents()

        keys = const_map.keys()
        for name in keys:
            assert isinstance(name, str)
            assert len(name) > 0

    def test_constituents_all_have_valid_ids(self) -> None:
        """Test that all constituents have valid integer IDs."""
        const_map = core.perth.constituents()

        values = const_map.values()
        for const_id in values:
            assert isinstance(const_id, int)
            assert const_id >= 0

    def test_constituents_map_items_consistency(self) -> None:
        """Test items are consistent with keys and values."""
        const_map = core.perth.constituents()

        keys = const_map.keys()
        values = const_map.values()
        items = const_map.items()

        # Should have same length
        assert len(keys) == len(values)
        assert len(keys) == len(items)

        # Items should match keys and values
        for (item_id, item_name), name, const_id in zip(
            items, keys, values, strict=False
        ):
            assert item_name == name
            assert item_id == const_id

    def test_constituents_map_keys_method(self) -> None:
        """Test keys() method of constituent map."""
        const_map = core.perth.constituents()

        keys = const_map.keys()

        assert isinstance(keys, list)
        assert len(keys) > 0
        assert 'M2' in keys
        assert 'K1' in keys

    def test_constituents_map_values_method(self) -> None:
        """Test values() method of constituent map."""
        const_map = core.perth.constituents()

        values = const_map.values()

        assert isinstance(values, list)
        assert len(values) > 0

        # All should be integers
        for const_id in values:
            assert isinstance(const_id, int)

    def test_constituents_map_len(self) -> None:
        """Test __len__ method of constituent map."""
        const_map = core.perth.constituents()

        length = len(const_map)

        # Should match number of keys
        assert length == len(const_map.keys())

        # Should match number of values
        assert length == len(const_map.values())

        # Should have reasonable size (at least major constituents)
        assert length >= 6

    def test_constituents_get_name_method(self) -> None:
        """Test get_name() method to retrieve names from IDs."""
        const_map = core.perth.constituents()

        # Get M2's ID
        m2_id = const_map['M2']

        # Retrieve name from ID
        name = const_map.get_name(m2_id)
        assert name == 'M2'

    def test_constituents_major_constituents_present(self) -> None:
        """Test that all major tidal constituents are present."""
        wt = core.perth.WaveTable()

        major = ['M2', 'S2', 'N2', 'K2', 'K1', 'O1', 'P1', 'Q1']

        for name in major:
            assert name in wt, f'{name} not found in WaveTable'
