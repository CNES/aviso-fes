# Copyright (c) 2025 CNES
#
# All rights reserved. Use of this source code is governed by a
# BSD-style license that can be found in the LICENSE file.
"""Custom build backend for setuptools with custom options."""

import argparse
from collections.abc import Mapping
import os
import sys

import setuptools.build_meta


def usage(args: dict[str, str | list[str] | None]) -> argparse.Namespace:
    """Parse the command line arguments."""
    parser = argparse.ArgumentParser('Custom build backend')
    parser.add_argument('--cxx-compiler', help='Preferred C++ compiler')
    parser.add_argument('--iers', help='Use IERS 2010 constants')
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

    def run_setup(self, setup_script: str = 'setup.py') -> None:
        """Run the setup script."""
        config_settings = getattr(self, 'config_settings', None)
        args = usage(config_settings or {})  # type: ignore[arg-type]
        setuptools_args = []
        if args.cxx_compiler:
            setuptools_args.append(f'--cxx-compiler={args.cxx_compiler}')
        if decode_bool(args.iers):
            setuptools_args.append('--iers=yes')
        if args.generator:
            setuptools_args.append(f'--generator={args.generator}')
        if args.cmake_args:
            setuptools_args.append(f'--cmake-args={args.cmake_args}')
        if decode_bool(args.mkl):
            setuptools_args.append('--mkl=yes')

        if setuptools_args:
            sys.argv = [
                *sys.argv[:1],
                'build_ext',
                *setuptools_args,
                *sys.argv[1:],
            ]
        return super().run_setup(setup_script)

    def build_wheel(
        self,
        wheel_directory: str | os.PathLike[str],
        config_settings: Mapping[str, str | list[str] | None] | None = None,
        metadata_directory: str | os.PathLike[str] | None = None,
    ) -> str:
        """Build the wheel.

        Args:
            wheel_directory: The directory to store the wheel.
            config_settings: The configuration settings.
            metadata_directory: The metadata directory.

        Returns:
            str: The path to the built wheel.

        """
        self.config_settings = config_settings
        return super().build_wheel(
            wheel_directory,
            config_settings,
            metadata_directory,
        )

    def build_sdist(
        self,
        sdist_directory: str | os.PathLike[str],
        config_settings: Mapping[str, str | list[str] | None] | None = None,
    ) -> str:
        """Build the source distribution.

        Args:
            sdist_directory: The directory to store the source distribution.
            config_settings: The configuration settings.

        Returns:
            str: The path to the built source distribution.

        """
        self.config_settings = config_settings
        return super().build_sdist(
            sdist_directory,
            config_settings,
        )

    def build_editable(
        self,
        wheel_directory: str | os.PathLike[str],
        config_settings: Mapping[str, str | list[str] | None] | None = None,
        metadata_directory: str | os.PathLike[str] | None = None,
    ) -> str:
        """Build an editable wheel.

        Args:
            wheel_directory: The directory to store the wheel.
            config_settings: The configuration settings.
            metadata_directory: The metadata directory.

        Returns:
            str: The path to the built editable wheel.

        """
        self.config_settings = config_settings
        return super().build_editable(
            wheel_directory,
            config_settings,
            metadata_directory,
        )


_backend = _CustomBuildMetaBackend()
build_wheel = _backend.build_wheel
build_editable = _backend.build_editable
build_sdist = _backend.build_sdist
get_requires_for_build_wheel = _backend.get_requires_for_build_wheel
get_requires_for_build_editable = _backend.get_requires_for_build_editable
prepare_metadata_for_build_editable = (
    _backend.prepare_metadata_for_build_editable
)
