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
#include "../Components/PlayerShootComponent.hpp"
#include "../ECS/ECS.hpp"
#include <SDL2/SDL.h>
#include <SDL2/SDL_timer.h>

class ProjectileEmitterSystem : public System {
public:
  ProjectileEmitterSystem() {
    require_component<ProjectileEmitterComponent>();
    require_component<TransformComponent>();
  }
  bool player_shooting = false;

  void ListenForEvents(std::unique_ptr<EventManager>& event_manager) {
    event_manager->listen_for_event(this, &ProjectileEmitterSystem::onKeyPressed);
  }

  void onKeyPressed(KeyPressedEvent& event) {
    if (event.key_pressed == SDLK_SPACE) {
      player_shooting = true; 
    }
  }

  void Update(const std::unique_ptr<Registry>& registry) {
    for (auto& entity: get_system_entities()) {
      // FIXME: Bullets not at proper angle and loops through all entities before getting back to actual player, causing delay
      if (entity.has_component<PlayerShootComponent>() && player_shooting == false)
        continue;

      auto& projectile_emitter = entity.get_component<ProjectileEmitterComponent>();
      const auto& transform = entity.get_component<TransformComponent>();

      if (SDL_GetTicks() - projectile_emitter.last_emission_time > projectile_emitter.repeat_speed) {
        glm::vec2 projectile_pos = transform.position;

        if (entity.has_component<SpriteComponent>()) {
          const auto& sprite = entity.get_component<SpriteComponent>();
          projectile_pos += (sprite.width / 2);
          projectile_pos += (sprite.height / 2);
        }

        Entity projectile = registry->create_entity();
        projectile.add_component<TransformComponent>(projectile_pos, glm::vec2(1.0, 1.0), 0);
        projectile.add_component<RigidBodyComponent>(projectile_emitter.projectile_velocity);
        projectile.add_component<SpriteComponent>("bullet-image", 4, 4, 0, 0, 3, false);
        projectile.add_component<BoxColliderComponent>(4, 4);
        projectile.add_component<CollisionComponent>();
        projectile.add_component<ProjectileComponent>(projectile_emitter.is_friendly, projectile_emitter.damage, projectile_emitter.projectile_duration);

        projectile_emitter.last_emission_time = SDL_GetTicks();

        if (entity.has_component<PlayerShootComponent>())
          player_shooting = false;
      }
    }
  }

};
