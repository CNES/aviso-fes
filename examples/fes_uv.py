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
    parser.add_argument('northward_velocity',
                        help='Path to the configuration file that contains '
                        'the definition of grids to use to compute the '
                        'northward velocity (V-component)',
                        type=argparse.FileType('r'))
    parser.add_argument('eastward_velocity',
                        help='Path to the configuration file that contains '
                        'the definition of grids to use to compute the '
                        'eastward velocity (U-component)',
                        type=argparse.FileType('r'))
    parser.add_argument('--date',
                        help='Date of calculation of the velocities tide.',
                        default=datetime.datetime.now(),
                        type=argparse.FileType('r'))
    return parser.parse_args()


def main():
    """
    Main program
    """
    args = usage()

    # Create handler
    eastward_velocity = pyfes.Handler('ocean', 'memory',
                                      args.eastward_velocity.name)
    northward_velocity = pyfes.Handler('ocean', 'memory',
                                       args.northward_velocity.name)

    # Creating a grid that will be used to interpolate the tide
    lats = np.arange(-90, 90, 1.0)
    lons = np.arange(-180, 180, 2.0)

    assert lons.shape == lats.shape
    size = lats.size

    lons, lats = np.meshgrid(lons, lats)

    dates = np.empty(lons.shape, dtype='datetime64[us]')
    dates.fill(args.date)

    # Create handler
    u_tide, lp, _ = eastward_velocity.calculate(lons.ravel(), lats.ravel(),
                                                dates.ravel())
    v_tide, _, _ = northward_velocity.calculate(lons.ravel(), lats.ravel(),
                                                dates.ravel())

    # Creating an image to see the result in meters
    u_tide = u_tide.reshape((size, size))
    u_tide = np.ma.masked_where(np.isnan(u_tide), u_tide)
    plt.figure(1)
    plt.title('eastward velocity')
    plt.pcolormesh(u_tide)

    v_tide = v_tide.reshape((size, size))
    v_tide = np.ma.masked_where(np.isnan(v_tide), v_tide)
    plt.figure(2)
    plt.title('northward velocity')
    plt.pcolormesh(v_tide)

    plt.show()


if __name__ == '__main__':
    main()
