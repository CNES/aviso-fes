# Copyright (c) 2026 CNES
#
# All rights reserved. Use of this source code is governed by a
# BSD-style license that can be found in the LICENSE file.
import numpy
import pytest

from pyfes import core


def test_axis() -> None:
    """Test the Axis class."""
    points = numpy.arange(0, 10, 0.1)
    x = core.Axis(points)
    assert len(x) == len(points)
    assert x[0] == points[0]
    with pytest.raises(IndexError):
        x[len(points)]
    assert x.start == points[0]
    assert x.step == points[1] - points[0]
    assert x.is_ascending
    assert not x.is_periodic
    assert x.end() == points[-1]
    assert x == x
    assert x.find_index(points[0]) == 0
    assert x.find_index(points[0] + 0.4 * x.step) == 0
    assert x.find_index(points[0] + 0.5 * x.step) == 1
    assert x.find_index(11, bounded=True) == 99
    assert x.find_index(-1, bounded=True) == 0
    assert x.find_index(11, bounded=False) == -1
    assert x.find_index(-1, bounded=False) == -1

    assert x.find_indices(0.01) == (0, 1)
    assert x.find_indices(0.1) == (1, 2)
