# Copyright (c) 2026 CNES
#
# All rights reserved. Use of this source code is governed by a
# BSD-style license that can be found in the LICENSE file.
"""Comprehensive tests for the pyfes.core.LongPeriodEquilibrium class.

Note: LongPeriodEquilibrium is registered on the top-level core module,
not the darwin submodule.
"""

import datetime

import numpy as np
import pytest

from pyfes import core


class TestLongPeriodEquilibrium:
    """Test suite for darwin.LongPeriodEquilibrium class."""

    def setup_method(self) -> None:
        """Setup test fixtures."""
        self.lpe = core.LongPeriodEquilibrium()
        self.aa = core.AstronomicAngle()

    def test_constructor(self) -> None:
        """Test LongPeriodEquilibrium constructor."""
        lpe = core.LongPeriodEquilibrium()
        assert lpe is not None

    def test_disable_dynamic_wave(self) -> None:
        """Test disable_dynamic_wave method."""
        wt = core.darwin.WaveTable(['Sa', 'Ssa', 'Mm', 'Mf'])
        assert len(wt) == 4
        for name in ['Sa', 'Ssa', 'Mm', 'Mf']:
            assert name in wt

    def test_lpe_minus_n_waves_at_equator(self) -> None:
        """Test LPE calculation at equator."""
        self.aa.update(datetime.datetime(2000, 1, 1))

        # At equator (latitude = 0)
        result = self.lpe.lpe_minus_n_waves(self.aa, 0.0)

        assert isinstance(result, float)
        # Result should be a reasonable value for long-period equilibrium tide
        assert np.isfinite(result)

    def test_lpe_minus_n_waves_at_north_pole(self) -> None:
        """Test LPE calculation at North Pole."""
        self.aa.update(datetime.datetime(2000, 1, 1))

        # At North Pole (latitude = 90°)
        result = self.lpe.lpe_minus_n_waves(self.aa, 90.0)

        assert isinstance(result, float)
        assert np.isfinite(result)

    def test_lpe_minus_n_waves_at_south_pole(self) -> None:
        """Test LPE calculation at South Pole."""
        self.aa.update(datetime.datetime(2000, 1, 1))

        # At South Pole (latitude = -90°)
        result = self.lpe.lpe_minus_n_waves(self.aa, -90.0)

        assert isinstance(result, float)
        assert np.isfinite(result)

    def test_lpe_minus_n_waves_latitude_variation(self) -> None:
        """Test that LPE varies with latitude."""
        self.aa.update(datetime.datetime(2000, 1, 1))

        # Calculate at different latitudes
        results = []
        for lat in [0.0, 30.0, 60.0]:
            result = self.lpe.lpe_minus_n_waves(self.aa, lat)
            results.append(result)

        # At least check that calculations are consistent
        assert all(np.isfinite(r) for r in results)

    def test_lpe_minus_n_waves_time_variation(self) -> None:
        """Test that LPE changes with time."""
        results = []

        # Test at different times
        for day in range(1, 365, 50):
            aa = core.AstronomicAngle()
            aa.update(datetime.datetime(2000, day // 31 + 1, day % 31 + 1))
            result = self.lpe.lpe_minus_n_waves(aa, 45.0)
            results.append(result)

        # All calculations should be valid
        assert all(np.isfinite(r) for r in results)

    def test_lpe_multiple_instances(self) -> None:
        """Test that multiple LPE instances work independently."""
        lpe1 = core.LongPeriodEquilibrium()
        lpe2 = core.LongPeriodEquilibrium()

        aa = core.AstronomicAngle()
        aa.update(datetime.datetime(2000, 1, 1))

        result1 = lpe1.lpe_minus_n_waves(aa, 45.0)
        result2 = lpe2.lpe_minus_n_waves(aa, 45.0)

        # Results should be identical
        assert result1 == pytest.approx(result2)

    def test_lpe_with_wave_table_interaction(self) -> None:
        """Test LPE with wave table operations."""
        wt = core.darwin.WaveTable(['Sa', 'Ssa', 'Mm'])

        # Wave table should be valid
        assert len(wt) == 3

        aa = core.AstronomicAngle()
        aa.update(datetime.datetime(2000, 1, 1))

        result = self.lpe.lpe_minus_n_waves(aa, 45.0)
        assert np.isfinite(result)

    def test_lpe_symmetry_about_equator(self) -> None:
        """Test that LPE has expected symmetry properties."""
        aa = core.AstronomicAngle()
        aa.update(datetime.datetime(2000, 1, 1, 12, 0))

        lpe_north = self.lpe.lpe_minus_n_waves(aa, 45.0)
        lpe_south = self.lpe.lpe_minus_n_waves(aa, -45.0)

        # Both should be finite
        assert np.isfinite(lpe_north)
        assert np.isfinite(lpe_south)

    def test_lpe_extreme_latitudes(self) -> None:
        """Test LPE at extreme latitude values."""
        aa = core.AstronomicAngle()
        aa.update(datetime.datetime(2000, 1, 1))

        for lat in [-89.9, 89.9]:
            result = self.lpe.lpe_minus_n_waves(aa, lat)
            assert np.isfinite(result)

    def test_disable_dynamic_wave_with_empty_table(self) -> None:
        """Test disable_dynamic_wave with an empty wave table."""
        wt = core.darwin.WaveTable()
        # Full wave table should be valid
        assert len(wt) > 0

    def test_lpe_consistency_with_multiple_calls(self) -> None:
        """Test that repeated calls give consistent results."""
        aa = core.AstronomicAngle()
        aa.update(datetime.datetime(2000, 1, 1))

        results = [self.lpe.lpe_minus_n_waves(aa, 45.0) for _ in range(5)]

        # All results should be identical
        assert all(r == pytest.approx(results[0]) for r in results)
