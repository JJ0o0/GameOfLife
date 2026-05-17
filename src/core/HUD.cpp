#include <GameOfLife/core/HUD.hpp>
#include <SDL3_ttf/SDL_ttf.h>
#include <utility>

namespace J0o0ll::GOL::HUD {
void renderText(SDL_Renderer *renderer, TTF_Font *font, const std::string &text,
                int x, int y, SDL_Color color) {
  SDL_Surface *surface =
      TTF_RenderText_Blended(font, text.c_str(), text.length(), color);
  SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, surface);

  float w, h;
  SDL_GetTextureSize(texture, &w, &h);
  SDL_FRect dst = {(float)x, (float)y, w, h};
  SDL_RenderTexture(renderer, texture, NULL, &dst);

  SDL_DestroySurface(surface);
  SDL_DestroyTexture(texture);
}

std::pair<float, float> getTextSize(TTF_Font *font, const std::string &text) {
  int width, height;
  TTF_GetStringSize(font, text.c_str(), text.length(), &width, &height);

  return {width, height};
}
} // namespace J0o0ll::GOL::HUD