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
add_executable(fes_slev fes_slev.c)
target_link_libraries(fes_slev fes ${NETCDF_LIBRARIES} ${MATH_LIBRARY})
target_compile_definitions(
  fes_slev PRIVATE INI="${CMAKE_SOURCE_DIR}/test/fes.ini"
                   FES_DATA="${CMAKE_SOURCE_DIR}/test/data")
