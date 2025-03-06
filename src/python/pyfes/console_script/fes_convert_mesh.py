# Copyright (c) 2025 CNES
#
# All rights reserved. Use of this source code is governed by a
# BSD-style license that can be found in the LICENSE file.
import argparse
import os

import netCDF4
import numpy

#: Map between wave names in the netCDF files and the names used by pytide
WAVE_MAP = {
    'S4': 'S4',
    'MKS2': 'MKS2',
    'R2': 'R2',
    'O1': 'O1',
    'MSqm': 'Msqm',
    'E2': 'Eps2',
    'Mm': 'Mm',
    'S2': 'S2',
    'M4': 'M4',
    'M3': 'M3',
    'Q1': 'Q1',
    'Ssa': 'Ssa',
    'P1': 'P1',
    'M8': 'M8',
    'N4': 'N4',
    'S1': 'S1',
    'K2': 'K2',
    'Mtm': 'Mtm',
    'MN4': 'MN4',
    'MSf': 'MSf',
    'MS4': 'MS4',
    'N2': 'N2',
    'J1': 'J1',
    'T2': 'T2',
    'K1': 'K1',
    'M6': 'M6',
    'Sa': 'Sa',
    'Mu2': 'Mu2',
    'Mf': 'Mf',
    'La2': 'Lambda2',
    'M2': 'M2',
    'L2': 'L2',
    '2N2': '2N2',
    'Nu2': 'Nu2'
}


def load_mesh(
    filename: str
) -> tuple[numpy.ndarray, numpy.ndarray, numpy.ndarray, numpy.ndarray]:
    """Load the mesh from a netCDF file."""
    with netCDF4.Dataset(filename) as ds:
        x = ds.variables['lon'][:]
        y = ds.variables['lat'][:]
        triangles = ds.variables['element'][:]
        lgp2 = ds.variables['C-LGP2'][:]
    return x, y, triangles, lgp2


def load_data(filename: str) -> tuple[numpy.ndarray, numpy.ndarray]:
    """Load data from a netCDF file."""
    with netCDF4.Dataset(filename) as ds:
        amp = ds.variables['a_eta_LGP2'][0, :]
        phase = ds.variables['G_eta_LGP2'][0, :]
        # Convert amplitude from meters to centimeters
        amp *= 100

    return amp, phase


def parse_args():
    """Parse the command line arguments."""
    parser = argparse.ArgumentParser()
    parser.add_argument('mesh',
                        help='The list of mesh files to convert.',
                        type=str,
                        nargs='+')
    parser.add_argument('output', help='The output file to write', type=str)
    return parser.parse_args()


def main():
    """Main function."""
    args = parse_args()
    waves = {}

    properties = None
    for path in args.mesh:
        basename = os.path.basename(path)
        wave_name = basename.split('.')[0]
        x, y, triangles, lgp2 = load_mesh(path)
        amp, phase = load_data(path)
        if properties is None:
            properties = x, y, triangles, lgp2
        else:
            assert numpy.all(x == properties[0])
            assert numpy.all(y == properties[1])
            assert numpy.all(triangles == properties[2])
            assert numpy.all(lgp2 == properties[3])
        waves[WAVE_MAP[wave_name]] = amp, phase

    assert properties is not None

    with netCDF4.Dataset(args.output, 'w') as ds:
        x, y, triangles, lgp2 = properties
        assert len(x) == len(y)
        ds.createDimension('coordinates', len(x))
        ds.createDimension('triangles', triangles.shape[0])
        ds.createDimension('three', 3)
        ds.createDimension('six', 6)
        ds.createDimension('lgp2_nodes', len(waves['O1'][0]))

        x_var = ds.createVariable('lon', 'f8', ('coordinates', ), zlib=True)
        x_var.units = 'degrees_east'
        x_var.long_name = 'longitude'
        y_var = ds.createVariable('lat', 'f8', ('coordinates', ), zlib=True)
        y_var.units = 'degrees_north'
        y_var.long_name = 'latitude'
        triangles_var = ds.createVariable('triangle',
                                          'i4', ('triangles', 'three'),
                                          zlib=True)
        triangles_var.long_name = 'element_connectivity'
        lgp2_var = ds.createVariable('lgp2',
                                     'i4', ('triangles', 'six'),
                                     zlib=True)
        lgp2_var.long_name = 'lgp2_connectivity'

        for wave_name in sorted(waves):
            var = ds.createVariable(wave_name + '_amplitude',
                                    'f4', ('lgp2_nodes', ),
                                    zlib=True)
            var.long_name = wave_name + ' amplitude'
            var.units = 'cm'
            var = ds.createVariable(wave_name + '_phase',
                                    'f4', ('lgp2_nodes', ),
                                    zlib=True)
            var.long_name = wave_name + ' phase'
            var.units = 'degrees'

        x_var[:] = x
        y_var[:] = y
        triangles_var[:] = triangles
        lgp2_var[:] = lgp2

        for wave_name, (amp, phase) in waves.items():
            ds.variables[wave_name + '_amplitude'][:] = amp
            ds.variables[wave_name + '_phase'][:] = phase


if __name__ == '__main__':
    main()
