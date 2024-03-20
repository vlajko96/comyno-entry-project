cmake_minimum_required(VERSION 3.24.0)

# Defined in root CMakeLists.txt
set(EXECUTABLE_NAME ${PGCLIENT_UNIT_TEST_NAME})

# Include directories
set(PGCLIENT_SRC "..")

include_directories(${PGCLIENT_SRC}/inc)
include_directories(${PGCLIENT_SRC}/pg-common)
include_directories(${PGCLIENT_SRC}/pg-connector/inc)
include_directories(${PGCLIENT_SRC}/../log)

# Initialize TEST_SOURCES as an empty list
set(TEST_SOURCES "")

# Append pgclient source files
file(GLOB TEST_SOURCES "src/*.cpp")

file(GLOB SRC_FILES "${PGCLIENT_SRC}/src/*.cpp")
list(APPEND TEST_SOURCES ${SRC_FILES})

file(GLOB SRC_FILES "${PGCLIENT_SRC}/pg-connector/src/*.cpp")
list(APPEND TEST_SOURCES ${SRC_FILES})

# Create the test executable
add_executable(${EXECUTABLE_NAME} ${TEST_SOURCES})

# Add compile definitions
target_compile_definitions(${EXECUTABLE_NAME} PRIVATE GTEST)

# Link Google Test and pqxx
target_link_libraries(${EXECUTABLE_NAME} gtest gtest_main gmock gmock_main pthread pqxx)

# Define a custom target for running qpid unit tests
add_custom_target(run_pgclient_unit_tests
  # Specify the command to run the qpidunit tests executable
  COMMAND ${CMAKE_BINARY_DIR}/${PGCLIENT_UNIT_TEST_DIR}/${PGCLIENT_UNIT_TEST_NAME}
  # Indicate that the unit tests executable is a dependency for this target
  DEPENDS ${CMAKE_BINARY_DIR}/${PGCLIENT_UNIT_TEST_DIR}/${PGCLIENT_UNIT_TEST_NAME}
  # Set the working directory to the project's root directory
  WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
)
