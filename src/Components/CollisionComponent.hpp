#pragma once

struct CollisionComponent {
  bool is_colliding;
  CollisionComponent(bool is_colliding = false ) : is_colliding(is_colliding) {}
};
