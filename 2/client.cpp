#include "client.h"

using namespace network;

void RunTask() {
  Recv recv;
  Send send;
  std::thread recv_thread(recv);
  std::thread send_thread(send);
  recv_thread.join();
  send_thread.join();
}

[[noreturn]] void driver(char *args[]) {
  ClientSocketManager::Initialize({args[1], 2333});
  auto sock_mgr = ClientSocketManager::GetSocketManager();
  sock_mgr->CreateSocket(sock_mgr->serverInfo());

  sock_mgr->LoginWithCommandLine();
  RunTask();

}

int main(int argc, char *args[]) {
  driver(args);
  return 0;
}