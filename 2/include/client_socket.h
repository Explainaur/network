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
  int _socket_fd;
  std::string _username;
  struct ServerInfo _server_info;

  ClientSocketManager() = default;

  static ClientSocketManager _instance;

  // Init the socket manager
  void InitManager() override;

public:
  // Get Client socket manager
  static ClientSocketManager *GetSocketManager() {
    return &_instance;
  }

  // Static initialize function
  static void Initialize() {
    _instance.InitManager();
  }

  static void Initialize(ServerInfo serverInfo) {
    _instance.SetServerInfo(std::move(serverInfo));
  }

public:
  /* Methods about sockets */

  // create socket
  int create_socket(int port, const char *address, bool if_connect);

  // create socket and connect to server
  int create_socket(int port, const char *address) override {
    create_socket(port, address, true);
  }

  // create socket wrapper
  int CreateSocket(const ServerInfo &serverInfo);

  // reconnect to server
  int ReConnect(int port, const char *address);

  // reconnect wrapper
  int ReConnect(ServerInfo &serverInfo) {
    return ReConnect(serverInfo.port, serverInfo.server_addr.c_str());
  }

  // receive message from server
  int Recv(char *buffer, int buf_size, int socket_fd) override;

  // send message to server
  int Send(std::string &msg, int socket_fd) override;

  // user login
  int Login();


public:
  /* Setter and getter */

  // Set server information
  void SetServerInfo(ServerInfo serverInfo);

  // Set server port
  void SetPort(int port) { _server_info.port = port; }

  // Set user name
  void SetUsername(std::string &username) { _username = username; }

  // Get user name
  const std::string &username() const { return _username; }

  // Get socket fd
  int socketfd() const { return _socket_fd; }

  // Get server information
  ServerInfo serverInfo() const { return _server_info; }

  void LoginWithCommandLine();

public:
  /* Error handlers */

  // Error handler
  void HandleError(int error_code);

  // Error handler wrapper
  void HandleError(NetworkException &networkException) {
    HandleError(networkException.GetErrorCode());
  }

  // Login failed handler
  void LoginFailedHandler();

  // Username conflicted handler
  void UsernameConflictHandler();

  // Server terminated handler
  void ServerTerminatedHandler();

};

}

#endif // CLIENT_SOCKET_H
