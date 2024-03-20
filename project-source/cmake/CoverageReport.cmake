# Variables COVERAGE_REPORT DOIP_TEST exported by CMakeLists.txt in root dir

# Custom target for GCC coverage report
add_custom_target(
  coverage_report
  COMMAND ${CMAKE_COMMAND} -E make_directory ${COVERAGE_REPORT}
  COMMAND lcov --capture --directory . --output-file ${COVERAGE_REPORT}/coverage.info
  COMMAND lcov --remove ${COVERAGE_REPORT}/coverage.info '/usr/*' --output-file ${COVERAGE_REPORT}/coverage.filtered.info
  COMMAND genhtml ${COVERAGE_REPORT}/coverage.filtered.info --output-directory ${COVERAGE_REPORT}/html
  WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
  COMMENT "Generating GCC coverage report in '${COVERAGE_REPORT}/html'"
  DEPENDS run_qpid_unit_tests
  DEPENDS run_pgclient_unit_tests
)
