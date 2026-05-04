#!/usr/bin/env python3
# Copyright (c) 2026 CNES
#
# All rights reserved. Use of this source code is governed by a
# BSD-style license that can be found in the LICENSE file.
"""Numerical validation harness for ``pyfes.evaluate_tide``.

Sister script of ``validate_lpe.py``. Builds a Cartesian tide model from
the bundled test dataset, evaluates the tide on a deterministic grid that
spans 10 years of hourly samples crossed with a handful of geographically
diverse gauge-like locations, and either:

* writes the result to ``--reference``, or
* reads the values from ``--reference`` and compares them against the
  freshly computed series, reporting max absolute / relative error and
  failing the process if they exceed the given tolerances.

The intended workflow is **same-build before/after a patch** -- there
the harness confirms bit-exact reproducibility (``max_abs_err == 0``)::

    # 1. Capture a baseline from the current build.
    scripts/validate_evaluate_tide.py --reference /tmp/tide_ref.npz --write

    # 2. Apply patches, rebuild.
    setup.py build

    # 3. Validate -- with the default tolerances any drift fails the run.
    scripts/validate_evaluate_tide.py --reference /tmp/tide_ref.npz

Cross-build comparisons (e.g. our build vs the published conda-forge
package) typically drift at ~1e-7 relative because of compiler
optimisation differences (``-O2`` vs ``-O3``, LTO, vectoriser choices)
without any algorithmic regression. For those, pass loose tolerances::

    # Compare against an installed pyfes (different compile flags).
    /path/to/pyfes-cf/bin/python scripts/validate_evaluate_tide.py \
        --reference /tmp/tide_ref.npz --write --installed
    /path/to/develop/bin/python scripts/validate_evaluate_tide.py \
        --reference /tmp/tide_ref.npz --abs-tol 1e-4 --rel-tol 1e-6

The grid is 10 stations x 10 years x 1 h = ~876 720 samples, which exercises
the full ``evaluate_tide`` pipeline (Cartesian interpolation + nodal
correction + harmonic summation + LPE add) over a window long enough to
cover the 18.6-year nodal cycle from realistic angles, while staying
tractable enough to fit a single NPZ file.
"""

from __future__ import annotations

from typing import Any
import argparse
import os
import pathlib
import sys
import sysconfig

import numpy


if '--installed' not in sys.argv and not os.environ.get(
    'FES_BENCH_USE_INSTALLED'
):
    _REPO_ROOT = pathlib.Path(__file__).resolve().parent.parent
    if (_REPO_ROOT / 'setup.py').is_file():
        _MAJOR = sys.version_info[0]
        _MINOR = sys.version_info[1]
        for _candidate in (
            _REPO_ROOT
            / 'build'
            / f'lib.{sysconfig.get_platform()}-{_MAJOR}.{_MINOR}',
            _REPO_ROOT
            / 'build'
            / f'lib.{sysconfig.get_platform()}-{sys.implementation.cache_tag}',
        ):
            if _candidate.is_dir():
                sys.path.insert(0, str(_candidate))
                break

# Make the bench helpers importable so we reuse the exact model-building
# code that the benchmark exercises -- keeps validation and benchmarking
# in lockstep.
sys.path.insert(0, str(pathlib.Path(__file__).resolve().parent))

from bench_predict import _build_cartesian_model

import pyfes


# Geographically diverse stations. Latitudes span from ~-34 deg to ~+60
# deg so the latitude-dependent terms (nodal corrections, LPE) and the
# longitude-dependent ones (interpolation grids, time-of-day) are both
# stressed. Coordinates are gauge-like rough centres -- the only thing we
# need is determinism, not real-world calibration.
STATIONS: tuple[tuple[str, float, float], ...] = (
    ('Brest', 48.40, -4.50),
    ('Honolulu', 21.30, -157.85),
    ('Singapore', 1.30, 103.85),
    ('Cape_Town', -33.90, 18.40),
    ('Sydney', -33.85, 151.20),
    ('Reykjavik', 64.10, -21.95),
    ('Buenos_Aires', -34.60, -58.40),
    ('Tokyo', 35.65, 139.80),
    ('Mumbai', 19.05, 72.85),
    ('Halifax', 44.65, -63.60),
)


