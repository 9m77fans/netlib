cmake_minimum_required(VERSION 3.2)
project(divider)

set(SOURCE_FILES tcpserver.cpp )

find_library(EFVI_LIBRARY NAMES ciul1 HINTS "${CMAKE_SOURCE_DIR}/lib")

add_executable(tcpserver ${SOURCE_FILES})
target_link_libraries(tcpserver sotcp ${EFVI_LIBRARY})

