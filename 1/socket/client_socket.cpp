#include <unistd.h>

#include <utility>
#include <iostream>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <netinet/tcp.h>

#include "client_socket.h"

namespace network {
ClientSocketManager ClientSocketManager::_instance;

int ClientSocketManager::create_socket(int port, const char *address, bool if_connect) {
  int socket_fd, on = 1;

  try {
    if ((socket_fd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
      THROW(CREATE_SOCKET_FAILED);
    }
  } catch (NetworkException &exception) {
    ERROR_RETURN(exception);
  }

  try {
    if (setsockopt(socket_fd, SOL_SOCKET, SO_REUSEADDR, &on, sizeof(on)) < 0)
      THROW(SET_SOCKET_FAILED);
  } catch (const char *msg) {
    ERROR("%s", msg);
  }


  struct sockaddr_in addr_in{};
  memset(&addr_in, 0, sizeof(addr_in));
  addr_in.sin_family = AF_INET;
  addr_in.sin_port = htons(port);

  try {
    if (inet_pton(AF_INET, address, &addr_in.sin_addr.s_addr) == 0) {
      THROW(INVALID_IPV4_PORT);
    }
  } catch (NetworkException &exception) {
    ERROR_RETURN(exception);
  }

  if (if_connect) {
    try {
      if (connect(socket_fd, (const sockaddr *) (&addr_in), sizeof(addr_in)) < 0) {
        THROW(CREATE_CHANNEL_FAILED);
      }
    } catch (NetworkException &exception) {
      ERROR_RETURN(exception);
    }
  }

  _socket_fd = socket_fd;
  return _socket_fd;
}

int ClientSocketManager::ReConnect(int port, const char *address) {
  close(_socket_fd);
  _socket_fd = create_socket(_server_info.port, _server_info.server_addr.c_str(), false);

  struct sockaddr_in addr_in{};
  memset(&addr_in, 0, sizeof(addr_in));
  addr_in.sin_family = AF_INET;
  addr_in.sin_port = htons(port);

  try {
    if (inet_pton(AF_INET, address, &addr_in.sin_addr.s_addr) == 0) {
      THROW(INVALID_IPV4_PORT);
    }
  } catch (NetworkException &exception) {
    ERROR_RETURN(exception);
  }

  try {
    if (connect(_socket_fd, (const sockaddr *) (&addr_in), sizeof(addr_in)) < 0) {
      THROW(CREATE_CHANNEL_FAILED);
    }
  } catch (NetworkException &exception) {
    ERROR_RETURN(exception);
  }
  return _socket_fd;
}

int ClientSocketManager::Recv(char *buffer, int buf_size, int socket_fd) {
  memset(buffer, 0, buf_size);
  int ret = -1;
//  fd_set read_set;
//
//  /* init the fd_set */
//  FD_ZERO(&read_set);
//
//  select(socket_fd + 1, &read_set, nullptr, nullptr, nullptr);

  try {
//    if (FD_ISSET(socket_fd, &read_set)) {
    ret = read(socket_fd, buffer, buf_size);
    if (ret < 0) {
      THROW(RECV_DATA_FAILED);
    } else if (ret == 0) {
      THROW(SERVER_TERMINATED);
    }
//    }
  } catch (NetworkException &exception) {
    ERROR_RETURN(exception);
  }

  return ret;
}

int ClientSocketManager::Send(std::string &msg, int socket_fd) {
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

  return ret;
}

int ClientSocketManager::Login() {
  char buffer[256];
  this->Send(_username, _socket_fd);
  this->Recv(buffer, 256, _socket_fd);

  try {
    if (!strcmp(buffer, LOGIN_SUCCESS)) {
      TRACE("%s", buffer);
    } else if (!strcmp(buffer, USERNAME_CONFLICT)) {
      THROW(USERNAME_CONFLICT);
    } else {
      THROW(LOGIN_FAILED);
    }
  } catch (NetworkException &exception) {
    ERROR_RETURN(exception);
  }

  return 0;
}

void ClientSocketManager::InitManager() {
  _server_info = {
      "127.0.0.1",
      SERVER_PORT
  };
}

void ClientSocketManager::SetServerInfo(ServerInfo serverInfo) {
  _server_info = std::move(serverInfo);
}

int ClientSocketManager::CreateSocket(const ServerInfo &serverInfo) {
  const char *addr = serverInfo.server_addr.c_str();
  int socket_fd = 0;
  socket_fd = this->create_socket(serverInfo.port, addr);
  return socket_fd;
}


void ClientSocketManager::HandleError(int err_code) {
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
    case SERVER_TERMINATED_CODE: {
      ServerTerminatedHandler();
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

void ClientSocketManager::LoginFailedHandler() {
  int err_code;
  do {
    std::cout << "Username: ";
    std::cin >> _username;
    err_code = this->Login();
  } while (!err_code);
}

void ClientSocketManager::UsernameConflictHandler() {
  std::cout << "Username is duplicate." << std::endl;
  this->LoginFailedHandler();
}

void ClientSocketManager::ServerTerminatedHandler() {
  _socket_fd = -1;
  while (true) {
    if (ReConnect(_server_info) < 0) {
      continue;
    } else {
      TRACE("Session reconnected");
      return;
    }
  }
}

}