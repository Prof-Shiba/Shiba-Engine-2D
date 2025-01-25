#pragma once
#include "../ECS/ECS.hpp"
#include "../Components/TransformComponent.hpp"
#include "../Components/SpriteComponent.hpp"
#include <SDL2/SDL.h>
#include <SDL2/SDL_render.h>

class RenderSystem : public System {
public:
  RenderSystem() {
    require_component<SpriteComponent>();
    require_component<TransformComponent>();
  }

  RenderSystem(const RenderSystem&) = default;
  ~RenderSystem() = default;

  void Update(SDL_Renderer* renderer) {
    for (auto entity: get_system_entities()) {
      const auto transform = entity.get_component<TransformComponent>();
      const auto sprite = entity.get_component<SpriteComponent>();

      SDL_Rect rect = {
        static_cast<int>(transform.position.x),
        static_cast<int>(transform.position.y),
        sprite.width,
        sprite.height
      };
      SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);
      SDL_RenderFillRect(renderer, &rect);
    }
  }

private:
  
};
