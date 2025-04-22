#pragma once
#include "../ECS/ECS.hpp"
#include "../Components/HealthComponent.hpp"
#include "../Components/TransformComponent.hpp"
#include "../Components/SpriteComponent.hpp"
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
      auto& sprite = entity.get_component<SpriteComponent>();

      const uint16_t X_OFFSET = 15;
      const uint16_t Y_OFFSET = 75;
      const uint16_t HEIGHT = 5;
      int16_t width = 60;

      if (health.health_amount != 100) {
        int16_t missing_health = 100 - health.health_amount;
        while (missing_health > 0) {
          width -= 6;
          missing_health -= 10;
        }
      }

      SDL_Rect rect {
        static_cast<int>(transform.position.x - camera.x) + X_OFFSET,
        static_cast<int>(transform.position.y - camera.y) + Y_OFFSET,
        width,
        HEIGHT
      };

      if (health.health_amount >= 71) {
        SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);
      }
      else if (health.health_amount <= 70 && health.health_amount >= 31) {
        SDL_SetRenderDrawColor(renderer, 255, 255, 0, 255);
        if (entity.has_tag("player"))
          sprite.asset_id = "player-hurt-image";
      }
      else {
        SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
        if (entity.has_tag("player"))
          sprite.asset_id = "player-dying-image";
      }

      SDL_RenderFillRect(renderer, &rect);
      SDL_RenderDrawRect(renderer, &rect);
    }
  }

};
