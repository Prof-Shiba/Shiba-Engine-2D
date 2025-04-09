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
      const uint16_t WIDTH = 60;
      const uint16_t HEIGHT = 5;
      const uint16_t Y_OFFSET = 65;

      SDL_Rect rect {
        static_cast<int>(transform.position.x - camera.x),
        static_cast<int>(transform.position.y - camera.y) + Y_OFFSET,
        WIDTH,
        HEIGHT
      };

      if (health.health_amount >= 71) {
        SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);
        SDL_RenderFillRect(renderer, &rect);
      }
      else if (health.health_amount <= 70 && health.health_amount >= 41) {
        SDL_SetRenderDrawColor(renderer, 255, 255, 0, 255);
        SDL_RenderFillRect(renderer, &rect);
      }
      else {
        SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
        SDL_RenderFillRect(renderer, &rect);
      }

      SDL_RenderDrawRect(renderer, &rect);
    }
  }

};
