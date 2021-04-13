#ifndef NET_SOCKET_H
#define NET_SOCKET_H

#include <string>
#include <fcntl.h>
#include <cstring>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/tcp.h>

#include "config.h"
#include "network_def.h"
#include "network_exception.h"

namespace network {

static int UpdateMaxfd(fd_set fds, int maxfd) {
  int new_maxfd = 0;
  for (int i = 0; i <= maxfd; i++) {
    if (FD_ISSET(i, &fds) && i > new_maxfd) {
      new_maxfd = i;
    }
  }
  return new_maxfd;
}

static int SetSockNonBlock(int socket_fd) {
  int flags;
  try {
    flags = fcntl(socket_fd, F_GETFL, 0);
    if (flags < 0) {
      THROW_WITH_SOCKET(F_GETFL_ERROR, socket_fd);
    }
    if (fcntl(socket_fd, F_SETFL, flags | O_NONBLOCK) < 0) {
      THROW_WITH_SOCKET(F_SETFL_ERROR, socket_fd);
    }
  } catch (NetworkException &exception) {
    ERROR_RETURN(exception);
  }
  return 0;
}

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
