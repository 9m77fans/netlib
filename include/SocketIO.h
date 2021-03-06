#pragma once
#include "Socket.h"

namespace solarflare {
class SocketIO {
 public:
  SocketIO(int sockfd);
  ~SocketIO();
  int readn(void* buf, int len);
  int writen(const char* buf, int len);
  int readline(void* buf, int len);

 private:
  int recvPeek(char* buf, int len);
  Socket _socket;
};
}  // namespace solarflare