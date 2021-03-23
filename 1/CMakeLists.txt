cmake_minimum_required(VERSION 3.17)
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

include_directories(include)

set(NETWORK_SRC client.cpp)

add_subdirectory(socket)
list(APPEND CLIENT_LIBS Client)
list(APPEND SERVER_LIBS Server)

# add client
add_executable(client ${CLIENT_LIBS})

target_link_libraries(client
        PRIVATE
        ${CLIENT_LIBS})

# add server
add_executable(server ${SERVER_LIBS})

target_link_libraries(server
        PRIVATE
        ${SERVER_LIBS})
