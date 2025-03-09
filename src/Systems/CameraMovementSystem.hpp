#pragma once
#include "../ECS/ECS.hpp"
#include "../Components/CameraComponent.hpp"
#include "../Components/TransformComponent.hpp"
#include <SDL2/SDL.h>


class CameraMovementSystem: public System {
public:
  CameraMovementSystem() {
   require_component<CameraComponent>();
   require_component<TransformComponent>();
  }

  // FIXME: Right side isn't capped properly
  void Update(SDL_Rect& camera) {
    for (auto& entity: get_system_entities()) {
      auto& transform = entity.get_component<TransformComponent>();
      
      if (transform.position.x  + (camera.w / 2) < Game::map_width)
        camera.x = transform.position.x - (Game::WIDTH / 2);

      if (transform.position.y + (camera.h / 2) < Game::map_height)
        camera.y = transform.position.y - (Game::HEIGHT / 2);

      // Keep cam rect view inside screen limits
      camera.x = (camera.x < 0) ? 0 : camera.x;
      camera.y = (camera.y < 0) ? 0 : camera.y;
      camera.x = (camera.x > camera.w) ? camera.w : camera.x;
      camera.y = (camera.y > camera.h) ? camera.h : camera.y;
    }
  }
};
