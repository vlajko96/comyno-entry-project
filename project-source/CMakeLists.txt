cmake_minimum_required(VERSION 3.24.0)

# Define all variables with its names
set(PROJECT_NAME "comyno")
set(QPID_LIB "qpid-messaging")
set(PG_LIB "pg-client")
set(CM_TEST "test")
set(TEST_EXECUTABLE_NAME "test-cm")
set(QPID_UNIT_TEST_DIR "qpid-messaging/unit-test")
set(QPID_UNIT_TEST_NAME "test-qpid")
set(PGCLIENT_UNIT_TEST_DIR "pg-client/unit-test")
set(PGCLIENT_UNIT_TEST_NAME "test-pgclient")
set(COVERAGE_REPORT "coverage_report")

add_compile_definitions(LOGGING)
add_compile_definitions(LOG_LEVEL=2)

option(ENABLE_COVERAGE "Enable code coverage reporting" ON)

include(cmake/CompilerSettings.cmake)

if(ENABLE_COVERAGE)
# Coverage report target
  include(cmake/CoverageReport.cmake)
endif()

project(${PROJECT_NAME})

# Add subdirectories
add_subdirectory(${QPID_LIB})
add_subdirectory(${PG_LIB})
add_subdirectory(${CM_TEST})
