cmake_minimum_required(VERSION 3.24.0)

set (LIB_NAME "qpidmessagingcomyno")

# Shared library linker flags
set(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} -shared")

# Include directories
include_directories(
    ../log
    inc
    qpid-common
    qpid-sender/inc
    qpid-receiver/inc
)

# File collection from each subdirectory
file(GLOB_RECURSE SOURCES
    "src/*.cpp"
    "qpid-sender/src/*.cpp"
    "qpid-receiver/src/*.cpp"
)

# Create a single library from all sources
add_library(${LIB_NAME} SHARED ${SOURCES})

target_link_libraries(${LIB_NAME} qpidmessaging)
