#include <GameOfLife/core/App.hpp>
#include <SDL3/SDL_events.h>
#include <SDL3/SDL_init.h>
#include <SDL3/SDL_render.h>
#include <SDL3/SDL_scancode.h>
#include <SDL3/SDL_stdinc.h>
#include <SDL3/SDL_timer.h>
#include <SDL3/SDL_video.h>
#include <algorithm>

namespace J0o0ll::GOL {
App::App(const char *title, int width, int height)
    : m_width(width), m_height(height), m_running(true),
      m_grid(m_width / 10, m_height / 10, 10) {
  SDL_Init(SDL_INIT_VIDEO);

  m_window = SDL_CreateWindow(title, width, height, 0);
  m_renderer = SDL_CreateRenderer(m_window, NULL);

  m_grid.randomize();

  m_accumulator = 0.0f;
  m_interval = 0.1f;
  m_paused = false;
}

App::~App() {
  SDL_DestroyRenderer(m_renderer);
  SDL_DestroyWindow(m_window);
  SDL_Quit();
}

void App::run() {
  SDL_Event event;

  m_last = SDL_GetTicks();
  while (m_running) {
    while (SDL_PollEvent(&event)) {
      handleEvents(event);
    }

    update();

    SDL_SetRenderDrawColor(m_renderer, 15, 15, 20, 255);
    SDL_RenderClear(m_renderer);

    render();

    SDL_RenderPresent(m_renderer);
  }
}

void App::update() {
  Uint64 now = SDL_GetTicks();
  m_deltatime = (now - m_last) / 1000.0f;
  m_last = now;

  if (m_paused)
    return;

  m_accumulator += m_deltatime;

  if (m_accumulator >= m_interval) {
    m_grid.update();
    m_accumulator = 0.0f;
  }
}

void App::render() { m_grid.render(m_renderer); }

void App::handleEvents(const SDL_Event &event) {
  switch (event.type) {
  case SDL_EVENT_QUIT:
    m_running = false;
    break;
  case SDL_EVENT_KEY_DOWN:
    if (event.key.scancode == SDL_SCANCODE_SPACE) {
      m_paused = !m_paused;
    } else if (event.key.scancode == SDL_SCANCODE_ESCAPE) {
      m_running = false;
    } else if (event.key.scancode == SDL_SCANCODE_R) {
      m_grid.randomize();
    }

    break;
  case SDL_EVENT_MOUSE_BUTTON_DOWN: {
    int currentCellSize = m_grid.getCellSize();
    bool cell = m_grid.getCell(event.button.x / currentCellSize,
                               event.button.y / currentCellSize);
    m_grid.setCell(event.button.x / currentCellSize,
                   event.button.y / currentCellSize, !cell);

    break;
  }
  case SDL_EVENT_MOUSE_WHEEL: {
    int lastCellSize = m_grid.getCellSize();
    int newCellSize = std::clamp(lastCellSize + (int)event.wheel.y * 10, 4, 64);

    if (lastCellSize != newCellSize)
      m_grid.resize(m_width / newCellSize, m_height / newCellSize, newCellSize);

    break;
  }
  }
}

} // namespace J0o0ll::GOL