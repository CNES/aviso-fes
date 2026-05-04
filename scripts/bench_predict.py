#!/usr/bin/env python3
# Copyright (c) 2026 CNES
#
# All rights reserved. Use of this source code is governed by a
# BSD-style license that can be found in the LICENSE file.
"""Standalone microbenchmark for the PyFES prediction hot paths.

This script measures the wall-clock latency of:

* :py:func:`pyfes.evaluate_tide` (model interpolation + harmonic summation)
  for a few representative batch sizes;
* :py:func:`pyfes.evaluate_tide_from_constituents` (pure summation) for the
  same range.

It depends only on :mod:`pyfes`, :mod:`numpy` and the standard library --
explicitly no ``pytest-benchmark`` or any other extra package -- so it can
be run on a fresh checkout once the C++ extension has been built. Results
are summarised on stdout as a Markdown table; ``--json`` writes the same
data to a machine-readable file for diffing across commits.

Typical workflow to compare two revisions::

    # Baseline
    python setup.py build_ext --inplace
    scripts/bench_predict.py --json baseline.json

    # Patch
    git checkout my-branch
    python setup.py build_ext --inplace
    scripts/bench_predict.py --json patch.json

    # Compare with any diff tool / jq / numpy script.
"""

from __future__ import annotations

from typing import TYPE_CHECKING, Any
import argparse
import json
import os
import pathlib
import statistics
import sys
import sysconfig
import time


# Mirror conftest.py: when running from a development checkout, prepend the
# ``build/lib.<platform>-<abi>`` directory so the freshly compiled C++
# extension is picked up before any installed copy of ``pyfes``. Set the
# ``FES_BENCH_USE_INSTALLED`` env var (or pass ``--installed``) to skip this
# path injection and benchmark whatever ``pyfes`` is already importable --
# typically a conda-forge install used as a baseline.
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

import numpy

import pyfes


if TYPE_CHECKING:
    from collections.abc import Callable


# --------------------------------------------------------------------------- #
# Benchmark configuration
# --------------------------------------------------------------------------- #


# Relative to the repository root. Matches what conftest.py downloads.
DATASET_DIR = (
    pathlib.Path(__file__).resolve().parent.parent
    / 'tests'
    / 'python'
    / 'dataset'
)


# Constituents available as Cartesian grids in the test dataset (see
# ``ls tests/python/dataset/*_tide.nc``). Picked to give a realistic mix of
# diurnal / semi-diurnal / long-period waves without dragging in the full
# FES2022 atlas.
CARTESIAN_TIDE_CONSTITUENTS: tuple[str, ...] = (
    '2N2',
    'K1',
    'K2',
    'M2',
    'Mf',
    'Mm',
    'Msqm',
    'Mtm',
    'N2',
    'O1',
    'P1',
    'Q1',
    'S1',
    'S2',
)


# NetCDF file holding the LGP2 mesh (1.46 M triangles, 2.98 M LGP2 nodes)
# and its associated tidal constituents.
LGP_DATASET_FILE = 'fes_2014.nc'


# Constituents stored in the LGP NetCDF mesh file. Variables are named
# ``{constituent}_amp`` / ``{constituent}_phase``.
LGP_TIDE_CONSTITUENTS: tuple[str, ...] = (
    '2N2',
    'K1',
    'K2',
    'M2',
    'N2',
    'O1',
    'P1',
    'Q1',
    'S1',
    'S2',
)


# Sizes used for the ``evaluate_tide`` benchmark. Scales chosen so a full
# run takes a few seconds on a recent laptop.
EVALUATE_TIDE_SIZES: tuple[int, ...] = (10_000, 100_000, 1_000_000)


# Sizes for the LGP scenario. The mesh interpolation is heavier than
# Cartesian (R-tree query + point-in-triangle + LGP polynomial), so we cap
# the largest case at 100 k to keep total runtime bounded.
LGP_TIDE_SIZES: tuple[int, ...] = (1_000, 10_000, 100_000)


