#include <iostream>
#include "client_socket.h"

using namespace network;

void LoginWithCommandLine(ClientSocketManager *sock_mgr) {
  int err_code;
  std::string username;
  std::cout << "Username: ";
  std::cin >> username;
  sock_mgr->SetUsername(username);
  err_code = sock_mgr->Login();
  sock_mgr->HandleError(err_code);
}

void driver(char *args[]) {
  int err_code = 0;
  ClientSocketManager::Initialize({args[1], 2333});
  auto sock_mgr = ClientSocketManager::GetSocketManager();
  sock_mgr->CreateSocket(sock_mgr->serverInfo());

  LoginWithCommandLine(sock_mgr);

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