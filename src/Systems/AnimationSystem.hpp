#pragma once
#include "../Components/AnimationComponent.hpp"
#include "../Components/SpriteComponent.hpp"
#include "../ECS/ECS.hpp"
#include <SDL2/SDL_timer.h>

class AnimationSystem : public System {
  public:
    AnimationSystem() {
      require_component<SpriteComponent>();
      require_component<AnimationComponent>();
    }
    ~AnimationSystem() = default;

    void Update() {
      for (auto& entity: get_system_entities()) {
        auto& animation = entity.get_component<AnimationComponent>();
        auto& sprite = entity.get_component<SpriteComponent>();

        animation.current_frame = ((SDL_GetTicks() - animation.start_time) * animation.frame_speed / 1000) % animation.num_of_frames; // milliseconds
        sprite.src_rect.x = animation.current_frame * sprite.width;
    }
  }
};
