#include <iostream>
#include <unistd.h>
#include <netinet/in.h>
#include "server_socket.h"

using namespace network;

void driver() {
  ServerSocketManager::Initialize();
  auto socket_mgr = ServerSocketManager::GetSocketManager();
  auto listen_fd = socket_mgr->create_socket(SERVER_PORT);

  struct sockaddr_in client_addr{};
  int new_sock_fd, pid;
  socklen_t client_size = sizeof(client_addr);

  listen(listen_fd, 5);

  for (;;) {
    try {
      new_sock_fd = accept(listen_fd, (struct sockaddr *) &client_addr, &client_size);
      if (new_sock_fd < 0) {
        throw "Accept socket failed";
      }
    } catch (const char *msg) {
      ERROR("%s", msg);
    }

    try {
      if ((pid = fork()) < 0) {
        throw "Fork sub process failed";
      }
    } catch (const char *msg) {
      ERROR("%s", msg);
    }

    if (pid == 0) {
      /* This is the client process */
      close(listen_fd);
      socket_mgr->Process(new_sock_fd);
      exit(0);
    } else {
      close(new_sock_fd);
    }

  }

}

int main() {
  driver();
}