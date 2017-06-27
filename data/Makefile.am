#   This file is part of FES library.

#   FES is free software: you can redistribute it and/or modify
#   it under the terms of the GNU LESSER GENERAL PUBLIC LICENSE as published by
#   the Free Software Foundation, either version 3 of the License, or
#   (at your option) any later version.

#   FES is distributed in the hope that it will be useful,
#   but WITHOUT ANY WARRANTY; without even the implied warranty of
#   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
#   GNU LESSER GENERAL PUBLIC LICENSE for more details.

#   You should have received a copy of the GNU LESSER GENERAL PUBLIC LICENSE
#   along with FES.  If not, see <http://www.gnu.org/licenses/>.
fesdatadir = ${datadir}/data/fes/2014/

fes_dir = ${datadir}/data/fes/2014
fes_autoload_dir = $(fes_dir)/autoload

eastward_velocity__DATA = fes2014/eastward_velocity/.gitkeep
eastward_velocity_dir = $(fes_dir)/eastward_velocity
load_tide__DATA = fes2014/load_tide/.gitkeep
load_tide_dir = $(fes_dir)/load_tide
northward_velocity__DATA = fes2014/northward_velocity/.gitkeep
northward_velocity_dir = $(fes_dir)/northward_velocity
ocean_tide__DATA = fes2014/ocean_tide/.gitkeep
ocean_tide_dir = $(fes_dir)/ocean_tide
ocean_tide_extrapolated__DATA = fes2014/ocean_tide_extrapolated/.gitkeep
ocean_tide_extrapolated_dir = $(fes_dir)/ocean_tide_extrapolated

fesdata_DATA = fes2014/README.rst \
    fes2014/eastward_velocity.ini \
    fes2014/load_tide.ini \
    fes2014/northward_velocity.ini \
    fes2014/ocean_tide.ini \
    fes2014/ocean_tide_extrapolated.ini

EXTRA_DIST = $(fesdata_DATA) \
    $(eastward_velocity__DATA) \
    $(load_tide__DATA) \
    $(northward_velocity__DATA) \
    $(ocean_tide__DATA) \
    $(ocean_tide_extrapolated__DATA)
