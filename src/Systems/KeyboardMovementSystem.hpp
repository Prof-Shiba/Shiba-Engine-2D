#pragma once
#include "../ECS/ECS.hpp"
#include "../EventManager/EventManager.hpp"
#include "../Events/KeyPressedEvent.hpp"
#include "../Logger/Logger.hpp"
#include "../Components/KeyboardControlComponent.hpp"
#include "../Components/SpriteComponent.hpp"
#include "../Components/RigidBodyComponent.hpp"
#include <SDL2/SDL_keycode.h>

class KeyboardMovementSystem : public System {
public:
  KeyboardMovementSystem() {
    require_component<KeyboardControlComponent>();
    require_component<SpriteComponent>();
    require_component<RigidBodyComponent>();
  };
  ~KeyboardMovementSystem() = default;

  void ListenForEvents(std::unique_ptr<EventManager>& event_manager) {
    event_manager->listen_for_event(this, &KeyboardMovementSystem::onKeyPressed);
  }

  void onKeyPressed(KeyPressedEvent& event) {
    for (auto& entity: get_system_entities()) {
      const auto& keyboard_control = entity.get_component<KeyboardControlComponent>();
      auto& sprite = entity.get_component<SpriteComponent>();
      auto& rigid_body = entity.get_component<RigidBodyComponent>();

      switch (event.key_pressed) {
        case SDLK_UP:
          rigid_body.velocity = keyboard_control.up_velocity;
          sprite.src_rect.y = sprite.height * 0;
          break;

        case SDLK_RIGHT:
          rigid_body.velocity = keyboard_control.right_velocity;
          sprite.src_rect.y = sprite.height * 1;
          break;

        case SDLK_DOWN:
          rigid_body.velocity = keyboard_control.down_velocity;
          sprite.src_rect.y = sprite.height * 2;
          break;

        case SDLK_LEFT:
          rigid_body.velocity = keyboard_control.left_velocity;
          sprite.src_rect.y = sprite.height * 3;
          break;
      }
    }
  }
};
