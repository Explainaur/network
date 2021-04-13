#include <iostream>
#include "client_socket.h"

using namespace network;

[[noreturn]] [[noreturn]] void driver(char *args[]) {
  ClientSocketManager::Initialize({args[1], 2333});
  auto sock_mgr = ClientSocketManager::GetSocketManager();
  sock_mgr->CreateSocket(sock_mgr->serverInfo());

  int err_code = 0;
  std::string msg = "";
  char buffer[256];
  for (;;) {
    std::cin >> msg;
    sock_mgr->Send(msg, sock_mgr->socketfd());
    err_code = sock_mgr->Recv(buffer, 256, sock_mgr->socketfd());
    sock_mgr->HandleError(err_code);
    std::cout << buffer;
  }
}

int main(int argc, char *args[]) {
  driver(args);
  return 0;
}