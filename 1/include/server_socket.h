#ifndef SERVER_SOCKET_H
#define SERVER_SOCKET_H

#include "socket.h"
#include <vector>

namespace network {
class ServerSocketManager : public SocketManager {
private:
  std::vector<bool> ports;

  ServerSocketManager() {}

  static ServerSocketManager _instance;

  void InitManager() override;

public:
  static ServerSocketManager *GetSocketManager() {
    return &_instance;
  }

  static void Initialize() {
    _instance.InitManager();
  }

  int create_socket(int port, const char *address) override;

  int Recv(char *buffer, int buf_size, int socket_fd) override;

  int Send(std::string &msg, int socket_fd) override;
};
}

#endif // SERVER_SOCKET_H
