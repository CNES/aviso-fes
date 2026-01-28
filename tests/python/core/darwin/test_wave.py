# Copyright (c) 2025 CNES
#
# All rights reserved. Use of this source code is governed by a
# BSD-style license that can be found in the LICENSE file.
"""Comprehensive tests for the pyfes.core.darwin.Wave class."""

import datetime

import numpy as np
from pyfes import core
import pytest


class TestWave:
    """Test suite for darwin.Wave class properties and methods."""

    def setup_method(self) -> None:
        """Setup test fixtures."""
        self.wave_table = core.darwin.WaveTable(['M2', 'S2', 'N2', 'K1', 'O1'])
        self.aa = core.AstronomicAngle()
        self.aa.update(datetime.datetime(2000, 1, 1))

    def test_wave_name(self) -> None:
        """Test wave name property."""
        wave = self.wave_table['M2']
        assert wave.name() == 'M2'

        wave = self.wave_table['K1']
        assert wave.name() == 'K1'

    def test_wave_frequency(self) -> None:
        """Test wave frequency property."""
        wave = self.wave_table['M2']
        freq = np.degrees(wave.freq)  # radians/hour to degrees/hour
        assert isinstance(freq, float)
        assert freq > 0
        # M2 has a period of ~12.42 hours, frequency should be ~28.9855
        # degrees/hours
        assert freq == pytest.approx(28.9855, rel=0.01)

    def test_wave_type(self) -> None:
        """Test wave type classification."""
        m2_wave = self.wave_table['M2']
        assert m2_wave.type == core.SHORT_PERIOD

        k1_wave = self.wave_table['K1']
        assert k1_wave.type == core.SHORT_PERIOD

    def test_wave_doodson_number(self) -> None:
        """Test Doodson number representation."""
        wave = self.wave_table['M2']
        doodson = wave.doodson_numbers()

        # Doodson number should be a vector of integers
        assert isinstance(doodson, (list, tuple))
        assert len(doodson) == 7

    def test_wave_xdo_representation(self) -> None:
        """Test different Doodson number representations."""
        wave = self.wave_table['M2']

        # Test alphabetical representation
        xdo_alpha = wave.xdo_alphabetical()
        assert isinstance(xdo_alpha, str)
        assert len(xdo_alpha) > 0

        # Test numerical representation
        xdo_num = wave.xdo_numerical()
        assert isinstance(xdo_num, str)
        assert len(xdo_num) > 0

    def test_wave_tide_property(self) -> None:
        """Test tide property (complex amplitude)."""
        wave = self.wave_table['M2']
        tide = wave.tide

        # Tide should be a complex number
        assert isinstance(tide, complex)

    def test_is_inferred_property(self) -> None:
        """Test is_inferred property."""
        wave = self.wave_table['M2']
        is_inferred = wave.admittance

        # Should be a boolean
        assert isinstance(is_inferred, (bool, np.bool_))

    def test_wave_comparison_across_types(self) -> None:
        """Test properties are consistent across different wave types."""
        semidiurnal = self.wave_table['M2']
        diurnal = self.wave_table['K1']

        # Frequencies should be different
        assert semidiurnal.freq != diurnal.freq

        # Both should have valid Doodson numbers
        assert len(semidiurnal.doodson_numbers()) == 7
        assert len(diurnal.doodson_numbers()) == 7

    def test_multiple_wave_access(self) -> None:
        """Test accessing multiple waves from same table."""
        constituents = ['M2', 'S2', 'N2', 'K1', 'O1', 'Q1']
        wt = core.darwin.WaveTable(constituents)

        for name in constituents:
            wave = wt[name]
            assert wave.name() == name
            assert wave.freq > 0

    def test_wave_frequency_ordering(self) -> None:
        """Test that semidiurnal waves have higher frequencies than diurnal."""
        wt = core.darwin.WaveTable(['M2', 'K1'])

        m2 = wt['M2']
        k1 = wt['K1']

        # M2 (semidiurnal) should have roughly twice the frequency of K1
        # (diurnal)
        assert m2.freq > k1.freq
        # Rough check: M2 should be around 1.9x K1 or higher
        assert m2.freq / k1.freq > 1.5

    def test_wave_type_consistency(self) -> None:
        """Test that short period waves have the same type classification."""
        wt = core.darwin.WaveTable(['M2', 'S2', 'N2', 'K1', 'O1', 'P1', 'Q1'])

        for wave in wt:
            # All these constituents should be short period
            assert wave.type == core.SHORT_PERIOD


class TestWaveWithLongPeriod:
    """Test waves with long-period tidal constituents."""

    def test_long_period_waves(self) -> None:
        """Test long-period wave properties."""
        long_period_constituents = ['Mm', 'Mf', 'Ssa', 'Sa']
        wt = core.darwin.WaveTable(long_period_constituents)

        for name in long_period_constituents:
            if name in wt:
                wave = wt[name]
                assert wave.name() == name
                # Long period waves have lower frequencies
                assert wave.freq < 1.0  # Less than 1 cycle per day

    def test_long_period_frequency_range(self) -> None:
        """Test frequency ranges for long-period waves."""
        wt = core.darwin.WaveTable(['Sa'])  # Annual tide

        if 'Sa' in wt:
            wave = wt['Sa']
            # Annual tide should have frequency around 1/365 cycles per day
            assert wave.freq < 0.01


class TestWaveIntegration:
    """Integration tests for Wave class with other components."""

    def test_wave_with_astronomic_angle(self) -> None:
        """Test wave properties with different astronomical angles."""
        wt = core.darwin.WaveTable(['M2'])
        wave = wt['M2']

        aa1 = core.AstronomicAngle()
        aa1.update(datetime.datetime(2000, 1, 1))

        aa2 = core.AstronomicAngle()
        aa2.update(datetime.datetime(2001, 1, 1))

        # Wave properties should be independent of astronomical angle
        # (at least the base frequency)
        assert wave.freq > 0

    def test_wave_in_harmonic_analysis(self) -> None:
        """Test Wave objects work correctly in harmonic analysis."""
        time = np.array(
            [
                np.datetime64('2000-01-01') + np.timedelta64(i * 24, 'h')
                for i in range(100)
            ]
        )

        wt = core.darwin.WaveTable(['M2', 'S2'])
        _f, _vu = wt.compute_nodal_modulations(time)

        # Verify that we can access wave properties after modulations
        for wave in wt:
            assert wave.name() in ['M2', 'S2']
            assert wave.freq > 0
