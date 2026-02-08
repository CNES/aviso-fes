"""*************************************************
Constituent Lookup: Darwin vs. Doodson Notation
*************************************************

This example provides tools to look up and compare constituent representations
across the two prediction engines.

It demonstrates:

* How to list all constituents supported by each engine
* Which major constituents are common to both engines
* Which constituents are unique to each engine
* How to check if a specific constituent is supported
"""

# %%
from __future__ import annotations

from IPython.display import HTML
import markdown

import pyfes


def md_to_html(md_string: str) -> HTML:
    """Convert a markdown string to HTML for display in Jupyter."""
    html_string = markdown.markdown(md_string, extensions=['tables'])
    return HTML(html_string)


# %%
# Get Constituent Lists
# =====================
#
# Each engine maintains its own list of supported constituents. Use
# :py:func:`pyfes.wave_table_factory` to create wave table instances for each
# engine.

darwin_constituents = pyfes.wave_table_factory(pyfes.DARWIN)
perth_constituents = pyfes.wave_table_factory(pyfes.DOODSON)

print('=' * 70)
print('PyFES Constituent Support by Engine')
print('=' * 70)
print(f'\nDarwin Engine:  {len(darwin_constituents):3d} constituents')
print(f'PERTH5 Engine:  {len(perth_constituents):3d} constituents')

# %%
# Major Tidal Constituents Comparison
# ====================================
#
# Let's compare support for the most important tidal constituents.

major_constituents_info = [
    ('M2', 'Principal lunar semidiurnal'),
    ('S2', 'Principal solar semidiurnal'),
    ('N2', 'Larger lunar elliptic semidiurnal'),
    ('K2', 'Lunisolar semidiurnal'),
    ('K1', 'Lunisolar diurnal'),
    ('O1', 'Lunar diurnal'),
    ('P1', 'Solar diurnal'),
    ('Q1', 'Larger lunar elliptic diurnal'),
    ('2N2', 'Lunar elliptic semidiurnal second-order'),
    ('Mu2', 'Variational'),
    ('Nu2', 'Larger lunar evectional'),
    ('Lambda2', 'Smaller lunar evectional'),
    ('L2', 'Smaller lunar elliptic'),
    ('T2', 'Larger solar elliptic'),
    ('Mf', 'Lunisolar fortnightly'),
    ('Mm', 'Lunar monthly'),
    ('Ssa', 'Solar semiannual'),
    ('Sa', 'Solar annual'),
]

lines = ['| Constituent | Darwin | PERTH5 | Description |']
lines.append('| :--- | :---: | :---: | :--- |')

for const, desc in major_constituents_info:
    in_darwin = '\u2713' if const in darwin_constituents else '\u2717'
    in_perth = '\u2713' if const in perth_constituents else '\u2717'
    lines.append(f'| {const} | {in_darwin} | {in_perth} | {desc} |')

md_to_html('\n'.join(lines))

# %%
# Common, Unique, and Engine-Specific Constituents
# =================================================

darwin_set = {item.name for item in darwin_constituents}
perth_set = {item.name for item in perth_constituents}

common = darwin_set & perth_set
darwin_only = darwin_set - perth_set
perth_only = perth_set - darwin_set

print('\n' + '=' * 70)
print('Constituent Set Analysis')
print('=' * 70)
print(f'\nCommon to both engines:  {len(common):3d} constituents')
print(f'Darwin-only constituents: {len(darwin_only):3d} constituents')
print(f'PERTH5-only constituents: {len(perth_only):3d} constituents')

# %%
# Show Darwin-Only Constituents
# ==============================

if darwin_only:
    print('\n' + '=' * 70)
    print('Darwin-Only Constituents')
    print('=' * 70)
    print(
        f'\nThese {len(darwin_only)} constituents are ONLY in '
        'the Darwin engine:'
    )
    print('-' * 70)

    # Sort alphabetically for easier reading
    sorted_darwin_only = sorted(darwin_only)

    # Print in columns
    cols = 6
    for i in range(0, len(sorted_darwin_only), cols):
        row = sorted_darwin_only[i : i + cols]
        print('  ' + '  '.join(f'{c:<10}' for c in row))

# %%
# Show PERTH5-Only Constituents
# ==============================

if perth_only:
    print('\n' + '=' * 70)
    print('PERTH5-Only Constituents')
    print('=' * 70)
    print(
        f'\nThese {len(perth_only)} constituents are ONLY in the PERTH5 engine:'
    )
    print('-' * 70)

    # Sort alphabetically
    sorted_perth_only = sorted(perth_only)

    # Print in columns
    cols = 6
    for i in range(0, len(sorted_perth_only), cols):
        row = sorted_perth_only[i : i + cols]
        print('  ' + '  '.join(f'{c:<10}' for c in row))

# %%
# Constituent Lookup Function
# ============================
#
# A utility function to check constituent availability.


