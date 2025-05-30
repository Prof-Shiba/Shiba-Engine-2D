#pragma once

#include "../../libs/glm/glm.hpp"

struct RigidBodyComponent {
  glm::vec2 velocity;
  
  RigidBodyComponent(glm::vec2 velocity = glm::vec2(0.0, 0.0)) : velocity{velocity} {}
};
