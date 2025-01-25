#pragma once
#include <cstdint>

struct SpriteComponent {
  uint8_t width;
  uint8_t height;

  SpriteComponent(uint8_t width = 5, uint8_t height = 5) 
    : width{width}, height{height} {
  }
};
