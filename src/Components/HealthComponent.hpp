#pragma once
#include <cstdint>

struct HealthComponent {
  int16_t health_amount;
  HealthComponent(int16_t health_amount = 0) : health_amount{health_amount} {}
};
