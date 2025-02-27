#pragma once
#include "../EventManager/Event.hpp"
#include <SDL2/SDL_events.h>

class KeyPressedEvent : public Event {
public:
  SDL_Keycode key_pressed;
  KeyPressedEvent(SDL_Event& SDL_Event) : key_pressed{ SDL_Event.key.keysym.sym } {}
  ~KeyPressedEvent() = default;
};
