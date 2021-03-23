//#include <cstdio>
//#include <cstdlib>
#include <unistd.h>

#include <utility>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <netinet/tcp.h>

#include "client_socket.h"

namespace network {
ClientSocketManager ClientSocketManager::_instance;

int ClientSocketManager::create_socket(int port, const char *address) {
  int socket_fd, on = 1;

  try {
    if ((socket_fd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
      throw "Create socket failed";
    }
  } catch (const char *msg) {
    ERROR("%s", msg);
  }

  try {
    if (setsockopt(socket_fd, SOL_SOCKET, SO_REUSEADDR, &on, sizeof(on)) < 0)
      throw "Set socket option failed";
  } catch (const char *msg) {
    ERROR("%s", msg);
  }


  struct sockaddr_in addr_in{};
  memset(&addr_in, 0, sizeof(addr_in));
  addr_in.sin_family = AF_INET;
  addr_in.sin_port = htons(port);

  try {
    if (inet_pton(AF_INET, address, &addr_in.sin_addr.s_addr) == 0) {
      throw "Invalid ipv4 port number";
    }
  } catch (const char *msg) {
    ERROR("%s", msg);
  }


  try {
    if (connect(socket_fd, (const sockaddr *) (&addr_in), sizeof(addr_in)) < 0) {
      throw "Create data channel failed";
    }
  } catch (const char *msg) {
    ERROR("%s", msg);
  }

  _socket_fd = socket_fd;
  return socket_fd;
}

int ClientSocketManager::Recv(char *buffer, int buf_size, int socket_fd) {
  memset(buffer, 0, buf_size);
  int ret = -1;
  try {
    ret = read(socket_fd, buffer, buf_size);
    if (ret < 0) {
      throw "Receive data failed";
    }
  } catch (const char *msg) {
    ERROR("%s", msg);
  }

  return ret;
}

int ClientSocketManager::Send(std::string &msg, int socketfd) {
  const char *msg_start = msg.c_str();
  int ret = -1;
  try {
    ret = write(socketfd, msg_start, strlen(msg_start));
    if (ret < 0) {
      throw "Send message failed";
    }
  } catch (const char *msg) {
    ERROR("%s", msg);
  }

  return ret;
}

void ClientSocketManager::InitManager() {
  _server_info = {
      "127.0.0.1",
      SERVER_PORT
  };
}

void ClientSocketManager::SetServerInfo(ServerInfo serverInfo) {
  _server_info = std::move(serverInfo);
}

int ClientSocketManager::CreateSocket(const ServerInfo &serverInfo) {
  const char *addr = serverInfo.server_addr.c_str();
  int socket_fd = create_socket(serverInfo.port, addr);
  return socket_fd;
}


}