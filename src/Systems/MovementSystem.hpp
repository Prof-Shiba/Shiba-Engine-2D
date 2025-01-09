#pragma once

class MovementSystem : public System {
public:
  MovementSystem();
  MovementSystem(const MovementSystem&) = default;
  ~MovementSystem();
  void Update();

private:
  
};
