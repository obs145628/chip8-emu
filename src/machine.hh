#pragma once

#include "defs.hh"

class Machine
{
public:

   //Initialize registers and memory
   void init();

   void run_cycle();

private:
   opcode_t opcode_;
   uint8_t mem_[MEMORY_SIZE];

   //Registers
   uint8_t regs_[NB_REGS];
   uint8_t i_;
   uint8_t pc_;
   uint8_t delay_timer_;
   uint8_t sound_timer_;

   bool screen_[SCREENW * SCREENH];
   bool keys_[NB_KEYS];

   uint8_t stack_[STACK_SIZE];
   uint8_t sp_;

   uint8_t vf_;
};
