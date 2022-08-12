#!/usr/bin/env python3
# This file is part of FES library.
#
# FES is free software: you can redistribute it and/or modify
# it under the terms of the GNU LESSER GENERAL PUBLIC LICENSE as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.
#
# FES is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU LESSER GENERAL PUBLIC LICENSE for more details.
#
# You should have received a copy of the GNU LESSER GENERAL PUBLIC LICENSE
# along with FES.  If not, see <http://www.gnu.org/licenses/>.
"""This script is the entry point for building, distributing and installing
this module using distutils/setuptools."""
from typing import List
import os
import pathlib
import platform
import re
import subprocess
import sys
import sysconfig

import packaging.version
import setuptools
import setuptools.command.build_ext
import setuptools.command.install
import setuptools.command.sdist
import setuptools.command.test

# Check Python requirement
MAJOR = sys.version_info[0]
MINOR = sys.version_info[1]

# Versioning tag
PATTERN = '#define FES_VERSION'

# Working directory
WORKING_DIRECTORY = pathlib.Path(__file__).parent.absolute()


def build_dirname(extname=None):
    """Returns the name of the build directory"""
    extname = '' if extname is None else os.sep.join(extname.split('.')[:-1])
    if packaging.version.parse(
            setuptools.__version__) >= packaging.version.parse('62.1'):
        return pathlib.Path(
            WORKING_DIRECTORY, 'build', f'lib.{sysconfig.get_platform()}-'
            f'{sys.implementation.cache_tag}', extname)
    return pathlib.Path(WORKING_DIRECTORY, 'build',
                        f'lib.{sysconfig.get_platform()}-{MAJOR}.{MINOR}',
                        extname)


def execute(cmd):
    """Executes a command and returns the lines displayed on the standard
    output"""
    process = subprocess.Popen(cmd,
                               shell=True,
                               stdout=subprocess.PIPE,
                               stderr=subprocess.PIPE)
    stream = process.stdout
    assert stream is not None
    return stream.read().decode()


def update_version(path, version, pattern, replaced_line):
    """Updating the version number description"""
    with open(path) as stream:
        lines = stream.readlines()
    pattern = re.compile(pattern)

    for idx, line in enumerate(lines):
        match = pattern.search(line)
        if match is not None:
            lines[idx] = replaced_line % version

    with open(path, 'w') as stream:
        stream.write(''.join(lines))


def update_meta(path, version):
    """Updating the version number description in conda/meta.yaml."""
    update_version(path, version, r'{% set version = ".*" %}',
                   '{%% set version = "%s" %%}\n')


def update_python_module(path, version):
    """Updating the version number in the python module."""
    update_version(path, version, r'm\.attr\("__version__"\) = "([\d\.]+)";',
                   'm.attr("__version__") = "%s";\n')


def revision():
    """Returns the software version."""
    os.chdir(WORKING_DIRECTORY)
    path = pathlib.Path(__file__).parent.joinpath('include', 'fes.h')

    # If the ".git" directory exists, this function is executed in the
    # development environment, otherwise it's a release.
    if not pathlib.Path(WORKING_DIRECTORY, '.git').exists():
        pattern = re.compile(PATTERN + r' "(.*)"').search
        with open(path) as stream:
            for line in stream:
                match = pattern(line)
                if match is not None:
                    return match.group(1)
        raise AssertionError()

    stdout = execute('git describe --tags --dirty --long --always').strip()
    pattern = re.compile(r'([\w\d\.]+)-(\d+)-g[\w\d]+(?:-(dirty))?')
    match = pattern.search(stdout)
    if match is None:
        # No tag found, use the last commit
        pattern = re.compile(r'[\w\d]+(?:-(dirty))?')
        match = pattern.search(stdout)
        assert match is not None, f'Unable to parse git output {stdout!r}'
        version = '0.0'
    else:
        version = match.group(1)
        commits = int(match.group(2))
        if commits != 0:
            version += f'.dev{commits}'

    with open(path) as stream:
        lines = stream.readlines()

    for idx, line in enumerate(lines):
        if PATTERN in line:
            lines[idx] = PATTERN + " \"%s\"\n" % version

    with open(path, 'w') as stream:
        stream.writelines(lines)

    update_meta(WORKING_DIRECTORY.joinpath('conda', 'meta.yaml'), version)
    update_python_module(WORKING_DIRECTORY.joinpath('python', 'main.cpp'),
                         version)
    return version


class CMakeExtension(setuptools.Extension):
    """Python extension to build"""

    def __init__(self, name):
        super().__init__(name, sources=[])


