cmake_minimum_required(VERSION 3.24.0)

set(LIB_NAME "pgclientcomyno")

# Shared library linker flags
set(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} -shared")

# Include directories
include_directories(
    ../log
    inc
    pg-common
    pg-connector/inc
)

# File collection from each subdirectory
file(GLOB_RECURSE SOURCES
    "src/*.cpp"
    "pg-connector/src/*.cpp"
)

# Create a single library from all sources
add_library(${LIB_NAME} SHARED ${SOURCES})

target_link_libraries(${LIB_NAME} pqxx)

add_subdirectory(unit-test)
