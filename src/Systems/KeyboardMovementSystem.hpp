#pragma once
#include "../ECS/ECS.hpp"
#include "../EventManager/EventManager.hpp"
#include "../Events/KeyPressedEvent.hpp"
#include "../Logger/Logger.hpp"

class KeyboardMovementSystem : public System {
public:
  KeyboardMovementSystem() = default;
  ~KeyboardMovementSystem() = default;

  void ListenForEvents(std::unique_ptr<EventManager>& event_manager) {
    event_manager->listen_for_event(this, &KeyboardMovementSystem::onKeyPressed);
  }

  void onKeyPressed(KeyPressedEvent& event) {
    std::string pressed_key = SDL_GetKeyName(event.key_pressed);
    Logger::Log("KeyPressed event occurred! Key: '" + pressed_key + "' was pressed!");
  }
};
