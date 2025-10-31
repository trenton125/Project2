cmake_minimum_required(VERSION 3.30)
project(Project2)

set(CMAKE_CXX_STANDARD 14)

add_executable(Project2
    heap.h
    heap.cpp
    main.cpp
    splay.cpp
    splay.h
    json.hpp)
