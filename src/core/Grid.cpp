#include <GameOfLife/core/Grid.hpp>
#include <SDL3/SDL_rect.h>
#include <SDL3/SDL_render.h>
#include <SDL3/SDL_stdinc.h>
#include <algorithm>
#include <random>
#include <utility>

namespace J0o0ll::GOL {
Grid::Grid(int width, int height, int cellSize)
    : m_width(width), m_height(height), m_cellSize(cellSize) {
  m_cells.resize(m_width * m_height);
  m_next.resize(m_width * m_height);
  m_alpha.resize(m_width * m_height);
}

void Grid::update(float deltatime) {
  for (int x = 0; x < m_width; x++) {
    for (int y = 0; y < m_height; y++) {
      int neighbors = countNeighbors(x, y);
      bool state = getCell(x, y);

      m_next[index(x, y)] = neighbors == 3 || state && neighbors == 2;
    }
  }

  std::swap(m_next, m_cells);
}

void Grid::updateAlpha(float deltatime) {
  float alphaSpeed = 5.0f * deltatime;

  for (int x = 0; x < m_width; x++) {
    for (int y = 0; y < m_height; y++) {
      float alpha = m_alpha[index(x, y)];
      alpha += m_next[index(x, y)] ? alphaSpeed : -alphaSpeed;
      alpha = std::clamp(alpha, 0.0f, 1.0f);
      m_alpha[index(x, y)] = alpha;
    }
  }
}

void Grid::render(SDL_Renderer *renderer) const {
  SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);

  for (int x = 0; x < m_width; x++) {
    for (int y = 0; y < m_height; y++) {
      bool state = getCell(x, y);
      SDL_FRect cell = {(float)x * m_cellSize, (float)y * m_cellSize,
                        (float)m_cellSize, (float)m_cellSize};

      SDL_SetRenderDrawColor(renderer, 255, 255, 255,
                             (Uint8)(getAlpha(x, y) * 255));
      SDL_RenderFillRect(renderer, &cell);
    }
  }
}

void Grid::resize(int newWidth, int newHeight, int newCellSize) {
  m_width = newWidth;
  m_height = newHeight;
  m_cellSize = newCellSize;

  m_cells.resize(m_width * m_height);
  m_next.resize(m_width * m_height);
  m_alpha.resize(m_width * m_height);

  randomize();
}

void Grid::randomize() {
  std::random_device rd;
  std::mt19937 gen(rd());
  std::uniform_int_distribution<> distr(0, 1);

  for (size_t i = 0; i < m_cells.size(); i++) {
    m_cells[i] = distr(gen);
  }
}

void Grid::setCell(int x, int y, bool alive) {
  m_cells[index(wrap(x, m_width), wrap(y, m_height))] = alive;
}

bool Grid::getCell(int x, int y) const {
  return m_cells[index(wrap(x, m_width), wrap(y, m_height))];
}

int Grid::countNeighbors(int x, int y) const {
  int counter = 0;

  for (int dx = -1; dx < 2; dx++) {
    for (int dy = -1; dy < 2; dy++) {
      if (dx == 0 && dy == 0)
        continue;

      counter += getCell(x + dx, y + dy);
    }
  }

  return counter;
}

float Grid::getAlpha(int x, int y) const { return m_alpha[index(x, y)]; }

int Grid::index(int x, int y) const { return x + y * m_width; }
int Grid::wrap(int val, int max) const { return ((val % max) + max) % max; }
int Grid::getCellSize() const { return m_cellSize; }

} // namespace J0o0ll::GOL