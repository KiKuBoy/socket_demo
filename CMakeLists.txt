cmake_minimum_required(VERSION 3.5)
project(client)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14")
include_directories( include)


ADD_EXECUTABLE(client linux_tcp_client.cpp)
#target_link_libraries(client
#lib
#)
ADD_EXECUTABLE(server linux_tcp_server.cpp)
target_link_libraries(server
pthread
)







