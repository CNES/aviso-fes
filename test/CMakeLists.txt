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
set(ENV{FES_DATA} ${CMAKE_CURRENT_SOURCE_DIR})
get_filename_component(INI "fes.ini" ABSOLUTE)

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../src)

macro (configure_test test)
    add_executable(${test} ${test}.c)
    target_link_libraries(${test} fes ${NETCDF_LIBRARIES})
    add_test(
        NAME ${test}
        COMMAND ${test}
        WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR})
endmacro()

configure_test (t_angle)
configure_test (t_grid)
configure_test (t_interp)
configure_test (t_prediction)

configure_test (t_fes)
target_compile_definitions(t_fes PRIVATE INI="${INI}")
