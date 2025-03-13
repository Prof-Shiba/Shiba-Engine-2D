#pragma once
#include "../ECS/ECS.hpp"
#include "../Components/ProjectileComponent.hpp"
#include <SDL2/SDL_timer.h>

class ProjectileDurationSystem : public System {
public:
  ProjectileDurationSystem() {
    require_component<ProjectileComponent>();
  }

  void Update() {
    for (auto& entity: get_system_entities()) {
      const auto& projectile = entity.get_component<ProjectileComponent>();

      if (SDL_GetTicks() - projectile.start_time > projectile.duration) {
        entity.remove();
      }
    }
  }

};
