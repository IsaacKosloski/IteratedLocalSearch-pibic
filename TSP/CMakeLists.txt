cmake_minimum_required(VERSION 3.22)
project(IteratedLocalSearch_pibic)

set(CMAKE_CXX_STANDARD 14)

include_directories(Sequencial)

add_executable(TspSeq
        Sequencial/comments.cpp
        Sequencial/functions.cpp
        Sequencial/functions.h
        Sequencial/main.cpp
        Sequencial/node.cpp
        Sequencial/node.h
        Sequencial/scanner.cpp
        Sequencial/scanner.h)
