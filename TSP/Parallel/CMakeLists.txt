cmake_minimum_required(VERSION 3.22)
project(IteratedLocalSearch_pibic)

set(CMAKE_CXX_STANDARD 14)

include_directories(Paralelo)

add_executable(TspSeq
        src/functions.cpp
        src/functions.h
        ../Deprecated/mainParallel1.cpp
        src/node.cpp
        src/node.h
        src/scanner.cpp
        src/scanner.h)