# Sizes used for the ``evaluate_tide_from_constituents`` benchmark.
CONSTITUENTS_SIZES: tuple[int, ...] = (10_000, 100_000, 1_000_000)


# Constituents passed to ``evaluate_tide_from_constituents``.
KNOWN_CONSTITUENTS: dict[str, tuple[float, float]] = {
    'M2': (50.0, 120.0),
    'S2': (20.0, 90.0),
    'N2': (10.0, 100.0),
    'K1': (15.0, 45.0),
    'O1': (10.0, 60.0),
    'P1': (5.0, 50.0),
    'Q1': (3.0, 30.0),
    'K2': (5.0, 80.0),
}


# --------------------------------------------------------------------------- #
# Timing helpers
# --------------------------------------------------------------------------- #


def _percentiles(samples: list[float]) -> dict[str, float]:
    """Return min / median / p95 / max in milliseconds."""
    if not samples:
        return {
            'min_ms': float('nan'),
            'median_ms': float('nan'),
            'p95_ms': float('nan'),
            'max_ms': float('nan'),
        }

    samples_ms = [s * 1_000.0 for s in samples]
    samples_ms.sort()
    return {
        'min_ms': samples_ms[0],
        'median_ms': statistics.median(samples_ms),
        # Plain order statistic; with the small sample counts we use
        # (5-9 reps) interpolated quantiles do not buy us much.
        'p95_ms': samples_ms[max(0, int(len(samples_ms) * 0.95) - 1)],
        'max_ms': samples_ms[-1],
    }


def _bench(
    label: str,
    fn: Callable[[], Any],
    *,
    repeat: int,
    warmup: int,
) -> dict[str, Any]:
    """Run *fn* ``warmup + repeat`` times and report timing percentiles."""
    for _ in range(warmup):
        fn()

    samples: list[float] = []
    for _ in range(repeat):
        t0 = time.perf_counter()
        fn()
        samples.append(time.perf_counter() - t0)

    stats = _percentiles(samples)
    print(
        f'  {label:<48s}'
        f' min={stats["min_ms"]:>9.2f} ms'
        f' med={stats["median_ms"]:>9.2f} ms'
        f' p95={stats["p95_ms"]:>9.2f} ms',
    )
    return {
        'label': label,
        'samples_ms': [s * 1_000.0 for s in samples],
        **stats,
    }


# --------------------------------------------------------------------------- #
# Model setup
# --------------------------------------------------------------------------- #


def _build_cartesian_model() -> pyfes.config.TidalModel:
    """Build a Cartesian tide model from the test-suite NetCDF dataset.

    Avoids YAML / temp files so the script stays self-contained.
    """
    if not DATASET_DIR.exists():
        sys.exit(
            f'Test dataset not found at {DATASET_DIR}. Run pytest once -- '
            'conftest.py downloads it on the first invocation.'
        )

    paths = {}
    for name in CARTESIAN_TIDE_CONSTITUENTS:
        nc_path = DATASET_DIR / f'{name}_tide.nc'
        if not nc_path.is_file():
            print(f'  [skip] missing dataset file: {nc_path.name}')
            continue
        paths[name] = str(nc_path)

    if not paths:
        sys.exit('No constituent files found, cannot build model.')

    cfg = pyfes.config.Cartesian(
        paths=paths,
        tidal_type='tide',
        engine=pyfes.config.Engine.DARWIN,
    )
    return cfg.load()


def _build_lgp_model() -> pyfes.config.TidalModel:
    """Build an LGP2 tide model from the bundled FES2014 mesh.

    The ``fes_2014.nc`` file ships ~1.46 M triangles and ~2.98 M LGP2 nodes,
    which is representative of a real production mesh and exercises the
    R-tree query / triangle covering / LGP polynomial path that the
    Cartesian benchmark cannot reach.
    """
    nc_path = DATASET_DIR / LGP_DATASET_FILE
    if not nc_path.is_file():
        sys.exit(
            f'LGP dataset not found at {nc_path}. Run pytest once -- '
            'conftest.py downloads it on the first invocation.'
        )

    cfg = pyfes.config.LGP(
        path=str(nc_path),
        codes='lgp2',
        type='lgp2',
        amplitude='{constituent}_amp',
        phase='{constituent}_phase',
        constituents=list(LGP_TIDE_CONSTITUENTS),
        tidal_type='tide',
        engine=pyfes.config.Engine.DARWIN,
    )
    return cfg.load()


