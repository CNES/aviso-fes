#!/usr/bin/env python3
# Copyright (c) 2024 CNES
#
# All rights reserved. Use of this source code is governed by a
# BSD-style license that can be found in the LICENSE file.
# Working directory
from typing import List, Tuple
import os
import pathlib
import platform
import re
import sys
import sysconfig
import warnings

import setuptools
import setuptools.command.build_ext

# Working directory
WORKING_DIRECTORY = pathlib.Path(__file__).parent.absolute()

# OSX deployment target
OSX_DEPLOYMENT_TARGET = '10.14'


def compare_setuptools_version(required: Tuple[int, ...]) -> bool:
    """Compare the version of setuptools with the required version."""
    current = tuple(map(int, setuptools.__version__.split('.')[:2]))
    return current >= required


def distutils_dirname(prefix=None, extname=None) -> pathlib.Path:
    """Returns the name of the build directory."""
    prefix = 'lib' or prefix
    extname = '' if extname is None else os.sep.join(extname.split('.')[:-1])
    if compare_setuptools_version((62, 1)):
        return pathlib.Path(
            WORKING_DIRECTORY, 'build', f'{prefix}.{sysconfig.get_platform()}-'
            f'{sys.implementation.cache_tag}', extname)
    return pathlib.Path(
        WORKING_DIRECTORY, 'build', f'{prefix}.{sysconfig.get_platform()}-'
        f'{sys.version_info[0]}.{sys.version_info[1]}', extname)


def version() -> str:
    """Returns the version number."""
    path = WORKING_DIRECTORY / 'src' / 'python' / 'pyfes' / 'version.py'
    if not path.exists():
        # Make sure that the working directory is the root of the project,
        # otherwise setuptools_scm will not be able to find the version number.
        os.chdir(WORKING_DIRECTORY)
        import setuptools_scm
        try:
            return setuptools_scm.get_version()
        except:  # noqa: E722
            warnings.warn(
                'Unable to find the version number with setuptools_scm.',
                RuntimeWarning)
            return '0.0.0'
    with path.open() as stream:
        for line in stream:
            if line.startswith('__version__'):
                return line.split('=')[1].strip()[1:-1]
    raise RuntimeError('Unable to find version string.')


def update_version_library() -> None:
    """Update the version of the library."""
    if not (WORKING_DIRECTORY / '.git').exists():
        return
    release = version()
    pattern = re.compile(r'(?:(\d+)\.)?(?:(\d+)\.)?(\*|\d+)(\.dev\d+)?')
    match = pattern.match(release)
    if match is None:
        raise RuntimeError(f'Invalid version number: {release}')
    major, minor, patch, dev = match.groups()
    if minor is None:
        minor = patch
        patch = '0'
    if dev is None:
        dev = ''

    header = WORKING_DIRECTORY / 'include' / 'fes' / 'version.hpp'

    if header.exists():
        with header.open() as stream:
            existing = ''.join(stream.read())
    else:
        existing = ''

    new = f"""/// @file include/fes/version.hpp
/// @brief Version of the library
#pragma once
/// Major version of the library
#define FES_VERSION_MAJOR {major}
/// Minor version of the library
#define FES_VERSION_MINOR {minor}
/// Patch version of the library
#define FES_VERSION_PATCH {patch}{dev}
"""
    if existing != ''.join(new):
        with header.open('w') as stream:
            stream.write(new)


# pylint: disable=too-few-public-methods
class CMakeExtension(setuptools.Extension):
    """Python extension to build."""

    def __init__(self, name):
        super().__init__(name, sources=[])

    # pylint: enable=too-few-public-methods


