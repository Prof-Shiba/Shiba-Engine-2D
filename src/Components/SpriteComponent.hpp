#pragma once
#include <cstdint>
#include <string>

struct SpriteComponent {
  uint16_t width;
  uint16_t height;
  std::string asset_id;

  SpriteComponent(std::string asset_id = "", uint16_t width = 0, uint16_t height = 0) 
    : asset_id(asset_id), width{width}, height{height} {
  }
};
