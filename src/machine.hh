#pragma once

#include "defs.hh"
#include <iostream>

class Machine
{
public:

   //Initialize registers and memory
   void init();
   void run_cycle();
   void load_rom(std::istream& in);

private:
   opcode_t opcode_;
   uint8_t mem_[MEMORY_SIZE];

   //Registers
   uint8_t regs_[NB_REGS];
   addr_t i_;
   addr_t pc_;
   uint8_t delay_timer_;
   uint8_t sound_timer_;

   uint8_t screen_[SCREENW * SCREENH];
   bool keys_[NB_KEYS];

   addr_t stack_[STACK_SIZE];
   uint8_t sp_;

};
