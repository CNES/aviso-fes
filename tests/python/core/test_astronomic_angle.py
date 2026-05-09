# Copyright (c) 2026 CNES
#
# All rights reserved. Use of this source code is governed by a
# BSD-style license that can be found in the LICENSE file.
import datetime
import threading

import pytest

from pyfes import core


# Expected angle values for ``2000-01-01T00:00:00``, organised by formulation
# and by build flavor. Tests pull from the entry matching ``constants_flavor``
# (``'schureman'`` or ``'iers2010'``); rebuilding the C++ library with
# ``FES_USE_IERS_CONSTANTS`` flips the fixture, and the corresponding
# ``iers2010`` block must hold the values produced by that build.
EXPECTED_ANGLES: dict[str, dict[str, dict[str, float]]] = {
    'MEEUS': {
        'schureman': {
            'h': 4.886470984554807,
            'n': 2.1829004947295840,
            'nu': 0.20721813091600161,
            'nuprim': 0.13805659123725886,
            'nusec': 0.132258440531486,
            'p': 1.4538178589080255,
            'p1': 4.93818437065075,
            'r': 0.10104533494633117,
            's': 3.6955259116230854,
            't': 3.1415926535897931,
            'x1ra': 1.1723204500596927,
            'xi': 0.19203231321420278,
        },
        'iers2010': {
            'h': 4.8864709410237595,
            'n': 2.1829004970682835,
            'nu': 0.20735602997621727,
            'nuprim': 0.1382466880942348,
            'nusec': 0.132515605173428,
            'p': 1.4538178539878968,
            'p1': 4.938184370648674,
            'r': 0.10098738315829554,
            's': 3.695525329688446,
            't': 3.141592653589793,
            'x1ra': 1.1720793639429103,
            'xi': 0.1921797482814256,
        },
    },
    'SCHUREMAN_ORDER_3': {
        'schureman': {
            'h': 4.886452090906138,
            'n': 2.1828609691751804,
            'nu': 0.20722218281018479,
            'nuprim': 0.13805962974049285,
            'nusec': 0.1322618637171819,
            'p': 1.4537574500601673,
            'p1': 4.938242280709079,
            'r': 0.10106627840451653,
            's': 3.6956256194908494,
            't': 3.141592653589793,
            'x1ra': 1.172310749175076,
            'xi': 0.19203593918558792,
        },
        'iers2010': {
            'h': 4.886452090906138,
            'n': 2.1828609691751804,
            'nu': 0.2073600834294561,
            'nuprim': 0.13824972990136372,
            'nusec': 0.13251903316860011,
            'p': 1.4537574500601673,
            'p1': 4.938242280709079,
            'r': 0.1010083032393994,
            's': 3.6956256194908494,
            't': 3.141592653589793,
            'x1ra': 1.1720696689212957,
            'xi': 0.19218337605697933,
        },
    },
    'IERS': {
        'schureman': {
            'h': 4.8864743802966597,
            'n': 2.1829004947295840,
            'nu': 0.20721813091600161,
            'nuprim': 0.13805659123725886,
            'nusec': 0.132258440531486,
            'p': 1.4538178589080255,
            'p1': 4.9381877667320335,
            'r': 0.10104533494633117,
            's': 3.6955293661624493,
            't': 3.1415926535897931,
            'x1ra': 1.1723204500596927,
            'xi': 0.19203231321420278,
        },
        'iers2010': {
            'h': 4.8864743367656125,
            'n': 2.182900621154722,
            'nu': 0.20735601725089692,
            'nuprim': 0.13824667854493217,
            'nusec': 0.1325155944118521,
            'p': 1.4538177950180629,
            'p1': 4.938187766729955,
            'r': 0.10098739264009973,
            's': 3.69552878422781,
            't': 3.141592653589793,
            'x1ra': 1.172079344249698,
            'xi': 0.1921797368924345,
        },
    },
}


def _check_angles(aa: core.AstronomicAngle, expected: dict[str, float]) -> None:
    for name, value in expected.items():
        assert getattr(aa, name) == pytest.approx(value, rel=1e-6), name


def test_astronomic_angle(constants_flavor: str) -> None:
    """Test the computation of astronomical angles."""
    aa = core.AstronomicAngle(core.MEEUS)
    aa.update(datetime.datetime(2000, 1, 1))
    assert isinstance(aa, core.AstronomicAngle)
    _check_angles(aa, EXPECTED_ANGLES['MEEUS'][constants_flavor])

    aa = core.AstronomicAngle(core.SCHUREMAN_ORDER_3)
    aa.update(datetime.datetime(2000, 1, 1))
    _check_angles(aa, EXPECTED_ANGLES['SCHUREMAN_ORDER_3'][constants_flavor])

    aa = core.AstronomicAngle(core.IERS)
    aa.update(datetime.datetime(2000, 1, 1))
    _check_angles(aa, EXPECTED_ANGLES['IERS'][constants_flavor])


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
