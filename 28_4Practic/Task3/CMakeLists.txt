cmake_minimum_required(VERSION 3.16)
project(Task3 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(Task3
        src/main.cpp
        src/Restaurant.cpp
        src/Kitchen.cpp
        src/Utils.cpp
        include/Order.h
        src/Utils.cpp
        include/Utils.h
        src/Kitchen.cpp
        include/Kitchen.h
        src/Restaurant.cpp
        include/Restaurant.h
)

target_include_directories(Task3 PRIVATE include)