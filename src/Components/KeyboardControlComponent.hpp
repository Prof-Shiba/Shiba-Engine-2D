#pragma once
#include "../../libs/glm/glm.hpp"

struct KeyboardControlComponent {
  glm::vec2 up_velocity;
  glm::vec2 right_velocity;
  glm::vec2 down_velocity;
  glm::vec2 left_velocity;

  KeyboardControlComponent(glm::vec2 up_velocity = glm::vec2(0), glm::vec2 right_velocity = glm::vec2(0), glm::vec2 down_velocity = glm::vec2(0), glm::vec2 left_velocity = glm::vec2(0))
  : up_velocity {up_velocity}, right_velocity {right_velocity}, down_velocity {down_velocity}, left_velocity {left_velocity} {}
};
