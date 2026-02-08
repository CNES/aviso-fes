"""**********************************************
Comparing FES/Darwin and PERTH/Doodson Engines
**********************************************

PyFES implements two prediction engines that share the same harmonic method
but differ in how they **name** their constituents and compute **nodal
corrections**.

Both engines solve the same prediction equation:

.. math::

    h(t) = H_0 + \\sum_k f_k H_k \\cos(\\omega_k t + V_k + u_k - G_k)

What changes between engines is how constituents are catalogued and how
the nodal factors :math:`f_k` and :math:`u_k` are obtained.

.. note::

   To run actual predictions you need tidal atlas files downloaded from AVISO
   (for FES) or NASA GSFC (for GOT models). This example only inspects the
   constituent catalogues and settings; it does not require any data files.
"""

# %%
from __future__ import annotations

from IPython.display import HTML
import markdown
import numpy as np

import pyfes


def md_to_html(md_string: str) -> HTML:
    """Convert a markdown string to HTML for display in Jupyter."""
    return HTML(markdown.markdown(md_string, extensions=['tables']))


# %%
# Constituent catalogues
# ======================
#
# Each engine defines its own catalogue of tidal constituents. Let's
# retrieve them and see how they compare.

darwin_wt = pyfes.wave_table_factory(pyfes.DARWIN)
perth_wt = pyfes.wave_table_factory(pyfes.DOODSON)

darwin_names = {w.name for w in darwin_wt}
perth_names = {w.name for w in perth_wt}
common = sorted(darwin_names & perth_names)
darwin_only = sorted(darwin_names - perth_names)
perth_only = sorted(perth_names - darwin_names)

print(f'Darwin catalogue :  {len(darwin_wt)} constituents')
print(f'Doodson catalogue:  {len(perth_wt)} constituents')
print(f'Common to both   :  {len(common)}')
print(f'Darwin-only      :  {len(darwin_only)}')
print(f'Doodson-only     :  {len(perth_only)}')

# %%
# Both catalogues cover the same major constituents. The differences
# are mostly in the higher-order compound tides.

print('\nDarwin-only constituents:')
print(', '.join(darwin_only))

print('\nDoodson-only constituents:')
print(', '.join(perth_only))

# %%
# Notation: Darwin vs Doodson
# ============================
#
# Both notations encode the same six integer coefficients that define a
# constituent's astronomical argument:
#
# .. math::
#
#     V_k = n_1 \tau + n_2 s + n_3 h + n_4 p + n_5 N' + n_6 p_1
#
# **Darwin notation** gives each constituent a traditional name (M₂, K₁, …)
# and lists the six integers explicitly.
#
# **Doodson notation** packs them into a six-digit number with an offset
# of +5 on every digit except the first:
#
# .. math::
#
#     \text{Doodson number} = n_1 \;
#     (n_2{+}5)(n_3{+}5).(n_4{+}5)(n_5{+}5)(n_6{+}5)
#
# PyFES also exposes the XDO alphabetical encoding used internally.
# Let's compare the representations for a few well-known constituents.

sample = ['M2', 'S2', 'K1', 'O1', 'Mf', 'M4']

lines = ['| Name | Speed (°/h) | Doodson | XDO |']
lines.append('| :--- | ---: | :---: | :---: |')
for name in sample:
    w = darwin_wt[name]
    lines.append(
        f'| {name} '
        f'| {w.frequency(pyfes.DEGREE_PER_HOUR):.7f} '
        f'| {w.xdo_numerical()} '
        f'| {w.xdo_alphabetical()} |'
    )

md_to_html('\n'.join(lines))

# %%
# Default settings for each engine
# ==================================
#
# The two settings classes differ only in their defaults.

fes_settings = pyfes.FESSettings()
perth_settings = pyfes.PerthSettings()

lines = [
    '| Setting | FESSettings | PerthSettings |',
    '| :--- | :---: | :---: |',
    f'| Engine type | {fes_settings.engine_type.name} '
    f'| {perth_settings.engine_type.name} |',
    f'| Inference | {fes_settings.inference_type.name} '
    f'| {perth_settings.inference_type.name} |',
    f'| Formulae | {fes_settings.astronomic_formulae.name} '
    f'| {perth_settings.astronomic_formulae.name} |',
    f'| Group modulations | {fes_settings.group_modulations} '
    f'| {perth_settings.group_modulations} |',
    f'| Long-period equilibrium | '
    f'{fes_settings.compute_long_period_equilibrium} '
    f'| {perth_settings.compute_long_period_equilibrium} |',
]

md_to_html('\n'.join(lines))

