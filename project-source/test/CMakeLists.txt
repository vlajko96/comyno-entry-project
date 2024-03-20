cmake_minimum_required(VERSION 3.24.0)

# Defined in root CMakeLists.txt
set(EXECUTABLE_NAME ${TEST_EXECUTABLE_NAME})

# Include directories
set(QPID_INCLUDES "../qpid-messaging")
set(PG_INCLUDES "../pg-client")

include_directories(${QPID_INCLUDES}/inc)
include_directories(${QPID_INCLUDES}/qpid-common)
include_directories(${QPID_INCLUDES}/qpid-sender/inc)
include_directories(${QPID_INCLUDES}/qpid-receiver/inc)
include_directories(${PG_INCLUDES}/inc)
include_directories(${PG_INCLUDES}/pg-common)
include_directories(inc)
include_directories(../log)

# Append test source files from different directories to TEST_SOURCES
file(GLOB TEST_SOURCES "src/*.cpp")

# Create the test executable
add_executable(${EXECUTABLE_NAME} ${TEST_SOURCES})

# Link Google Test
target_link_libraries(${EXECUTABLE_NAME} qpidmessagingcomyno pgclientcomyno)
