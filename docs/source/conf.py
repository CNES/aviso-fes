# Configuration file for the Sphinx documentation builder.
#
# For the full list of built-in configuration values, see the documentation:
# https://www.sphinx-doc.org/en/master/usage/configuration.html

import importlib.metadata
import pathlib
import sys
import sysconfig

#: The directory containing this file.
HERE = pathlib.Path(__file__).absolute().parent


def build_dirname(extname=None):
    """Returns the name of the build directory."""
    return pathlib.Path(
        HERE.parent.parent, 'build',
        f'lib.{sysconfig.get_platform()}-{sys.implementation.cache_tag}')


def push_front_syspath():
    """Add the build directory to the front of sys.path."""
    if HERE.parent.parent.joinpath('setup.py').exists():
        # We are in the root directory of the development tree
        sys.path.insert(0, str(build_dirname().resolve()))


# Insert the project root dir as the first element in the PYTHONPATH.
push_front_syspath()

# -- Project information -----------------------------------------------------
# https://www.sphinx-doc.org/en/master/usage/configuration.html#project-information

project = 'FES'
copyright = '2024, CNES'
author = 'CNES'

# The full version, including alpha/beta/rc tags
try:
    release = importlib.metadata.version(project)
except importlib.metadata.PackageNotFoundError:
    try:
        import pyfes
        release = pyfes.__version__
        release = '.'.join(release.split('.')[:3])
    except ImportError:
        release = '0.0.0'
version = '.'.join(release.split('.')[:2])

# -- General configuration ---------------------------------------------------
# https://www.sphinx-doc.org/en/master/usage/configuration.html#general-configuration

extensions = [
    'sphinx_gallery.gen_gallery',
    'sphinx.ext.autodoc',
    'sphinx.ext.autosummary',
    'sphinx.ext.mathjax',
    'sphinx.ext.napoleon',
]

autosummary_generate = True

numpydoc_class_members_toctree = True
numpydoc_show_class_members = False

templates_path = ['_templates']

autodoc_type_aliases = {
    'VectorInt8': 'VectorInt8',
    'VectorFloat64': 'VectorFloat64',
    'VectorComplex64': 'VectorComplex64',
    'VectorComplex128': 'VectorComplex128',
    'VectorDateTime64': 'VectorDateTime64',
    'MatrixInt32': 'MatrixInt32',
    'MatrixFloat64': 'MatrixFloat64',
    'MatrixComplex128': 'MatrixComplex128',
}

sphinx_gallery_conf = {
    'examples_dirs': [HERE.parent.parent.joinpath('examples')],
    'filename_pattern': r'[\\\/]ex_',
    'pypandoc': False,
    'binder': {
        'org':
        'CNES',
        'repo':
        'aviso-fes',
        'branch':
        'main',
        'binderhub_url':
        'https://mybinder.org',
        'dependencies':
        [HERE.joinpath('..', '..', 'binder', 'environment.yml').resolve()],
        'use_jupyter_lab':
        True,
    }
}
print([HERE.joinpath('..', '..', 'binder', 'environment.yml').resolve()])
exclude_patterns = []

# -- Options for HTML output -------------------------------------------------
# https://www.sphinx-doc.org/en/master/usage/configuration.html#options-for-html-output

html_theme = 'sphinx_book_theme'
html_static_path = ['_static']
