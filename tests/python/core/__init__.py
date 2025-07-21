# Copyright (c) 2025 CNES
#
# All rights reserved. Use of this source code is governed by a
# BSD-style license that can be found in the LICENSE file.
import sys


def is_free_threaded():
    """Check if Python is running in free-threaded mode."""
    return hasattr(sys, '_is_gil_enabled') and not sys._is_gil_enabled()
