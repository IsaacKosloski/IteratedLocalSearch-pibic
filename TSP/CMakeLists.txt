cmake_minimum_required(VERSION 3.22)
project(IteratedLocalSearch_pibic)

set(CMAKE_CXX_STANDARD 14)

include_directories(Sequential)

add_executable(TspSeq
        Sequential/comments.cpp
        Sequential/functions.cpp
        Sequential/functions.h
        Sequential/main.cpp
        Sequential/node.cpp
        Sequential/node.h
        Sequential/scanner.cpp
        Sequential/scanner.h)
