cmake_minimum_required(VERSION 3.10)

add_library(rpi-many-ws2812b STATIC HwBase.cpp LockedMboxAlloc.cpp Mailbox.cpp MboxAlloc.cpp MemoryFile.cpp Model.cpp SmiOut.cpp WS2812BOut.cpp)
target_include_directories(rpi-many-ws2812b PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
