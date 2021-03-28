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