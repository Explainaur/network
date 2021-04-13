#ifndef SERVER_SOCKET_H
#define SERVER_SOCKET_H

#include <map>


#include "user.h"
#include "base_socket.h"

namespace network {
class ServerSocketManager : public SocketManager {
private:
  int _listen_port{};
  std::map<int, User> _clients;

  ServerSocketManager() = default;

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
  int Process(int socket_fd);

  // register usesr
  int RegisterUser(const std::string &username, int socket_fd);

  // forward message to others
  void ForwardToOthers(int src_socket, const std::string &msg);

public:
  /* Error handlers */

  // Error handler
  void HandleError(int error_code, int socket_fd);

  // Error handler wrapper
  void HandleError(NetworkException &networkException) {
    HandleError(networkException.GetErrorCode(), networkException.GetSocketFD());
  }

  // Login failed handler
  void LoginFailedHandler();

  // Username conflicted handler
  void UsernameConflictHandler(int socket_fd);

public:
  // ProcessHandler
  void ProcessHandler(const char *buffer, int socket_fd);

  // Handle the user login
  int LoginHandler(const char *username, int socket_fd);

  // Add user to the _clients
  void AddUser(const std::string &username, int socket_fd);

  // Check if socket has been registered in the _clients
  bool UserExist(int socket_fd) { return _clients.count(socket_fd); }

  // delete broken connections
  void DeleteUser(const std::string &username);

  void DeleteUser(int socket_fd);
};
}

#endif // SERVER_SOCKET_H
