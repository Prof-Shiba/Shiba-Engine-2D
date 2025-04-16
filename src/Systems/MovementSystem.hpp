#pragma once
#include "../ECS/ECS.hpp"
#include "../Components/RigidBodyComponent.hpp"
#include "../Components/TransformComponent.hpp"
#include "../Game/Game.hpp"

class MovementSystem : public System {
public:
  MovementSystem() {
    require_component<TransformComponent>();
    require_component<RigidBodyComponent>();
  }

  MovementSystem(const MovementSystem&) = default;
  ~MovementSystem() = default;

  void Update(double delta_time) {
    for (auto entity : get_system_entities()) {
      auto& transform = entity.get_component<TransformComponent>();
      const auto rigid_body = entity.get_component<RigidBodyComponent>();

      transform.position.x += rigid_body.velocity.x * delta_time;
      transform.position.y += rigid_body.velocity.y * delta_time;

      bool entity_out_of_bounds = (
        transform.position.x < 0 ||
        transform.position.x > Game::map_width ||
        transform.position.y < 0 ||
        transform.position.y > Game::map_height
      );

      if (entity_out_of_bounds && !entity.has_tag("player")) {
        entity.remove();
        Logger::Warn("Killed entity that was out of bounds!");
      }

    }
  }

private:
  
};