class BuildExt(setuptools.command.build_ext.build_ext):
    """Build the Python extension using cmake"""
    user_options = setuptools.command.build_ext.build_ext.user_options
    user_options += [
        ('c-compiler=', None, 'Preferred C compiler'),
        ('cxx-compiler=', None, 'Preferred C++ compiler'),
        ('generator=', None, 'Selected CMake generator'),
        ('netcdf-root=', None, 'Preferred NetCDF installation prefix'),
        ('reconfigure', None, 'Forces CMake to reconfigure this project')
    ]

    def initialize_options(self) -> None:
        """Set default values for all the options that this command
        supports."""
        super().initialize_options()
        self.c_compiler = None
        self.cxx_compiler = None
        self.generator = None
        self.netcdf_root = None
        self.reconfigure = None

    def finalize_options(self) -> None:
        """Set final values for all the options that this command supports."""
        super().finalize_options()

    def run(self):
        """Carry out the action"""
        for ext in self.extensions:
            self.build_cmake(ext)
        super().run()

    @staticmethod
    def is_conda():
        """Detect if the Python interpreter is part of a conda distribution."""
        result = pathlib.Path(sys.prefix, 'conda-meta').exists()
        if not result:
            try:
                # pylint: disable=unused-import
                import conda  # noqa: F401

                # pylint: enable=unused-import
            except ImportError:
                result = False
            else:
                result = True
        return result

    def set_cmake_user_options(self):
        """Sets the options defined by the user."""
        is_conda = self.is_conda()
        result = []

        if self.c_compiler is not None:
            result.append('-DCMAKE_C_COMPILER=' + self.c_compiler)

        if self.netcdf_root is not None:
            result.append('-DNETCDF_INCLUDE_DIR=' +
                          os.path.join(self.netcdf_root, 'include'))
            result.append(
                '-DNETCDF_LIBRARY=' +
                os.path.join(self.netcdf_root, 'lib', 'libnetcdf.so'))
        elif is_conda:
            result.append('-DCMAKE_PREFIX_PATH=' + sys.prefix)

        return result

    def get_cmake_args(self, cfg: str, extdir: str) -> List[str]:
        """build cmake arguments.

        # Args:
        * `cfg`: config, one of {"debug", "release"}
        * `extdir`: output directory.
        """
        cmake_args = [
            '-DBUILD_PYTHON=on',
            '-DBUILD_SHARED_LIBS=off',
            '-DCMAKE_LIBRARY_OUTPUT_DIRECTORY=' + str(extdir),
            '-DCMAKE_PREFIX_PATH=' + sys.prefix,
            '-DPython3_EXECUTABLE=' + sys.executable,
        ] + self.set_cmake_user_options()

        is_windows = platform.system() == 'Windows'

        if self.generator is not None:
            cmake_args.append('-G' + self.generator)
        elif is_windows:
            cmake_args.append('-G' + 'Visual Studio 16 2019')

        if is_windows:
            cmake_args += [
                '-DCMAKE_GENERATOR_PLATFORM=x64',
                '-DCMAKE_LIBRARY_OUTPUT_DIRECTORY_{}={}'.format(
                    cfg.upper(), extdir),
            ]
        else:
            cmake_args += ['-DCMAKE_BUILD_TYPE=' + cfg]
            if platform.system() == 'Darwin':
                cmake_args += ['-DCMAKE_OSX_DEPLOYMENT_TARGET=10.14']
        return cmake_args

    def get_build_args(self, cfg: str) -> List[str]:
        """make compiler build arguments.

        # Args:
        * `cfg`: config, one of {"debug", "release"}
        """
        build_args = ['--config', cfg]
        is_windows = platform.system() == 'Windows'
        if is_windows:
            build_args += ['--', '/m']
        else:
            build_args += ['--', '-j%d' % os.cpu_count()]

        return build_args

    def build_cmake(self, ext):
        """execute cmake to build the python extension"""
        # these dirs will be created in build_py, so if you don't have
        # any python sources to bundle, the dirs will be missing
        build_temp = pathlib.Path(WORKING_DIRECTORY, self.build_temp)
        build_temp.mkdir(parents=True, exist_ok=True)
        extdir = build_dirname(ext.name)

        cfg = 'debug' if self.debug else 'release'

        os.chdir(str(build_temp))

        # Has CMake ever been executed?
        if pathlib.Path(build_temp, 'CMakeFiles',
                        'TargetDirectories.txt').exists():
            # The user must force the reconfiguration
            configure = self.reconfigure is not None
        else:
            configure = True

        if configure:
            cmake_args = self.get_cmake_args(cfg, str(extdir))
            self.spawn(['cmake', str(WORKING_DIRECTORY)] + cmake_args)
        if not self.dry_run:  # type: ignore
            build_args = self.get_build_args(cfg)
            self.spawn(['cmake', '--build', '.', '--target', 'pyfes'] +
                       build_args)
        os.chdir(str(WORKING_DIRECTORY))


class Revision(setuptools.Command):
    """Get the current git revision"""

    description = 'get the current git revision'
    user_options = []

    def initialize_options(self):
        """initialize options"""
        pass

    def finalize_options(self):
        """finalize options"""
        pass

    def run(self):
        """Carry out the action"""
        print(revision())


def main():
    setuptools.setup(
        name='pyfes',
        version=revision(),
        classifiers=[
            'Development Status :: 3 - Stable',
            'Topic :: Scientific/Engineering :: Physics',
            'License :: OSI Approved :: GNU General Public License v3 (GPLv3)',
            'Natural Language :: English',
            'Operating System :: POSIX',
            'Operating System :: MacOS',
            'Operating System :: Microsoft :: Windows',
            'Programming Language :: Python :: 3.6',
            'Programming Language :: Python :: 3.7',
            'Programming Language :: Python :: 3.8',
            'Programming Language :: Python :: 3.9',
            'Programming Language :: Python :: 3.10',
        ],
        description='FES2014 prediction software.',
        url='https://github.com/CNES/aviso-fes',
        author='NES/CLS/LEGOS',
        license='GNU General Public License v3 (GPLv3)',
        ext_modules=[CMakeExtension(name='pyfes')],
        setup_requires=[],
        # install_requires=["numpy"],
        # tests_require=["netCDF4", "numpy"],
        # package_dir={'': 'src'},
        # packages=setuptools.find_packages(where="src"),
        cmdclass={
            'build_ext': BuildExt,
            'revision': Revision
        },  # type: ignore
        zip_safe=False)


if __name__ == '__main__':
    main()
