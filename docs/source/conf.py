# Configuration file for the Sphinx documentation builder.
#
# For the full list of built-in configuration values, see the documentation:
# https://www.sphinx-doc.org/en/master/usage/configuration.html

import importlib.metadata
import pathlib
import sys
import sysconfig
from typing import cast

#: The directory containing this file.
HERE = pathlib.Path(__file__).absolute().parent

#: LaTeX representations of Greek letters.
GREEK_LETTERS = {
    'Alpha': '{\\alpha}',
    'Beta': '{\\beta}',
    'Gamma': '{\\gamma}',
    'Delta': '{\\delta}',
    'Psi': '{\\psi}',
    'Phi': '{\\phi}',
    'Theta': '{\\theta}',
    'Chi': '{\\chi}',
    'Pi': '{\\pi}',
    'Mu': '{\\mu}',
    'Nu': '{\\nu}',
    'Lambda': '{\\lambda}',
    'Eps': '{\\epsilon}',
    'Eta': '{\\eta}',
    'Sigma': '{\\sigma}',
    'Ups': '{\\upsilon}',
    'Rho': '{\\rho}',
    'Tau': '{\\tau}',
}


def pretty_name(name: str) -> tuple[str, str | None]:
    """Return the name and its LaTeX representation.

    Convert any Greek letter in name to its LaTeX form. If the name does
    not contain a Greek letter, return the original name and None.
    """
    for greek, latex in GREEK_LETTERS.items():
        if greek in name:
            return name, f':math:`{name.replace(greek, latex)}`'
    return name, None


def create_constituent_rst(
    table: object,
) -> str:
    """Generate an RST table for the tidal constituents."""
    table = cast(pyfes.core.WaveTableInterface, table)
    data = {}
    for component in table:
        name, greek_notation = pretty_name(component.name)
        if greek_notation:
            name = f'{name} ({greek_notation})'
        frequency = component.frequency(pyfes.core.DEGREE_PER_HOUR)
        xdo = component.xdo_alphabetical()
        data[component.name] = {
            'speed': frequency,
            'name': name,
            'xdo': f'{xdo[:1]} {xdo[1:4]} {xdo[4:]}',
        }

    # Sort by speed
    sorted_data = sorted(data.values(), key=lambda item: item['speed'])

    # Calculate column widths
    name_width = max(
        len('Name'),
        max((len(item['name']) for item in sorted_data), default=0),
    )
    speed_width = max(len('Speed (deg/h)'), 12)
    xdo_width = max(
        len('XDO'), max((len(item['xdo']) for item in sorted_data), default=0)
    )

    # Create table
    lines = []
    separator = (
        '=' * name_width + '  ' + '=' * speed_width + '  ' + '=' * xdo_width
    )
    lines.append(separator)
    lines.append(
        f'{"Name":<{name_width}}  '
        f'{"Speed (deg/h)":<{speed_width}}  '
        f'{"XDO"}'
    )
    lines.append(separator)

    for item in sorted_data:
        lines.append(
            f'{item["name"]:<{name_width}}  '
            f'{item["speed"]:<{speed_width}.7f}  '
            f'{item["xdo"]:<{xdo_width}}'
        )

    lines.append(separator)

    return '\n'.join(lines)


def generate_constituent_rst_content(
    engine_name: str,
    wave_table: object,
) -> str:
    """Generate RST content for tidal constituents."""
    title = f'{engine_name}'
    content = f"""{title}
{'=' * len(title)}

This page documents all tidal constituents supported by the {engine_name}
prediction engine.

{create_constituent_rst(wave_table)}

.. note::

    For cross-validation, the table above is compared with the `official list
    of tidal constituents
    <https://iho.int/mtg_docs/com_wg/IHOTC/IHOTC_Misc/TWCWG_Constituent_list.pdf>`_,
    which contains the same information.
"""
    return content


def write_rst_if_changed(file_path: pathlib.Path, content: str) -> bool:
    """Write RST content to file only if it has changed."""
    try:
        existing_content = file_path.read_text()
        if existing_content == content:
            return False
    except FileNotFoundError:
        pass

    file_path.write_text(content)
    return True


def fes_constituent_rst() -> None:
    """Generate and write the RST table for the FES tidal constituents."""
    rst_path = HERE / 'constituents' / 'darwin.rst'
    rst_path.parent.mkdir(parents=True, exist_ok=True)
    content = generate_constituent_rst_content(
        engine_name='DARWIN',
        wave_table=pyfes.darwin.WaveTable(),
    )
    write_rst_if_changed(rst_path, content)


def perth_constituent_rst() -> None:
    """Generate and write the RST table for the PERTH tidal constituents."""
    rst_path = HERE / 'constituents' / 'perth.rst'
    rst_path.parent.mkdir(parents=True, exist_ok=True)
    content = generate_constituent_rst_content(
        engine_name='DOODSON',
        wave_table=pyfes.perth.WaveTable(),
    )
    write_rst_if_changed(rst_path, content)


def build_dirname(extname=None):
    """Returns the name of the build directory."""
    return pathlib.Path(
        HERE.parent.parent,
        'build',
        f'lib.{sysconfig.get_platform()}-{sys.implementation.cache_tag}',
    )


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

# Generate the constituent RST files
fes_constituent_rst()
perth_constituent_rst()

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
    'MatrixComplex128': 'MatrixComplex128',
    'MatrixFloat64': 'MatrixFloat64',
    'MatrixInt32': 'MatrixInt32',
    'VectorComplex128': 'VectorComplex128',
    'VectorComplex64': 'VectorComplex64',
    'VectorDateTime64': 'VectorDateTime64',
    'VectorFloat64': 'VectorFloat64',
    'VectorInt8': 'VectorInt8',
    'VectorUInt16': 'VectorUInt16',
}
dependencies = HERE.joinpath('..', '..', 'binder', 'environment.yml').resolve()
sphinx_gallery_conf = {
    'examples_dirs': [HERE.parent.parent.joinpath('examples')],
    'filename_pattern': r'[\\\/]ex_',
    'pypandoc': False,
    'capture_repr': ('_repr_html_', '__repr__'),
    'binder': {
        'org': 'CNES',
        'repo': 'aviso-fes',
        'branch': 'main',
        'binderhub_url': 'https://mybinder.org',
        'dependencies': [dependencies],
        'use_jupyter_lab': True,
    },
}
exclude_patterns = []

# -- Options for HTML output -------------------------------------------------
# https://www.sphinx-doc.org/en/master/usage/configuration.html#options-for-html-output

html_theme = 'sphinx_book_theme'
html_static_path = ['_static']
