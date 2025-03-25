#pragma once

#include "../ECS/ECS.hpp"
#include "../Components/BoxColliderComponent.hpp"
#include "../Components/CollisionComponent.hpp"
#include "../Components/ProjectileComponent.hpp"
#include "../Components/HealthComponent.hpp"
#include "../EventManager/EventManager.hpp"
#include "../Events/CollisionEvent.hpp"
#include "../Logger/Logger.hpp"

class DamageSystem: public System {
public:
  DamageSystem() {
    require_component<BoxColliderComponent>();
    require_component<CollisionComponent>();
  }

  void ListenForEvents(std::unique_ptr<EventManager>& event_manager) {
    event_manager->listen_for_event(this, &DamageSystem::onCollision);
  }

  void onCollision(CollisionEvent& event) {
    Logger::Log("DamageSystem event occured! Entities: " + std::to_string(event.lhs.get_entity_id()) + " and " + std::to_string(event.rhs.get_entity_id()) + "!");

    event.lhs.get_component<CollisionComponent>().is_colliding = true;
    event.rhs.get_component<CollisionComponent>().is_colliding = true;

    if ( (event.lhs.belongs_to_group("projectile") && event.rhs.has_tag("player")) || (event.lhs.has_tag("player") && event.rhs.belongs_to_group("projectile")) ) 
      (event.lhs.belongs_to_group("projectile")) ? Projectile_hit_player(event.lhs, event.rhs) : Projectile_hit_player(event.rhs, event.lhs);

    else if ( (event.lhs.belongs_to_group("projectile") && event.rhs.belongs_to_group("enemy")) || (event.lhs.belongs_to_group("enemy") && event.rhs.belongs_to_group("projectile")) )
      (event.lhs.belongs_to_group("projectile")) ? Projectile_hit_enemy(event.lhs, event.rhs) : Projectile_hit_enemy(event.rhs, event.lhs);
    else
     return;
  }

  void Projectile_hit_player(Entity& projectile, Entity& player) {
    Logger::Log("Projectile hit player!");

    auto& projectile_component = projectile.get_component<ProjectileComponent>();
    auto& health_component = player.get_component<HealthComponent>();

    if (!projectile_component.is_friendly) {
      health_component.health_amount -= projectile_component.damage;
      projectile.remove();
    }

    if (health_component.health_amount <= 0) {
      Logger::Log("Player with ID " + std::to_string(player.get_entity_id()) + " was killed!");
      player.remove();
    }

  }
  void Projectile_hit_enemy(Entity& projectile, Entity& enemy) {
    Logger::Log("Projectile hit enemy!");

    auto& projectile_component = projectile.get_component<ProjectileComponent>();
    auto& health_component = enemy.get_component<HealthComponent>();

    if (projectile_component.is_friendly) {
      health_component.health_amount -= projectile_component.damage;
      projectile.remove();
    }

    if (health_component.health_amount <= 0) {
      Logger::Log("Enemy with ID " + std::to_string(enemy.get_entity_id()) + " was killed!");
      enemy.remove();
    }
  }

  void Update() {
    // TODO:
  }
};
