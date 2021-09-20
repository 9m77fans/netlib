cmake_minimum_required(VERSION 3.2)
project(divider)

set(SOURCE_FILES main.cpp SocketIO.cpp TcpConnection.cpp TcpServer.cpp InetAddress.cpp Socket.cpp Acceptor.cpp EventLoop.cpp )

add_executable(testnet ${SOURCE_FILES})
#target_link_libraries(divider division)

