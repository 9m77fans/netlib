#include "EventLoop.h"
#include <errno.h>
#include <sys/eventfd.h>
#include <unistd.h>

namespace solarflare {
EventLoop::EventLoop(Acceptor &acceptor)
    : _epfd(createEpollFd()),
      _acceptor(acceptor),
      _evs(1024),
      _tcpMap(),
      _isLoop(false) {
  epollAdd(_acceptor.fd());
}

void EventLoop::Loop() {
  if (_isLoop)
    return;
  else {
    _isLoop = true;
    epollWait();
  }
}

void EventLoop::UnLoop() {
  if (_isLoop) {
    _isLoop = false;
  }
}

void EventLoop::setConnectionCallBack(const TcpConnectionCallBack &cb) {
  _connectionCB = std::move(cb);
}

void EventLoop::setMessageCallBack(const TcpConnectionCallBack &cb) {
  _messageCB = std::move(cb);
}

void EventLoop::setCloseCallBack(const TcpConnectionCallBack &cb) {
  _closeCB = std::move(cb);
}

void EventLoop::epollWait() {
  while (_isLoop) {
    int nready = epoll_wait(_epfd, &*_evs.begin(), _evs.size(), 10000);
    if (-1 == nready && errno == EINTR) {
      continue;
    } else if (-1 == nready) {
      perror("epoll_wait");
    } else if (0 == nready) {
      cout << "TimeOut!" << endl;
    } else if ((int)_evs.size() == nready) {
      _evs.resize(_evs.size() * 2);
    } else {
      for (int i = 0; i < nready; i++) {
        int fd = _evs[i].data.fd;
        if (_acceptor.fd() == fd) {
          addNewConnection(_acceptor.accept());
        } else {
          handleMessage(fd);
        }
      }
    }
  }
}

void EventLoop::addNewConnection(int fd) {
  epollAdd(fd);
  TcpConnectionPtr p(new TcpConnection(fd, this));
  p->setConnectionCallBack(_connectionCB);
  p->setMessageCallBack(_messageCB);
  p->setCloseCallBack(_closeCB);
  _tcpMap.insert(std::make_pair(fd, p));
  p->handleConnectionCallBack();
}

void EventLoop::handleMessage(int fd) {
  TcpConnectionPtr p = _tcpMap.at(fd);
  if (!p.get()) {
    perror("TcpConnectionPtr ERROR");
  }
  if (isClientQuit(fd)) {
    p->handleCloseCallBack();
    epollDel(fd);
    _tcpMap.erase(fd);
  } else {
    p->handleMessageCallBack();
  }
}

int EventLoop::createEpollFd() {
  int epfd = epoll_create(1);
  if (-1 == epfd) {
    perror("epoll_create");
  }
  return epfd;
}

void EventLoop::epollDel(int fd) {
  epoll_event ev;
  ev.data.fd = fd;
  ev.events = EPOLLIN;
  int ret = epoll_ctl(_epfd, EPOLL_CTL_DEL, fd, &ev);
  if (-1 == ret) {
    perror("epoll_ctl_del");
  }
}

void EventLoop::epollAdd(int fd) {
  epoll_event ev;
  ev.data.fd = fd;
  ev.events = EPOLLIN;
  int ret = epoll_ctl(_epfd, EPOLL_CTL_ADD, fd, &ev);
  if (-1 == ret) {
    perror("epoll_ctl_add");
  }
}

bool EventLoop::isClientQuit(int fd) {
  int ret;
  do {
    char buf[1000];
    ret = recv(fd, buf, sizeof(buf), MSG_PEEK);
  } while (-1 == ret && errno == EINTR);
  return (0 == ret);
}

}  // namespace solarflare