#!/usr/bin/env python3
# Copyright (c) 2024 CNES
#
# All rights reserved. Use of this source code is governed by a
# BSD-style license that can be found in the LICENSE file.
# Working directory
from typing import Any
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


def compare_setuptools_version(required: tuple[int, ...]) -> bool:
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
    user_options += [
        ('boost-root=', None, 'Preferred Boost installation prefix'),
        ('conda-forge', None, 'Generation of the conda-forge package'),
        ('cxx-compiler=', None, 'Preferred C++ compiler'),
        ('eigen-root=', None, 'Preferred Eigen3 include directory'),
        ('generator=', None, 'Selected CMake generator'),
        ('mkl-root=', None, 'Preferred MKL installation prefix'),
        ('mkl=', None, 'Using MKL as BLAS library'),
        ('reconfigure', None, 'Forces CMake to reconfigure this project')
    ]

    boolean_options = setuptools.command.build_ext.build_ext.boolean_options
    boolean_options += ['mkl', 'conda-forge']

    def initialize_options(self) -> None:
        """Set default values for all the options that this command
        supports."""
        super().initialize_options()
        self.boost_root = None
        self.conda_forge = None
        self.cxx_compiler = None
        self.eigen_root = None
        self.generator = None
        self.mkl = None
        self.mkl_root = None
        self.reconfigure = None

    def finalize_options(self) -> None:
        """Set final values for all the options that this command supports."""
        super().finalize_options()
        if self.mkl_root is not None:
            self.mkl = True
        if not self.mkl and self.mkl_root:
            raise RuntimeError(
                'argument --mkl_root not allowed with argument --mkl=no')

    def run(self) -> None:
        """Carry out the action."""
        for ext in self.extensions:
            self.build_cmake(ext)
        super().run()

    def boost(self) -> list[str] | None:
        """Return the Boost installation prefix."""
        # Do not search system for Boost & disable the search for boost-cmake
        boost_option = '-DBoost_NO_SYSTEM_PATHS=TRUE ' \
            '-DBoost_NO_BOOST_CMAKE=TRUE'
        boost_root = pathlib.Path(sys.prefix)
        if (boost_root / 'include' / 'boost').exists():
            return f'{boost_option} -DBOOSTROOT={boost_root}'.split()
        boost_root = pathlib.Path(sys.prefix, 'Library', 'include')
        if not boost_root.exists():
            if self.conda_forge:
                raise RuntimeError(
                    'Unable to find the Boost library in the conda '
                    'distribution used.')
            return None
        return f'{boost_option} -DBoost_INCLUDE_DIR={boost_root}'.split()

    def eigen(self) -> str | None:
        """Return the Eigen3 installation prefix."""
        eigen_include_dir = pathlib.Path(sys.prefix, 'include', 'eigen3')
        if eigen_include_dir.exists():
            return f'-DEIGEN3_INCLUDE_DIR={eigen_include_dir}'
        eigen_include_dir = pathlib.Path(sys.prefix, 'Library', 'include',
                                         'eigen3')
        if not eigen_include_dir.exists():
            eigen_include_dir = eigen_include_dir.parent
        if not eigen_include_dir.exists():
            if self.conda_forge:
                raise RuntimeError(
                    'Unable to find the Eigen3 library in the conda '
                    'distribution used.')
            return None
        return f'-DEIGEN3_INCLUDE_DIR={eigen_include_dir}'

    @staticmethod
    def set_conda_mklroot() -> None:
        """Set the MKLROOT environment variable."""
        mkl_header = pathlib.Path(sys.prefix, 'include', 'mkl.h')
        if mkl_header.exists():
            os.environ['MKLROOT'] = sys.prefix
            return

    @staticmethod
    def is_conda() -> bool:
        """Return True if the current Python distribution is conda."""
        result = pathlib.Path(sys.prefix, 'conda-meta').exists()
        if not result:
            try:
                # pylint: disable=unused-import,import-outside-toplevel
                import conda  # noqa: F401

                # pylint: enable=unused-import,import-outside-toplevel
            except ImportError:
                result = False
            else:
                result = True
        return result

    def set_cmake_user_options(self) -> list[str]:
        """Set the CMake user options."""
        cmake_variable: Any

        is_conda = self.is_conda()
        result = []

        if self.cxx_compiler is not None:
            result.append('-DCMAKE_CXX_COMPILER=' + self.cxx_compiler)

        if self.conda_forge:
            result.append('-DCONDA_FORGE=ON')

        if self.boost_root is not None:
            result.append('-DBOOSTROOT=' + self.boost_root)
        elif is_conda:
            cmake_variable = self.boost()
            if cmake_variable:
                result += cmake_variable

        if self.eigen_root is not None:
            result.append('-DEIGEN3_INCLUDE_DIR=' + self.eigen_root)
        elif is_conda:
            cmake_variable = self.eigen()
            if cmake_variable:
                result.append(cmake_variable)

        if self.mkl_root is not None:
            os.environ['MKLROOT'] = self.mkl_root
        elif is_conda and self.mkl:
            self.set_conda_mklroot()

        return result

    def build_cmake(self, ext) -> None:
        """Execute cmake to build the Python extension."""
        # These dirs will be created in build_py, so if you don't have
        # any python sources to bundle, the dirs will be missing
        build_temp = pathlib.Path(WORKING_DIRECTORY, self.build_temp)
        build_temp.mkdir(parents=True, exist_ok=True)
        extdir = str(
            pathlib.Path(self.get_ext_fullpath(ext.name)).parent.resolve())

        cfg = 'Debug' if self.debug else 'Release'

        cmake_args = [
            '-DCMAKE_BUILD_TYPE=' + cfg, '-DCMAKE_LIBRARY_OUTPUT_DIRECTORY=' +
            str(extdir), '-DPython3_EXECUTABLE=' + sys.executable,
            '-DFES_BUILD_PYTHON_BINDINGS=ON'
        ] + self.set_cmake_user_options()

        build_args = ['--config', cfg]

        is_windows = platform.system() == 'Windows'

        if self.generator is not None:
            cmake_args.append('-G' + self.generator)
        elif is_windows:
            cmake_args.append('-G' + 'Visual Studio 17 2022')

        if self.verbose:  # type: ignore
            build_args += ['--verbose']

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

        os.chdir(str(build_temp))

        # Has CMake ever been executed?
        if pathlib.Path(build_temp, 'CMakeFiles',
                        'TargetDirectories.txt').exists():
            # The user must force the reconfiguration
            configure = self.reconfigure is not None
        else:
            configure = True

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


def typehints() -> list[tuple[str, list[str]]]:
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
