add_library(Client STATIC
        base_socket.cpp
        client_socket.cpp
        )

add_library(Server STATIC
        base_socket.cpp
        server_socket.cpp
        )