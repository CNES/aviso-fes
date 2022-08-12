#!/usr/bin/env python3
# This file is part of FES library.
#
#  FES is free software: you can redistribute it and/or modify
#  it under the terms of the GNU LESSER GENERAL PUBLIC LICENSE as published by
#  the Free Software Foundation, either version 3 of the License, or
#  (at your option) any later version.
#
#  FES is distributed in the hope that it will be useful,
#  but WITHOUT ANY WARRANTY; without even the implied warranty of
#  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
#  GNU LESSER GENERAL PUBLIC LICENSE for more details.
#
#  You should have received a copy of the GNU LESSER GENERAL PUBLIC LICENSE
#  along with FES.  If not, see <http://www.gnu.org/licenses/>.
"""
Example of using the FES Python interface
"""
import argparse
import datetime

import matplotlib.pyplot as plt
import numpy as np
import pyfes


def usage():
    """
    Command syntax
    """
    parser = argparse.ArgumentParser(
        description='Program example using the Python API for FES.')
    parser.add_argument('ocean',
                        help='Path to the configuration file that contains '
                        'the definition of grids to use to compute the '
                        'ocean tide',
                        type=argparse.FileType('r'))
    parser.add_argument('--load',
                        help='Path to the configuration file that contains '
                        'the definition of grids to use to compute the '
                        'load tide',
                        type=argparse.FileType('r'))
    parser.add_argument('--date',
                        help='Date of calculation of the oceanic tide.',
                        default=datetime.datetime.now(),
                        type=argparse.FileType('r'))
    return parser.parse_args()


def main():
    """
    Main program
    """
    args = usage()

    # Create handler
    short_tide = pyfes.Handler('ocean', 'memory', args.ocean.name)
    if args.load is not None:
        radial_tide = pyfes.Handler('radial', 'memory', args.load.name)
    else:
        radial_tide = None

    # Creating a grid that will be used to interpolate the tide
    lats = np.arange(-90, 90, 0.5)
    lons = np.arange(-180, 180, 0.5)

    lons, lats = np.meshgrid(lons, lats)

    shape = lons.shape

    dates = np.empty(shape, dtype='datetime64[us]')
    dates.fill(args.date)

    # Create handler
    tide, lp, _ = short_tide.calculate(lons.ravel(), lats.ravel(),
                                       dates.ravel())
    tide, lp = tide.reshape(shape), lp.reshape(shape)
    if radial_tide is not None:
        load, load_lp, _ = radial_tide.calculate(lons.ravel(), lats.ravel(),
                                                 dates.ravel())
        load, load_lp = load.reshape(shape), load_lp.reshape(shape)
    else:
        load = np.zeros(lons.shape)
        load_lp = load

    # Creating an image to see the result in meters
    geo_tide = (tide + lp + load) * 0.01
    geo_tide = geo_tide.reshape(lons.shape)
    geo_tide = np.ma.masked_where(np.isnan(geo_tide), geo_tide)
    plt.pcolormesh(geo_tide)
    plt.show()


if __name__ == '__main__':
    main()
