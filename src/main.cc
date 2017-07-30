#include <iostream>
#include <fstream>
#include <unistd.h>
#include "machine.hh"
#include "window.hh"

int main()
{
  auto& c8 = Machine::instance();
  auto& win = Window::instance();

  c8.init();
  std::ifstream fs("../roms/pong.rom");
  c8.load_rom(fs);

  for (;;)
    {
      c8.run_cycle();
      win.render();
      win.handle_events();
      usleep(1000 * PERIOD_MS);
    }
}
