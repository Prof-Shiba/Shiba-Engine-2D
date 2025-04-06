#pragma once
#include "../../libs/glm/glm.hpp"
#include <SDL2/SDL_pixels.h>
#include <string>

struct MovingTextComponent {
  int16_t offset_x;
  int16_t offset_y;
  std::string text;
  std::string asset_id;
  SDL_Color color;

  MovingTextComponent(int16_t offset_x = 0, int16_t offset_y = 0, std::string text = "", std::string asset_id = "", const SDL_Color& color = {0, 0, 0})
                : offset_x{offset_x}, offset_y{offset_y}, text {text}, asset_id {asset_id}, color {color} {}
};
