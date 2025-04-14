#pragma once
#include "../ECS/ECS.hpp"
#include "../Components/HealthComponent.hpp"
#include "../Components/TransformComponent.hpp"
#include <SDL2/SDL.h>
#include <SDL2/SDL_render.h>
#include <SDL2/SDL_surface.h>

class RenderHealthSystem : public System {
public:
  RenderHealthSystem() {
    require_component<HealthComponent>();
    require_component<TransformComponent>();
  }

  void Update(SDL_Renderer* renderer, const SDL_Rect& camera) {
    for (auto& entity: get_system_entities()) {
      const auto& health = entity.get_component<HealthComponent>();
      const auto& transform = entity.get_component<TransformComponent>();
      const uint16_t Y_OFFSET = 65;
      const uint16_t HEIGHT = 5;
      int16_t width = 60;

      // NOTE: Running into this while loop
      // every single time! Find a better way
      // of doing this! 2025-04-09
      if (health.health_amount != 100) {
        int16_t missing_health = 100 - health.health_amount;
        while (missing_health > 0) {
          width -= 6;
          missing_health -= 10;
        }
      }

      SDL_Rect rect {
        static_cast<int>(transform.position.x - camera.x),
        static_cast<int>(transform.position.y - camera.y) + Y_OFFSET,
        width,
        HEIGHT
      };

      if (health.health_amount >= 71)
        SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);
      else if (health.health_amount <= 70 && health.health_amount >= 31)
        SDL_SetRenderDrawColor(renderer, 255, 255, 0, 255);
      else 
        SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);

      SDL_RenderFillRect(renderer, &rect);
      SDL_RenderDrawRect(renderer, &rect);
    }
  }

};
