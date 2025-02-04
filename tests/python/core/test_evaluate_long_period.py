# Copyright (c) 2024 CNES
#
# All rights reserved. Use of this source code is governed by a
# BSD-style license that can be found in the LICENSE file.
import pathlib

import netCDF4
import numpy
from pyfes import core


def test_evaluate_long_period():
    """Test the evaluation of the long period tide."""
    dates = numpy.arange(numpy.datetime64('2020-01-01'),
                         numpy.datetime64('2020-01-30'),
                         numpy.timedelta64(1, 'h'))
    zeros = numpy.zeros(dates.shape)

    long_period = core.evaluate_long_period(dates, zeros, zeros, None, 0)
    assert long_period.shape == dates.shape
    assert numpy.all(long_period != 0)