def check_constituent(name: str) -> None:
    """Check if a constituent is supported by each engine."""
    in_darwin = name in darwin_constituents
    in_perth = name in perth_constituents

    def status(supported: bool) -> str:
        """Return a checkmark if supported, otherwise a cross."""
        return '\u2713 Supported' if supported else '\u2717 Not supported'

    print(f'\nConstituent: {name}')
    print(f'  Darwin Engine:  {status(in_darwin)}')
    print(f'  PERTH5 Engine:  {status(in_perth)}')

    if in_darwin and in_perth:
        print('  \u2192 Available in BOTH engines')
    elif in_darwin:
        print('  \u2192 Only in Darwin engine (use FES atlases)')
    elif in_perth:
        print('  \u2192 Only in PERTH5 engine (use GOT atlases)')
    else:
        print('  \u2192 NOT available in either engine')


# %%
# Example Constituent Lookups
# ============================

print('\n' + '=' * 70)
print('Example Constituent Lookups')
print('=' * 70)

# Check some common constituents
check_constituent('M2')
check_constituent('K1')
check_constituent('Mf')

# Check some that might differ
if darwin_only:
    example_darwin = next(iter(darwin_only))
    check_constituent(example_darwin)

if perth_only:
    example_perth = next(iter(perth_only))
    check_constituent(example_perth)

# %%
# Constituent Groups by Frequency
# ================================
#
# Group constituents by their tidal frequency type.

# Common constituent name patterns for classification
long_period_patterns = ['Sa', 'Ssa', 'Mm', 'Mf', 'Mt', 'Msqm', 'Mq']
diurnal_patterns = ['1', 'J1', 'K1', 'M1', 'O1', 'OO1', 'P1', 'Q1', 'S1']
semidiurnal_patterns = ['2', 'K2', 'L2', 'M2', 'N2', 'Nu2', 'S2', 'T2', 'Mu2']
terdiurnal_patterns = ['M3', 'MK3', 'S3']
compound_patterns = ['M4', 'M6', 'M8', 'MS4', 'MN4', 'N4', 'S4']


def _match_patterns(name_upper: str, patterns: list[str]) -> bool:
    """Check if any pattern matches the name."""
    for pattern in patterns:
        if pattern.upper() in name_upper:
            return True
    return False


def classify_constituent(name: str) -> str:
    """Classify a constituent by its tidal frequency."""
    name_upper = name.upper()

    classifications = [
        (long_period_patterns, 'Long-period'),
        (diurnal_patterns, 'Diurnal'),
        (semidiurnal_patterns, 'Semidiurnal'),
        (terdiurnal_patterns, 'Terdiurnal'),
        (compound_patterns, 'Compound'),
    ]
    for patterns, tide_type in classifications:
        if _match_patterns(name_upper, patterns):
            return tide_type
    return 'Other'


# Classify all constituents
darwin_classified: dict[str, list[pyfes.WaveInterface]] = {}
perth_classified: dict[str, list[pyfes.WaveInterface]] = {}

for fes_tidal_constituent in darwin_constituents:
    tide_type = classify_constituent(fes_tidal_constituent.name)
    if tide_type not in darwin_classified:
        darwin_classified[tide_type] = []
    darwin_classified[tide_type].append(fes_tidal_constituent)

for got_tidal_constituent in perth_constituents:
    tide_type = classify_constituent(got_tidal_constituent.name)
    if tide_type not in perth_classified:
        perth_classified[tide_type] = []
    perth_classified[tide_type].append(got_tidal_constituent)

# Build markdown table for classification
all_types = sorted(
    set(list(darwin_classified.keys()) + list(perth_classified.keys()))
)

lines = ['| Type | Darwin | PERTH5 |']
lines.append('| :--- | ---: | ---: |')
for tide_type in all_types:
    darwin_count = len(darwin_classified.get(tide_type, []))
    perth_count = len(perth_classified.get(tide_type, []))
    lines.append(f'| {tide_type} | {darwin_count} | {perth_count} |')

md_to_html('\n'.join(lines))

# %%
# Summary
# =======

print('\n' + '=' * 70)
print('Summary')
print('=' * 70)
print(f"""
Constituent Support Summary:
  \u2022 Darwin engine supports {len(darwin_constituents)} constituents
  \u2022 PERTH5 engine supports {len(perth_constituents)} constituents
  \u2022 {len(common)} constituents are common to both engines
  \u2022 {len(darwin_only)} constituents are unique to Darwin
  \u2022 {len(perth_only)} constituents are unique to PERTH5

Most major tidal constituents (M2, S2, K1, O1, etc.) are available in both
engines. The differences primarily lie in minor constituents and specific
modeling choices for each engine.

When working with tidal constituents:
  \u2022 Use pyfes.wave_table_factory(pyfes.DARWIN) or
    pyfes.wave_table_factory(pyfes.DOODSON) to list each engine's
    supported constituents
  \u2022 Use pyfes.known_constituents() for a combined list of all known names
  \u2022 Match your constituent list to your tidal atlas format
  \u2022 Use Darwin engine with FES atlases
  \u2022 Use PERTH5 engine with GOT atlases
""")

print('=' * 70)
print('End of Constituent Lookup Example')
print('=' * 70)
