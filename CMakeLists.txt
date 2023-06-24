cmake_minimum_required(VERSION 3.25)
project(wordle)

set(CMAKE_CXX_STANDARD 11)

add_executable(wordle main.cpp Letter.cpp Letter.hpp)
