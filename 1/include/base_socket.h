#ifndef NET_SOCKET_H
#define NET_SOCKET_H

#include <string>
#include <sys/socket.h>
#include "config.h"
#include "network_def.h"

namespace network {

class SocketManager {
private:
  virtual void InitManager() = 0;

protected:
  SocketManager() = default;

public:
  virtual int create_socket(int port, const char *address) = 0;

  virtual int Recv(char *buffer, int buf_size, int socket_fd) = 0;

  virtual int Send(std::string &msg, int socketfd) = 0;

  virtual void PrintMsg(const std::string &) const;

};
}

#endif
