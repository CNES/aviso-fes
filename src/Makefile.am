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
AM_CPPFLAGS = -I$(srcdir)/../lib
lib_LTLIBRARIES = libfes.la
libfes_la_SOURCES = angle.h angle.c cache.c cache.h compat.h compat.c   \
	dlist.c dlist.h error.c error.h fes.c fes_int.h grid.c grid.h ini.c \
	ini.h interp.c interp.h prediction.c prediction.h

# These linker flags specify libtool version info.
# See http://www.gnu.org/software/libtool/manual/libtool.html#Libtool-versioning
# Note:
#  version-info 9:0:7 results in soname of libnfes.so.2 -> libfes.so.2.7.0 (9 = 2 + 7)
#  version-info 9:0:2 results in soname of libnfes.so.7 -> libfes.so.7.2.0 (9 = 7 + 2)
libfes_la_LDFLAGS = -version-info 9:0:7
nodist_libfes_la_SOURCES = uthash.h
include_HEADERS = fes.h
