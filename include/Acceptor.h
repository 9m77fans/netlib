#pragma once

#include "InetAddress.h"
#include "Socket.h"

namespace solarflare {
class Acceptor {
 public:
  Acceptor(const string& Ip, unsigned short port);
  Acceptor(unsigned short port);
  Acceptor(const sockaddr_in ser);

  void ready();
  int accept();
  int fd();

 private:
  void reuseAddr(bool);
  void reusePort(bool);
  void bind();
  void listen(int);

 private:
  Socket _socket;
  InetAddress _addr;
};
}  // namespace solarflare