# Copyright (c) 2025 CNES
#
# All rights reserved. Use of this source code is governed by a
# BSD-style license that can be found in the LICENSE file.
import datetime
import threading

from pyfes import core
import pytest


def test_astronomic_angle() -> None:
    """Test the computation of astronomical angles."""
    aa = core.AstronomicAngle(core.MEEUS)
    aa.update(datetime.datetime(2000, 1, 1))
    assert isinstance(aa, core.AstronomicAngle)
    assert aa.h == pytest.approx(4.886470984554807, rel=1e-6)
    assert aa.n == pytest.approx(2.1829004947295840, rel=1e-6)
    assert aa.nu == pytest.approx(0.20721813091600161, rel=1e-6)
    assert aa.nuprim == pytest.approx(0.13805659123725886, rel=1e-6)
    assert aa.nusec == pytest.approx(0.132258440531486, rel=1e-6)
    assert aa.p == pytest.approx(1.4538178589080255, rel=1e-6)
    assert aa.p1 == pytest.approx(4.93818437065075, rel=1e-6)
    assert aa.r == pytest.approx(0.10104533494633117, rel=1e-6)
    assert aa.s == pytest.approx(3.6955259116230854, rel=1e-6)
    assert aa.t == pytest.approx(3.1415926535897931, rel=1e-6)
    assert aa.x1ra == pytest.approx(1.1723204500596927, rel=1e-6)
    assert aa.xi == pytest.approx(0.19203231321420278, rel=1e-6)

    aa = core.AstronomicAngle(core.SCHUREMAN_ORDER_3)
    aa.update(datetime.datetime(2000, 1, 1))
    assert aa.h == pytest.approx(4.886452090906138, rel=1e-6)
    assert aa.n == pytest.approx(2.1828609691751804, rel=1e-6)
    assert aa.p == pytest.approx(1.4537574500601673, rel=1e-6)
    assert aa.p1 == pytest.approx(4.938241458772235, rel=1e-6)
    assert aa.s == pytest.approx(3.6956256194908494, rel=1e-6)
    assert aa.t == pytest.approx(3.141592653589793, rel=1e-6)

    aa = core.AstronomicAngle(core.IERS)
    aa.update(datetime.datetime(2000, 1, 1))
    assert aa.h == pytest.approx(4.8864743802966597, 1e-6)
    assert aa.n == pytest.approx(2.1829004947295840, 1e-6)
    assert aa.nu == pytest.approx(0.20721813091600161, 1e-6)
    assert aa.nuprim == pytest.approx(0.13805659123725886, 1e-6)
    assert aa.nusec == pytest.approx(0.132258440531486, 1e-6)
    assert aa.p == pytest.approx(1.4538178589080255, 1e-6)
    assert aa.p1 == pytest.approx(4.9381877667320335, 1e-6)
    assert aa.r == pytest.approx(0.10104533494633117, 1e-6)
    assert aa.s == pytest.approx(3.6955293661624493, 1e-6)
    assert aa.t == pytest.approx(3.1415926535897931, 1e-6)
    assert aa.x1ra == pytest.approx(1.1723204500596927, 1e-6)
    assert aa.xi == pytest.approx(0.19203231321420278, 1e-6)


def test_astronomical_angle_thread_safety() -> None:
    """Test that datetime operations properly acquire GIL."""

    def update_astronomical_angles(thread_id, results) -> None:
        """Function that manipulates dates - should acquire GIL."""
        try:
            angle = core.AstronomicAngle()

            # These operations involve Python datetime objects
            dates = [
                datetime.datetime(2024, 1, 1, 0, 0, 0),
                datetime.datetime(2024, 6, 1, 12, 0, 0),
                datetime.datetime(2024, 12, 31, 23, 59, 59),
            ]

            for date in dates:
                # This should handle GIL internally
                angle.update(date)

            results[thread_id] = {
                'success': True,
                'h': angle.h,
                's': angle.s,
                'p': angle.p,
            }
        except Exception as e:
            results[thread_id] = {'success': False, 'error': str(e)}

    # Run multiple threads manipulating dates
    num_threads = 8
    results: dict[int, dict[str, float | bool]] = {}
    threads = []

    for i in range(num_threads):
        thread = threading.Thread(
            target=update_astronomical_angles, args=(i, results)
        )
        threads.append(thread)
        thread.start()

    for thread in threads:
        thread.join()

    # All threads should complete successfully
    for thread_id, result in results.items():
        assert result['success'], (
            f'Thread {thread_id} failed: {result.get("error")}'
        )
        assert 'h' in result
        assert 's' in result
        assert 'p' in result
