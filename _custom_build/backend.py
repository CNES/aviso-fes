# Copyright (c) 2025 CNES
#
# All rights reserved. Use of this source code is governed by a
# BSD-style license that can be found in the LICENSE file.
import argparse
import sys

import setuptools.build_meta


def usage(args: dict[str, str | list[str] | None]) -> argparse.Namespace:
    """Parse the command line arguments."""
    parser = argparse.ArgumentParser('Custom build backend')
    parser.add_argument('--cxx-compiler', help='Preferred C++ compiler')
    parser.add_argument('--generator', help='Selected CMake generator')
    parser.add_argument('--cmake-args', help='Additional arguments for CMake')
    parser.add_argument('--mkl', help='Using MKL as BLAS library')
    return parser.parse_args(args=[f'--{k}={v}' for k, v in args.items()])


def decode_bool(value: str | None) -> bool:
    """Decode a boolean value."""
    if value is None:
        return False
    value = value.lower()
    return value in {'1', 'true', 'yes'}


class _CustomBuildMetaBackend(setuptools.build_meta._BuildMetaBackend):
    """Custom build backend.

    This class is used to pass the option from pip to the setup.py script.

    Reference: https://setuptools.pypa.io/en/latest/build_meta.html
    """

    def run_setup(self, setup_script='setup.py'):
        """Run the setup script."""
        args = usage(self.config_settings or {})  # type: ignore[arg-type]
        setuptools_args = []
        if args.cxx_compiler:
            setuptools_args.append(f'--cxx-compiler={args.cxx_compiler}')
        if args.generator:
            setuptools_args.append(f'--generator={args.generator}')
        if args.cmake_args:
            setuptools_args.append(f'--cmake-args={args.cmake_args}')
        if decode_bool(args.mkl):
            setuptools_args.append('--mkl=yes')

        if setuptools_args:
            sys.argv = (sys.argv[:1] + ['build_ext'] + setuptools_args +
                        sys.argv[1:])
        return super().run_setup(setup_script)

    def build_wheel(
        self,
        wheel_directory,
        config_settings=None,
        metadata_directory=None,
    ):
        """Build the wheel."""
        self.config_settings = config_settings
        return super().build_wheel(
            wheel_directory,
            config_settings,
            metadata_directory,
        )


build_wheel = _CustomBuildMetaBackend().build_wheel
