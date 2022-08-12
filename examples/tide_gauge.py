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
    parser.add_argument('load',
                        help='Path to the configuration file that contains '
                        'the definition of grids to use to compute the '
                        'load tide',
                        type=argparse.FileType('r'))
    parser.add_argument('--date',
                        help='Date of calculation of the oceanic tide.',
                        default=datetime.datetime(1983, 1, 1))
    return parser.parse_args()


def main():
    """
    Main program
    """
    args = usage()

    # Create handler
    short_tide = pyfes.Handler('ocean', 'io', args.ocean.name)
    radial_tide = pyfes.Handler('radial', 'io', args.load.name)

    # Creating the time series
    dates = np.array([
        args.date + datetime.timedelta(seconds=item * 3600)
        for item in range(24)
    ])

    lats = np.full(dates.shape, 59.195)
    lons = np.full(dates.shape, -7.688)

    # Computes tides
    tide, lp, _ = short_tide.calculate(lons, lats, dates)
    load, load_lp, _ = radial_tide.calculate(lons, lats, dates)

    for idx, date in enumerate(dates):
        print('%s %9.3f %9.3f %9.3f %9.3f %9.3f %9.3f %9.3f' %
              (date, lats[idx], lons[idx], tide[idx], lp[idx], tide[idx] +
               lp[idx], tide[idx] + lp[idx] + load[idx], load[idx]))


if __name__ == '__main__':
    main()
