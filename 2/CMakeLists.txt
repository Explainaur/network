include_directories(include)

add_subdirectory(socket)
list(APPEND CLIENT_LIBS_T Begonia)
list(APPEND SERVER_LIBS_T BegoniaServer)
MESSAGE(STATUS "${CLIENT_LIBS_T}")

# add client
add_executable(begonia ${CLIENT_LIBS_T})

target_link_libraries(begonia
        pthread
        ${CLIENT_LIBS_T})

# add server

add_executable(begonia_server ${SERVER_LIBS_T})
target_link_libraries(begonia_server
        ${SERVER_LIBS_T})
