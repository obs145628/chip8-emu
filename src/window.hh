#pragma once

#include <SDL2/SDL.h>

class Window
{
public:

  void handle_events();
  void redraw();

  static Window& instance();

private:
  Window();
  Window(const Window&) = delete;
  Window& operator=(const Window&) = delete;

  SDL_Window* win_;
  SDL_Renderer* renderer_;

  bool redraw_;

  void render_();
};
