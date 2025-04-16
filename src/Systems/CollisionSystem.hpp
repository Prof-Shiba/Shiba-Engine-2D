#pragma once
#include "../ECS/ECS.hpp"
#include "../Components/BoxColliderComponent.hpp"
#include "../Components/TransformComponent.hpp"
#include "../Components/CollisionComponent.hpp"
#include "../EventManager/EventManager.hpp"
#include "../Events/CollisionEvent.hpp"

class CollisionSystem : public System {
public:
  CollisionSystem() {
    require_component<BoxColliderComponent>();
    require_component<TransformComponent>();
    require_component<CollisionComponent>();
  }
  ~CollisionSystem() = default;

  // FIXME: Doesn't work when scaling entities up. Hitbox acts as if they were still scaled down.
  // Must account for scale factor.
  void Update(std::unique_ptr<EventManager>& event_manager) {
    auto entities = get_system_entities();
    
    for (auto i = entities.begin(); i != entities.end(); i++) {
        auto& collider = i->get_component<BoxColliderComponent>();
        auto& transform = i->get_component<TransformComponent>();

        for (auto j = i + 1; j != entities.end(); j++) {
            auto& rhs_collider = j->get_component<BoxColliderComponent>();
            auto& rhs_transform = j->get_component<TransformComponent>();

            if (transform.position.x + collider.offset.x < rhs_transform.position.x + (rhs_collider.width * rhs_transform.scale.x) &&
                transform.position.x + collider.offset.x + (collider.width * transform.scale.x) > rhs_transform.position.x &&
                transform.position.y + collider.offset.y < rhs_transform.position.y + (rhs_collider.height * rhs_transform.scale.y) &&
                transform.position.y + collider.offset.y + (collider.height * transform.scale.y) > rhs_transform.position.y) {
                  event_manager->emit_event<CollisionEvent>(*i, *j);
            }
        }
      }
    }
};
