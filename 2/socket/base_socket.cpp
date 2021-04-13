#include "base_socket.h"
#include <exception>
#include <iostream>

namespace network {
void SocketManager::PrintMsg(const std::string &msg) const {
  std::cout << msg << std::endl;
}
}
