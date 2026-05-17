#pragma once

#include <SDL3/SDL_pixels.h>
#include <SDL3/SDL_render.h>
#include <SDL3_ttf/SDL_ttf.h>
#include <string>
#include <utility>

namespace J0o0ll::GOL::HUD {
void renderText(SDL_Renderer *renderer, TTF_Font *font, const std::string &text,
                int x, int y, SDL_Color color);
std::pair<float, float> getTextSize(TTF_Font *font, const std::string &text);

} // namespace J0o0ll::GOL::HUD