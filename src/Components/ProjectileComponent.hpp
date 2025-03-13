#pragma once
#include <SDL2/SDL_timer.h>
#include <cstdint>

struct ProjectileComponent {
  bool is_friendly;
  uint16_t damage;
  uint16_t duration;
  uint32_t start_time;

  ProjectileComponent(bool is_friendly = false, uint16_t damage = 0, uint16_t duration = 0) 
  : is_friendly(is_friendly), damage(damage), duration(duration), start_time(SDL_GetTicks()) {}
};