# pylint: disable=too-many-instance-attributes
class BuildExt(setuptools.command.build_ext.build_ext):
    """Build everything needed to install."""
    user_options = setuptools.command.build_ext.build_ext.user_options
    user_options += [('cmake-args=', None, 'Additional arguments for CMake'),
                     ('cxx-compiler=', None, 'Preferred C++ compiler'),
                     ('generator=', None, 'Selected CMake generator'),
                     ('mkl=', None, 'Using MKL as BLAS library'),
                     ('reconfigure', None,
                      'Forces CMake to reconfigure this project')]

    boolean_options = setuptools.command.build_ext.build_ext.boolean_options
    boolean_options += ['mkl']

    def initialize_options(self) -> None:
        """Set default values for all the options that this command
        supports."""
        super().initialize_options()
        self.cmake_args = None
        self.cxx_compiler = None
        self.generator = None
        self.mkl = None
        self.reconfigure = None

    def run(self) -> None:
        """Carry out the action."""
        for ext in self.extensions:
            self.build_cmake(ext)
        super().run()

    @staticmethod
    def set_mklroot() -> None:
        """Set the MKLROOT environment variable if the MKL header is found."""
        mkl_header = pathlib.Path(sys.prefix, 'include', 'mkl.h')
        if not mkl_header.exists():
            mkl_header = pathlib.Path(sys.prefix, 'Library', 'include',
                                      'mkl.h')

        if mkl_header.exists():
            os.environ['MKLROOT'] = sys.prefix

    @staticmethod
    def conda_prefix() -> str | None:
        """Returns the conda prefix."""
        if 'CONDA_PREFIX' in os.environ:
            return os.environ['CONDA_PREFIX']
        return None

    def set_cmake_user_options(self) -> list[str]:
        """Sets the options defined by the user."""
        result = []

        conda_prefix = self.conda_prefix()

        if self.cxx_compiler is not None:
            result.append('-DCMAKE_CXX_COMPILER=' + self.cxx_compiler)

        if conda_prefix is not None:
            result.append('-DCMAKE_PREFIX_PATH=' + conda_prefix)

        if self.mkl:
            self.set_mklroot()

        return result

    def cmake_arguments(self, cfg: str, extdir: str) -> list[str]:
        """Returns the cmake arguments."""
        cmake_args: list[str] = [
            '-DCMAKE_BUILD_TYPE=' + cfg,
            '-DCMAKE_LIBRARY_OUTPUT_DIRECTORY=' + extdir,
            '-DPython3_EXECUTABLE=' + sys.executable,
            '-DFES_BUILD_PYTHON_BINDINGS=ON',
            *self.set_cmake_user_options(),
        ]
        return cmake_args

    def build_cmake(self, ext) -> None:
        """Execute cmake to build the Python extension."""
        # These dirs will be created in build_py, so if you don't have
        # any python sources to bundle, the dirs will be missing
        build_temp = pathlib.Path(WORKING_DIRECTORY, self.build_temp)
        build_temp.mkdir(parents=True, exist_ok=True)
        extdir = str(
            pathlib.Path(self.get_ext_fullpath(ext.name)).parent.resolve())

        cfg = 'Debug' if self.debug else 'Release'

        cmake_args = self.cmake_arguments(cfg, extdir)
        build_args = ['--config', cfg]

        is_windows = platform.system() == 'Windows'

        if self.generator is not None:
            cmake_args.append('-G' + self.generator)
        elif is_windows:
            cmake_args.append(
                '-G' + os.environ.get('CMAKE_GEN', 'Visual Studio 17 2022'))

        if not is_windows:
            build_args += ['--', f'-j{os.cpu_count()}']
            if platform.system() == 'Darwin':
                cmake_args += [
                    f'-DCMAKE_OSX_DEPLOYMENT_TARGET={OSX_DEPLOYMENT_TARGET}'
                ]
        else:
            cmake_args += [
                '-DCMAKE_GENERATOR_PLATFORM=x64',
                f'-DCMAKE_LIBRARY_OUTPUT_DIRECTORY_{cfg.upper()}={extdir}',
            ]
            build_args += ['--', '/m']

        if self.cmake_args:
            cmake_args.extend(self.cmake_args.split())

        os.chdir(str(build_temp))

        # Has CMake ever been executed?
        configure = (self.reconfigure is not None) if pathlib.Path(
            build_temp, 'CMakeFiles',
            'TargetDirectories.txt').exists() else True

        if configure:
            self.spawn(['cmake', str(WORKING_DIRECTORY)] + cmake_args)
        if not self.dry_run:  # type: ignore
            cmake_cmd = [
                'cmake', '--build', '.', '--target',
                ext.name.split('.')[-1]
            ]
            self.spawn(cmake_cmd + build_args)
        os.chdir(str(WORKING_DIRECTORY))

    # pylint: enable=too-many-instance-attributes


def typehints() -> List[Tuple[str, List[str]]]:
    """Get the list of type information files."""
    pyi = []
    for root, _, files in os.walk(WORKING_DIRECTORY):
        pyi += [
            str(pathlib.Path(root, item).relative_to(WORKING_DIRECTORY))
            for item in files if item.endswith('.pyi')
        ]
    return [(str(pathlib.Path('pyfes', 'core')), pyi)]


def main() -> None:
    """Execute the setup."""
    update_version_library()
    setuptools.setup(
        cmdclass={
            'build_ext': BuildExt,
        },  # type: ignore
        data_files=typehints(),
        ext_modules=[CMakeExtension(name='pyfes.core')],
        package_data={
            'pyfes': [
                'py.typed', 'core/*.pyi', 'tests/dataset/*.nc',
                'data/leap-seconds.txt'
            ],
        },
    )


if __name__ == '__main__':
    if platform.system() == 'Darwin':
        os.environ['MACOSX_DEPLOYMENT_TARGET'] = OSX_DEPLOYMENT_TARGET
    main()
