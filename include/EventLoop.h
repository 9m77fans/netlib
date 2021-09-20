#pragma once

#include <sys/epoll.h>
#include <iostream>
#include <map>
#include <vector>
#include "Acceptor.h"
#include "TcpConnection.h"

using std::cout;
using std::endl;
using std::map;
using std::vector;

namespace solarflare {
using Functor = function<void()>;
class EventLoop {
 public:
  EventLoop(Acceptor &);
  void Loop();
  void UnLoop();

  void setConnectionCallBack(const TcpConnectionCallBack &);
  void setMessageCallBack(const TcpConnectionCallBack &);
  void setCloseCallBack(const TcpConnectionCallBack &);

 private:
  void epollWait();
  void addNewConnection(int fd);
  void handleMessage(int fd);
  int createEpollFd();
  void epollDel(int fd);
  void epollAdd(int fd);
  bool isClientQuit(int fd);

 private:
  int _epfd;
  Acceptor &_acceptor;
  vector<epoll_event> _evs;
  map<int, TcpConnectionPtr> _tcpMap;
  bool _isLoop;

  TcpConnectionCallBack _connectionCB;
  TcpConnectionCallBack _messageCB;
  TcpConnectionCallBack _closeCB;
};
}  // namespace solarflare