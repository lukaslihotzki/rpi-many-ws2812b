cmake_minimum_required(VERSION 3.0)

add_executable(demo1 Color.cpp PingPongStrip.cpp main.cpp)
target_link_libraries(demo1 rpi-many-ws2812b)
