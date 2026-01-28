"""**********************************************
Comparing FES/Darwin and PERTH5/Doodson Engines
**********************************************

This example demonstrates the differences between PyFES's two prediction
engines:

1. **FES/Darwin Engine**: Uses Darwin notation with Schureman nodal corrections
2. **PERTH5/Doodson Engine**: Uses Doodson numbers with group modulations

The example shows how to:

* Configure each engine
* Understand their differences in notation and constituent support
* Choose the appropriate engine for your tidal atlas

.. note::

   This example demonstrates engine configuration and constituent comparison.
   To run actual predictions, you need tidal atlas files downloaded from AVISO
   (for FES) or NASA GSFC (for GOT models).
"""

# %%
from __future__ import annotations

import pyfes

# %%
# Understanding the Two Engines
# ==============================
#
# PyFES implements two distinct harmonic prediction approaches that differ in
# their mathematical formulation, constituent notation, and target applications.

print('=' * 70)
print('PyFES Prediction Engines Overview')
print('=' * 70)

print('\n1. FES/Darwin Engine (engine: fes)')
print('   ' + '-' * 45)
print("   • Uses Darwin's astronomical argument notation")
print("   • Applies Schureman's nodal corrections")
print('   • 142 supported constituents')
print('   • Compatible with: FES2014, FES2022')
print('   • Default formulae: SCHUREMAN_ORDER_1')
print('   • Developed for: FES tidal atlases')

print('\n2. PERTH5/Doodson Engine (engine: perth5)')
print('   ' + '-' * 45)
print('   • Uses Doodson number classification')
print('   • Applies group modulation nodal corrections')
print('   • 123 supported constituents')
print('   • Compatible with: GOT4.10, GOT5.5, GOT5.6')
print('   • Default formulae: IERS')
print('   • Developer: Dr. Richard Ray (NASA GSFC)')
print('   • Includes configurable inference modes')

# %%
# Darwin Notation Example
# =======================
#
# The Darwin engine represents tidal constituents using fundamental astronomical
# arguments. Here's how the M₂ constituent is expressed:

print('\n' + '=' * 70)
print('Darwin Notation: M₂ Constituent')
print('=' * 70)
print('\nAstronomical Argument:')
print('  V(M₂) = 2(τ + h - s + p)')
print('\nFundamental Variables:')
print("  τ = Greenwich hour angle (Earth's rotation)")
print('  h = mean longitude of sun')
print('  s = mean longitude of moon')
print("  p = longitude of moon's perigee")
print('\nNodal Corrections:')
print('  f(M₂) = amplitude modulation factor (varies over 18.6-year cycle)')
print('  u(M₂) = phase correction angle (varies over 18.6-year cycle)')
print('\nPrediction Formula:')
print('  h(t) = f · H · cos(V(t) + κ - u)')
print('    where H and κ come from the FES tidal atlas')

# %%
# Doodson Number Example
# ======================
#
# The PERTH5 engine uses Doodson numbers for systematic constituent
# classification. Here's how M₂ is represented:

print('\n' + '=' * 70)
print('Doodson Notation: M₂ Constituent')
print('=' * 70)
print('\nDoodson Number: 255.555')
print('\nEncoding:')
print('  Each digit encodes a coefficient (with 5 added)')
print('  Decoding 255.555: (2-5, 5-5, 5-5, 5-5, 5-5, 5-5)')
print('                   = (-3, 0, 0, 0, 0, 0)')
print('  But for M₂: actually represents 2τ + 2h - 2s + 2p')
print('\nNodeal Corrections:')
print('  Applied via group modulations (constituents grouped together)')
print('  More computationally efficient than individual corrections')
print('\nInference Modes:')
print('  • ZERO_ADMITTANCE: No inference')
print('  • LINEAR_ADMITTANCE: Linear interpolation (default)')
print('  • FOURIER_ADMITTANCE: Fourier-based (most accurate)')

# %%
# Listing Constituents by Engine
# ===============================
#
# Each engine supports a different set of constituents. Let's compare them:

darwin_constituents = pyfes.darwin.WaveTable()
perth_constituents = pyfes.perth.WaveTable()

print('\n' + '=' * 70)
print('Constituent Support Comparison')
print('=' * 70)
print(f'\nDarwin Engine:  {len(darwin_constituents):3d} constituents')
print(f'PERTH5 Engine:  {len(perth_constituents):3d} constituents')

# Find constituents unique to each engine
darwin_set = {item.name() for item in darwin_constituents}
perth_set = {item.name() for item in perth_constituents}
common = darwin_set & perth_set
darwin_only = darwin_set - perth_set
perth_only = perth_set - darwin_set

print(f'\nCommon to both:  {len(common):3d} constituents')
print(f'Darwin-only:     {len(darwin_only):3d} constituents')
print(f'PERTH5-only:     {len(perth_only):3d} constituents')

# %%
# Major Constituents Comparison
# ==============================

