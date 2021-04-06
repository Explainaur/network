#include <unistd.h>
#include <iostream>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <netinet/tcp.h>

#include "server_socket.h"

namespace network {
ServerSocketManager ServerSocketManager::_instance;

int ServerSocketManager::create_socket(int port, const char *address) {
  int listen_fd, on = 1;

  try {
    if ((listen_fd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
      THROW(CREATE_SOCKET_FAILED);
    }
  } catch (NetworkException &exception) {
    ERROR_RETURN(exception);
  }

  try {
    if (setsockopt(listen_fd, SOL_SOCKET, SO_REUSEADDR, &on, sizeof(on)) < 0)
      THROW(SET_SOCKET_FAILED);
  } catch (NetworkException &exception) {
    ERROR_RETURN(exception);
  }

  struct sockaddr_in serv_addr{};
  memset(&serv_addr, 0, sizeof(serv_addr));
  serv_addr.sin_family = AF_INET;
  serv_addr.sin_addr.s_addr = INADDR_ANY;
  serv_addr.sin_port = htons(port);

  try {
    if (bind(listen_fd, (const sockaddr *) &serv_addr, sizeof(serv_addr)) < 0) {
      THROW(BIND_ADDRESS_FAILED);
    }
  } catch (NetworkException &exception) {
    ERROR_RETURN(exception);
  }

  return listen_fd;
}

void ServerSocketManager::InitManager() {
  _listen_port = SERVER_PORT;
}

int ServerSocketManager::Recv(char *buffer, int buf_size, int socket_fd) {
  memset(buffer, 0, buf_size);
  int ret = -1;
  try {
    if ((ret = read(socket_fd, buffer, buf_size)) < 0) {
      THROW(RECV_DATA_FAILED);
    }
  } catch (NetworkException &exception) {
    ERROR_RETURN(exception);
  }
  return ret;
}

int ServerSocketManager::Send(std::string &msg, int socket_fd) {
  const char *msg_start = msg.c_str();
  int ret = -1;
  try {
    ret = write(socket_fd, msg_start, strlen(msg_start));
    if (ret < 0) {
      THROW(SEND_MESSAGE_FAILED);
    }
  } catch (NetworkException &exception) {
    ERROR_RETURN(exception);
  }
  return 0;
}

void ServerSocketManager::Process(int socket_fd) {
  TRACE("%s\n", "Processing");
  char *buffer = (char *) calloc(SERVER_BUFFER_SIZE, 1);
  int ret = 0;
  std::string msg = "Ketchup!\n";
  while (true) {
    ret = this->Recv(buffer, SERVER_BUFFER_SIZE, socket_fd);
    if (ret < 0) break;

    std::cout << buffer << std::endl;

    ret = this->Send(msg, socket_fd);
    if (ret < 0) break;
  }
}

int ServerSocketManager::RegisterUser(std::string &username, int socket_fd) {
  User user;
  user.SetUserInfo(username, socket_fd);
  _clients.push_back(user);
}


void ServerSocketManager::HandleError(int err_code) {
  switch (err_code) {
    case LOGIN_FAILED_CODE: {
      LoginFailedHandler();
      break;
    }
    case RECV_DATA_FAILED_CODE: {
      break;
    }
    case USERNAME_CONFLICT_CODE: {
      UsernameConflictHandler();
    }
    case INVALID_IPV4_PORT_CODE: {
      break;
    }
    case SET_SOCKET_FAILED_CODE: {
      break;
    }
    case BIND_ADDRESS_FAILED_CODE: {
      break;
    }
    case SEND_MESSAGE_FAILED_CODE: {
      break;
    }
    case CREATE_SOCKET_FAILED_CODE: {
      break;
    }
    case ACCEPT_SOCKET_FAILED_CODE: {
      break;
    }
    case CREATE_CHANNEL_FAILED_CODE: {
      break;
    }
    case FORK_SUBPROCESS_FAILED_CODE: {
      break;
    }
    default: {
      break;
    }
  }
}

void ServerSocketManager::LoginFailedHandler() {

}

void ServerSocketManager::UsernameConflictHandler() {

}

}