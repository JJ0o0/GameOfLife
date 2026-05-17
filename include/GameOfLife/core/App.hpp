#pragma once

#include <GameOfLife/core/Grid.hpp>
#include <SDL3/SDL_events.h>
#include <SDL3/SDL_render.h>
#include <SDL3/SDL_stdinc.h>
#include <SDL3/SDL_video.h>

namespace J0o0ll::GOL {
class App {
public:
  App(const char *title, int width, int height);
  ~App();

  void run();

private:
  SDL_Window *m_window;
  SDL_Renderer *m_renderer;

  Uint64 m_last;

  int m_width, m_height;
  float m_deltatime, m_accumulator, m_interval;
  bool m_running, m_paused;

  Grid m_grid;

  void update();
  void render();
  void handleEvents(const SDL_Event &event);
};
} // namespace J0o0ll::GOL