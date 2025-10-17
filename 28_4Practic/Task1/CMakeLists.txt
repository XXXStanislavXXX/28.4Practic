cmake_minimum_required(VERSION 4.0)
project(Task1)

set(CMAKE_CXX_STANDARD 20)

include_directories(include)
add_executable(Task1 main.cpp
        src/Simulation.cpp
        src/Swimmer.cpp)
