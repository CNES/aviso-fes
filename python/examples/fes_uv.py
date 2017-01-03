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
    parser.add_argument('northward_velocity',
                        help='Path to the configuration file that contains '
                             'the defintion of grids to use to compute the '
                             'northward velocity (V-component)',
                        type=argparse.FileType('r'))
    parser.add_argument('eastward_velocity',
                        help='Path to the configuration file that contains '
                             'the defintion of grids to use to compute the '
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
    eastward_velocity = fes.Handler(
        "ocean", "memory", args.eastward_velocity.name)
    northward_velocity = fes.Handler(
        "ocean", "memory", args.northward_velocity.name)

    # Creating a grid that will be used to interpolate the tide
    lats = np.arange(-90, 90, 1.0)
    lons = np.arange(-180, 180, 2.0)

    assert lons.shape == lats.shape
    size = lats.size

    lons, lats = np.meshgrid(lons, lats)

    dates = np.empty(lons.shape, dtype='datetime64[us]')
    dates.fill(args.date)

    # Create handler
    u_tide, lp = eastward_velocity.vector(
        lats.ravel(), lons.ravel(), dates.ravel())
    v_tide, _ = northward_velocity.vector(
        lats.ravel(), lons.ravel(), dates.ravel())

    # Creating an image to see the result in meters
    u_tide = u_tide.reshape((size, size))
    u_tide = np.ma.masked_where(np.isnan(u_tide), u_tide)
    plt.figure(1)
    plt.title("eastward velocity")
    plt.pcolormesh(u_tide)

    v_tide = v_tide.reshape((size, size))
    v_tide = np.ma.masked_where(np.isnan(v_tide), v_tide)
    plt.figure(2)
    plt.title("northward velocity")
    plt.pcolormesh(v_tide)

    plt.show()


if __name__ == '__main__':
    main()
