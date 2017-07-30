#include "machine.hh"
#include <algorithm>
#include <cassert>
#include <ctime>
#include <cstdlib>
#include <iomanip>

namespace
{
  uint8_t fonts[80] =
    {
      0xF0, 0x90, 0x90, 0x90, 0xF0, // 0
      0x20, 0x60, 0x20, 0x20, 0x70, // 1
      0xF0, 0x10, 0xF0, 0x80, 0xF0, // 2
      0xF0, 0x10, 0xF0, 0x10, 0xF0, // 3
      0x90, 0x90, 0xF0, 0x10, 0x10, // 4
      0xF0, 0x80, 0xF0, 0x10, 0xF0, // 5
      0xF0, 0x80, 0xF0, 0x90, 0xF0, // 6
      0xF0, 0x10, 0x20, 0x40, 0x40, // 7
      0xF0, 0x90, 0xF0, 0x90, 0xF0, // 8
      0xF0, 0x90, 0xF0, 0x10, 0xF0, // 9
      0xF0, 0x90, 0xF0, 0x90, 0x90, // A
      0xE0, 0x90, 0xE0, 0x90, 0xE0, // B
      0xF0, 0x80, 0x80, 0x80, 0xF0, // C
      0xE0, 0x90, 0x90, 0x90, 0xE0, // D
      0xF0, 0x80, 0xF0, 0x80, 0xF0, // E
      0xF0, 0x80, 0xF0, 0x80, 0x80  // F
    };
}

void Machine::init()
{
   std::fill(mem_, mem_ + MEMORY_SIZE, 0);

   std::copy(fonts, fonts + 80, mem_ + FONTS_ADDR);

   std::fill(regs_, regs_ + NB_REGS, 0);
   i_ = 0;
   pc_ = APP_ADDR;
   delay_timer_ = 0;
   sound_timer_ = 0;
   std::fill(screen_, screen_ + SCREENW * SCREENH, 0);
   std::fill(keys_, keys_ + NB_KEYS, 0);
   std::fill(stack_, stack_ + STACK_SIZE, 0);
   sp_ = 0;
   srand(time(nullptr));
}

void Machine::load_rom(std::istream& in)
{
  int addr = APP_ADDR;
  while (!in.eof())
    mem_[addr++] = in.get();
  std::cout << "Rom loaded: " << (addr - APP_ADDR) << "b.\n";
}

