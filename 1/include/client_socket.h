#ifndef CLIENT_SOCKET_H
#define CLIENT_SOCKET_H

#include "base_socket.h"

#include <utility>


namespace network {

class ClientSocketManager : public SocketManager {
public:
  struct ServerInfo {
    std::string server_addr;
    int port;
  };

private:
  struct ServerInfo _server_info;
  int _socket_fd;

  ClientSocketManager() = default;

  static ClientSocketManager _instance;

  void InitManager() override;

public:
  static ClientSocketManager *GetSocketManager() {
    return &_instance;
  }

  static void Initialize() {
    _instance.InitManager();
  }

  static void Initialize(ServerInfo serverInfo) {
    _instance.SetServerInfo(std::move(serverInfo));
  }

  int create_socket(int port, const char *address, bool if_connect);

  int create_socket(int port, const char *address) override {
    create_socket(port, address, true);
  }

  int CreateSocket(const ServerInfo &serverInfo);

  int ReConnect(int port, const char *address);

  int ReConnect(ServerInfo &serverInfo) {
    return ReConnect(serverInfo.port, serverInfo.server_addr.c_str());
  }

  void HandleError(NetworkException &networkException) {
    HandleError(networkException.GetErrorCode());
  }

  void HandleError(int error_code);

  void SetServerInfo(ServerInfo serverInfo);

  void SetPort(int port) { _server_info.port = port; }

  // receive message from server
  int Recv(char *buffer, int buf_size, int socket_fd) override;

  // send message to server
  int Send(std::string &msg, int socketfd) override;

  int socketfd() const { return _socket_fd; }

  ServerInfo serverInfo() const { return _server_info; }

};

}

#endif // CLIENT_SOCKET_H
