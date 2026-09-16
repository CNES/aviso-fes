"""Generate Delta T table from IERS finals.all and IANA leap seconds data.

Delta T = TT - UT1 = 32.184 + (TAI-UTC) - (UT1-UTC)

The daily values of finals.all (IERS values followed by the IERS Bulletin A
predictions) are averaged over calendar months. Each mean is stamped with the
mean Modified Julian Date of the days averaged, so that interpolating linearly
between consecutive means introduces no time shift.

The table depends only on the content of the two source files: running the
script with the same files (``--finals`` and ``--leap-seconds``) reproduces
it, and the SHA-256 of both files is recorded next to the table.

Sources:
    - IERS finals.all:
        https://maia.usno.navy.mil/ser7/finals.all
    - IANA leap-seconds.list:
        https://data.iana.org/time-zones/data/leap-seconds.list
"""

import argparse
from bisect import bisect_right
from collections import defaultdict
import datetime
import hashlib
import pathlib
import ssl
import textwrap
import urllib.request


# Folder containing this script
HERE = pathlib.Path(__file__).parent

# URLs for data sources
URL_FINALS = 'https://maia.usno.navy.mil/ser7/finals.all'
URL_LEAP = 'https://data.iana.org/time-zones/data/leap-seconds.list'

# Output C++ source file
CPP_SRC = HERE.parent / 'include/fes/delta_time.hpp'

# Brief of the generated table
BRIEF = '/// @brief Table generated dynamically from IERS (finals.all)'

# NTP Epoch (1900-01-01) is MJD 15020
NTP_TO_MJD_OFFSET = 15020

# Origin of the Modified Julian Dates
MJD_EPOCH = datetime.date(1858, 11, 17)

# First column index to process in finals.all
COL_MJD_END = 68

# First year to include in the output
START_YEAR = 1973

# Number of items expected in the IANA leap seconds list
LEAP_SECONDS_LEN = 2

# Number of table entries written per line
ENTRIES_PER_LINE = 3


def get_ssl_context() -> ssl.SSLContext:
    """Create a relaxed SSL context to handle government server certs."""
    ctx = ssl.create_default_context()
    ctx.check_hostname = False
    ctx.verify_mode = ssl.CERT_NONE
    return ctx


def download(url: str) -> bytes:
    """Download the content of a URL."""
    print(f'Downloading {url}...')
    try:
        with urllib.request.urlopen(url, context=get_ssl_context()) as response:
            return response.read()
    except Exception as exc:
        raise RuntimeError(f'Failed to download {url}: {exc}') from exc


def read_source(path: pathlib.Path | None, url: str) -> bytes:
    """Read a source file, downloading it if no local path is given."""
    if path is None:
        return download(url)
    print(f'Reading {path}...')
    return path.read_bytes()


def mjd_to_date(mjd: float) -> datetime.date:
    """Convert a Modified Julian Date to a calendar date."""
    return MJD_EPOCH + datetime.timedelta(days=int(mjd))


