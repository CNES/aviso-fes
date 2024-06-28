# Copyright (c) 2023 CNES
#
# All rights reserved. Use of this source code is governed by a
# BSD-style license that can be found in the LICENSE file.
import datetime

import pytest

from ... import core


def test_astronomic_angle():
    aa = core.AstronomicAngle(core.kMeeus)
    aa.update(datetime.datetime(2000, 1, 1), 32)
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

    aa = core.AstronomicAngle(core.kSchuremanOrder3)
    aa.update(datetime.datetime(2000, 1, 1), 32)
    assert aa.h == pytest.approx(4.886452090906138, rel=1e-6)
    assert aa.n == pytest.approx(2.1828609691751804, rel=1e-6)
    assert aa.p == pytest.approx(1.4537574500601673, rel=1e-6)
    assert aa.p1 == pytest.approx(4.938241458772235, rel=1e-6)
    assert aa.s == pytest.approx(3.6956256194908494, rel=1e-6)
    assert aa.t == pytest.approx(3.141592653589793, rel=1e-6)
