cmake_minimum_required(VERSION 3.13)
project(co_mobile)

set(CMAKE_CXX_STANDARD 14)

add_executable(co_mobile src/main.cpp src/Nanomachine.cpp inc/Nanomachine.h src/aux.cpp inc/aux.h)