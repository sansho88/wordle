cmake_minimum_required(VERSION 3.24)
project(wordle)

set(CMAKE_CXX_STANDARD 11)

add_executable(wordle main.cpp Letter.cpp ../includes/Letter.hpp Game.cpp ../includes/Game.hpp)
