#pragma once

#include <SDL3/SDL_render.h>
#include <vector>

namespace J0o0ll::GOL {
class Grid {
public:
  Grid(int width, int height, int cellSize);

  void setCell(int x, int y, bool alive);
  bool getCell(int x, int y) const;
  int getCellSize() const;

  void update();
  void render(SDL_Renderer *renderer) const;
  void resize(int newWidth, int newHeight, int newCellSize);
  void randomize();

private:
  int m_width, m_height, m_cellSize;

  std::vector<bool> m_cells, m_next;

  int index(int x, int y) const;
  int wrap(int val, int max) const;
  int countNeighbors(int x, int y) const;
};
} // namespace J0o0ll::GOL