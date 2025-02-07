#pragma once

#include <SDL2/SDL.h>
#include <SDL2/SDL_render.h>
#include "../ECS/ECS.hpp"
#include "../Components/BoxColliderComponent.hpp"
#include "../Components/TransformComponent.hpp"

class RenderCollisionSystem : public System {
  public:
    RenderCollisionSystem() {
      require_component<BoxColliderComponent>();
      require_component<TransformComponent>();
  }
   ~RenderCollisionSystem() = default;

  void Update(SDL_Renderer* renderer, bool& is_colliding) {
    for (auto& entity: get_system_entities()) {
      auto& collider = entity.get_component<BoxColliderComponent>();
      auto& transform = entity.get_component<TransformComponent>();

      SDL_Rect rect {
        static_cast<int>(transform.position.x + collider.offset.x),
        static_cast<int>(transform.position.y + collider.offset.y),
        static_cast<int>(collider.width),
        static_cast<int>(collider.height)
      };

      if (is_colliding)
        SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
      else
        SDL_SetRenderDrawColor(renderer, 255, 255, 0, 255);

      SDL_RenderDrawRect(renderer, &rect);
    }
  }

};