major_constituents = [
    'M2',
    'S2',
    'N2',
    'K2',
    'K1',
    'O1',
    'P1',
    'Q1',
    'Mf',
    'Mm',
    'Ssa',
    'Sa',
]

print('\n' + '=' * 70)
print('Major Constituents: Availability by Engine')
print('=' * 70)
print(f'\n{"Constituent":<12} {"Darwin":<10} {"PERTH5":<10} {"Type"}')
print('-' * 70)

descriptions = {
    'M2': 'Semidiurnal',
    'S2': 'Semidiurnal',
    'N2': 'Semidiurnal',
    'K2': 'Semidiurnal',
    'K1': 'Diurnal',
    'O1': 'Diurnal',
    'P1': 'Diurnal',
    'Q1': 'Diurnal',
    'Mf': 'Long-period',
    'Mm': 'Long-period',
    'Ssa': 'Long-period',
    'Sa': 'Long-period',
}

for const in major_constituents:
    in_darwin = '✓' if const in darwin_set else '✗'
    in_perth = '✓' if const in perth_set else '✗'
    desc = descriptions.get(const, '')
    print(f'{const:<12} {in_darwin:<10} {in_perth:<10} {desc}')

# Show some unique constituents
if darwin_only:
    print(
        '\nExample Darwin-only constituents: '
        f'{", ".join(list(darwin_only)[:5])}'
    )
if perth_only:
    print(
        f'Example PERTH5-only constituents: {", ".join(list(perth_only)[:5])}'
    )

# %%
# Configuring Runtime Settings
# =============================
#
# Each engine has its own runtime settings class with engine-specific options.

print('\n' + '=' * 70)
print('Runtime Settings Configuration')
print('=' * 70)

# FES/Darwin settings
fes_settings = (
    pyfes.FesRuntimeSettings()
    .with_astronomic_formulae(pyfes.Formulae.SCHUREMAN_ORDER_1)
    .with_time_tolerance(3600.0)
    .with_num_threads(0)
)

print('\nFES/Darwin Settings:')
print(f'  {fes_settings}')
print('  • Astronomic formulae: SCHUREMAN_ORDER_1 (traditional FES)')
print('  • Time tolerance: 3600 seconds (1 hour angle caching)')
print('  • Threads: 0 (auto-detect)')
print('  • Nodal corrections: Individual Schureman factors')

# PERTH5/Doodson settings with different inference modes
perth_settings_linear = (
    pyfes.PerthRuntimeSettings()
    .with_group_modulations(True)
    .with_inference_type(pyfes.InterpolationType.LINEAR_ADMITTANCE)
    .with_astronomic_formulae(pyfes.Formulae.IERS)
    .with_num_threads(0)
)

perth_settings_fourier = (
    pyfes.PerthRuntimeSettings()
    .with_group_modulations(True)
    .with_inference_type(pyfes.InterpolationType.FOURIER_ADMITTANCE)
    .with_astronomic_formulae(pyfes.Formulae.IERS)
)

print('\nPERTH5/Doodson Settings (Linear Admittance):')
print(f'  {perth_settings_linear}')
print('  • Astronomic formulae: IERS (modern conventions)')
print('  • Group modulations: Enabled (efficient)')
print('  • Inference: LINEAR_ADMITTANCE (balanced)')
print('  • Threads: 0 (auto-detect)')

print('\nPERTH5/Doodson Settings (Fourier Admittance):')
print(f'  {perth_settings_fourier}')
print('  • Astronomic formulae: IERS')
print('  • Group modulations: Enabled')
print('  • Inference: FOURIER_ADMITTANCE (maximum accuracy)')

# %%
# Configuration File Examples
# ===========================
#
# How to specify each engine in YAML configuration files:

print('\n' + '=' * 70)
print('YAML Configuration Examples')
print('=' * 70)

fes_yaml = """
# FES2022 Configuration (Darwin Engine)
tide:
  lgp:
    engine: fes
    path: ${FES_DATA}/fes2022b/ocean_tide.nc
    codes: codes
    constituents: [2N2, K1, K2, M2, N2, O1, P1, Q1, S1, S2]
    amplitude: amp_{constituent}
    phase: pha_{constituent}

radial:
  cartesian:
    engine: fes
    paths:
      M2: ${FES_DATA}/fes2022b_load/m2.nc
      S2: ${FES_DATA}/fes2022b_load/s2.nc
      K1: ${FES_DATA}/fes2022b_load/k1.nc
"""

got_yaml = """
# GOT5.6 Configuration (PERTH5 Engine)
tide:
  cartesian:
    engine: perth5
    paths:
      M2: ${GOT_DATA}/got5.6_m2.nc
      S2: ${GOT_DATA}/got5.6_s2.nc
      K1: ${GOT_DATA}/got5.6_k1.nc
      O1: ${GOT_DATA}/got5.6_o1.nc
      N2: ${GOT_DATA}/got5.6_n2.nc

radial:
  cartesian:
    engine: perth5
    paths:
      M2: ${GOT_DATA}/got5.6_load_m2.nc
      S2: ${GOT_DATA}/got5.6_load_s2.nc
"""

