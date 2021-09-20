#include <string.h>
#include <iostream>
#include <string>
#include "TcpServer.h"

using namespace std;
using namespace solarflare;

void ConnectionCallBack(const TcpConnectionPtr& ptr) {
  cout << ">>client has connected " << ptr->Address() << endl;
  ptr->send("welcome to server");
}

void MessageCallBack(const TcpConnectionPtr& ptr) {
  cout << ">>from client:" << ptr->Address() << endl;
  string s = ptr->recv();
  cout << s;
  // ptr->send(s);
}

void CloseCallBack(const TcpConnectionPtr& ptr) {
  cout << "client has broken up " << ptr->Address() << endl;
}

int main() {
  TcpServer server("192.168.50.251", 2000);
  server.setConnectionCallBack(ConnectionCallBack);
  server.setMessageCallBack(MessageCallBack);
  server.setCloseCallBack(CloseCallBack);
  server.start();
  return 0;
}