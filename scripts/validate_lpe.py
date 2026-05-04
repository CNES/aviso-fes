#!/usr/bin/env python3
# Copyright (c) 2026 CNES
#
# All rights reserved. Use of this source code is governed by a
# BSD-style license that can be found in the LICENSE file.
"""Numerical validation harness for ``lpe_minus_n_waves``.

The long-period equilibrium tide is at the heart of every PyFES prediction;
any optimisation of the routine must preserve its scientific output. This
script computes the reference series with whatever ``pyfes`` is currently
importable and either:

* writes the result to ``--reference``, or
* reads the values from ``--reference`` and compares them against the
  freshly computed series, reporting max absolute / relative error and
  failing the process if they exceed the given tolerances.

Typical workflow::

    # 1. Capture a trustworthy baseline (conda-forge install).
    /path/to/pyfes-cf/bin/python scripts/validate_lpe.py \
        --reference /tmp/lpe_ref.npz --write

    # 2. Apply the optimisation, rebuild, then validate.
    /path/to/develop/bin/python scripts/validate_lpe.py \
        --reference /tmp/lpe_ref.npz \
        --abs-tol 1e-13 --rel-tol 1e-12

The grid spans ~30 years (3 hourly samples) crossed with 181 latitudes from
-89.9° to 89.9°, which exercises every relevant doodson combination and
geometry term.
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

import pyfes


def _build_grid() -> tuple[numpy.ndarray, numpy.ndarray]:
    """Return ``(dates, latitudes)`` paired arrays covering the test grid.

    Both arrays have the same length so they can be passed directly to
    ``pyfes.evaluate_equilibrium_long_period``.
    """
    # Time axis: every 3 hours from 2000 to 2030. 87 672 samples.
    base = numpy.datetime64('2000-01-01T00:00:00', 'us')
    step = numpy.timedelta64(3, 'h').astype('timedelta64[us]')
    n_dates = int(30 * 365.25 * 24 / 3)
    dates = base + numpy.arange(n_dates, dtype='int64') * step

    # Latitude axis: 181 values from -89.9 to 89.9 (avoid exact poles where
    # the geometric factor is rigorously 0/special).
    lats = numpy.linspace(-89.9, 89.9, 181)

    # Cartesian product flattened to two parallel arrays.
    date_grid, lat_grid = numpy.meshgrid(dates, lats, indexing='ij')
    return date_grid.ravel(), lat_grid.ravel()


def _compute(dates: numpy.ndarray, lats: numpy.ndarray) -> numpy.ndarray:
    return pyfes.evaluate_equilibrium_long_period(dates, lats)


#: Minimum reference magnitude (in centimetres) below which relative error
#: is meaningless: tide measurements have a ~mm noise floor, so deviations
#: at 0.01 cm (100 microns) cannot impact any downstream science.
RELATIVE_FLOOR = 1e-2


def _summary(a: numpy.ndarray, b: numpy.ndarray) -> dict[str, Any]:
    diff = a - b
    abs_err = numpy.abs(diff)

    # Restrict the relative-error scan to samples whose reference is at
    # least :data:`RELATIVE_FLOOR` cm in magnitude. Below that floor any
    # absolute error at machine epsilon (~3e-15) explodes into a meaningless
    # relative ratio.
    significant = numpy.abs(b) >= RELATIVE_FLOOR
    if numpy.any(significant):
        rel_err = abs_err[significant] / numpy.abs(b[significant])
        max_rel = float(numpy.max(rel_err))
        argmax_rel_idx = int(
            numpy.flatnonzero(significant)[int(numpy.argmax(rel_err))]
        )
    else:
        max_rel = float('nan')
        argmax_rel_idx = -1

    return {
        'n_samples': int(a.size),
        'n_significant': int(numpy.sum(significant)),
        'reference_min': float(numpy.min(b)),
        'reference_max': float(numpy.max(b)),
        'reference_rms': float(numpy.sqrt(numpy.mean(b * b))),
        'max_abs_err': float(numpy.max(abs_err)),
        'mean_abs_err': float(numpy.mean(abs_err)),
        'max_rel_err_significant': max_rel,
        'argmax_abs': int(numpy.argmax(abs_err)),
        'argmax_rel': argmax_rel_idx,
    }


def main(argv: list[str] | None = None) -> int:
    """Entry point for the validation script."""
    parser = argparse.ArgumentParser(
        description='Validate lpe_minus_n_waves against a reference grid.'
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
        default=1e-13,
        help='Maximum absolute error allowed (default: 1e-13).',
    )
    parser.add_argument(
        '--rel-tol',
        type=float,
        default=1e-12,
        help='Maximum relative error allowed for |ref| > 1e-12 (default: '
        '1e-12).',
    )
    parser.add_argument(
        '--installed',
        action='store_true',
        help='Use whichever pyfes is importable instead of the local build.',
    )
    args = parser.parse_args(argv)

    print(f'pyfes {pyfes.__version__} from {pyfes.__file__}')

    dates, lats = _build_grid()
    print(
        f'grid: {dates.size:,d} samples '
        f'({numpy.unique(lats).size} latitudes x '
        f'{dates.size // numpy.unique(lats).size} dates)'
    )

    values = _compute(dates, lats)

    if args.write:
        args.reference.parent.mkdir(parents=True, exist_ok=True)
        numpy.savez(
            args.reference,
            dates=dates.astype('datetime64[us]').view('int64'),
            lats=lats,
            values=values,
            version=numpy.array(pyfes.__version__),
        )
        print(f'wrote {args.reference} ({values.nbytes / 1e6:.1f} MB)')
        return 0

    if not args.reference.is_file():
        sys.exit(
            f'reference file not found: {args.reference}. '
            'Pass --write first to create it.'
        )

    ref = numpy.load(args.reference, allow_pickle=False)
    if ref['dates'].shape != dates.astype('datetime64[us]').view('int64').shape:
        sys.exit('reference grid shape mismatch -- regenerate with --write')

    summary = _summary(values, ref['values'])
    for key, value in summary.items():
        print(f'  {key:18s} = {value!r}')

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

    if failed:
        return 1

    print('\nOK -- LPE output stays within the requested tolerance.')
    return 0


if __name__ == '__main__':
    raise SystemExit(main())