def ntp_to_date(ntp_seconds: int) -> datetime.date:
    """Convert an NTP timestamp to a calendar date."""
    return mjd_to_date(NTP_TO_MJD_OFFSET + ntp_seconds // 86400)


def parse_leap_seconds(
    lines: list[str],
) -> tuple[list[tuple[float, int]], dict[str, datetime.date]]:
    """Parse the IANA leap-seconds.list.

    Returns a sorted list of tuples: (start_mjd, tai_utc_offset), and the
    dates of last update ("#$" line) and expiration ("#@" line) of the list.
    """
    leap_history: list[tuple[float, int]] = []
    dates: dict[str, datetime.date] = {}

    for line in lines:
        # Last update and expiration dates of the list
        if line.startswith(('#$', '#@')):
            key = 'updated' if line.startswith('#$') else 'expires'
            dates[key] = ntp_to_date(int(line[2:].split()[0]))
            continue

        # Skip comments and blank lines
        if line.strip().startswith('#') or not line.strip():
            continue

        parts = line.split()
        if len(parts) < LEAP_SECONDS_LEN:
            continue

        try:
            ntp_seconds = int(parts[0])
            tai_offset = int(parts[1])

            # Convert NTP timestamp to Modified Julian Date (MJD)
            # MJD = 15020 + (seconds / 86400)
            mjd = NTP_TO_MJD_OFFSET + (ntp_seconds / 86400.0)

            leap_history.append((mjd, tai_offset))
        except ValueError:
            continue

    if not leap_history:
        raise RuntimeError('No valid leap second entries found.')

    leap_history.sort()
    return leap_history, dates


def get_current_leap_seconds(
    mjd: float,
    leap_history: list[tuple[float, int]],
) -> float:
    """Find the valid TAI-UTC offset for a specific MJD using binary search."""
    keys = [x[0] for x in leap_history]
    idx = bisect_right(keys, mjd)

    if idx == 0:
        return 10.0  # Default fallback (Pre-1972)
    return leap_history[idx - 1][1]


def parse_finals_entry(
    line: str, leap_history: list[tuple[float, int]]
) -> tuple[float, float, str] | None:
    """Parse a single finals.all entry and calculate Delta T.

    Returns (mjd, delta_t, flag) or None if parsing fails. The flag is "I" for
    IERS values and "P" for predictions.
    """
    if len(line) < COL_MJD_END:
        return None

    try:
        mjd_str = line[7:15].strip()
        dut1_str = line[58:68].strip()

        if not mjd_str or not dut1_str:
            return None

        mjd = float(mjd_str)
        dut1 = float(dut1_str)

        tai_utc = get_current_leap_seconds(mjd, leap_history)
        delta_t = 32.184 + tai_utc - dut1

        return (mjd, delta_t, line[57])
    except ValueError:
        return None


def process_finals_data(
    finals_lines: list[str],
    leap_history: list[tuple[float, int]],
) -> tuple[
    defaultdict[tuple[int, int], list[tuple[float, float]]],
    dict[str, float],
]:
    """Group the daily Delta T values of finals.all by calendar month.

    Returns the (mjd, delta_t) samples of each (year, month), and the last MJD
    of the IERS values and of the predictions.
    """
    data_by_month: defaultdict[tuple[int, int], list[tuple[float, float]]] = (
        defaultdict(list)
    )
    last_mjd: dict[str, float] = {}

    for line in finals_lines:
        result = parse_finals_entry(line, leap_history)
        if result is None:
            continue
        mjd, delta_t, flag = result
        date = mjd_to_date(mjd)
        if date.year < START_YEAR:
            continue
        data_by_month[(date.year, date.month)].append((mjd, delta_t))
        last_mjd[flag] = max(last_mjd.get(flag, mjd), mjd)

    if not data_by_month:
        raise RuntimeError('No data retrieved from finals.all')
    return data_by_month, last_mjd


def find_table_bounds(cpp_lines: list[str]) -> tuple[int, int]:
    """Find the start and end indices of the generated table block."""
    start_ix = -1
    end_ix = -1

    for ix, item in enumerate(cpp_lines):
        line = item.strip()
        if line == BRIEF:
            start_ix = ix
        elif start_ix != -1 and line.endswith(';'):
            end_ix = ix
            break

    if start_ix == -1 or end_ix == -1:
        raise RuntimeError(
            'Could not locate generated table block in delta_t.hpp'
        )

    return start_ix, end_ix


def format_mjd(mjd: float) -> str:
    """Format a Modified Julian Date without trailing zeros."""
    text = f'{mjd:.6f}'.rstrip('0')
    return text + '0' if text.endswith('.') else text


def comment_lines(text: str) -> list[str]:
    """Format a paragraph as Doxygen comment lines of at most 80 columns."""
    return textwrap.wrap(
        text,
        width=80,
        initial_indent='/// ',
        subsequent_indent='/// ',
        break_long_words=False,
        break_on_hyphens=False,
    )


def generate_updated_tables(
    data_by_month: defaultdict[tuple[int, int], list[tuple[float, float]]],
    last_mjd: dict[str, float],
    leap_dates: dict[str, datetime.date],
    checksums: dict[str, str],
) -> list[str]:
    """Generate C++ table entries from Delta T data."""
    entries = []
    for month in sorted(data_by_month):
        samples = data_by_month[month]
        mean_mjd = sum(mjd for mjd, _ in samples) / len(samples)
        mean_dt = sum(dt for _, dt in samples) / len(samples)
        entries.append(f'{{{format_mjd(mean_mjd)}, {mean_dt:.4f}}},')

    first_mjd = min(mjd for mjd, _ in data_by_month[min(data_by_month)])
    note = (
        '@note Calendar-month means of the daily values, each stamped with '
        'the mean Modified Julian Date (UTC) of the days averaged. IERS '
        f'values from {mjd_to_date(first_mjd)} to '
        f'{mjd_to_date(last_mjd["I"])}, IERS Bulletin A predictions to '
        f'{mjd_to_date(last_mjd["P"])}. leap-seconds.list updated on '
        f'{leap_dates["updated"]}, expires on {leap_dates["expires"]}.'
    )
    updated_tables = [
        BRIEF,
        *comment_lines(note),
        '/// SHA-256 of finals.all:',
        f'/// {checksums["finals"]}',
        '/// SHA-256 of leap-seconds.list:',
        f'/// {checksums["leap"]}',
        f'constexpr std::array<DeltaTEntry, {len(entries)}> IERS_TABLE = {{{{',
    ]
    updated_tables.extend(
        '    ' + ' '.join(entries[ix : ix + ENTRIES_PER_LINE])
        for ix in range(0, len(entries), ENTRIES_PER_LINE)
    )
    updated_tables.append('}};')
    return updated_tables


def write_updated_cpp_file(
    updated_tables: list[str], start_ix: int, end_ix: int
) -> None:
    """Write the updated C++ source file."""
    with open(CPP_SRC, encoding='utf-8') as cpp_file:
        cpp_lines = cpp_file.readlines()

    with open(CPP_SRC, 'w', encoding='utf-8') as cpp_file:
        cpp_file.writelines(cpp_lines[:start_ix])
        cpp_file.writelines('\n'.join(updated_tables) + '\n')
        cpp_file.writelines(cpp_lines[end_ix + 1 :])


def generate_table(
    finals: pathlib.Path | None = None,
    leap_seconds: pathlib.Path | None = None,
) -> None:
    """Generate table of Delta T from IERS finals.all and IANA leap seconds."""
    # 1. Get Leap Seconds Data
    leap_content = read_source(leap_seconds, URL_LEAP)
    leap_history, leap_dates = parse_leap_seconds(
        leap_content.decode('utf-8').splitlines()
    )

    # 2. Get Earth Rotation Data (finals.all)
    finals_content = read_source(finals, URL_FINALS)
    finals_lines = finals_content.decode('utf-8').splitlines()
    if not finals_lines:
        raise RuntimeError('No data retrieved from finals.all')

    # 3. Process Data
    data_by_month, last_mjd = process_finals_data(finals_lines, leap_history)

    # 4. Generate and Write C++ Code
    checksums = {
        'finals': hashlib.sha256(finals_content).hexdigest(),
        'leap': hashlib.sha256(leap_content).hexdigest(),
    }
    updated_tables = generate_updated_tables(
        data_by_month, last_mjd, leap_dates, checksums
    )
    start_ix, end_ix = find_table_bounds(
        CPP_SRC.read_text(encoding='utf-8').splitlines()
    )
    write_updated_cpp_file(updated_tables, start_ix, end_ix)


def main() -> None:
    """Parse the command line and generate the table."""
    parser = argparse.ArgumentParser(description=__doc__.splitlines()[0])
    parser.add_argument(
        '--finals',
        type=pathlib.Path,
        help=f'local copy of finals.all (default: download {URL_FINALS})',
    )
    parser.add_argument(
        '--leap-seconds',
        type=pathlib.Path,
        help=f'local copy of leap-seconds.list (default: download {URL_LEAP})',
    )
    args = parser.parse_args()
    generate_table(args.finals, args.leap_seconds)


if __name__ == '__main__':
    main()
