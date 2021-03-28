#ifndef NETWORK_USER_H
#define NETWORK_USER_H

namespace network {

class User {
private:
  std::string _username;
  int _socket_fd;
public:
  User() {
    _socket_fd = -1;
  }

  void SetUserInfo(std::string &username, int socket_fd) {
    _username = username;
    _socket_fd = socket_fd;
  }

  int GetSocketFd() const {
    return _socket_fd;
  }

  std::string GetUsername() const {
    return _username;
  }
};
}

#endif //NETWORK_USER_H
