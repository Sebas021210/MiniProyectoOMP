cmake_minimum_required(VERSION 3.28)
project(MiniProyectoOMP C)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -fopenmp")

add_executable(MiniProyectoOMP main.c)
