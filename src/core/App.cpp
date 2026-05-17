#include "GameOfLife/core/Utils.hpp"
#include <GameOfLife/core/App.hpp>
#include <GameOfLife/core/HUD.hpp>
#include <SDL3/SDL_events.h>
#include <SDL3/SDL_init.h>
#include <SDL3/SDL_mouse.h>
#include <SDL3/SDL_pixels.h>
#include <SDL3/SDL_render.h>
#include <SDL3/SDL_scancode.h>
#include <SDL3/SDL_stdinc.h>
#include <SDL3/SDL_surface.h>
#include <SDL3/SDL_timer.h>
#include <SDL3/SDL_video.h>
#include <SDL3_ttf/SDL_ttf.h>
#include <algorithm>
#include <string>

namespace J0o0ll::GOL {
App::App(const char *title, int width, int height)
    : m_width(width), m_height(height), m_running(true),
      m_grid(m_width / 10, m_height / 10, 10), m_camera({0, 0, 1.0f}) {
  SDL_Init(SDL_INIT_VIDEO);
  TTF_Init();

  m_window = SDL_CreateWindow(title, width, height, 0);
  m_renderer = SDL_CreateRenderer(m_window, NULL);
  m_font = TTF_OpenFont("assets/fonts/FiraCode-Bold.ttf", 16);

  m_grid.randomize();

  m_accumulator = 0.0f;
  m_interval = 0.1f;
  m_generationCount = 0;
  m_panning = false;
  m_paused = false;
}

App::~App() {
  SDL_DestroyRenderer(m_renderer);
  SDL_DestroyWindow(m_window);
  TTF_CloseFont(m_font);
  TTF_Quit();
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

    SDL_SetRenderDrawColor(m_renderer, 10, 22, 40, 255);
    SDL_RenderClear(m_renderer);

    render();

    SDL_RenderPresent(m_renderer);
  }
}

void App::update() {
  Uint64 now = SDL_GetTicks();
  m_deltatime = (now - m_last) / 1000.0f;
  m_last = now;

  m_grid.updateAlpha(m_deltatime);

  if (m_paused)
    return;

  m_accumulator += m_deltatime;

  if (m_accumulator >= m_interval) {
    m_generationCount++;
    m_grid.update(m_deltatime);
    m_accumulator = 0.0f;
  }
}

void App::render() {
  m_grid.render(m_renderer, m_camera);

  SDL_Color textColor = {100, 220, 255, 255};

  std::string speedText = "speed: " + Utils::FloatToString(m_interval, 2);
  HUD::renderText(m_renderer, m_font, speedText,
                  m_width - HUD::getTextSize(m_font, speedText).first - 10,
                  m_height - HUD::getTextSize(m_font, speedText).second - 50,
                  textColor);

  std::string generationText =
      "generation: " + Utils::IntToString(m_generationCount);
  HUD::renderText(m_renderer, m_font, generationText,
                  m_width - HUD::getTextSize(m_font, generationText).first - 10,
                  m_height - HUD::getTextSize(m_font, generationText).second -
                      30,
                  textColor);

  std::string pausedText = m_paused ? "paused" : "running";
  HUD::renderText(m_renderer, m_font, pausedText,
                  m_width - HUD::getTextSize(m_font, pausedText).first - 10,
                  m_height - HUD::getTextSize(m_font, pausedText).second - 10,
                  textColor);

  float mouseX, mouseY;
  SDL_GetMouseState(&mouseX, &mouseY);

  int cellX = (mouseX + m_camera.x) / (m_grid.getCellSize() * m_camera.zoom);
  int cellY = (mouseY + m_camera.y) / (m_grid.getCellSize() * m_camera.zoom);
  std::string mousePosXText = "x: " + Utils::IntToString(cellX);
  HUD::renderText(m_renderer, m_font, mousePosXText, 10,
                  m_height - HUD::getTextSize(m_font, mousePosXText).second -
                      50,
                  textColor);

  std::string mousePosYText = "y: " + Utils::IntToString(cellY);
  HUD::renderText(m_renderer, m_font, mousePosYText, 10,
                  m_height - HUD::getTextSize(m_font, mousePosYText).second -
                      30,
                  textColor);

  bool state = m_grid.getCell(cellX, cellY);
  std::string cellStateText = "state: " + std::string(state ? "alive" : "dead");
  HUD::renderText(m_renderer, m_font, cellStateText, 10,
                  m_height - HUD::getTextSize(m_font, cellStateText).second -
                      10,
                  textColor);
}

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
    } else if (event.key.scancode == SDL_SCANCODE_R && !m_paused) {
      m_grid.randomize();
      m_generationCount = 0;
    }

    break;
  case SDL_EVENT_MOUSE_BUTTON_UP: {
    if (event.button.button == SDL_BUTTON_RIGHT)
      m_panning = false;
    break;
  }
  case SDL_EVENT_MOUSE_BUTTON_DOWN: {
    if (event.button.button == SDL_BUTTON_LEFT) {
      int cellX = (event.button.x + m_camera.x) /
                  (m_grid.getCellSize() * m_camera.zoom);
      int cellY = (event.button.y + m_camera.y) /
                  (m_grid.getCellSize() * m_camera.zoom);
      bool cell = m_grid.getCell(cellX, cellY);
      m_grid.setCell(cellX, cellY, !cell);
    } else if (event.button.button == SDL_BUTTON_RIGHT) {
      m_panning = true;
    }

    break;
  }
  case SDL_EVENT_MOUSE_MOTION: {
    if (m_panning) {
      m_camera.x -= event.motion.xrel;
      m_camera.y -= event.motion.yrel;

      m_camera.x =
          std::clamp(m_camera.x, 0.0f,
                     std::max(0.0f, m_grid.getWidth() * m_grid.getCellSize() *
                                            m_camera.zoom -
                                        m_width));
      m_camera.y =
          std::clamp(m_camera.y, 0.0f,
                     std::max(0.0f, m_grid.getHeight() * m_grid.getCellSize() *
                                            m_camera.zoom -
                                        m_height));
    }
    break;
  }
  case SDL_EVENT_MOUSE_WHEEL: {
    float oldZoom = m_camera.zoom;
    m_camera.zoom = std::clamp(oldZoom + event.wheel.y * 0.1f, 1.0f, 5.0f);

    float centerX = m_width / 2.0f;
    float centerY = m_height / 2.0f;
    m_camera.x = (m_camera.x + centerX) * (m_camera.zoom / oldZoom) - centerX;
    m_camera.y = (m_camera.y + centerY) * (m_camera.zoom / oldZoom) - centerY;

    m_camera.x =
        std::clamp(m_camera.x, 0.0f,
                   std::max(0.0f, m_grid.getWidth() * m_grid.getCellSize() *
                                          m_camera.zoom -
                                      m_width));
    m_camera.y =
        std::clamp(m_camera.y, 0.0f,
                   std::max(0.0f, m_grid.getHeight() * m_grid.getCellSize() *
                                          m_camera.zoom -
                                      m_height));

    break;
  }
  }
}

} // namespace J0o0ll::GOL