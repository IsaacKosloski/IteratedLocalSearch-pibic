cmake_minimum_required(VERSION 3.22)
project(IteratedLocalSearch_pibic)

set(CMAKE_CXX_STANDARD 14)

include_directories(Sequential)

add_executable(TspSeq
        Sequential/src/functions.cpp
        Sequential/src/functions.h
        Sequential/src/main.cpp
        Sequential/src/node.cpp
        Sequential/src/node.h
        Sequential/src/scanner.cpp
        Sequential/src/scanner.h)
