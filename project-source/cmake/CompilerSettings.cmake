# Specify the required C++ standard and add compiler flags
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -Wall -Wextra -Werror")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} --coverage -fPIC -fprofile-arcs -ftest-coverage ")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} --coverage -fPIC -fprofile-arcs -ftest-coverage ")
set(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} --coverage -fPIC -fprofile-arcs -ftest-coverage")