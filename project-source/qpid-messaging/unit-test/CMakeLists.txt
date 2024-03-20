cmake_minimum_required(VERSION 3.24.0)

# Defined in root CMakeLists.txt
set(EXECUTABLE_NAME ${QPID_UNIT_TEST_NAME})

# Include directories
set(QPID_SRC "..")

include_directories(${QPID_SRC}/inc)
include_directories(${QPID_SRC}/qpid-common)
include_directories(${QPID_SRC}/qpid-sender/inc)
include_directories(${QPID_SRC}/qpid-receiver/inc)
include_directories(${QPID_SRC}/qpid-broker-connector/inc)
include_directories(${QPID_SRC}/../log)

# Initialize TEST_SOURCES as an empty list
set(TEST_SOURCES "")

# Append qpid source files
file(GLOB TEST_SOURCES "src/*.cpp")

file(GLOB SRC_FILES "${QPID_SRC}/src/*.cpp")
list(APPEND TEST_SOURCES ${SRC_FILES})

file(GLOB SRC_FILES "${QPID_SRC}/qpid-sender/src/*.cpp")
list(APPEND TEST_SOURCES ${SRC_FILES})

file(GLOB SRC_FILES "${QPID_SRC}/qpid-receiver/src/*.cpp")
list(APPEND TEST_SOURCES ${SRC_FILES})

file(GLOB SRC_FILES "${QPID_SRC}/qpid-broker-connector/src/*.cpp")
list(APPEND TEST_SOURCES ${SRC_FILES})

# Create the test executable
add_executable(${EXECUTABLE_NAME} ${TEST_SOURCES})

# Add compile definitions
target_compile_definitions(${EXECUTABLE_NAME} PRIVATE GTEST)

# Link Google Test and qpid
target_link_libraries(${EXECUTABLE_NAME} gtest gtest_main gmock gmock_main pthread qpidmessaging qpidtypes)

# Define a custom target for running qpid unit tests
add_custom_target(run_qpid_unit_tests
  # Specify the command to run the qpidunit tests executable
  COMMAND ${CMAKE_BINARY_DIR}/${QPID_UNIT_TEST_DIR}/${QPID_UNIT_TEST_NAME}
  # Indicate that the unit tests executable is a dependency for this target
  DEPENDS ${CMAKE_BINARY_DIR}/${QPID_UNIT_TEST_DIR}/${QPID_UNIT_TEST_NAME}
  # Set the working directory to the project's root directory
  WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
)
