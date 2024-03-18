cmake_minimum_required(VERSION 3.24.0)

# Define all variables with its names
set (PROJECT_NAME "comyno")
set (QPID_LIB "qpid-messaging")
set (CM_TEST "test")
set (TEST_EXECUTABLE_NAME "test-cm")

include(cmake/CompilerSettings.cmake)

project(${PROJECT_NAME})

# Add subdirectories
add_subdirectory(${QPID_LIB})
add_subdirectory(${CM_TEST})