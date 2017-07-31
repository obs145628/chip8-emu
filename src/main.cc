#include <iostream>
#include <fstream>
#include <unistd.h>
#include "machine.hh"
#include "window.hh"

int main(int argc, char** argv)
{
  if (argc < 2)
    {
      std::cerr << "Missing file !\n";
      return 1;
    }

  auto& c8 = Machine::instance();
  auto& win = Window::instance();

  if (argc > 2 && argv[2] == std::string("-d"))
    c8.dump_set(true);

  c8.init();
  std::ifstream fs(argv[1]);
  c8.load_rom(fs);

  for (int i = 0; ; ++i)
    {
      c8.run_cycle();
      //if (i % 10 == 0)
      //getchar();
      win.handle_events();
      usleep(1000 * PERIOD_MS);
    }
}
