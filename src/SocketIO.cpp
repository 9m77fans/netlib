#include "SocketIO.h"
#include <errno.h>
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

namespace solarflare {
SocketIO::SocketIO(int sockfd) : _socket(sockfd) {}

SocketIO::~SocketIO() { _socket.shutdown_write(); }

int SocketIO::readn(void *buf, int len) {
  int left = len;
  char *p = (char *)buf;
  while (left > 0) {
    int ret = ::read(_socket.getFd(), p, left);
    if (ret == -1 && errno == EINTR)
      continue;
    else if (ret == -1) {
      perror("read");
      return len - left;
    } else if (ret == 0) {
      return len - left;
    } else {
      left -= ret;
      p += ret;
    }
  }
  return len - left;
}

int SocketIO::writen(const char *buf, int len) {
  int left = len;
  char *p = (char *)buf;
  while (left > 0) {
    int ret = ::write(_socket.getFd(), p, left);
    if (ret == -1 && errno == EINTR)
      continue;
    else if (ret == -1) {
      perror("write");
      return len - left;
    } else if (ret == 0) {
      return len - left;
    } else {
      left -= ret;
      p += ret;
    }
  }
  return len - left;
}

int SocketIO::readline(void *buff, int maxlen) {
  int left = maxlen - 1;
  char *p = (char *)buff;
  int ret;
  int total = 0;
  while (left > 0) {
    ret = recvPeek(p, left);
    for (int idx = 0; idx != ret; ++idx) {
      if (p[idx] == '\n') {
        int sz = idx + 1;
        readn(p, sz);
        total += sz;
        p += sz;
        *p = '\0';
        return total;
      }
    }
    readn(p, ret);
    left -= ret;
    p += ret;
    total += ret;
  }
  *p = '\0';
  return total;
}

int SocketIO::recvPeek(
    char *buf,
    int len)
{
  int ret;
  do {
    ret = recv(_socket.getFd(), buf, len, MSG_PEEK);
  } while (-1 == ret &&
           errno == EINTR);
  if (-1 == ret) {
    perror("recv MSG_PEEK");
  }
  return ret;
}

}  // namespace solarflare