#: Reference magnitude floor (cm) below which the relative error is
#: meaningless. Tide observations have a ~mm noise floor, so deviations
#: at 0.01 cm cannot impact any downstream science.
RELATIVE_FLOOR = 1e-2


def _build_grid() -> tuple[
    numpy.ndarray, numpy.ndarray, numpy.ndarray, list[str]
]:
    """Build paired ``(dates, lon, lat)`` arrays and a station-name list.

    Each station carries the same hourly time axis (10 years), so we end
    up with ``len(STATIONS) * n_dates`` parallel samples ready to feed
    ``pyfes.evaluate_tide``.
    """
    base = numpy.datetime64('2010-01-01T00:00:00', 'us')
    step = numpy.timedelta64(1, 'h').astype('timedelta64[us]')
    n_per_station = 10 * 365 * 24 + 2 * 24  # 10 years + 2 leap days margin
    dates_one = base + numpy.arange(n_per_station, dtype='int64') * step

    lons = numpy.empty(n_per_station * len(STATIONS), dtype=numpy.float64)
    lats = numpy.empty_like(lons)
    dates = numpy.empty(lons.size, dtype='datetime64[us]')
    names: list[str] = []
    for ix, (name, lat, lon) in enumerate(STATIONS):
        sl = slice(ix * n_per_station, (ix + 1) * n_per_station)
        lons[sl] = lon
        lats[sl] = lat
        dates[sl] = dates_one
        names.append(name)
    return dates, lons, lats, names


def _compute(
    dates: numpy.ndarray, lons: numpy.ndarray, lats: numpy.ndarray
) -> tuple[numpy.ndarray, numpy.ndarray, numpy.ndarray]:
    model = _build_cartesian_model()
    settings = pyfes.FESSettings()
    return pyfes.evaluate_tide(
        model,
        dates,
        lons,
        lats,
        settings=settings,
    )


def _summary(a: numpy.ndarray, b: numpy.ndarray) -> dict[str, Any]:
    # Stations near the edges of the bundled test dataset may fall outside
    # the model coverage, in which case ``evaluate_tide`` returns NaN.
    # Both runs should return NaN at the same indices (we cross-check this
    # via ``flag_mismatches`` in main); excluding them here lets the
    # numerical metrics focus on the valid samples.
    valid = numpy.isfinite(a) & numpy.isfinite(b)
    n_nan_ref = int(numpy.sum(~numpy.isfinite(b)))
    n_nan_run = int(numpy.sum(~numpy.isfinite(a)))
    a_v = a[valid]
    b_v = b[valid]

    diff = a_v - b_v
    abs_err = numpy.abs(diff)

    significant = numpy.abs(b_v) >= RELATIVE_FLOOR
    if numpy.any(significant):
        rel_err = abs_err[significant] / numpy.abs(b_v[significant])
        max_rel = float(numpy.max(rel_err))
        argmax_rel_idx = int(
            numpy.flatnonzero(valid)[
                int(numpy.flatnonzero(significant)[int(numpy.argmax(rel_err))])
            ]
        )
    else:
        max_rel = float('nan')
        argmax_rel_idx = -1

    return {
        'n_samples': int(a.size),
        'n_valid': int(numpy.sum(valid)),
        'n_nan_ref': n_nan_ref,
        'n_nan_run': n_nan_run,
        'n_significant': int(numpy.sum(significant)),
        'reference_min': float(numpy.min(b_v)) if b_v.size else float('nan'),
        'reference_max': float(numpy.max(b_v)) if b_v.size else float('nan'),
        'reference_rms': float(numpy.sqrt(numpy.mean(b_v * b_v)))
        if b_v.size
        else float('nan'),
        'max_abs_err': float(numpy.max(abs_err))
        if abs_err.size
        else float('nan'),
        'mean_abs_err': float(numpy.mean(abs_err))
        if abs_err.size
        else float('nan'),
        'max_rel_err_significant': max_rel,
        'argmax_abs': int(numpy.flatnonzero(valid)[int(numpy.argmax(abs_err))])
        if abs_err.size
        else -1,
        'argmax_rel': argmax_rel_idx,
    }


