#!/usr/bin/env python3
"""
Example of using the FES Python interface
"""
import argparse
import datetime
import numpy as np
import matplotlib.pyplot as plt
import fes


def usage():
    """
    Command syntax
    """
    parser = argparse.ArgumentParser(
        description='Program example using the Python API for FES.')
    parser.add_argument('ocean',
                        help='Path to the configuration file that contains '
                             'the defintion of grids to use to compute the '
                             'ocean tide',
                        type=argparse.FileType('r'))
    parser.add_argument('--load',
                        help='Path to the configuration file that contains '
                             'the defintion of grids to use to compute the '
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
    short_tide = fes.Handler("ocean", "memory", args.ocean.name)
    if args.load is not None:
        radial_tide = fes.Handler("radial", "memory", args.load.name)
    else:
        radial_tide = None

    # Creating a grid that will be used to interpolate the tide
    lats = np.arange(-90, 90, 1.0)
    lons = np.arange(-180, 180, 2.0)

    assert lons.shape == lats.shape
    size = lats.size

    lons, lats = np.meshgrid(lons, lats)

    dates = np.empty(lons.shape, dtype='datetime64[us]')
    dates.fill(args.date)

    # Create handler
    tide, lp = short_tide.vector(
        lats.ravel(), lons.ravel(), dates.ravel())
    if radial_tide is not None:
        load, load_lp = radial_tide.vector(
            lats.ravel(), lons.ravel(), dates.ravel())
    else:
        load = np.zeros((size * size))
        load_lp = load

    # Creating an image to see the result in meters
    geo_tide = (tide + lp + load) * 0.01
    geo_tide = geo_tide.reshape((size, size))
    geo_tide = np.ma.masked_where(np.isnan(geo_tide), geo_tide)
    plt.pcolormesh(geo_tide)
    plt.show()


if __name__ == '__main__':
    main()
