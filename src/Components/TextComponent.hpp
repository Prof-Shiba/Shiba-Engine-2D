#pragma once
#include "../../libs/glm/glm.hpp"
#include <SDL2/SDL_pixels.h>
#include <string>

struct TextComponent {
  bool is_fixed;
  glm::vec2 position;
  std::string text;
  std::string asset_id;
  SDL_Color color;

  TextComponent(bool is_fixed = true, glm::vec2 position = glm::vec2(0, 0), std::string text = "", std::string asset_id = "", const SDL_Color& color = {0, 0, 0})
                : is_fixed {is_fixed}, position {position}, text {text}, asset_id {asset_id}, color {color} {}
};
