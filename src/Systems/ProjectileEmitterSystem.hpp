#pragma once
#include "../EventManager/EventManager.hpp"
#include "../Events/KeyPressedEvent.hpp"
#include "../Components/ProjectileEmitterComponent.hpp"
#include "../Components/TransformComponent.hpp"
#include "../Components/RigidBodyComponent.hpp"
#include "../Components/SpriteComponent.hpp"
#include "../Components/BoxColliderComponent.hpp"
#include "../Components/CollisionComponent.hpp"
#include "../Components/ProjectileComponent.hpp"
#include "../ECS/ECS.hpp"
#include <SDL2/SDL.h>
#include <SDL2/SDL_timer.h>

static double ms_last_frame = 0;

class ProjectileEmitterSystem : public System {
public:
  ProjectileEmitterSystem() {
    require_component<ProjectileEmitterComponent>();
    require_component<TransformComponent>();
  }

  void ListenForEvents(std::unique_ptr<EventManager>& event_manager) {
    event_manager->listen_for_event(this, &ProjectileEmitterSystem::onKeyPressed);
  }

  void onKeyPressed(KeyPressedEvent& event) {
    // rate limit, will fire every frame otherwise
    double time_since_last_bullet = (SDL_GetTicks() - ms_last_frame) / 1000;

    if (event.key_pressed == SDLK_SPACE && time_since_last_bullet >= 0.30) {
      for (auto& entity: get_system_entities()) {
        if (entity.has_tag("player")) {
          const auto& projectile_emitter = entity.get_component<ProjectileEmitterComponent>();
          const auto& transform = entity.get_component<TransformComponent>();
          const auto& rigid_body = entity.get_component<RigidBodyComponent>();

          glm::vec2 projectile_pos = transform.position;

          if (entity.has_component<SpriteComponent>()) {
            auto& sprite = entity.get_component<SpriteComponent>();
            const static uint8_t x_offset = 20;
            const static uint8_t y_offset = 20;
            projectile_pos.x += (static_cast<float>(sprite.width) / 2) + x_offset;
            projectile_pos.y += (static_cast<float>(sprite.height) / 2) + y_offset;
          }

          glm::vec2 projectile_velocity = projectile_emitter.projectile_velocity;

          int8_t x_direction = 0;
          int8_t y_direction = 0;

          if (rigid_body.velocity.x > 0) x_direction = +1;
          if (rigid_body.velocity.x < 0) x_direction = -1;
          if (rigid_body.velocity.y > 0) y_direction = +1;
          if (rigid_body.velocity.y < 0) y_direction = -1;

          projectile_velocity.x = projectile_emitter.projectile_velocity.x * x_direction;
          projectile_velocity.y = projectile_emitter.projectile_velocity.y * y_direction;

          Entity projectile = entity.registry->create_entity();
          projectile.group("projectile");
          projectile.add_component<TransformComponent>(projectile_pos, glm::vec2(1.0, 1.0), 0);
          projectile.add_component<RigidBodyComponent>(projectile_velocity);
          projectile.add_component<SpriteComponent>("bullet-image", 4, 4, 0, 0, 3, false);
          projectile.add_component<BoxColliderComponent>(4, 4);
          projectile.add_component<CollisionComponent>();
          projectile.add_component<ProjectileComponent>(projectile_emitter.is_friendly, projectile_emitter.damage, projectile_emitter.projectile_duration);

          ms_last_frame = SDL_GetTicks();
        }
      }
    }
  }

  void Update(const std::unique_ptr<Registry>& registry) {
    for (auto& entity: get_system_entities()) {
      auto& projectile_emitter = entity.get_component<ProjectileEmitterComponent>();
      const auto& transform = entity.get_component<TransformComponent>();

      if (projectile_emitter.repeat_speed == 0)
        continue;

      if (SDL_GetTicks() - projectile_emitter.last_emission_time > projectile_emitter.repeat_speed) {
        glm::vec2 projectile_pos = transform.position;

        if (entity.has_component<SpriteComponent>()) {
          const auto& sprite = entity.get_component<SpriteComponent>();
          projectile_pos += (sprite.width / 2);
          projectile_pos += (sprite.height / 2);
        }

        Entity projectile = registry->create_entity();
        projectile.group("projectile");
        projectile.add_component<TransformComponent>(projectile_pos, glm::vec2(1.0, 1.0), 0);
        projectile.add_component<RigidBodyComponent>(projectile_emitter.projectile_velocity);
        projectile.add_component<SpriteComponent>("bullet-image", 4, 4, 0, 0, 3, false);
        projectile.add_component<BoxColliderComponent>(4, 4);
        projectile.add_component<CollisionComponent>();
        projectile.add_component<ProjectileComponent>(projectile_emitter.is_friendly, projectile_emitter.damage, projectile_emitter.projectile_duration);

        projectile_emitter.last_emission_time = SDL_GetTicks();
      }
    }
  }

};
