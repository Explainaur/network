#include <arpa/inet.h>
#include <netinet/in.h>
#include <netinet/tcp.h>
#include "server_socket.h"

namespace network {
ServerSocketManager ServerSocketManager::_instance;

int ServerSocketManager::create_socket(int port, const char *address)  {
  int listenfd, connfd;

}

}