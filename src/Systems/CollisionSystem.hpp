#pragma once
#include "../ECS/ECS.hpp"
#include "../Components/BoxColliderComponent.hpp"
#include "../Components/TransformComponent.hpp"
#include "../Logger/Logger.hpp"

class CollisionSystem : public System {
public:
  CollisionSystem() {
    require_component<BoxColliderComponent>();
    require_component<TransformComponent>();
  }
  ~CollisionSystem() = default;

  // NOTE: Smelly
  void Update() {
    for (auto& entity: get_system_entities()) {
      if (entity.has_component<BoxColliderComponent>()) {
        auto& collider = entity.get_component<BoxColliderComponent>();
        auto& transform = entity.get_component<TransformComponent>();

        for (auto& rhs: get_system_entities()) {
          if (rhs.has_component<BoxColliderComponent>() && rhs.get_entity_id() != entity.get_entity_id()) {
            auto& rhs_collider = rhs.get_component<BoxColliderComponent>();
            auto& rhs_transform = rhs.get_component<TransformComponent>();

            if(transform.position.x < rhs_transform.position.x + rhs_collider.width &&
                transform.position.x + collider.width > rhs_transform.position.x &&
                transform.position.y < rhs_transform.position.y + rhs_collider.height &&
                transform.position.y + collider.height > rhs_transform.position.y)
                  Logger::Warn("Collision Detected!");
          }
        }
      }
    }

  }
};