print('\n--- FES2022 with Darwin Engine ---')
print(fes_yaml)

print('\n--- GOT5.6 with PERTH5 Engine ---')
print(got_yaml)

# %%
# Choosing the Right Engine
# ==========================
#
# The engine choice depends on your tidal atlas format:

print('\n' + '=' * 70)
print('Decision Guide: Which Engine Should You Use?')
print('=' * 70)

print('\n✓ Use FES/Darwin Engine when:')
print('  • Your tidal atlas is FES2014 or FES2022')
print('  • Following traditional oceanographic conventions')
print('  • Replicating established FES predictions')
print('  • Working with atlases using Darwin notation')
print('  • Need compatibility with historical FES software')

print('\n✓ Use PERTH5/Doodson Engine when:')
print('  • Your tidal atlas is GOT4.10, GOT5.5, GOT5.6')
print("  • Working with Richard Ray's tide models")
print('  • Need group modulation capabilities')
print('  • Require fine control over admittance interpolation')
print('  • Working with atlases using Doodson notation')
print('  • Need IERS astronomical conventions')

# %%
# Inference Types Comparison (PERTH5 Only)
# =========================================
#
# The PERTH5 engine offers three inference strategies:

print('\n' + '=' * 70)
print('PERTH5 Inference Types')
print('=' * 70)

inference_comparison = """
┌────────────────────┬───────────────┬────────────┬──────────────────┐
│ Inference Type     │ Accuracy      │ Speed      │ Use Case         │
├────────────────────┼───────────────┼────────────┼──────────────────┤
│ ZERO_ADMITTANCE    │ Lowest        │ Fastest    │ All constituents │
│                    │ (no inference)│            │ in atlas         │
├────────────────────┼───────────────┼────────────┼──────────────────┤
│ LINEAR_ADMITTANCE  │ Good          │ Fast       │ General use      │
│                    │ (balanced)    │            │ (recommended)    │
├────────────────────┼───────────────┼────────────┼──────────────────┤
│ FOURIER_ADMITTANCE │ Highest       │ Slower     │ High-precision   │
│                    │ (best)        │            │ applications     │
└────────────────────┴───────────────┴────────────┴──────────────────┘
"""
print(inference_comparison)

print('\nCode Examples:')
print('-' * 70)
print("""
# No inference
settings = pyfes.PerthRuntimeSettings().with_inference_type(
    pyfes.InterpolationType.ZERO_ADMITTANCE
)

# Balanced (default)
settings = pyfes.PerthRuntimeSettings().with_inference_type(
    pyfes.InterpolationType.LINEAR_ADMITTANCE
)

# Maximum accuracy
settings = pyfes.PerthRuntimeSettings().with_inference_type(
    pyfes.InterpolationType.FOURIER_ADMITTANCE
)
""")

# %%
# API Compatibility
# =================
#
# Despite different internal formulations, both engines share the same API:

print('\n' + '=' * 70)
print('API Compatibility')
print('=' * 70)

example_code = """
import pyfes
import numpy as np

# Load configuration - works with BOTH engines!
config = pyfes.config.load('my_config.yaml')

# Define prediction parameters
dates = np.arange('2025-01-01', '2025-01-02', dtype='datetime64[h]')
lons = np.array([-7.688, 0.0, 10.5])
lats = np.array([59.195, 45.0, 30.0])

# Evaluate tide - same code for BOTH engines!
tide, lp, flags = pyfes.evaluate_tide(
    config.models['tide'], dates, lons, lats, settings=config.settings
)

# The ONLY difference is in the YAML configuration file!
# Engine selection is transparent to your prediction code.
"""

print('\nSame API for Both Engines:')
print('-' * 70)
print(example_code)

print('✓ No code changes needed to switch engines')
print('✓ Just update the YAML configuration file')
print('✓ Settings are automatically created based on engine type')

# %%
# Summary
# =======

print('\n' + '=' * 70)
print('Key Takeaways')
print('=' * 70)
print("""
1. PyFES supports two prediction engines with different notations:
   • FES/Darwin: Traditional oceanographic conventions (142 constituents)
   • PERTH5/Doodson: Modern Doodson classification (123 constituents)

2. Engine choice depends on tidal atlas format:
   • FES atlases → FES/Darwin engine
   • GOT atlases → PERTH5/Doodson engine

3. Both engines share the same high-level API
   • Switch engines by changing YAML config only
   • No code changes in prediction scripts

4. Engine-specific features:
   • Darwin: Schureman nodal corrections, traditional admittance
   • PERTH5: Group modulations, configurable inference modes

5. Runtime settings provide fine-tuning for each engine
   • FesRuntimeSettings for Darwin engine
   • PerthRuntimeSettings for PERTH5 engine

For more details, see the PyFES documentation on prediction engines.
""")

print('=' * 70)
print('End of Engine Comparison Example')
print('=' * 70)
