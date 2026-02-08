"""Generate Delta T table from IERS finals.all and IANA leap seconds data.

Delta T = TT - UT1 = 32.184 + (TAI-UTC) - (UT1-UTC)

Sources:
    - IERS finals.all:
        https://maia.usno.navy.mil/ser7/finals.all
    - IANA leap-seconds.list:
        https://data.iana.org/time-zones/data/leap-seconds.list
"""

from bisect import bisect_right
from collections import defaultdict
import datetime
import pathlib
import ssl
import urllib.request


# Folder containing this script
HERE = pathlib.Path(__file__).parent

# URLs for data sources
URL_FINALS = 'https://maia.usno.navy.mil/ser7/finals.all'
URL_LEAP = 'https://data.iana.org/time-zones/data/leap-seconds.list'

# Output C++ source file
CPP_SRC = HERE.parent / 'include/fes/delta_t.hpp'

# Brief of the generated table
BRIEF = '/// @brief Table generated dynamically from IERS (finals.all)'

# NTP Epoch (1900-01-01) is MJD 15020
NTP_TO_MJD_OFFSET = 15020

# First column index to process in finals.all
COL_MJD_END = 68

# First year to include in the output
START_YEAR = 1973

# Number of items expected in the IANA leap seconds list
LEAP_SECONDS_LEN = 2


def get_ssl_context() -> ssl.SSLContext:
    """Create a relaxed SSL context to handle government server certs."""
    ctx = ssl.create_default_context()
    ctx.check_hostname = False
    ctx.verify_mode = ssl.CERT_NONE
    return ctx


def download_lines(url: str) -> list[str]:
    """Download text lines from a URL."""
    print(f'Downloading {url}...')
    try:
        with urllib.request.urlopen(url, context=get_ssl_context()) as response:
            return response.read().decode('utf-8').splitlines()
    except Exception as exc:
        raise RuntimeError(f'Failed to download {url}: {exc}') from exc


def parse_leap_seconds() -> list[tuple[float, int]]:
    """Parse the IANA leap-seconds.list.

    Returns a sorted list of tuples: (start_mjd, tai_utc_offset)
    """
    lines = download_lines(URL_LEAP)
    leap_history: list[tuple[float, int]] = []

    for line in lines:
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
    return leap_history


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


def mjd_to_year(mjd: float) -> float:
    """Approximate conversion of MJD to Decimal Year."""
    # MJD 41683 = 1973-01-01
    return 1973.0 + (mjd - 41683.0) / 365.25


def parse_finals_entry(
    line: str, leap_history: list[tuple[float, int]]
) -> tuple[float, int] | None:
    """Parse a single finals.all entry and calculate Delta T.

    Returns (delta_t, year) or None if parsing fails.
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

        year = int(mjd_to_year(mjd))
        return (delta_t, year)
    except ValueError:
        return None


def process_finals_data(
    finals_lines: list[str],
    leap_history: list[tuple[float, int]],
) -> defaultdict[int, list[float]]:
    """Process finals.all data and organize Delta T values by year."""
    data_by_year: defaultdict[int, list[float]] = defaultdict(list)

    for line in finals_lines:
        result = parse_finals_entry(line, leap_history)
        if result:
            delta_t, year = result
            data_by_year[year].append(delta_t)

    return data_by_year


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


def generate_updated_tables(
    data_by_year: defaultdict[int, list[float]],
) -> list[str]:
    """Generate C++ table entries from Delta T data."""
    sorted_years = sorted(data_by_year.keys())
    now = datetime.datetime.now(datetime.UTC)
    array_size = len(sorted_years)
    updated_tables = [
        BRIEF,
        f'/// @note Generated on {now.isoformat(timespec="seconds")}Z',
        f'constexpr std::array<DeltaTEntry, {array_size}> IERS_TABLE = {{{{',
    ]

    for year in sorted_years:
        if year < START_YEAR:
            continue

        values = data_by_year[year]
        avg_dt = sum(values) / len(values)
        updated_tables.append(f'{{{year}.0, {avg_dt:.4f}}},')

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


def generate_table() -> None:
    """Generate table of Delta T from IERS finals.all and IANA leap seconds."""
    # 1. Get Leap Seconds Data
    leap_history = parse_leap_seconds()

    # 2. Get Earth Rotation Data (finals.all)
    finals_lines = download_lines(URL_FINALS)
    if not finals_lines:
        raise RuntimeError('No data retrieved from finals.all')

    # 3. Process Data
    data_by_year = process_finals_data(finals_lines, leap_history)

    # 4. Generate and Write C++ Code
    updated_tables = generate_updated_tables(data_by_year)
    start_ix, end_ix = find_table_bounds(
        CPP_SRC.read_text(encoding='utf-8').splitlines()
    )
    write_updated_cpp_file(updated_tables, start_ix, end_ix)


if __name__ == '__main__':
    generate_table()
