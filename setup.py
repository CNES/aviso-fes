# -*- coding: utf-8 -*-
import os
import pathlib
import sys
from subprocess import check_call

from setuptools import Extension, setup, find_packages
from setuptools.command.build_ext import build_ext


class CMakeExtension(Extension):
    def __init__(self, name, sourcedir=""):
        Extension.__init__(self, name, sources=[])
        self.sourcedir = os.path.abspath(sourcedir)


class CMakeBuild(build_ext):
    def build_extension(self, ext):
        # Download submodules
        base_dir = pathlib.Path(__file__).parent
        check_call(['git', 'submodule', 'update', '--init', '--recursive'], cwd=base_dir)

        extdir = os.path.abspath(os.path.dirname(self.get_ext_fullpath(ext.name)))

        # required for auto-detection & inclusion of auxiliary "native" libs
        if not extdir.endswith(os.path.sep):
            extdir += os.path.sep

        debug = int(os.environ.get("DEBUG", 0)) if self.debug is None else self.debug
        cfg = "Debug" if debug else "Release"

        if not os.path.exists(self.build_temp):
            os.makedirs(self.build_temp)

        check_call(
            [
                "cmake", "-B", self.build_temp,
                "-DCMAKE_LIBRARY_OUTPUT_DIRECTORY={}".format(extdir),
                "-DBUILD_PYTHON=on",
                "-DPYTHON_EXECUTABLE={}".format(sys.executable),
                "-DCMAKE_BUILD_TYPE={}".format(cfg),
                "-DBUILD_SHARED_LIBS=on",
            ]
        )

        check_call(["cmake", "--build", self.build_temp, "--config", cfg])


setup(
    name="pyfes",
    version="2.9.3",
    author="""Fabien Lefevre (flevere@cls.fr)
Frederic Briol (fbriol@cls.fr)
Loren Carrere (lcarrere@cls.fr)""",
    packages=find_packages(),
    description="FES2014 prediction software",
    long_description="""This package is the fully revised version of the FES2014 distribution.
This distribution includes the FES2014 tidal prediction software and
the FES2014 tides databases""",
    ext_modules=[CMakeExtension("pyfes")],
    cmdclass={
        "build_ext": CMakeBuild
    },
    zip_safe=False,
)
