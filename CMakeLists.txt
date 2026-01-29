cmake_minimum_required(VERSION 4.1)
project(rosatom-olympiad)

set(CMAKE_CXX_STANDARD 20)

add_executable(A A.cpp)
add_executable(B B/B.cpp
        B/brute.cpp
        B/brute.h)
add_executable(C C/C.cpp)
add_executable(E E.cpp)