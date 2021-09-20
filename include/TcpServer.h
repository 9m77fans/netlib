#pragma once
#include <string>
#include "Acceptor.h"
#include "EventLoop.h"
using std::string;

namespace solarflare {
class TcpServer {
 public:
  TcpServer(const string &Ip, unsigned short port);
  void start();
  void setConnectionCallBack(const TcpConnectionCallBack &);
  void setMessageCallBack(const TcpConnectionCallBack &);
  void setCloseCallBack(const TcpConnectionCallBack &);

 private:
  Acceptor _acceptor;
  EventLoop _eventLoop;
};
}  // namespace solarflare