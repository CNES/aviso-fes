#!/usr/bin/env python3
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
    parser.add_argument(
        'ocean',
        help='Path to the configuration file that contains '
        'the defintion of grids to use to compute the '
        'ocean tide',
        type=argparse.FileType('r'))
    parser.add_argument(
        'load',
        help='Path to the configuration file that contains '
        'the defintion of grids to use to compute the '
        'load tide',
        type=argparse.FileType('r'))
    parser.add_argument(
        '--date',
        help='Date of calculation of the oceanic tide.',
        default=datetime.datetime(1983, 1, 1))
    return parser.parse_args()


def main():
    """
    Main program
    """
    args = usage()

    # Create handler
    short_tide = pyfes.Handler("ocean", "io", args.ocean.name)
    radial_tide = pyfes.Handler("radial", "io", args.load.name)

    # Creating the time series
    dates = np.array([
        args.date + datetime.timedelta(seconds=item * 3600)
        for item in range(24)
    ])

    lons = [59.195] * dates.size
    lons = np.array(lons)
    lats = [-7.688] * dates.size
    lats = np.array(lats)

    # Computes tides
    tide, lp, _ = short_tide.calculate(lats, lons, dates)
    load, load_lp, _ = radial_tide.calculate(lats, lons, dates)

    for idx, date in enumerate(dates):
        print(
            "%s %9.3f %9.3f %9.3f %9.3f %9.3f %9.3f %9.3f" %
            (date, lats[idx], lons[idx], tide[idx], lp[idx],
             tide[idx] + lp[idx], tide[idx] + lp[idx] + load[idx], load[idx]))


if __name__ == '__main__':
    main()
