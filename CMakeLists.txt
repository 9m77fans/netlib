cmake_minimum_required(VERSION 3.2)

project(netcode)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -ggdb")

include_directories(${PROJECT_SOURCE_DIR}/include)
add_subdirectory(src)
