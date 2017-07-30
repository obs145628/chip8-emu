#include "window.hh"
#include <iostream>
#include <cstdlib>
#include "defs.hh"
#include "machine.hh"

static constexpr int ZOOM = 5;

Window::Window()
{
  if (SDL_Init(SDL_INIT_VIDEO) < 0)
  {
    std::cerr << "Error: SDL_Init:"
              << SDL_GetError() << "\n";
    exit(1);
  }

  win_ = SDL_CreateWindow("chip-8", 200, 300, SCREENW * ZOOM, SCREENH * ZOOM, 0);

  if (!win_)
  {
    std::cerr << "Error: CreateWindow: "
              << SDL_GetError() << std::endl;
    exit(1);
  }

  renderer_ = SDL_CreateRenderer(win_, -1, SDL_RENDERER_ACCELERATED);
  if (!renderer_)
  {
    std::cerr << "Error: CreateRenderer:"
              << SDL_GetError() << "\n";
    exit(1);
  }

}

Window& Window::instance()
{
  static Window res;
  return res;
}

void Window::render()
{
  SDL_RenderClear(renderer_);

  auto arr = Machine::instance().screen_get();

  for (int y = 0; y < SCREENH; ++y)
    for (int x = 0; x < SCREENW; ++x)
      {
        if (arr[y * SCREENW + x])
          SDL_SetRenderDrawColor(renderer_, 255, 255, 255, 255);
        else
          SDL_SetRenderDrawColor(renderer_, 0, 0, 0, 255);

        SDL_Rect rect;
        rect.x = x * ZOOM;
        rect.y = y * ZOOM;
        rect.w = ZOOM;
        rect.h = ZOOM;
        SDL_RenderFillRect(renderer_, &rect);
      }

  SDL_RenderPresent(renderer_);
}

void Window::handle_events()
{
  SDL_Event e;

  while (SDL_PollEvent(&e))
    {
      if (e.type == SDL_QUIT)
        exit(0);
    }
}
