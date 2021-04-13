#include <iostream>
#include "server_socket.h"
#include "network_exception.h"

#include <arpa/inet.h>

using namespace network;

int driver() {
  ServerSocketManager::Initialize();
  auto socket_mgr = ServerSocketManager::GetSocketManager();
  auto listen_fd = socket_mgr->create_socket(SERVER_PORT);

  struct sockaddr_in client_addr{};
  int new_sock_fd, pid;
  socklen_t client_size = sizeof(client_addr);

  listen(listen_fd, 5);

  //创建并初始化select需要的参数(这里仅监视read)，并把sock添加到fd_set中
  fd_set read_fds;
  fd_set read_fds_bak; //backup for read_fds(由于每次select之后会更新read_fds，因此需要backup)
  int maxfd = listen_fd, res;
  struct timeval timeout{};
  FD_ZERO(&read_fds);
  FD_ZERO(&read_fds_bak);
  FD_SET(listen_fd, &read_fds_bak);

  char client_ip_str[INET_ADDRSTRLEN];



//#if 0
  while (true) {

    //注意select之后readfds和timeout的值都会被修改，因此每次都进行重置
    read_fds = read_fds_bak;
    maxfd = UpdateMaxfd(read_fds, maxfd);        //更新maxfd
    timeout.tv_sec = SELECT_TIMEOUT;
    timeout.tv_usec = 0;
//    printf("selecting maxfd=%d\n", maxfd);

    //select(这里没有设置writefds和errorfds，如有需要可以设置)
    try {
      res = select(maxfd + 1, &read_fds, nullptr, nullptr, &timeout);
      if (res == -1) {
        THROW(SELECT_ERROR);
      }
    } catch (NetworkException &exception) {
      ERROR_RETURN(exception);
    }

    for (int i = 0; i <= maxfd; i++) {
      if (!FD_ISSET(i, &read_fds)) {
        continue;
      }

      if (i == listen_fd) {
        try {
          new_sock_fd = accept(listen_fd, (struct sockaddr *) &client_addr, &client_size);
          if (new_sock_fd < 0) {
            THROW(ACCEPT_SOCKET_FAILED);
          }
        } catch (NetworkException &exception) {
          ERROR_RETURN(exception);
        }

        if (!inet_ntop(AF_INET, &(client_addr.sin_addr), client_ip_str, sizeof(client_ip_str))) {
          perror("inet_ntop failed");
          exit(EXIT_FAILURE);
        }
        printf("accept a client from: %s\n", client_ip_str);

        SetSockNonBlock(new_sock_fd);

        if (new_sock_fd > maxfd) {
          maxfd = new_sock_fd;
        }
        FD_SET(new_sock_fd, &read_fds_bak);
      } else {
        socket_mgr->Process(i);
      }
    }


#if 0
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
#endif
  }
}

int main() {
  driver();
}