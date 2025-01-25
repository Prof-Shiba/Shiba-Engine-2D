#pragma once
#include <cstdint>
#include <string>
#include <SDL2/SDL.h>

struct SpriteComponent {
  uint16_t width;
  uint16_t height;
  std::string asset_id;
  SDL_Rect src_rect;

  SpriteComponent(std::string asset_id = "", uint16_t width = 0, uint16_t height = 0, uint16_t src_rect_x = 0, int16_t src_rect_y = 0) 
    :width{width}, height{height}, asset_id(asset_id), src_rect{src_rect_x, src_rect_y, width, height} {
  }
};
