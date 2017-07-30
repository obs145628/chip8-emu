cmake_minimum_required(VERSION 3.0)
project(ship8-emu)

set(CMAKE_CXX_COMPILER "g++")
set(CMAKE_CXX_FLAGS "-g -Wall -Wextra -Werror -pedantic -std=c++14")

set(SRC
  src/machine.cc
  src/main.cc
)

add_executable(ship8-emu ${SRC})
