cmake_minimum_required(VERSION 3.22)
project(IteratedLocalSearch_pibic)

set(CMAKE_CXX_STANDARD 14)

include_directories(Paralelo)

add_executable(TspSeq
        comments.cpp
        functions.cpp
        functions.h
        main.cpp
        node.cpp
        node.h
        scanner.cpp
        scanner.h)
