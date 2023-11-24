cmake_minimum_required(VERSION 3.24)
project(PEA_tabu)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_EXE_LINKER_FLAGS "-static")

add_executable(PEA_tabu main.cpp)
