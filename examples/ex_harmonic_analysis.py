"""
*****************
Harmonic Analysis
*****************

This example demonstrates how to perform harmonic analysis on a signal using
the :func:`pyfes.harmonic_analysis` function.

First, we import the required modules.
"""
# %%
from __future__ import annotations

import pathlib

import matplotlib.pyplot
import netCDF4
import pyfes

# %%
# The distribution contains a time series fes_tide_time_series.nc that will be
# used in this help.
SIGNAL = (pathlib.Path().absolute().parent / 'tests' / 'python' / 'dataset' /
          'fes_tide_time_series.nc')

# %%
# Load the signal.
with netCDF4.Dataset(SIGNAL, 'r') as ds:  # type: ignore
    time = (ds.variables['time'][:]).astype('datetime64[us]')
    h = ds['ocean'][:] * 1e-2  # cm to m

# %%
# Then, we will create an instance of a :py:class:`pyfes.WaveTable
# <pyfes.wave_table.WaveTable>` object:
wt = pyfes.WaveTable()

# %%
# By default, all components known by this object are loaded into memory. The
# list of components known by this object can be retrieved using the
# :py:func:`pyfes.constituents.known`
# method.
print(pyfes.constituents.known())

# %%
# If you want to restrict the analysis to only a few components, you must
# provide a list to the constructor in order to specify the waves to be
# analyzed.
wt = pyfes.WaveTable([
    'Mm', 'Mf', 'Mtm', 'Msqm', '2Q1', 'Sigma1', 'Q1', 'Rho1', 'O1', 'MP1',
    'M11', 'M12', 'M13', 'Chi1', 'Pi1', 'P1', 'S1', 'K1', 'Psi1', 'Phi1',
    'Theta1', 'J1', 'OO1', 'MNS2', 'Eps2', '2N2', 'Mu2', '2MS2', 'N2', 'Nu2',
    'M2', 'MKS2', 'Lambda2', 'L2', '2MN2', 'T2', 'S2', 'R2', 'K2', 'MSN2',
    'Eta2', '2SM2', 'MO3', '2MK3', 'M3', 'MK3', 'N4', 'MN4', 'M4', 'SN4',
    'MS4', 'MK4', 'S4', 'SK4', 'R4', '2MN6', 'M6', 'MSN6', '2MS6', '2MK6',
    '2SM6', 'MSK6', 'S6', 'M8', 'MSf', 'Ssa', 'Sa'
])

# %%
# The :py:meth:`pyfes.WaveTable.keys <pyfes.core.WaveTable.keys>` method
# allows to retrieve the list of waves defined during the construction of the
# object.
print(wt.keys())

# %%
# In order to compute the astronomical angular argument, the leap seconds must
# be known. The leap seconds can be obtained using the
# :py:func:`pyfes.get_leap_seconds` function.
leap_seconds = pyfes.get_leap_seconds(time)

# %%
# The different nodal corrections are then calculated from the time series to
# be analyzed:
f, vu = wt.compute_nodal_modulations(time, leap_seconds)

# %%
# These coefficients are used by :meth:`harmonic analysis
# <pyfes.wave_table.WaveTable.harmonic_analysis>` to determine the properties
# of the different tidal waves defined during the construction of the instance.
w = wt.harmonic_analysis(h, f, vu)

# %%
# This result can then be used to determine a tidal height for the analyzed time
# series:
h_tide = wt.tide_from_tide_series(time, leap_seconds, w)

# %%
# Finally, the result can be compared with the original time series:
matplotlib.pyplot.plot(time, h, label='Original')
matplotlib.pyplot.plot(time, h_tide, label='Tide')
matplotlib.pyplot.legend()
