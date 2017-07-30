#include "machine.hh"
#include <algorithm>
#include <cassert>
#include <ctime>
#include <cstdlib>

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
   srand(time(nullptr));
}

void Machine::run_cycle()
{
   opcode_ = mem_[pc_] << 8 | mem_[pc_ + 1];
   auto op = opcode_;
   pc_ += 2;

   if ((op & 0xF000) == 0) //0NNN
   {
     //[FIXME]
   }

   else if (op == 0x00E0) //00E0
   {
     //Clear screen
     std::fill(screen_, screen_ + SCREENW * SCREENH, 0);
   }

   else if (op == 0x00EE) //00EE
   {
     //return
     --sp_;
     assert(sp_ >= 0);
     pc_ = stack_[sp_];
   }

   else if ((op & 0xF000) == 0x1000) //1NNN
   {
     //jump
     auto addr = op & 0x0FFF;
     pc_ = addr;
   }

   else if ((op & 0xF000) == 0x2000) //2NNN
   {
     //call
     auto addr = op & 0x0FFF;
     assert(sp_ < STACK_SIZE);
     stack_[sp_] = pc_;
     ++sp_;
     pc_ = addr;
   }

   else if ((op & 0xF000) == 0x3000) //3XNN
   {
     //Vx == NN
     auto x = (op & 0x0F00) >> 8;
     auto val = op & 0x00FF;
     if (val == regs_[x])
       pc_ += 2;
   }

   else if ((op & 0xF000) == 0x4000) //4XNN
   {
     //Vx != NN
     auto x = (op & 0x0F00) >> 8;
     auto val = op & 0x00FF;
     if (val != regs_[x])
       pc_ += 2;
   }

   else if ((op & 0xF00F) == 0x5000) //5XY0
   {
     //VX == VY
     auto x = (op & 0x0F00) >> 8;
     auto y = (op & 0x00F0) >> 4;
     if (regs_[x] == regs_[y])
       pc_ += 2;
   }

   else if ((op & 0xF000) == 0x6000) //6XNN
   {
     //VX = NN
     auto x = (op & 0x0F00) >> 8;
     auto val = op & 0x00FF;
     regs_[x] = val;
   }

   else if ((op & 0xF000) == 0x7000) //7XNN
   {
     //VX += NN
     auto x = (op & 0x0F00) >> 8;
     auto val = op & 0x00FF;
     regs_[x] += val;
   }

   else if ((op & 0xF00F) == 0x8000) //8XY0
   {
     //VX = VY
     auto x = (op & 0x0F00) >> 8;
     auto y = (op & 0x00F0) >> 4;
     regs_[x] = regs_[y];
   }

   else if ((op & 0xF00F) == 0x8001) //8XY1
   {
     //VX = VX | VY
     auto x = (op & 0x0F00) >> 8;
     auto y = (op & 0x00F0) >> 4;
     regs_[x] |= regs_[y];
   }

   else if ((op & 0xF00F) == 0x8002) //8XY2
   {
     //VX = VX & VY
     auto x = (op & 0x0F00) >> 8;
     auto y = (op & 0x00F0) >> 4;
     regs_[x] &= regs_[y];
   }

   else if ((op & 0xF00F) == 0x8003) //8XY3
   {
     //VX = VX ^ VY
     auto x = (op & 0x0F00) >> 8;
     auto y = (op & 0x00F0) >> 4;
     regs_[x] ^= regs_[y];
   }

   else if ((op & 0xF00F) == 0x8004) //8XY4
   {
     //VX = VX + VY (update VF)
     auto x = (op & 0x0F00) >> 8;
     auto y = (op & 0x00F0) >> 4;
     vf_ = (int) regs_[x] + (int) regs_[y] > 255;
     regs_[x] += regs_[y];
   }

   else if ((op & 0xF00F) == 0x8005) //8XY5
   {
     //VX = VX - VY (update VF)
     auto x = (op & 0x0F00) >> 8;
     auto y = (op & 0x00F0) >> 4;
     vf_ = regs_[x] < regs_[y];
     regs_[x] -= regs_[y];
   }

   else if ((op & 0xF00F) == 0x8006) //8XY6
   {
     //Vx = Vx >> 1
     auto x = (op & 0x0F00) >> 8;
     vf_ = regs_[x] & 1;
     regs_[x] >>= 1;
   }

   else if ((op & 0xF00F) == 0x8007) //8XY7
   {
     //VX = VY - VX
     auto x = (op & 0x0F00) >> 8;
     auto y = (op & 0x00F0) >> 4;
     vf_ = regs_[y] < regs_[x];
     regs_[x] = regs_[y] - regs_[x];
   }

   else if ((op & 0xF00F) == 0x800E) //8XYE
   {
     //Vx = Vx << 1
     auto x = (op & 0x0F00) >> 8;
     vf_ = regs_[x] >> 15;
     regs_[x] <<= 1;
   }

   else if ((op & 0xF00F) == 0x9000) //9XY0
   {
     //VX != VY
     auto x = (op & 0x0F00) >> 8;
     auto y = (op & 0x00F0) >> 4;
     if (regs_[x] != regs_[y])
       pc_ += 2;
   }

   else if ((op & 0xF000) == 0xA000) //ANNN
   {
     //I = NNN
     auto val = op & 0x0FFF;
     i_ = val;
   }

   else if ((op & 0xF000) == 0xB000) //BNNN
   {
     //PC = V0 + NNN
     auto val = op & 0x0FFF;
     pc_ = regs_[0] + val;
   }

   else if ((op & 0xF000) == 0xC000) //CNNN
   {
     //VX = rand() & NN
     auto x = (op & 0x0F00) >> 8;
     auto val = op & 0x00FF;
     regs_[x] = rand() & val;
   }

   else if ((op & 0xF000) == 0xD000) //DXYN
   {
     auto x = (op & 0x0F00) >> 8;
     auto y = (op & 0x00F0) >> 4;
     auto n = op & 0x000F;
     vf_ = 0;

     for (int i = 0; i < 8; ++i)
       for (int j = 0; j < n; ++j)
         {
           //[FIXME]
           (void) x;
           (void) y;
         }
   }

   else if ((op & 0xF0FF) == 0xE09E) //EX9E
   {
     //Vx key pressed ?
     auto x = (op & 0x0F00) >> 8;
     assert(regs_[x] < NB_KEYS);
     if (keys_[regs_[x]])
       pc_ += 2;
   }

   else if ((op & 0xF0FF) == 0xE0A1) //EXA1
   {
     //Vx key not pressed ?
     auto x = (op & 0x0F00) >> 8;
     assert(regs_[x] < NB_KEYS);
     if (!keys_[regs_[x]])
       pc_ += 2;
   }

   else if ((op & 0xF0FF) == 0xF007) //FX07
   {
     //Vx = delay_timer
     auto x = (op & 0x0F00) >> 8;
     regs_[x] = delay_timer_;
   }

   else if ((op & 0xF0FF) == 0xF00A) //FX0A
   {
     //Wait for key
     auto x = (op & 0x0F00) >> 8;
     (void) x;
     //[FIXME]
   }

   else if ((op & 0xF0FF) == 0xF015) //FX15
   {
     //delay_timer = VX
     auto x = (op & 0x0F00) >> 8;
     delay_timer_ = regs_[x];
   }

   else if ((op & 0xF0FF) == 0xF018) //FX18
   {
     //sound_timer = VX
     auto x = (op & 0x0F00) >> 8;
     sound_timer_ = regs_[x];
   }

   else if ((op & 0xF0FF) == 0xF01E) //FX1E
   {
     //I += VX
     auto x = (op & 0x0F00) >> 8;
     i_ += regs_[x];
   }

   else if ((op & 0xF0FF) == 0xF029) //FX29
   {
     //I = sprite_char_addr(vx)
     auto x = (op & 0x0F00) >> 8;
     (void) x;
     //[FIXME]
   }

   else if ((op & 0xF0FF) == 0xF033) //FX33
   {
     //I = BCD(VX)
     auto x = (op & 0x0F00) >> 8;
     auto n = regs_[x];
     mem_[i_] = n / 100;
     mem_[i_ + 1] = (n % 100) / 10;
     mem_[i_ + 2] = n % 10;
   }

   else if ((op & 0xF0FF) == 0xF055) //FX55
   {
     //*i = [V0, VX]
     auto x = (op & 0x0F00) >> 8;
     for (int i = 0; i <= x; ++i)
       mem_[i_ + i] = regs_[i];
   }

   else if ((op & 0xF0FF) == 0xF065) //FX65
   {
     //[V0, VX] = *i
     auto x = (op & 0x0F00) >> 8;
     for (int i = 0; i <= x; ++i)
       regs_[i] = mem_[i_ + i];
   }
}
