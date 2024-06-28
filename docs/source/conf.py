# Configuration file for the Sphinx documentation builder.
#
# For the full list of built-in configuration values, see the documentation:
# https://www.sphinx-doc.org/en/master/usage/configuration.html

# -- Project information -----------------------------------------------------
# https://www.sphinx-doc.org/en/master/usage/configuration.html#project-information

project = 'FES'
copyright = '2022, CNES'
author = 'CNES'
release = '1.0.0'

# -- General configuration ---------------------------------------------------
# https://www.sphinx-doc.org/en/master/usage/configuration.html#general-configuration

extensions = [
    'sphinx.ext.autodoc',
    'sphinx.ext.autosummary',
    'sphinx.ext.mathjax',
    'sphinx.ext.napoleon',
    'breathe',
]

autosummary_generate = True

numpydoc_class_members_toctree = True
numpydoc_show_class_members = False

templates_path = ['_templates']

breathe_default_project = 'FES'

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

exclude_patterns = []

# -- Options for HTML output -------------------------------------------------
# https://www.sphinx-doc.org/en/master/usage/configuration.html#options-for-html-output

html_theme = 'furo'
html_static_path = ['_static']
