#pragma once

#include "GameOfLife/core/Camera.hpp"
#include <GameOfLife/core/Grid.hpp>
#include <SDL3/SDL_events.h>
#include <SDL3/SDL_oldnames.h>
#include <SDL3/SDL_render.h>
#include <SDL3/SDL_stdinc.h>
#include <SDL3/SDL_video.h>
#include <SDL3_ttf/SDL_ttf.h>

namespace J0o0ll::GOL {
class App {
public:
  App(const char *title, int width, int height);
  ~App();

  void run();

private:
  SDL_Window *m_window;
  SDL_Renderer *m_renderer;
  TTF_Font *m_font;

  Uint64 m_last;

  int m_width, m_height, m_generationCount;
  float m_deltatime, m_accumulator, m_interval, m_panStartX, m_panStartY;
  bool m_running, m_paused, m_panning;

  Grid m_grid;
  Camera m_camera;

  void update();
  void render();
  void handleEvents(const SDL_Event &event);
};
} // namespace J0o0ll::GOL