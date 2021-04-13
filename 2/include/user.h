#ifndef NETWORK_USER_H
#define NETWORK_USER_H
#include "base_socket.h"

namespace network {

class User {
private:
  std::string _username;
  int _socket_fd;
public:
  User(const std::string& string, int i) {
    _socket_fd = -1;
  }

  void SetUserInfo(const std::string &username, int socket_fd) {
    _username = username;
    _socket_fd = socket_fd;
  }

  int GetSocketFd() const {
    return _socket_fd;
  }

  std::string GetUsername() const {
    return _username;
  }

  void Delete() const {
    close(_socket_fd);
  }
};
}

#endif //NETWORK_USER_H
