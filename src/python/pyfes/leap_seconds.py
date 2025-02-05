# Copyright (c) 2024 CNES
#
# All rights reserved. Use of this source code is governed by a
# BSD-style license that can be found in the LICENSE file.
"""
Get the leap seconds from the IERS website.
===========================================
"""
from __future__ import annotations

from typing import TYPE_CHECKING
import datetime
import functools
import pathlib
import re
from re import Match
import ssl
import sys
import urllib.request
import warnings

import numpy

if TYPE_CHECKING:
    from collections.abc import Callable

    from .typing import (
        NDArrayStructured,
        VectorDateTime64,
        VectorInt64,
        VectorUInt16,
    )

#: URL of the IERS leap second file.
LEAP_SECOND_URL = 'https://data.iana.org/time-zones/data/leap-seconds.list'

#: Path to the IERS leap second file.
LEAP_SECOND_FILE = pathlib.Path(__file__).parent / 'data' / 'leap-seconds.txt'

#: User agent used for downloading files.
USER_AGENT = f'Python/{sys.version_info.major}.{sys.version_info.minor}'

#: Month abbreviations.
MONTH_ABBREVIATIONS = [
    'Jan', 'Feb', 'Mar', 'Apr', 'May', 'Jun', 'Jul', 'Aug', 'Sep', 'Oct',
    'Nov', 'Dec'
]

#: Standard response for successful HTTP requests.
HTTP_OK = 200


def _build_urlopener() -> urllib.request.OpenerDirector:
    """Helper for building a `urllib.request.build_opener` which handles
    TLS/SSL."""
    ssl_context = ssl.create_default_context()
    ssl_context.verify_mode = ssl.CERT_REQUIRED

    https_handler = urllib.request.HTTPSHandler(context=ssl_context)
    return urllib.request.build_opener(https_handler)


def _download_leap_second_file() -> None:
    """Download the IERS leap second file."""
    urlopener = _build_urlopener()
    req = urllib.request.Request(LEAP_SECOND_URL,
                                 headers={
                                     'User-Agent': USER_AGENT,
                                     'Accept': '*/*'
                                 })

    response = urlopener.open(req, timeout=None)
    if response.status != HTTP_OK:
        raise RuntimeError(
            f'Failed to download leap second file: {response.status} '
            f'{response.reason}')
    LEAP_SECOND_FILE.parent.mkdir(parents=True, exist_ok=True)
    with LEAP_SECOND_FILE.open('wb') as stream:
        stream.write(response.read())


def _read_leap_second_file(
        download: bool = False) -> tuple[numpy.datetime64, list[str]]:
    """Read the IERS leap second file."""
    re_expires: Callable[[str], Match[str] | None] = re.compile(
        r'^#.*File expires on[:\s]+(\d+\s\w+\s\d+)\s*$').match

    if not LEAP_SECOND_FILE.exists() or download:
        _download_leap_second_file()

    with LEAP_SECOND_FILE.open('r') as stream:
        lines = stream.readlines()

    expires: numpy.datetime64 | None = None
    for line in lines:
        match = re_expires(line)
        if match:
            day, month_abbreviation, year = match.groups()[0].split()
            day, year = map(int, (day, year))
            month = MONTH_ABBREVIATIONS.index(month_abbreviation[:3]) + 1
            expires = numpy.datetime64(f'{year}-{month:02d}-{day:02d}')
            break
    if expires is None:
        raise RuntimeError(
            f'Did not find expiration date in {LEAP_SECOND_FILE.name}')

    return expires, lines


@functools.lru_cache(maxsize=1)
def _load_leap_second_file() -> NDArrayStructured:
    """Load the IERS leap second file."""

    # Regex for matching leap second entries.
    re_entry: Callable[[str], Match[str] | None] = re.compile(
        r'^(\d+)\s+(\d+)\s+#\s1\s\w{3}\s\d{4}$').match

    expires: numpy.datetime64
    lines: list[str]

    # Read the leap second file.
    expires, lines = _read_leap_second_file()
    if expires < numpy.datetime64('today'):
        # If the leap second file has expired, try to download a new one.
        warnings.warn(
            f'Leap second file {LEAP_SECOND_FILE.name} has expired. '
            'Downloading a new version.',
            UserWarning,
            stacklevel=2)
        try:
            expires, lines = _read_leap_second_file(download=True)
        except RuntimeError:
            warnings.warn(
                'Failed to download new leap second file. '
                'Using expired file.',
                UserWarning,
                stacklevel=2)

    # Number of seconds elapsed between 1900-01-01T00:00:00:00+0000 and
    # 1970-01-01T00:00:00:00+0000
    duration = 25_567 * 86_400

    # List of leap second entries.
    entries: list[tuple[numpy.datetime64, int]] = []

    for line in lines:
        match = re_entry(line)
        if match:
            # Convert the epoch to a numpy.datetime64 object.
            epoch, leap_seconds = map(int, match.groups())
            epoch -= duration
            entries.append((
                numpy.datetime64(epoch, 's'),
                leap_seconds,
            ))

    return numpy.array(entries,
                       dtype=[
                           ('utc', 'datetime64[s]'),
                           ('seconds', 'uint16'),
                       ])


def get_leap_seconds(
    utc: VectorDateTime64 | datetime.datetime | numpy.datetime64,
    sorter: VectorInt64 | None = None,
) -> VectorUInt16:
    """Return the UTC-TAI difference for the given UTC times.

    Args:
        utc: The UTC times.
        sorter: The indices which would sort the `utc` array. If not
            given, the `utc` array is sorted internally.
    Returns:
        The UTC-TAI difference for the given UTC times.
    """
    # Leaps seconds are defined before the january 1st 1972.
    if isinstance(utc, datetime.datetime):
        utc = numpy.datetime64(utc)
    if isinstance(utc, numpy.datetime64):
        utc = numpy.array([utc], dtype='datetime64[ns]')
    date_before_1972 = utc < numpy.datetime64('1972-01-01')
    if numpy.any(date_before_1972):
        warnings.warn(
            'Leap seconds are not defined before January 1, 1972. '
            'Setting them to zero.', UserWarning)

    table = _load_leap_second_file()
    index = numpy.searchsorted(table['utc'], utc, side='right', sorter=sorter)
    result = table['seconds'][index - 1]
    result[date_before_1972] = 0
    return result
