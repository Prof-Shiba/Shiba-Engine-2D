#pragma once

#include <SDL2/SDL.h>
#include <SDL2/SDL_render.h>
#include "../ECS/ECS.hpp"
#include "../Components/BoxColliderComponent.hpp"
#include "../Components/TransformComponent.hpp"
#include "../Components/CollisionComponent.hpp"

class RenderCollisionSystem : public System {
  public:
    RenderCollisionSystem() {
      require_component<BoxColliderComponent>();
      require_component<TransformComponent>();
  }
   ~RenderCollisionSystem() = default;

  void Update(SDL_Renderer* renderer, SDL_Rect& camera) {
    for (auto& entity: get_system_entities()) {
      auto& collider = entity.get_component<BoxColliderComponent>();
      auto& transform = entity.get_component<TransformComponent>();
      auto& is_colliding = entity.get_component<CollisionComponent>().is_colliding;

      SDL_Rect rect {
        static_cast<int>(transform.position.x + collider.offset.x - camera.x),
        static_cast<int>(transform.position.y + collider.offset.y - camera.y),
        static_cast<int>(collider.width * transform.scale.x),
        static_cast<int>(collider.height * transform.scale.y)
      };

      if (is_colliding) // draw red if colliding
        SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
      else // else draw yellow
        SDL_SetRenderDrawColor(renderer, 255, 255, 0, 255);

      SDL_RenderDrawRect(renderer, &rect);
      entity.get_component<CollisionComponent>().is_colliding = false;
    }
  }

};
