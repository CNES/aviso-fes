"""******************
Prediction example
******************.

In this example, we will use the model to predict the tidal elevation on a
specific location, like a tide gauge.

.. warning::

      The model employed is an older FES tidal-atlas model due to its
      significantly smaller size compared to newer models. **Do not use it for
      real applications**. You can download the model from the `AVISO website
      <https://www.aviso.altimetry.fr/en/data/products/auxiliary-products/global-tide-fes.html>`_.

First, we import the required modules.
"""

# %%
from __future__ import annotations

import os
import pathlib

import numpy
import pyfes

# %%
# First we create an environment variable to store the path to the model file.
os.environ['DATASET_DIR'] = str(
    pathlib.Path().absolute().parent / 'tests' / 'python' / 'dataset'
)

# %%
# Now we need to create the instances of the model used to calculate the ocean
# tide and the radial tide. To do this, we need to create a YAML file that
# describes the models and their parameters. The configuration file is fully
# documented in the :ref:`documentation <configuration_file>`.
#
# .. note::
#
#     The content of the configuration file is viewable in the
#     `GitHub repository
#     <https://github.com/CNES/aviso-fes/blob/main/examples/fes_slev.yml>`_.
#
handlers = pyfes.load_config(pathlib.Path().absolute() / 'fes_slev.yml')

# %%
# ``handlers`` is a dictionary that contains the handlers to the ocean and
# radial tide models.
print(handlers)

# %%
# .. hint::
#
#     By default, the function :func:`pyfes.load_config` loads the entire
#     numeric grid into memory. To predict the tide for a specific region, you
#     can use the ``bbox`` keyword argument to specify the region's bounding
#     box. This bounding box is a tuple of four elements: minimum longitude,
#     minimum latitude, maximum longitude, and maximum latitude. Example:
#
#     .. code-block:: python
#
#         handlers = pyfes.load_config('fes_slev.yaml', bbox=(-10, 40, 10, 60))
#
# Setup the longitude and latitude of the location where we want to calculate
# the tide.
lon = -7.688
lat = 59.195
date = numpy.datetime64('1983-01-01T00:00:00')

# %%
# Generate the coordinates where we want to calculate the tide.
dates = numpy.arange(
    date, date + numpy.timedelta64(1, 'D'), numpy.timedelta64(1, 'h')
)
lons = numpy.full(dates.shape, lon)
lats = numpy.full(dates.shape, lat)

# %%
# We can now calculate the ocean tide and the radial tide.
tide, lp, _ = pyfes.evaluate_tide(
    handlers['tide'], dates, lons, lats, num_threads=1
)
load, load_lp, _ = pyfes.evaluate_tide(
    handlers['radial'], dates, lons, lats, num_threads=1
)

# %%
# Print the results
cnes_julian_days = (dates - numpy.datetime64('1950-01-01T00:00:00')).astype(
    'M8[s]'
).astype(float) / 86400
hours = cnes_julian_days % 1 * 24
print(
    f'{"JulDay":>6s} {"Hour":>5s} {"Latitude":>10s} {"Longitude":>10s} '
    f'{"Short_tide":>10s} {"LP_tide":>10s} {"Pure_Tide":>10s} '
    f'{"Geo_Tide":>10s} {"Rad_Tide":>10s}'
)
print('=' * 89)
for ix, jd in enumerate(cnes_julian_days):
    print(
        f'{jd:>6.0f} {hours[ix]:>5.0f} {lats[ix]:>10.3f} {lons[ix]:>10.3f} '
        f'{tide[ix]:>10.3f} {lp[ix]:>10.3f} {tide[ix] + lp[ix]:>10.3f} '
        f'{tide[ix] + lp[ix] + load[ix]:>10.3f} {load[ix]:>10.3f}'
    )
