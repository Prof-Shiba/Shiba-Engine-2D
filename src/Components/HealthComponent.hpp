#pragma once
#include <cstdint>

struct HealthComponent {
  uint8_t health_amount;
  HealthComponent(uint8_t health_amount = 0) : health_amount{health_amount} {}
};
