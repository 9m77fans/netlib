cmake_minimum_required(VERSION 3.2)

set(SOURCE_FILES SocketIO.cpp TcpConnection.cpp TcpServer.cpp InetAddress.cpp Socket.cpp Acceptor.cpp EventLoop.cpp )

add_library(sotcp STATIC ${SOURCE_FILES})
#add_executable(testnet ${SOURCE_FILES})
#target_link_libraries(divider division)

