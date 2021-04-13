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

  SetSockNonBlock(listen_fd);

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

int ServerSocketManager::Process(int socket_fd) {
  char *buffer = (char *) calloc(SERVER_BUFFER_SIZE, 1);

  int ret = this->Recv(buffer, SERVER_BUFFER_SIZE, socket_fd);
  HandleError(ret, socket_fd);
  ProcessHandler(buffer, socket_fd);
  return ret;
}

int ServerSocketManager::RegisterUser(const std::string &username, int socket_fd) {
  User user(std::string(), 0);
  user.SetUserInfo(username, socket_fd);
  _clients.insert(std::make_pair(socket_fd, user));
  return 0;
}


void ServerSocketManager::HandleError(int err_code, int socket_fd) {
  switch (err_code) {
    case LOGIN_FAILED_CODE: {
      LoginFailedHandler();
      break;
    }
    case RECV_DATA_FAILED_CODE: {
      TRACE0();
      this->DeleteUser(socket_fd);
      break;
    }
    case USERNAME_CONFLICT_CODE: {
      UsernameConflictHandler(socket_fd);
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

void ServerSocketManager::UsernameConflictHandler(int socket_fd) {
  std::string msg = USERNAME_CONFLICT;
  this->Send(msg, socket_fd);
}

void ServerSocketManager::ProcessHandler(const char *buffer, int socket_fd) {
  char head[HEAD_LEN];
  head[0] = buffer[0];
  head[1] = buffer[1];
  int err_code = 0;

  if (memcmp(LOGIN_CODE, head, HEAD_LEN) == 0) {
    err_code = LoginHandler(buffer + 2, socket_fd);
  } else {
    std::string msg = buffer;
    if (UserExist(socket_fd)) {
      this->ForwardToOthers(socket_fd, msg);
    }
  }

  HandleError(err_code, socket_fd);
}

int ServerSocketManager::LoginHandler(const char *username, int socket_fd) {
  std::string name = std::string(username);
  std::cout << "name: " << name << std::endl;

  try {
    this->AddUser(name, socket_fd);
  } catch (NetworkException &exception) {
    ERROR_RETURN(exception);
  }

  std::string msg = LOGIN_SUCCESS;
  this->Send(msg, socket_fd);
  return 0;
}

void ServerSocketManager::AddUser(const std::string &username, int socket_fd) {
  for (const auto &it : _clients) {
    if (it.second.GetUsername() == username) {
      THROW_WITH_SOCKET(USERNAME_CONFLICT, socket_fd);
    }
  }
  this->RegisterUser(username, socket_fd);
}

void ServerSocketManager::DeleteUser(const std::string &username) {
  for (auto it = _clients.begin(); it != _clients.end(); it++) {
    if (it->second.GetUsername() == username) {
      it->second.Delete();
      break;
    }
    _clients.erase(it);
  }
}

void ServerSocketManager::DeleteUser(int socket_fd) {
  auto res = _clients.find(socket_fd);
  if (res != _clients.end()) {
    _clients.erase(socket_fd);
  }
}

void ServerSocketManager::ForwardToOthers(int src_socket, const std::string &msg) {
  if (UserExist(src_socket)) {
    std::string username = _clients.find(src_socket)->second.GetUsername();
    std::string message = username + ": " + msg + "\n";
    for (const auto &it : _clients) {
      if (it.first == src_socket) continue;
      this->Send(message, it.first);
    }
  }
}

}