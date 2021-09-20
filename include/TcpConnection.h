#pragma once

#include <functional>
#include <memory>
#include <string>
#include "InetAddress.h"
#include "Socket.h"
#include "SocketIO.h"

using std::function;
using std::shared_ptr;
using std::string;

namespace solarflare {
class EventLoop;
class TcpConnection;
using TcpConnectionPtr = shared_ptr<TcpConnection>;
using TcpConnectionCallBack = function<void(const TcpConnectionPtr&)>;

class TcpConnection : public std::enable_shared_from_this<TcpConnection> {
 public:
  TcpConnection(int fd, EventLoop*);
  ~TcpConnection();
  string recv();
  void send(const string& s);
  void shutdownWrite();
  string Address() const;

  void setConnectionCallBack(const TcpConnectionCallBack&);
  void setMessageCallBack(const TcpConnectionCallBack&);
  void setCloseCallBack(const TcpConnectionCallBack&);

  void handleConnectionCallBack();
  void handleMessageCallBack();
  void handleCloseCallBack();

 private:
  InetAddress getLocal(int);
  InetAddress getPeer(int);

 private:
  Socket _socket;
  SocketIO _sio;
  InetAddress _localAddress;
  InetAddress _peerAddress;
  bool _isShutdownWrite;
  EventLoop* _pEventLoop;

  TcpConnectionCallBack _connectionCB;
  TcpConnectionCallBack _messageCB;
  TcpConnectionCallBack _closeCB;
};
}  // namespace solarflare