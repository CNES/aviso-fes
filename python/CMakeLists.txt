# This file is part of FES library.
#
# FES is free software: you can redistribute it and/or modify it under the terms
# of the GNU LESSER GENERAL PUBLIC LICENSE as published by the Free Software
# Foundation, either version 3 of the License, or (at your option) any later
# version.
#
# FES is distributed in the hope that it will be useful, but WITHOUT ANY
# WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR
# A PARTICULAR PURPOSE.  See the GNU LESSER GENERAL PUBLIC LICENSE for more
# details.
#
# You should have received a copy of the GNU LESSER GENERAL PUBLIC LICENSE along
# with FES.  If not, see <http://www.gnu.org/licenses/>.
file(GLOB_RECURSE WRAPPED_SOURCES "*.cpp")
pybind11_add_module(pyfes ${WRAPPED_SOURCES})
target_link_libraries(pyfes PUBLIC fes ${NETCDF_LIBRARIES})
install(TARGETS pyfes DESTINATION ${PYTHON_SITE_DIR})