def _make_inputs(
    n: int,
    *,
    seed: int = 0,
) -> tuple[numpy.ndarray, numpy.ndarray, numpy.ndarray]:
    """Create deterministic ``(dates, lon, lat)`` arrays of length ``n``.

    Coordinates span the global ocean so the benchmark exercises both
    interpolation (most points) and extrapolation (a few near-coastal
    points), which is the realistic regime.
    """
    rng = numpy.random.default_rng(seed)
    lon = rng.uniform(-180.0, 180.0, size=n)
    lat = rng.uniform(-80.0, 80.0, size=n)
    # All points share the same epoch so ``evaluate_tide``'s astronomic
    # angle cache stays warm -- this isolates the per-point work.
    base = numpy.datetime64('2024-06-21T12:00:00')
    dates = numpy.full(n, base, dtype='datetime64[us]')
    return dates, lon, lat


def _make_dates(n: int) -> numpy.ndarray:
    """Hourly time series used for ``evaluate_tide_from_constituents``."""
    base = numpy.datetime64('2024-01-01T00:00:00', 'us')
    step = numpy.timedelta64(3600, 's').astype('timedelta64[us]')
    return base + numpy.arange(n, dtype='int64') * step


def _make_run_evaluate_tide(
    *,
    model: pyfes.config.TidalModel,
    dates: numpy.ndarray,
    lon: numpy.ndarray,
    lat: numpy.ndarray,
    settings: pyfes.FESSettings,
) -> Callable[[], None]:
    """Return a no-arg closure for one ``evaluate_tide`` benchmark case."""

    def _run() -> None:
        pyfes.evaluate_tide(
            model,
            dates,
            lon,
            lat,
            settings=settings,
        )

    return _run


def _make_run_evaluate_tide_from_constituents(
    *,
    dates: numpy.ndarray,
    settings: pyfes.FESSettings,
) -> Callable[[], None]:
    """Return a no-arg closure for one constituents benchmark case."""

    def _run() -> None:
        pyfes.evaluate_tide_from_constituents(
            KNOWN_CONSTITUENTS,
            dates,
            latitude=48.0,
            settings=settings,
        )

    return _run


# --------------------------------------------------------------------------- #
# Benchmark scenarios
# --------------------------------------------------------------------------- #


def bench_evaluate_tide(
    model: pyfes.config.TidalModel,
    *,
    repeat: int,
    warmup: int,
) -> list[dict[str, Any]]:
    """Time ``pyfes.evaluate_tide`` at a few batch sizes."""
    print('evaluate_tide (Cartesian, FES default settings)')
    settings = pyfes.FESSettings()
    results: list[dict[str, Any]] = []

    for n in EVALUATE_TIDE_SIZES:
        dates, lon, lat = _make_inputs(n)
        _run = _make_run_evaluate_tide(
            model=model,
            dates=dates,
            lon=lon,
            lat=lat,
            settings=settings,
        )

        results.append(
            _bench(
                f'n={n:>9d}',
                _run,
                repeat=repeat,
                warmup=warmup,
            )
        )
        results[-1]['n_points'] = n
    return results


def bench_evaluate_tide_lgp(
    model: pyfes.config.TidalModel,
    *,
    repeat: int,
    warmup: int,
) -> list[dict[str, Any]]:
    """Time ``pyfes.evaluate_tide`` on an LGP2 mesh model."""
    print('evaluate_tide (LGP2 mesh, FES default settings)')
    settings = pyfes.FESSettings()
    results: list[dict[str, Any]] = []

    for n in LGP_TIDE_SIZES:
        dates, lon, lat = _make_inputs(n)
        _run = _make_run_evaluate_tide(
            model=model,
            dates=dates,
            lon=lon,
            lat=lat,
            settings=settings,
        )

        results.append(
            _bench(
                f'n={n:>9d}',
                _run,
                repeat=repeat,
                warmup=warmup,
            )
        )
        results[-1]['n_points'] = n
    return results


