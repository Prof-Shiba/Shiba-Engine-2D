#pragma once
#include "../../libs/glm/glm.hpp"
#include <SDL2/SDL.h>

struct ProjectileEmitterComponent {
  glm::vec2 projectile_velocity;
  uint32_t repeat_speed;
  uint16_t projectile_duration;
  uint16_t damage;
  bool is_friendly;
  uint32_t last_emission_time;

  ProjectileEmitterComponent(glm::vec2 projectile_velocity = glm::vec2(0, 0), uint32_t repeat_speed = 0, 
                          uint16_t projectile_duration = 10000, uint16_t damage = 10, 
                          bool is_friendly = false) 
                          : projectile_velocity(projectile_velocity), repeat_speed(repeat_speed), projectile_duration(projectile_duration),
                          damage(damage), is_friendly(is_friendly), last_emission_time(SDL_GetTicks()) {}
};