# %%
# Note that **group modulations are disabled by default** on both engines.
# They can be enabled explicitly when needed:
#
# .. code-block:: python
#
#     settings = pyfes.PerthSettings().with_group_modulations(True)
#
# Nodal corrections: individual vs group
# ========================================
#
# Every constituent has a slowly varying amplitude factor *f* and phase
# correction *u* that account for the 18.61-year lunar nodal cycle.
#
# * **Individual corrections** (both engines, default): each constituent
#   gets its own *f* and *u* computed from Schureman's formulae. This is
#   the classical approach.
#
# * **Group modulations** (optional, PERTH engine only): closely related
#   constituents sharing the same first two Doodson digits are modulated
#   together, summing over satellite frequencies within the group.
#
# Let's compute the individual nodal corrections for a sample date and
# compare the two engines.

date = np.datetime64('2024-07-01T00:00:00')

f_darwin, vu_darwin = darwin_wt.compute_nodal_modulations(
    np.array([date]),
    formulae=pyfes.Formulae.SCHUREMAN_ORDER_1,
)

f_perth, vu_perth = perth_wt.compute_nodal_modulations(
    np.array([date]),
    formulae=pyfes.Formulae.IERS,
)

# %%
# Display nodal factors for major shared constituents.

lines = ['| Constituent | f (Darwin) | f (Doodson) |']
lines.append('| :--- | ---: | ---: |')

for name in ['M2', 'S2', 'K1', 'O1', 'N2', 'K2', 'Mf', 'Mm']:
    idx_d = darwin_wt.constituents.index(name)
    idx_p = perth_wt.constituents.index(name)
    lines.append(
        f'| {name} | {f_darwin[idx_d, 0]:.6f} | {f_perth[idx_p, 0]:.6f} |'
    )

md_to_html('\n'.join(lines))

# %%
# The amplitude factors are nearly identical because both engines implement
# the same Schureman obliquity formulae; the small differences come from
# the different astronomic angle polynomials (SCHUREMAN_ORDER_1 vs IERS).
# Solar constituents like S₂ always have *f* = 1 regardless of engine.
#
# Enabling group modulations
# ===========================
#
# Group modulations are an optional feature of the PERTH engine. When
# enabled, related constituents within the same tidal group are modulated
# together instead of individually. Let's compare.

f_perth_grp, vu_perth_grp = perth_wt.compute_nodal_modulations(
    np.array([date]),
    formulae=pyfes.Formulae.IERS,
    group_modulations=True,
)

lines = ['| Constituent | f (individual) | f (group) |']
lines.append('| :--- | ---: | ---: |')
for name in ['M2', 'S2', 'K1', 'O1', 'N2', 'K2', 'Mf', 'Mm']:
    idx = perth_wt.constituents.index(name)
    lines.append(
        f'| {name} | {f_perth[idx, 0]:.6f} | {f_perth_grp[idx, 0]:.6f} |'
    )

md_to_html('\n'.join(lines))

# %%
# Inference modes
# ================
#
# Both engines support the same four inference modes for estimating minor
# constituents that are absent from the atlas. The mode is set independently
# of the engine choice.

for mode in [pyfes.ZERO, pyfes.LINEAR, pyfes.SPLINE, pyfes.FOURIER]:
    print(f'  {mode.name:8s}  -  available on both engines')

# %%
# The recommended defaults are:
#
# * **SPLINE** for FES atlases (default of ``FESSettings``)
# * **LINEAR** for GOT atlases (default of ``PerthSettings``)

# %%
# Generating constituent summary tables
# =======================================
#
# :func:`pyfes.generate_markdown_table` produces a complete summary
# of the engine settings and shows which constituents are modeled
# (provided by the atlas) versus inferred.

modeled = ['M2', 'S2', 'N2', 'K2', 'K1', 'O1', 'P1', 'Q1']

md_to_html(
    pyfes.generate_markdown_table(
        fes_settings,
        modeled_constituents=modeled,
    )
)

# %%
# The same call with the PERTH engine settings:

md_to_html(
    pyfes.generate_markdown_table(
        perth_settings,
        modeled_constituents=modeled,
    )
)

# %%
# You can also inspect the wave table directly. The table lists every
# constituent with its frequency, Doodson number and XDO encoding.

wt = pyfes.wave_table_factory(
    pyfes.DARWIN,
    ['M2', 'S2', 'N2', 'K2', 'K1', 'O1', 'P1', 'Q1', 'Mf', 'Mm'],
)
md_to_html(wt.generate_markdown_table())

# %%
# Choosing an engine
# ===================
#
# The engine is determined by the tidal atlas you use. Set it once in
# the YAML configuration file; the Python prediction code is identical.
#
# * ``engine: darwin`` → FES atlases (FES2014, FES2022)
# * ``engine: perth``  → GOT atlases (GOT4.10, GOT5.5, GOT5.6)
#
# .. code-block:: python
#
#     # Exactly the same code regardless of engine
#     config = pyfes.config.load('my_atlas.yaml')
#     tide, lp, flags = pyfes.evaluate_tide(
#         config.models['tide'], dates, lons, lats,
#         settings=config.settings,
#     )
