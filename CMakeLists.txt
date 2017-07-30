cmake_minimum_required(VERSION 3.0)
project(chip8-emu)

set(CMAKE_CXX_COMPILER "g++")
set(CMAKE_CXX_FLAGS "-g -Wall -Wextra -Werror -pedantic -std=c++14")

set(SRC
  src/machine.cc
  src/main.cc
  src/window.cc
)

add_executable(chip8-emu ${SRC})
target_link_libraries(chip8-emu SDL2)
