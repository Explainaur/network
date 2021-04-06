#ifndef SERVER_SOCKET_H
#define SERVER_SOCKET_H

#include <vector>


#include "user.h"
#include "base_socket.h"

namespace network {
class ServerSocketManager : public SocketManager {
private:
  int _listen_port;
  std::vector<User> _clients;

  ServerSocketManager() {}

  static ServerSocketManager _instance;

  // Init the socket manager
  void InitManager() override;

public:
  // Get Server socket manager
  static ServerSocketManager *GetSocketManager() {
    return &_instance;
  }

  // Static initialize function
  static void Initialize() {
    _instance.InitManager();
  }

public:
  /* Methods about sockets */

  // create socket
  int create_socket(int port, const char *address = nullptr) override;

  // receive message from server
  int Recv(char *buffer, int buf_size, int socket_fd) override;

  // send message to server
  int Send(std::string &msg, int socket_fd) override;

  // process received messages
  void Process(int socket_fd);

  //
  int RegisterUser(std::string &username, int socket_fd);

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
};
}

#endif // SERVER_SOCKET_H
