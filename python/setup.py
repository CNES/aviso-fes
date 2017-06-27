try:
    import configparser
except ImportError:
    import ConfigParser as configparser
import distutils.dist
import os
import setuptools
import numpy.distutils.misc_util as np


class SetupConfig(object):
    """
    Package configuration
    """
    CWD = os.path.dirname(os.path.abspath(__file__))

    def __init__(self):
        super(SetupConfig, self).__init__()
        self.path = os.path.join(self.CWD, 'setup.cfg')
        self.parser = configparser.ConfigParser()
        self.parser.read(self.path)

    def write(self):
        """
        Write the configuration file
        """
        with open(self.path, 'w') as handle:
            self.parser.write(handle)

    def get_option(self, section, option, sep=':'):
        """
        Read an option from the configuration file
        """
        if self.parser.has_option(section, option):
            value = self.parser.get(section, option)
            return value.split(sep) if value else None
        return None

    def get_build_ext(self):
        """
        Gets the parameters to build the extension
        """
        if self.parser.has_section('build_ext'):
            include_dirs = self.get_option('build_ext', 'include_dirs')
            libraries = self.get_option('build_ext', 'libraries', ' ')
            library_dirs = self.get_option('build_ext', 'library_dirs')

            return include_dirs, libraries, library_dirs
        return None, None, None

    @classmethod
    def cython_include(cls):
        """
        Get the include pah for Cython pxd
        """
        return os.path.join(cls.CWD, 'extension', 'wrapper')

    def set_build_ext(self, include_dirs, libraries, library_dirs):
        """
        Sets the parameters to build the extension
        """
        if not self.parser.has_section('build_ext'):
            self.parser.add_section('build_ext')
        if library_dirs:
            self.parser.set('build_ext',
                            'library_dirs',
                            ":".join(library_dirs))
        elif self.parser.has_option('build_ext', 'library_dirs'):
            self.parser.remove_option('build_ext', 'library_dirs')
        include_dirs += [
            os.path.abspath(os.path.join(self.CWD, '..', 'src'))]
        include_dirs += np.get_numpy_include_dirs()
        self.parser.set('build_ext',
                        'include_dirs',
                        ":".join(include_dirs))
        self.parser.set('build_ext',
                        'libraries',
                        " ".join(libraries))

    @classmethod
    def sources(cls):
        """
        Returns the list of necessary files to build the extension
        """
        result = []
        for root, _, files in os.walk(os.path.join(cls.CWD, '..', 'src')):
            for item in files:
                _, ext = os.path.splitext(item)
                if ext == '.c':
                    result.append(os.path.abspath(os.path.join(root, item)))
        return result


class Setup(setuptools.Command, SetupConfig):
    """
    Configuration commands
    """
    description = "Configuration commands"
    user_options = [
        ('netcdf-includes=', None,
         "Location of NetCDF headers"),
        ('netcdf-libraries=', None,
         "Location of NetCDF libraries"),
        ('uthash-includes=', None,
         "Location of uthash headers"),
    ]

    def __init__(self, *args, **kwargs):
        # super does not work because the class "setuptools.Command"
        # does not inherit of object.
        setuptools.Command.__init__(self, *args, **kwargs)
        SetupConfig.__init__(self)

    def initialize_options(self):
        """
        Initialize options to default values.
        """
        self.netcdf_includes = None
        self.netcdf_libraries = None
        self.uthash_includes = None

    def finalize_options(self):
        """
        Finalize options.
        """
        pass

    def run(self):
        """
        Set the values set by the user to build the library
        """
        include_dirs = []
        library_dirs = []

        if self.netcdf_libraries:
            library_dirs.append(self.netcdf_libraries)

        if self.netcdf_includes:
            include_dirs.append(self.netcdf_includes)
        if self.uthash_includes:
            include_dirs.append(self.uthash_includes)

        libraries = ["netcdf"]

        self.set_build_ext(include_dirs, libraries, library_dirs)
        self.write()


def main():
    """
    Main function
    """
    try:
        import Cython.Build
        has_cython = True
        sources = ['extension/wrapper/fes.pyx']
    except ImportError:
        has_cython = False
        sources = ['extension/wrapper/fes.c']

    extensions = [
        setuptools.Extension(
            'fes',
            sources + SetupConfig.sources(),
            language='c',
            libraries=['netcdf']
        )
    ]

    if has_cython:
        ext_modules = Cython.Build.cythonize(
            extensions, include_path=[SetupConfig.cython_include()])
    else:
        ext_modules = extensions

    classifiers = [
        'Development Status :: 5 - Production/Stable',
        'Intended Audience :: Science/Research',
        'OSI Approved :: GNU LESSER GENERAL PUBLIC LICENSE v3 or later '
        '(LGPLv3+)',
        'Programming Language :: Python',
        'Programming Language :: Python :: 2',
        'Programming Language :: Python :: 3',
        'Topic :: Scientific/Engineering',
    ]

    # Create the default setup configuration
    setup = Setup(distutils.dist.Distribution())
    if not os.path.exists(setup.path):
        setup.initialize_options()
        setup.run()

    setuptools.setup(
        name="libfes",
        version="2.7.4",
        author="CLS",
        keywords='FES tide oceanography',
        setup_requires=['numpy'],
        install_requires=['numpy'],
        classifiers=classifiers,
        ext_modules=ext_modules,
        cmdclass={
            'setup': Setup
        },
        zip_safe=False,
    )

if __name__ == '__main__':
    main()
