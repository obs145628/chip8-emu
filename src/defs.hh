#pragma once

#include <cstdint>

using opcode_t = uint16_t;

constexpr int MEMORY_SIZE = 4096;
constexpr int NB_REGS = 16;
constexpr int SCREENW = 64;
constexpr int SCREENH = 32;
constexpr int STACK_SIZE = 16;
constexpr int NB_KEYS = 16;
