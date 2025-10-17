cmake_minimum_required(VERSION 4.0)
project(Task2)

set(CMAKE_CXX_STANDARD 20)

include_directories(include)
add_executable(Task2 main.cpp
        src/station.cpp
        src/train.cpp)
