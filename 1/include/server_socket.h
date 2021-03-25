#ifndef SERVER_SOCKET_H
#define SERVER_SOCKET_H

#include <vector>
#include "base_socket.h"

namespace network {
class ServerSocketManager : public SocketManager {
private:
  int _listen_port;
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

  int create_socket(int port, const char *address = nullptr) override;

  int Recv(char *buffer, int buf_size, int socket_fd) override;

  int Send(std::string &msg, int socket_fd) override;

  void Process(int socket_fd);
};
}

#endif // SERVER_SOCKET_H