def bench_evaluate_tide_from_constituents(
    *,
    repeat: int,
    warmup: int,
) -> list[dict[str, Any]]:
    """Time ``pyfes.evaluate_tide_from_constituents`` at a few sizes."""
    print('evaluate_tide_from_constituents (FES default settings)')
    settings = pyfes.FESSettings()
    results: list[dict[str, Any]] = []
    for n in CONSTITUENTS_SIZES:
        dates = _make_dates(n)
        _run = _make_run_evaluate_tide_from_constituents(
            dates=dates,
            settings=settings,
        )

        results.append(
            _bench(
                f'n={n:>9d}',
                _run,
                repeat=repeat,
                warmup=warmup,
            )
        )
        results[-1]['n_points'] = n
    return results


# --------------------------------------------------------------------------- #
# Driver
# --------------------------------------------------------------------------- #


def _parse_args(argv: list[str] | None = None) -> argparse.Namespace:
    parser = argparse.ArgumentParser(
        description='Microbenchmark for pyfes prediction hot paths.'
    )
    parser.add_argument(
        '--repeat',
        type=int,
        default=7,
        help='Timed repetitions per scenario (default: 7).',
    )
    parser.add_argument(
        '--warmup',
        type=int,
        default=2,
        help='Warm-up runs before timing (default: 2).',
    )
    parser.add_argument(
        '--json',
        dest='json_path',
        type=pathlib.Path,
        default=None,
        help='Write the raw measurements as JSON for offline comparison.',
    )
    parser.add_argument(
        '--skip',
        choices=('tide', 'lgp', 'constituents'),
        action='append',
        default=[],
        help='Skip a benchmark group (can be passed multiple times).',
    )
    parser.add_argument(
        '--installed',
        action='store_true',
        help=(
            'Benchmark whichever pyfes is importable (typically a '
            'conda-forge install) instead of the local build/ tree.'
        ),
    )
    return parser.parse_args(argv)


def main(argv: list[str] | None = None) -> int:
    """Entry point for the benchmark script."""
    args = _parse_args(argv)

    print(f'pyfes {pyfes.__version__}')
    print(f'numpy {numpy.__version__}')
    print(f'CPU count: {os.cpu_count()}')
    print(f'repeat={args.repeat}, warmup={args.warmup}')
    print()

    payload: dict[str, Any] = {
        'pyfes_version': pyfes.__version__,
        'numpy_version': numpy.__version__,
        'cpu_count': os.cpu_count(),
        'repeat': args.repeat,
        'warmup': args.warmup,
        'evaluate_tide': [],
        'evaluate_tide_lgp': [],
        'evaluate_tide_from_constituents': [],
    }

    if 'tide' not in args.skip:
        model = _build_cartesian_model()
        payload['evaluate_tide'] = bench_evaluate_tide(
            model,
            repeat=args.repeat,
            warmup=args.warmup,
        )
        print()

    if 'lgp' not in args.skip:
        lgp_model = _build_lgp_model()
        payload['evaluate_tide_lgp'] = bench_evaluate_tide_lgp(
            lgp_model,
            repeat=args.repeat,
            warmup=args.warmup,
        )
        print()

    if 'constituents' not in args.skip:
        payload['evaluate_tide_from_constituents'] = (
            bench_evaluate_tide_from_constituents(
                repeat=args.repeat,
                warmup=args.warmup,
            )
        )

    if args.json_path is not None:
        args.json_path.write_text(json.dumps(payload, indent=2))
        print(f'\nwrote {args.json_path}')

    return 0


if __name__ == '__main__':
    raise SystemExit(main())
