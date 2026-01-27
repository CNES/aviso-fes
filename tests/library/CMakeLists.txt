# Copyright (c) 2025 CNES
#
# All rights reserved. Use of this source code is governed by a BSD-style
# license that can be found in the LICENSE file.
enable_testing()

# Create a new test suite.
macro(ADD_TESTCASE testname)
  set(FILES "${CMAKE_CURRENT_SOURCE_DIR}/${testname}.cpp")
  add_executable(test_${testname} ${FILES})
  target_include_directories(test_${testname}
                             PRIVATE ${CMAKE_BINARY_DIR}/include)
  target_link_libraries(test_${testname} GTest::gtest_main ${ARGN})
  add_test(NAME test_${testname}
           COMMAND ${EXECUTABLE_OUTPUT_PATH}/test_${testname})
  if(FES_ENABLE_COVERAGE)
    add_coverage(test_${testname})
  endif()
endmacro()

add_subdirectory(angle)
add_subdirectory(detail)
add_subdirectory(geometry)
add_subdirectory(mesh)
add_subdirectory(tidal_model)

add_testcase(axis fes)
add_testcase(darwin fes)
add_testcase(delta_t fes)
add_testcase(enum_mapper fes)

add_subdirectory(darwin)
add_subdirectory(perth)
