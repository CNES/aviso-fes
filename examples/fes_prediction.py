"""
******************
Prediction example
******************

In this example, we will use the model to predict the tidal elevation on a
global grid. The model used is an old FES tidel-atlas model. Do not use it for
real applications. You can download the model from the AVISO `website
<https://www.aviso.altimetry.fr/en/data/products/auxiliary-products/global-tide-fes.html>`_.

First, we import the required modules.
"""
# %%
from __future__ import annotations

import os
import pathlib

import cartopy.crs as ccrs
import matplotlib.pyplot as plt
import numpy as np
import pyfes

# %%
# First we create an environment variable to store the path to the model file.
os.environ['DATASET_DIR'] = str(pathlib.Path().absolute().parent / 'tests' /
                                'python' / 'dataset')

# %%
# Now we need to create the instances of the model used to calculate the ocean
# tide and the radial tide. To do this, we need to create a YAML file that
# describes the models and their parameters. The configuration file is fully
# documented in the :ref:`documentation <confguration_file>`.
handlers: dict[str, pyfes.core.AbstractTidalModelComplex128
               | pyfes.core.AbstractTidalModelComplex64]
handlers = pyfes.load_config(pathlib.Path(__file__).parent / 'fes_slev.yml')

# %%
# ``handlers`` is a dictionary that contains the handlers to the ocean and
# radial tide models.
print(handlers)

# %%
# We can now create a global grid to calculate the geocentric ocean tide.
# The grid is defined by its extent and its resolution.
lons = np.arange(-180, 180, 1)
lats = np.arange(-90, 90, 1)
lons, lats = np.meshgrid(lons, lats)
shape = lons.shape
dates = np.full(shape, 'now', dtype='datetime64[us]')

# %%
# We can now calculate the ocean tide and the radial tide.
tide, lp, _ = pyfes.evaluate_tide(handlers['tide'],
                                  dates.ravel(),
                                  lons.ravel(),
                                  lats.ravel(),
                                  num_threads=0)
load, load_lp, _ = pyfes.evaluate_tide(handlers['radial'],
                                       dates.ravel(),
                                       lons.ravel(),
                                       lats.ravel(),
                                       num_threads=0)

# %%
# We can now calculate the geocentric ocean tide (as seen by a satellite).
geo_tide = tide + lp + load
geo_tide = geo_tide.reshape(lons.shape)

# %%
# Mask the land values.
geo_tide = np.ma.masked_where(np.isnan(geo_tide), geo_tide)

# %%
# We can now plot the result.
fig = plt.figure(figsize=(22, 10))
ax = fig.add_subplot(1, 1, 1, projection=ccrs.PlateCarree())
ax.coastlines()
ax.set_global()
ax.set_title(f'Tide on {dates[0, 0]}')
ax.set_xlabel('Longitude')
ax.set_ylabel('Latitude')
mesh = ax.pcolormesh(lons,
                     lats,
                     geo_tide,
                     cmap='jet',
                     transform=ccrs.PlateCarree())
colorbar = fig.colorbar(mesh, ax=ax)
colorbar.set_label('Geocentric ocean tide (cm)', rotation=270, labelpad=20)
