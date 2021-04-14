cmake_minimum_required(VERSION 3.16)
project(network)

set(CMAKE_CXX_STANDARD 14)

if (NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Debug)
endif ()

if (CMAKE_BUILD_TYPE STREQUAL "Debug")
    add_definitions(-DDEBUG -DIs_True_On)
    set(OPT_FLAGS "-O0")
else ()
    add_definitions(-DNDEBUG)
    set(OPT_FLAGS "-O2")
endif ()

add_subdirectory(1)
#add_subdirectory(2)
#add_subdirectory(3)
