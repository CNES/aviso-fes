# Copyright (c) 2024 CNES
#
# All rights reserved. Use of this source code is governed by a
# BSD-style license that can be found in the LICENSE file.
"""Testing leap seconds."""
import numpy

from pyfes.leap_seconds import get_leap_seconds


def test_leap_seconds():
    """Test the leaps seconds gap determination."""
    utc = numpy.array([
        numpy.datetime64('2005-12-31T23:59:55', 's'),
        numpy.datetime64('2005-12-31T23:59:56', 's'),
        numpy.datetime64('2005-12-31T23:59:57', 's'),
        numpy.datetime64('2005-12-31T23:59:58', 's'),
        numpy.datetime64('2005-12-31T23:59:59', 's'),
        numpy.datetime64('2006-01-01T00:00:00', 's'),
        numpy.datetime64('2006-01-01T00:00:01', 's'),
        numpy.datetime64('2006-01-01T00:00:02', 's'),
        numpy.datetime64('2006-01-01T00:00:03', 's'),
        numpy.datetime64('2006-01-01T00:00:04', 's'),
    ])
    leap_seconds = get_leap_seconds(utc)
    assert numpy.all(leap_seconds == numpy.array(
        [32, 32, 32, 32, 32, 33, 33, 33, 33, 33], dtype='uint16'))
