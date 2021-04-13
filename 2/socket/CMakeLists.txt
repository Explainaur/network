add_library(Begonia
        STATIC
        ./base_socket.cpp
        ./client_socket.cpp
        )

add_library(BegoniaServer
        STATIC
        ./base_socket.cpp
        ./server_socket.cpp
        )