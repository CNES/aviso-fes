# Doxygen target
find_package(Doxygen REQUIRED)

set(DOXYGEN_OUTPUT_DIR ${CMAKE_CURRENT_BINARY_DIR})
set(DOXYGEN_INDEX_FILE ${DOXYGEN_OUTPUT_DIR}/html/index.html)
set(DOXYFILE_IN ${CMAKE_CURRENT_SOURCE_DIR}/Doxyfile.in)
set(DOXYFILE_OUT ${CMAKE_CURRENT_BINARY_DIR}/Doxyfile)

# Request to configure the file
configure_file(${DOXYFILE_IN} ${DOXYFILE_OUT} @ONLY)
message("-- Configure Doxygen documentation")

# Find all the public headers
get_target_property(PUBLIC_HEADER_DIR fes INTERFACE_INCLUDE_DIRECTORIES)
file(GLOB_RECURSE PUBLIC_HEADERS ${PUBLIC_HEADER_DIR}/*.hpp)

file(MAKE_DIRECTORY ${DOXYGEN_OUTPUT_DIR})
add_custom_command(
  OUTPUT ${DOXYGEN_INDEX_FILE}
  DEPENDS ${PUBLIC_HEADERS}
  COMMAND ${DOXYGEN_EXECUTABLE} ${DOXYFILE_OUT}
  WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
  MAIN_DEPENDENCY ${DOXYFILE_OUT}
  COMMENT "Generating docs")
add_custom_target(
  Doxygen ALL
  DEPENDS ${DOXYGEN_INDEX_FILE}
  COMMENT "Generating docs")

install(DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/html/ DESTINATION share/fes/doc)
