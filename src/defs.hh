#pragma once

#include <cstdint>

using opcode_t = uint16_t;
using addr_t = uint16_t;

constexpr int MEMORY_SIZE = 4096;
constexpr int NB_REGS = 16;
constexpr int SCREENW = 64;
constexpr int SCREENH = 32;
constexpr int STACK_SIZE = 16;
constexpr int NB_KEYS = 16;
constexpr int FONTS_ADDR = 0x50;
constexpr int APP_ADDR = 0X200;
constexpr int FREQ = 60;
constexpr int PERIOD_MS = 1000 / FREQ;
