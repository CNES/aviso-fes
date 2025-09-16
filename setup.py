#!/usr/bin/env python3
# Copyright (c) 2025 CNES
#
# All rights reserved. Use of this source code is governed by a
# BSD-style license that can be found in the LICENSE file.
# Working directory
from typing import List, Optional, Tuple
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

# Python version file path
PY_VERSION = WORKING_DIRECTORY / 'src' / 'python' / 'pyfes' / 'version.py'

# C++ version file path
CXX_VERSION = WORKING_DIRECTORY / 'include' / 'fes' / 'version.hpp'

# Python version file content
PY_VERSION_TEMPLATE = '''# Copyright (c) 2025 CNES
#
# All rights reserved. Use of this source code is governed by a
# BSD-style license that can be found in the LICENSE file.
\"\"\"
Get software version information
================================
\"\"\"
__version__ = "{release}"
'''

# C++ version file content
CXX_VERSION_TEMPLATE = """/// @file perth/version.hpp
/// @brief Version of the library
#pragma once
/// Major version of the library
#define FES_VERSION_MAJOR {major}
/// Minor version of the library
#define FES_VERSION_MINOR {minor}
/// Patch version of the library
#define FES_VERSION_PATCH {patch}{dev}
"""


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


def fetch_package_version() -> str:
    if not (WORKING_DIRECTORY / '.git').exists():
        with PY_VERSION.open() as stream:
            for line in stream:
                if line.startswith('__version__'):
                    return line.split('=')[1].strip()[1:-1]

    # Make sure that the working directory is the root of the project,
    # otherwise setuptools_scm will not be able to find the version number.
    os.chdir(WORKING_DIRECTORY)
    import setuptools_scm

    try:
        return setuptools_scm.get_version()
    except:  # noqa: E722
        warnings.warn(
            'Unable to find the version number with setuptools_scm.',
            RuntimeWarning,
            stacklevel=2,
        )
        return '0.0.0'


def update_version_file(
    file_path: pathlib.Path,
    template: str,
    **kwargs,
) -> None:
    """Update a version file with the given template and keyword arguments."""
    if file_path.exists():
        with file_path.open() as stream:
            existing = stream.read()
    else:
        existing = ''

    new_content = template.format(**kwargs)

    if existing != new_content:
        with file_path.open('w') as stream:
            stream.write(new_content)


def update_version_library(release: str) -> None:
    """Update the version of the library."""
    if not (WORKING_DIRECTORY / '.git').exists():
        return

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

    update_version_file(
        CXX_VERSION,
        CXX_VERSION_TEMPLATE,
        major=major,
        minor=minor,
        patch=patch,
        dev=dev,
    )
    update_version_file(
        PY_VERSION,
        PY_VERSION_TEMPLATE,
        release=release,
    )


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
                     ('iers=', None, 'Use IERS 2010 constants'),
                     ('reconfigure', None,
                      'Forces CMake to reconfigure this project')]

    boolean_options = setuptools.command.build_ext.build_ext.boolean_options
    boolean_options += ['mkl', 'iers']

    def initialize_options(self) -> None:
        """Set default values for all the options that this command
        supports."""
        super().initialize_options()
        self.cmake_args = None
        self.cxx_compiler = None
        self.generator = None
        self.iers = None
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
    def conda_prefix() -> Optional[str]:
        """Returns the conda prefix."""
        if 'CONDA_PREFIX' in os.environ:
            return os.environ['CONDA_PREFIX']
        return None

    def set_cmake_user_options(self) -> List[str]:
        """Sets the options defined by the user."""
        result = []

        conda_prefix = self.conda_prefix()

        if self.cxx_compiler is not None:
            result.append('-DCMAKE_CXX_COMPILER=' + self.cxx_compiler)

        if conda_prefix is not None:
            result.append('-DCMAKE_PREFIX_PATH=' + conda_prefix)

        if self.iers:
            result.append('-DFES_USE_IERS_CONSTANTS=ON')

        if self.mkl:
            self.set_mklroot()

        return result

    def cmake_arguments(self, cfg: str, extdir: str) -> List[str]:
        """Returns the cmake arguments."""
        cmake_args: list[str] = [
            '-DCMAKE_BUILD_TYPE=' + cfg,
            '-DCMAKE_LIBRARY_OUTPUT_DIRECTORY=' + extdir,
            '-DPython3_EXECUTABLE=' + sys.executable,
            '-DFES_BUILD_PYTHON_BINDINGS=ON',
            *self.set_cmake_user_options(),
        ]
        if self.cmake_args:
            cmake_args.extend(self.cmake_args.split())
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


class DownloadTestData(setuptools.Command):
    """Custom command to download test data."""

    description = 'Download test data required for running tests'

    def initialize_options(self) -> None:
        """Set default values for all the options that this command supports."""
        pass

    def finalize_options(self) -> None:
        """Set final values for all the options that this command supports."""
        pass

    def run(self) -> None:
        """Download the test data."""
        # Prevent creation of __pycache__ folders
        old_dont_write_bytecode = sys.dont_write_bytecode
        sys.dont_write_bytecode = True

        # Import the function from conftest.py
        sys.path.insert(0, str(WORKING_DIRECTORY))
        try:
            from conftest import download_test_data
            download_test_data()
        finally:
            # Restore original setting
            sys.dont_write_bytecode = old_dont_write_bytecode
            # Clean up sys.path
            if str(WORKING_DIRECTORY) in sys.path:
                sys.path.remove(str(WORKING_DIRECTORY))


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
    release = fetch_package_version()
    update_version_library(release)
    setuptools.setup(
        cmdclass={
            'build_ext': BuildExt,
            'download_test_data': DownloadTestData,
        },  # type: ignore
        data_files=typehints(),
        ext_modules=[CMakeExtension(name='pyfes.core')],
        package_data={
            'pyfes': [
                'py.typed', 'core/*.pyi', 'tests/dataset/*.nc',
                'data/leap-seconds.txt'
            ],
        },
        version=release,
    )


if __name__ == '__main__':
    if platform.system() == 'Darwin':
        os.environ['MACOSX_DEPLOYMENT_TARGET'] = OSX_DEPLOYMENT_TARGET
    main()
