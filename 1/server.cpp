#include "server_socket.h"
#include "network_exception.h"

using namespace network;

int driver() {
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
        THROW(ACCEPT_SOCKET_FAILED);
      }
    } catch (NetworkException &exception) {
      ERROR_RETURN(exception);
    }

    try {
      if ((pid = fork()) < 0) {
        THROW(FORK_SUBPROCESS_FAILED);
      }
    } catch (NetworkException &exception) {
      ERROR_RETURN(exception);
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
//#endif

}

int main() {
  driver();
}