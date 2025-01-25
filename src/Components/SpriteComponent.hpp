#pragma once
#include <cstdint>

struct SpriteComponent {
  uint16_t width;
  uint16_t height;

  SpriteComponent(uint16_t width = 0, uint16_t height = 0) 
    : width{width}, height{height} {
  }
};
