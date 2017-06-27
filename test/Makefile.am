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
AM_CPPFLAGS = -I$(srcdir)/../src
LDADD = $(srcdir)/../src/libfes.la -lm -lnetcdf
TESTPROGRAMS = t_interp t_angle t_prediction t_grid t_fes

test_data = data/2N2_radial.nc data/2N2_tide.nc data/K1_radial.nc \
	data/K1_tide.nc data/K2_radial.nc data/K2_tide.nc data/M2_radial.nc \
	data/M2_tide.nc data/M4_tide.nc data/MF_tide.nc data/MM_tide.nc \
	data/MSQM_tide.nc data/MTM_tide.nc data/N2_radial.nc data/N2_tide.nc \
	data/O1_radial.nc data/O1_tide.nc data/P1_radial.nc data/P1_tide.nc \
	data/Q1_radial.nc data/Q1_tide.nc data/S1_tide.nc data/S2_radial.nc \
	data/S2_tide.nc

EXTRA_DIST = dummy.nc fes.ini $(test_data)
t_interp_SOURCES = t_interp.c test.h
t_angle_SOURCES = t_angle.c test.h
t_prediction_SOURCES = t_prediction.c test.h
t_grid_SOURCES = t_grid.c test.h
t_fes_SOURCES = t_fes.c test.h

check_PROGRAMS = $(TESTPROGRAMS)
TESTS = $(TESTPROGRAMS)

test: check
