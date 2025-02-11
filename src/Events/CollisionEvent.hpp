#pragma once
#include "../ECS/ECS.hpp"
#include "../EventManager/Event.hpp"

class CollisionEvent : public Event {
  public:
    Entity lhs;
    Entity rhs;
    CollisionEvent(Entity lhs, Entity rhs) : lhs{lhs}, rhs{rhs} {}
    ~CollisionEvent() = default;
};
