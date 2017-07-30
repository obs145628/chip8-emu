#include "machine.hh"
#include <algorithm>

void Machine::init()
{
   std::fill(mem_, mem_ + MEMORY_SIZE, 0);
   std::fill(regs_, regs_ + NB_REGS, 0);
   i_ = 0; //???
   pc_ = 0; //???
   delay_timer_ = 0; //????
   sound_timer_ = 0; //????
   std::fill(screen_, screen_ + SCREENW * SCREENH, 0);
   std::fill(keys_, keys_ + NB_KEYS, 0);
   std::fill(stack_, stack_ + STACK_SIZE, 0);
   sp_ = 0;
}

void Machine::run_cycle()
{
   opcode_ = mem_[pc_] << 8 | mem_[pc_ + 1];
   auto op = opcode_;
   pc_ += 2;

   if ((op & 0xF000) == 0) //0NNN
   {

   }

   else if (op == 0x00E0) //00E0
   {

   }

   else if (op == 0x00EE) //00EE
   {

   }

   else if ((op & 0xF000) == 0x1000) //1NNN
   {

   }

   else if ((op & 0xF000) == 0x2000) //2NNN
   {

   }

   else if ((op & 0xF000) == 0x3000) //3XNN
   {

   }

   else if ((op & 0xF000) == 0x4000) //4XNN
   {

   }

   else if ((op & 0xF00F) == 0x5000) //5XY0
   {

   }

   else if ((op & 0xF000) == 0x6000) //6XNN
   {

   }

   else if ((op & 0xF000) == 0x7000) //7XNN
   {

   }

   else if ((op & 0xF00F) == 0x8000) //8XY0
   {

   }

   else if ((op & 0xF00F) == 0x8001) //8XY1
   {

   }

   else if ((op & 0xF00F) == 0x8002) //8XY2
   {

   }

   else if ((op & 0xF00F) == 0x8003) //8XY3
   {

   }

   else if ((op & 0xF00F) == 0x8004) //8XY4
   {

   }

   else if ((op & 0xF00F) == 0x8005) //8XY5
   {

   }

   else if ((op & 0xF00F) == 0x8006) //8XY6
   {

   }

   else if ((op & 0xF00F) == 0x8007) //8XY7
   {

   }

   else if ((op & 0xF00F) == 0x800E) //8XYE
   {

   }

   else if ((op & 0xF00F) == 0x9000) //9XY0
   {

   }

   else if ((op & 0xF000) == 0xA000) //ANNN
   {

   }

   else if ((op & 0xF000) == 0xB000) //BNNN
   {

   }

   else if ((op & 0xF000) == 0xC000) //CNNN
   {

   }

   else if ((op & 0xF000) == 0xD000) //DXYN
   {

   }

   else if ((op & 0xF0FF) == 0xE09E) //EX9E
   {

   }

   else if ((op & 0xF0FF) == 0xE0A1) //EXA1
   {

   }

   else if ((op & 0xF0FF) == 0xF007) //FX07
   {

   }

   else if ((op & 0xF0FF) == 0xF00A) //FX0A
   {

   }

   else if ((op & 0xF0FF) == 0xF015) //FX15
   {

   }

   else if ((op & 0xF0FF) == 0xF018) //FX18
   {

   }

   else if ((op & 0xF0FF) == 0xF01E) //FX1E
   {

   }

   else if ((op & 0xF0FF) == 0xF029) //FX29
   {

   }

   else if ((op & 0xF0FF) == 0xF033) //FX33
   {

   }

   else if ((op & 0xF0FF) == 0xF055) //FX55
   {

   }

   else if ((op & 0xF0FF) == 0xF065) //FX65
   {

   }
}
