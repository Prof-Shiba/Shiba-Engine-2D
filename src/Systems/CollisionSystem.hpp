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

  void Update() {
    for (auto& entity: get_system_entities()) {
      auto& collider = entity.get_component<BoxColliderComponent>();
      auto& transform = entity.get_component<TransformComponent>();

      // TODO: check if entity collider component is overlapping with another collider component
      // then send a Logger Message!
    }

  }
};
