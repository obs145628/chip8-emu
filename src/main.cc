#include <iostream>
#include <fstream>
#include "machine.hh"

int main()
{
  Machine c8;
  c8.init();
  std::ifstream fs("../roms/pong.rom");
  c8.load_rom(fs);

  for (;;)
    {
      c8.run_cycle();
    }
}