def main(argv: list[str] | None = None) -> int:
    """Entry point for the validation script."""
    parser = argparse.ArgumentParser(
        description=(
            'Validate evaluate_tide against a reference grid spanning '
            '10 years of hourly samples on 10 stations.'
        )
    )
    parser.add_argument(
        '--reference',
        type=pathlib.Path,
        required=True,
        help='Reference NPZ file. Read by default; pass --write to create.',
    )
    parser.add_argument(
        '--write',
        action='store_true',
        help='Write the freshly computed values to --reference and exit.',
    )
    parser.add_argument(
        '--abs-tol',
        type=float,
        default=1e-10,
        help='Maximum absolute error allowed in cm (default: 1e-10).',
    )
    parser.add_argument(
        '--rel-tol',
        type=float,
        default=1e-10,
        help=(
            'Maximum relative error allowed for |ref| > 1e-2 cm '
            '(default: 1e-10).'
        ),
    )
    parser.add_argument(
        '--installed',
        action='store_true',
        help='Use whichever pyfes is importable instead of the local build.',
    )
    args = parser.parse_args(argv)

    print(f'pyfes {pyfes.__version__} from {pyfes.__file__}')

    dates, lons, lats, names = _build_grid()
    print(
        f'grid: {dates.size:,d} samples '
        f'({len(names)} stations x '
        f'{dates.size // len(names)} hourly timestamps)'
    )

    tide, lp, flags = _compute(dates, lons, lats)
    # ``tide`` is the diurnal/semi-diurnal sum -- the most sensitive piece
    # numerically and the one that depends on the model interpolation.
    # ``lp`` is independently validated by ``validate_lpe.py``; we still
    # keep it in the NPZ so the reference file stays self-describing.

    if args.write:
        args.reference.parent.mkdir(parents=True, exist_ok=True)
        numpy.savez(
            args.reference,
            dates=dates.astype('datetime64[us]').view('int64'),
            lons=lons,
            lats=lats,
            tide=tide,
            lp=lp,
            flags=flags,
            stations=numpy.array(names),
            version=numpy.array(pyfes.__version__),
        )
        print(
            f'wrote {args.reference} '
            f'({(tide.nbytes + lp.nbytes + flags.nbytes) / 1e6:.1f} MB)'
        )
        return 0

    if not args.reference.is_file():
        sys.exit(
            f'reference file not found: {args.reference}. '
            'Pass --write first to create it.'
        )

    ref = numpy.load(args.reference, allow_pickle=False)
    if ref['tide'].shape != tide.shape:
        sys.exit('reference grid shape mismatch -- regenerate with --write')

    summary = _summary(tide, ref['tide'])
    for key, value in summary.items():
        print(f'  {key:24s} = {value!r}')

    # Quality flags must match exactly: they are integer per-point
    # interpolation counts and any drift means the model behaved
    # differently for at least one location.
    flag_diff = int(numpy.sum(flags != ref['flags']))
    print(f'  {"flag_mismatches":24s} = {flag_diff}')

    failed = False
    if summary['max_abs_err'] > args.abs_tol:
        print(
            f'\nFAIL: max_abs_err {summary["max_abs_err"]:.3e} '
            f'> abs_tol {args.abs_tol:.3e}'
        )
        failed = True
    if summary['max_rel_err_significant'] > args.rel_tol:
        print(
            f'FAIL: max_rel_err_significant '
            f'{summary["max_rel_err_significant"]:.3e} '
            f'> rel_tol {args.rel_tol:.3e}'
        )
        failed = True
    if flag_diff != 0:
        print(f'FAIL: {flag_diff} interpolation-flag mismatches vs reference')
        failed = True

    if failed:
        return 1

    print('\nOK -- evaluate_tide stays within the requested tolerance.')
    return 0


if __name__ == '__main__':
    raise SystemExit(main())
