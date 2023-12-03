cmake_minimum_required(VERSION 3.24)
project(PEA_tabu)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_EXE_LINKER_FLAGS "-static")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O3 -march=native")


add_executable(PEA_tabu main.cpp SW.cpp SW.h SW2.cpp SW2.h)

