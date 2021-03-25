#include <unistd.h>
#include <iostream>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <netinet/tcp.h>

#include "server_socket.h"

namespace network {
ServerSocketManager ServerSocketManager::_instance;

int ServerSocketManager::create_socket(int port, const char *address) {
  int listen_fd, on = 1;

  try {
    if ((listen_fd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
      throw "Create socket failed";
    }
  } catch (const char *msg) {
    ERROR("%s", msg);
  }

  try {
    if (setsockopt(listen_fd, SOL_SOCKET, SO_REUSEADDR, &on, sizeof(on)) < 0)
      throw "Set socket option failed";
  } catch (const char *msg) {
    ERROR("%s", msg);
  }

  struct sockaddr_in serv_addr{};
  memset(&serv_addr, 0, sizeof(serv_addr));
  serv_addr.sin_family = AF_INET;
  serv_addr.sin_addr.s_addr = INADDR_ANY;
  serv_addr.sin_port = htons(port);

  try {
    if (bind(listen_fd, (const sockaddr *) &serv_addr, sizeof(serv_addr)) < 0) {
      throw "Bind address failed";
    }
  } catch (const char *msg) {
    ERROR("%s", msg);
  }

  return listen_fd;
}

void ServerSocketManager::InitManager() {
  _listen_port = SERVER_PORT;
}

int ServerSocketManager::Recv(char *buffer, int buf_size, int socket_fd) {
  memset(buffer, 0, buf_size);
  int ret = -1;
  try {
    if ((ret = read(socket_fd, buffer, buf_size)) < 0) {
      throw "Receive data failed";
    }
  } catch (const char *msg) {
    ERROR("%s", msg);
  }
  return ret;
}

int ServerSocketManager::Send(std::string &msg, int socket_fd) {
  const char *msg_start = msg.c_str();
  int ret = -1;
  try {
    ret = write(socket_fd, msg_start, strlen(msg_start));
    if (ret < 0) {
      throw "Send message failed";
    }
  } catch (const char *msg) {
    ERROR("%s", msg);
  }
  return 0;
}

void ServerSocketManager::Process(int socket_fd) {
  TRACE("%s\n", "Processing");
  char *buffer = (char *) calloc(SERVER_BUFFER_SIZE, 1);
  int ret = 0;
  std::string msg = "Ketchup!\n";
  while (true) {
    ret = this->Recv(buffer, SERVER_BUFFER_SIZE, socket_fd);
    if (ret < 0) break;

    std::cout << buffer << std::endl;

    ret = this->Send(msg, socket_fd);
    if (ret < 0) break;
  }
}


}