#ifndef NETWORK_NETWORK_EXCEPTION_H
#define NETWORK_NETWORK_EXCEPTION_H

#include <exception>
#include "network_def.h"

#define LOGIN_FAILED                "User login failed"
#define LOGIN_FAILED_CODE           -108

#define RECV_DATA_FAILED            "Receive data failed"
#define RECV_DATA_FAILED_CODE       -109

#define USERNAME_CONFLICT           "User name conflict"
#define USERNAME_CONFLICT_CODE      -110

#define INVALID_IPV4_PORT           "Invalid ipv4 port number"
#define INVALID_IPV4_PORT_CODE      -111

#define SERVER_TERMINATED           "server terminated prematurely"
#define SERVER_TERMINATED_CODE      -112

#define SET_SOCKET_FAILED           "Set socket option failed"
#define SET_SOCKET_FAILED_CODE      -113

#define BIND_ADDRESS_FAILED         "Bind address failed"
#define BIND_ADDRESS_FAILED_CODE    -114

#define SEND_MESSAGE_FAILED         "Send message failed"
#define SEND_MESSAGE_FAILED_CODE    -115

#define CREATE_SOCKET_FAILED        "Create socket failed"
#define CREATE_SOCKET_FAILED_CODE   -116

#define ACCEPT_SOCKET_FAILED        "Accept socket failed"
#define ACCEPT_SOCKET_FAILED_CODE   -117

#define CREATE_CHANNEL_FAILED       "Create data channel failed"
#define CREATE_CHANNEL_FAILED_CODE  -118

#define FORK_SUBPROCESS_FAILED      "Fork sub process failed"
#define FORK_SUBPROCESS_FAILED_CODE -119


namespace network {
class NetworkException : public std::exception {
  int _err_code;
  std::string _err_message;
public:
  NetworkException(const char *err_message, int err_code) :
      _err_message(err_message), _err_code(err_code) {}

  const char *GetErrorMessage() const {
    return _err_message.c_str();
  }

  int GetErrorCode() const {
    return _err_code;
  }

  const char *what() const noexcept override {
    return _err_message.c_str();
  }
};

}

#define THROW(MESSAGE) \
  throw NetworkException(MESSAGE, MESSAGE##_CODE);

#define ERROR_RETURN(exception) \
    TRACE("Code:%d:%s\n", exception.GetErrorCode(), exception.GetErrorMessage()); \
    return exception.GetErrorCode();

#endif //NETWORK_NETWORK_EXCEPTION_H
