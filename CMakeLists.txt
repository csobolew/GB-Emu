cmake_minimum_required(VERSION 3.23)
project(GB_Emu)

set(CMAKE_CXX_STANDARD 14)

include_directories(.)

add_executable(GB_Emu
        cpu.cpp
        cpu.h
        main.cpp)
