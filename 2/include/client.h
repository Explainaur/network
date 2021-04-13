#ifndef NETWORK_CLIENT_H
#define NETWORK_CLIENT_H

#include <thread>
#include <iostream>
#include "client_socket.h"

namespace network {
class Recv {
public:
  [[noreturn]] void operator()() {
    int err_code = 0;
    char buffer[256];
    auto sock_mgr = ClientSocketManager::GetSocketManager();
    while (true) {
      err_code = sock_mgr->Recv(buffer, 256, sock_mgr->socketfd());
      sock_mgr->HandleError(err_code);

      printf("%s",buffer);
      // flush stdout instantly
      fflush(stdout);
    }
  }
};

class Send {
public:
  [[noreturn]] void operator()() {
    char buffer[256];
    auto sock_mgr = ClientSocketManager::GetSocketManager();
    std::string msg;
    while (true) {
      scanf("%s", buffer);
      msg = buffer;
      sock_mgr->Send(msg, sock_mgr->socketfd());
    }
  }
};
}

#endif //NETWORK_CLIENT_H
