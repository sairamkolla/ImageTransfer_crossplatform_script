cmake_minimum_required(VERSION 2.8)
project( transport )
find_package( OpenCV REQUIRED )
add_executable(server sender.cpp)
add_executable(client receiver.cpp)

target_link_libraries( server ${OpenCV_LIBS} )
target_link_libraries( client ${OpenCV_LIBS} )