void Machine::run_cycle()
{
   assert(pc_ >= 0 && pc_ < MEMORY_SIZE);
   opcode_ = mem_[pc_] << 8 | mem_[pc_ + 1];
   auto op = opcode_;
   pc_ += 2;

   std::cout << std::setw(4) << std::hex << (pc_ - 2) << "|   " << op << "  " << std::dec;

   if (op == 0x00E0) //00E0
   {
     //Clear screen
     std::cout << "CLEAR SCREEN\n";
     std::fill(screen_, screen_ + SCREENW * SCREENH, 0);
   }

   else if (op == 0x00EE) //00EE
   {
     //return
     std::cout << "RETURN\n";
     --sp_;
     assert(sp_ >= 0);
     pc_ = stack_[sp_];
   }

   else if ((op & 0xF000) == 0) //0NNN
   {
     //[FIXME]
     std::cout << "CALL RCA 1802\n";
   }


   else if ((op & 0xF000) == 0x1000) //1NNN
   {
     //jump NNN
     std::cout << "JUMP NNN\n";
     auto addr = op & 0x0FFF;
     pc_ = addr;
   }

   else if ((op & 0xF000) == 0x2000) //2NNN
   {
     //call NNN
     std::cout << "CALL NNN\n";
     auto addr = op & 0x0FFF;
     assert(sp_ < STACK_SIZE);
     stack_[sp_] = pc_;
     ++sp_;
     pc_ = addr;
   }

   else if ((op & 0xF000) == 0x3000) //3XNN
   {
     //Vx == NN
     std::cout << "VX == NN\n";
     auto x = (op & 0x0F00) >> 8;
     auto val = op & 0x00FF;
     if (val == regs_[x])
       pc_ += 2;
   }

   else if ((op & 0xF000) == 0x4000) //4XNN
   {
     //Vx != NN
     std::cout << "VX != NN\n";
     auto x = (op & 0x0F00) >> 8;
     auto val = op & 0x00FF;
     if (val != regs_[x])
       pc_ += 2;
   }

   else if ((op & 0xF00F) == 0x5000) //5XY0
   {
     //VX == VY
     std::cout << "VX == VY\n";
     auto x = (op & 0x0F00) >> 8;
     auto y = (op & 0x00F0) >> 4;
     if (regs_[x] == regs_[y])
       pc_ += 2;
   }

   else if ((op & 0xF000) == 0x6000) //6XNN
   {
     //VX = NN
     std::cout << "VX == NN\n";
     auto x = (op & 0x0F00) >> 8;
     auto val = op & 0x00FF;
     regs_[x] = val;
   }

   else if ((op & 0xF000) == 0x7000) //7XNN
   {
     //VX += NN
     std::cout << "VX += NN\n";
     auto x = (op & 0x0F00) >> 8;
     auto val = op & 0x00FF;
     regs_[x] += val;
   }

   else if ((op & 0xF00F) == 0x8000) //8XY0
   {
     //VX = VY
     std::cout << "VX = VY\n";
     auto x = (op & 0x0F00) >> 8;
     auto y = (op & 0x00F0) >> 4;
     regs_[x] = regs_[y];
   }

   else if ((op & 0xF00F) == 0x8001) //8XY1
   {
     //VX = VX | VY
     std::cout << "VX = VX | VY\n";
     auto x = (op & 0x0F00) >> 8;
     auto y = (op & 0x00F0) >> 4;
     regs_[x] |= regs_[y];
   }

   else if ((op & 0xF00F) == 0x8002) //8XY2
   {
     //VX = VX & VY
     std::cout << "VX = VX & VY\n";
     auto x = (op & 0x0F00) >> 8;
     auto y = (op & 0x00F0) >> 4;
     regs_[x] &= regs_[y];
   }

   else if ((op & 0xF00F) == 0x8003) //8XY3
   {
     //VX = VX ^ VY
     std::cout << "VX = VX ^ VY\n";
     auto x = (op & 0x0F00) >> 8;
     auto y = (op & 0x00F0) >> 4;
     regs_[x] ^= regs_[y];
   }

   else if ((op & 0xF00F) == 0x8004) //8XY4
   {
     //VX = VX + VY (update VF)
     std::cout << "VX = VX + VY\n";
     auto x = (op & 0x0F00) >> 8;
     auto y = (op & 0x00F0) >> 4;
     regs_[0xF] = (int) regs_[x] + (int) regs_[y] > 255;
     regs_[x] += regs_[y];
   }

   else if ((op & 0xF00F) == 0x8005) //8XY5
   {
     //VX = VX - VY (update VF)
     std::cout << "VX = VX - VY\n";
     auto x = (op & 0x0F00) >> 8;
     auto y = (op & 0x00F0) >> 4;
     regs_[0xF] = regs_[x] < regs_[y];
     regs_[x] -= regs_[y];
   }

   else if ((op & 0xF00F) == 0x8006) //8XY6
   {
     //Vx = Vx >> 1
     std::cout << "VX = VX >> 1\n";
     auto x = (op & 0x0F00) >> 8;
     regs_[0xF] = regs_[x] & 1;
     regs_[x] >>= 1;
   }

   else if ((op & 0xF00F) == 0x8007) //8XY7
   {
     //VX = VY - VX
     std::cout << "VX = VX - VY\n";
     auto x = (op & 0x0F00) >> 8;
     auto y = (op & 0x00F0) >> 4;
     regs_[0xF] = regs_[y] < regs_[x];
     regs_[x] = regs_[y] - regs_[x];
   }

   else if ((op & 0xF00F) == 0x800E) //8XYE
   {
     //Vx = Vx << 1
     std::cout << "VX = VX << 1\n";
     auto x = (op & 0x0F00) >> 8;
     regs_[0xF] = regs_[x] >> 15;
     regs_[x] <<= 1;
   }

   else if ((op & 0xF00F) == 0x9000) //9XY0
   {
     //VX != VY
     std::cout << "VX |= VY\n";
     auto x = (op & 0x0F00) >> 8;
     auto y = (op & 0x00F0) >> 4;
     if (regs_[x] != regs_[y])
       pc_ += 2;
   }

   else if ((op & 0xF000) == 0xA000) //ANNN
   {
     //I = NNN
     std::cout << "I = NNN\n";
     auto val = op & 0x0FFF;
     i_ = val;
   }

   else if ((op & 0xF000) == 0xB000) //BNNN
   {
     //PC = V0 + NNN
     std::cout << "PC = V0 + NNN\n";
     auto val = op & 0x0FFF;
     pc_ = regs_[0] + val;
   }

   else if ((op & 0xF000) == 0xC000) //CNNN
   {
     //VX = rand() & NN
     std::cout << "VX = RAND() & NN\n";
     auto x = (op & 0x0F00) >> 8;
     auto val = op & 0x00FF;
     regs_[x] = rand() & val;
   }

   else if ((op & 0xF000) == 0xD000) //DXYN
   {
     //draw
     std::cout << "DRAW\n";
     auto x = (op & 0x0F00) >> 8;
     auto y = (op & 0x00F0) >> 4;
     auto n = op & 0x000F;
     regs_[0xF] = 0;

     for (int i = 0; i < n; ++i)
     {
       auto row = mem_[i_ + i];
       for (int j = 0; j < 8; ++j)
       {
         if ((row >> (7 - j) & 1))
         {
           //MIGHT be done in one if
           if (screen_[(y + i) * SCREENW + (x + j)])
             regs_[0xF] = 1;
           screen_[(y + i) * SCREENW + (x + j)] ^= 1;
         }
       }
     }
   }

   else if ((op & 0xF0FF) == 0xE09E) //EX9E
   {
     //Vx key pressed ?
     std::cout << "VX PRESSED ?\n";
     auto x = (op & 0x0F00) >> 8;
     assert(regs_[x] < NB_KEYS);
     if (keys_[regs_[x]])
       pc_ += 2;
   }

   else if ((op & 0xF0FF) == 0xE0A1) //EXA1
   {
     //Vx key not pressed ?
     std::cout << "VX NOT PRESSED ?\n";
     auto x = (op & 0x0F00) >> 8;
     assert(regs_[x] < NB_KEYS);
     if (!keys_[regs_[x]])
       pc_ += 2;
   }

   else if ((op & 0xF0FF) == 0xF007) //FX07
   {
     //Vx = delay_timer
     std::cout << "VX = DELAY_TIMER\n";
     auto x = (op & 0x0F00) >> 8;
     regs_[x] = delay_timer_;
   }

   else if ((op & 0xF0FF) == 0xF00A) //FX0A
   {
     //Wait for key
     std::cout << "GET KEY\n";
     auto x = (op & 0x0F00) >> 8;
     (void) x;
     //[FIXME]
   }

   else if ((op & 0xF0FF) == 0xF015) //FX15
   {
     //delay_timer = VX
     std::cout << "DELAY_TIMER = VX\n";
     auto x = (op & 0x0F00) >> 8;
     delay_timer_ = regs_[x];
   }

   else if ((op & 0xF0FF) == 0xF018) //FX18
   {
     //sound_timer = VX
     std::cout << "SOUND_TIMER = VX\n";
     auto x = (op & 0x0F00) >> 8;
     sound_timer_ = regs_[x];
   }

   else if ((op & 0xF0FF) == 0xF01E) //FX1E
   {
     //I += VX
     std::cout << "I += VX\n";
     auto x = (op & 0x0F00) >> 8;
     i_ += regs_[x];
   }

   else if ((op & 0xF0FF) == 0xF029) //FX29
   {
     //I = sprite_char_addr(vx)
     std::cout << "I = FONT(VX)\n";
     auto x = (op & 0x0F00) >> 8;
     assert(regs_[x] <= 0xF);
     i_ = FONTS_ADDR + 5 * regs_[x];
   }

   else if ((op & 0xF0FF) == 0xF033) //FX33
   {
     //I = BCD(VX)
     std::cout << "I = BCD(VX)\n";
     auto x = (op & 0x0F00) >> 8;
     auto n = regs_[x];
     mem_[i_] = n / 100;
     mem_[i_ + 1] = (n % 100) / 10;
     mem_[i_ + 2] = n % 10;
   }

   else if ((op & 0xF0FF) == 0xF055) //FX55
   {
     //*i = [V0, VX]
     std::cout << "*I = [V0, VX]\n";
     auto x = (op & 0x0F00) >> 8;
     for (int i = 0; i <= x; ++i)
       mem_[i_ + i] = regs_[i];
   }

   else if ((op & 0xF0FF) == 0xF065) //FX65
   {
     //[V0, VX] = *i
     std::cout << "[V0, VX] = *I\n";
     auto x = (op & 0x0F00) >> 8;
     for (int i = 0; i <= x; ++i)
       regs_[i] = mem_[i_ + i];
   }

   else
   {
     std::cout << "UNKNOWN\n";
   }

   if (delay_timer_ > 0)
     --delay_timer_;
   if (sound_timer_ > 0)
     --sound_timer_;
}
