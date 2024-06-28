find_package(Doxygen REQUIRED)
find_package(Sphinx REQUIRED)

set(DOXYGEN_OUTPUT_DIR ${CMAKE_CURRENT_BINARY_DIR}/doxygen)
set(DOXYGEN_INDEX_FILE ${DOXYGEN_OUTPUT_DIR}/xml/index.xml)
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

set(SPHINX_SOURCE ${CMAKE_CURRENT_SOURCE_DIR}/source)
set(SPHINX_BUILD ${CMAKE_CURRENT_BINARY_DIR}/build)
set(SPHINX_INDEX_FILE ${SPHINX_BUILD}/index.html)

# Only regenerate Sphinx when: - Doxygen has rerun - Our doc files have been
# updated - The Sphinx config has been updated
file(GLOB_RECURSE RST_SOURCES "${SPHINX_SOURCE}/*.rst")

add_custom_command(
  OUTPUT ${SPHINX_INDEX_FILE}
  COMMAND
    ${SPHINX_EXECUTABLE} -b html
    # Tell Breathe where to find the Doxygen output
    -Dbreathe_projects.FES=${DOXYGEN_OUTPUT_DIR}/xml ${SPHINX_SOURCE}
    ${SPHINX_BUILD}
  WORKING_DIRECTORY ${SPHINX_SOURCE}
  DEPENDS ${RST_SOURCES} ${DOXYGEN_INDEX_FILE}
  MAIN_DEPENDENCY ${SPHINX_SOURCE}/conf.py
  COMMENT "Generating documentation with Sphinx")

# Nice named target so we can run the job easily
add_custom_target(
  Sphinx ALL
  DEPENDS ${SPHINX_INDEX_FILE}
  COMMENT "Generating documentation with Sphinx")

# Add an install target to install the docs
include(GNUInstallDirs)
install(DIRECTORY ${SPHINX_BUILD} DESTINATION ${CMAKE_INSTALL_DOCDIR})
