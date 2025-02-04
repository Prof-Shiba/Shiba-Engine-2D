#pragma once
#include <cstdint>
#include "../../libs/glm/glm.hpp"

struct BoxColliderComponent {
  uint16_t width;
  uint16_t height;
  glm::vec2 offset;

  BoxColliderComponent(uint16_t width = 0, uint16_t height = 0, glm::vec2 offset = glm::vec2(0)) // glm takes one 0 as x and y = 0
  : width{width}, height{height}, offset(offset) {}
};
