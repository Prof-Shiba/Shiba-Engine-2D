#pragma once
#include "../ECS/ECS.hpp"
#include "../EventManager/EventManager.hpp"
#include "../Events/CollisionEvent.hpp"
#include "../Components/RigidBodyComponent.hpp"
#include "../Components/TransformComponent.hpp"
#include "../Components/CollisionComponent.hpp"
#include "../Components/SpriteComponent.hpp"
#include "../Game/Game.hpp"

const static uint8_t resolution_offset = 60; // NOTE: w/o this the borders aren't properly defined.

class MovementSystem : public System {
public:
  MovementSystem() {
    require_component<TransformComponent>();
    require_component<RigidBodyComponent>();
  }

  MovementSystem(const MovementSystem&) = default;
  ~MovementSystem() = default;

  void ListenForEvents(const std::unique_ptr<EventManager>& event_manager) {
    event_manager->listen_for_event(this, &MovementSystem::onCollision);
  }

  void onCollision(CollisionEvent& event) {
    Logger::Log("Collision event occured! Entities: " + std::to_string(event.lhs.get_entity_id()) + " and " + std::to_string(event.rhs.get_entity_id()) + "!");

    event.lhs.get_component<CollisionComponent>().is_colliding = true;
    event.rhs.get_component<CollisionComponent>().is_colliding = true;

    if ( (event.lhs.belongs_to_group("object") && event.rhs.has_tag("player")) || (event.lhs.has_tag("player") && event.rhs.belongs_to_group("object")) ) 
      (event.lhs.belongs_to_group("object")) ? object_hit_player(event.lhs, event.rhs) : object_hit_player(event.rhs, event.lhs);

    else if ( (event.lhs.belongs_to_group("object") && event.rhs.belongs_to_group("enemy")) || (event.lhs.belongs_to_group("enemy") && event.rhs.belongs_to_group("object")) )
      (event.lhs.belongs_to_group("object")) ? object_hit_enemy(event.lhs, event.rhs) : object_hit_enemy(event.rhs, event.lhs);
    else
     return;
  }

  void object_hit_player(Entity object, Entity player) {
    auto& rigid_body = player.get_component<RigidBodyComponent>();
    rigid_body.velocity.x = 0;
    rigid_body.velocity.y = 0;
  }

  void update_out_of_bounds_pos(TransformComponent& transform, const bool x, const bool y) {
    if (x) (transform.position.x >= Game::map_width - resolution_offset) ? transform.position.x -= 1 : transform.position.x += 1;
    if (y) (transform.position.y >= Game::map_height - resolution_offset) ? transform.position.y -= 1 : transform.position.y += 1;
  }

  // TODO: Fix flipping for new models
  void object_hit_enemy(Entity object, Entity enemy) {
    if (enemy.has_component<RigidBodyComponent>() && enemy.has_component<SpriteComponent>()) {
      auto& rigid_body = enemy.get_component<RigidBodyComponent>();
      auto& transform = enemy.get_component<TransformComponent>();

      if (rigid_body.velocity.x != 0) {
        rigid_body.velocity.x *= -1;
        transform.rotation *= -1;
      }
      if (rigid_body.velocity.y != 0) {
        rigid_body.velocity.y *= -1;
        transform.rotation *= -1;
      }
    }
  }

  void Update(double delta_time) {
    for (auto& entity : get_system_entities()) {
      auto& transform = entity.get_component<TransformComponent>();
      auto& rigid_body = entity.get_component<RigidBodyComponent>();

      transform.position.x += rigid_body.velocity.x * delta_time;
      transform.position.y += rigid_body.velocity.y * delta_time;

      bool entity_x_out_of_bounds = (
        transform.position.x <= 0 || transform.position.x >= Game::map_width - resolution_offset
      );

      bool entity_y_out_of_bounds = (
        transform.position.y <= 0 || transform.position.y >= Game::map_height - resolution_offset
      );

      if ((entity_x_out_of_bounds && !entity.has_tag("player")) || (entity_y_out_of_bounds && !entity.has_tag("player"))) {
        entity.remove();
        Logger::Warn("Killed entity that was out of bounds!");
      }

      if ((entity_x_out_of_bounds && entity.has_tag("player")) || (entity_y_out_of_bounds && entity.has_tag("player"))) {
        Logger::Warn("Player at map boundary!");
        if (entity_x_out_of_bounds && entity_y_out_of_bounds) {
          rigid_body.velocity.x = 0;
          rigid_body.velocity.y = 0;
          update_out_of_bounds_pos(transform, true, true);
        }
        else if (entity_x_out_of_bounds) {
          rigid_body.velocity.x = 0;
          update_out_of_bounds_pos(transform, true, false);
        }
        else {
          rigid_body.velocity.y = 0;
          update_out_of_bounds_pos(transform, false, true);
        }
      }

    }
  }

private:
  
};
