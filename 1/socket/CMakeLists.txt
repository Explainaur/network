add_library(Client STATIC
        socket.cpp
        client_socket.cpp
        )

add_library(Server STATIC
        socket.cpp
        server_socket.cpp
        )