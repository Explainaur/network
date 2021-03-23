#include "client_socket.h"
#include <iostream>

using namespace network;

void driver() {
  ClientSocketManager::Initialize({"127.0.0.1", 2333});
  auto sock_mgr = ClientSocketManager::GetSocketManager();
  int socket_fd = sock_mgr->CreateSocket(sock_mgr->serverInfo());

  std::string msg = "";
  char buffer[256];
  for (;;) {
    std::cin >> msg;
    sock_mgr->Send(msg, socket_fd);
    sock_mgr->Recv(buffer, 256, socket_fd);
    std::cout <<buffer;
  }
}

int main() {
  driver();
  return 0;
}