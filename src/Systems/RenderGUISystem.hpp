#pragma once
#include <SDL2/SDL_render.h>
#include "../ECS/ECS.hpp"
#include "../../libs/imgui/imgui.h"
#include "../../libs/imgui/backends/imgui_impl_sdl2.h"
#include "../../libs/imgui/backends/imgui_impl_sdlrenderer2.h"
#include "../Components/TransformComponent.hpp"
#include "../Components/RigidBodyComponent.hpp"
#include "../Components/SpriteComponent.hpp"
#include "../Components/BoxColliderComponent.hpp"
#include "../Components/CollisionComponent.hpp"
#include "../Components/HealthComponent.hpp"
#include "../Components/ProjectileEmitterComponent.hpp"
#include "../Components/MovingTextComponent.hpp"
#include "../Components/GodModeComponent.hpp"

class RenderGUISystem : public System {
public:
  RenderGUISystem() = default;
  ~RenderGUISystem() = default;

  void Update(SDL_Renderer* renderer, const std::unique_ptr<Registry>& registry) {
    ImGui_ImplSDLRenderer2_NewFrame();
    ImGui_ImplSDL2_NewFrame();
    ImGui::NewFrame();

    if (ImGui::Begin("Spawn Enemies")) {
      static int32_t enemy_x_pos = 0;
      static int32_t enemy_y_pos = 0;
      static int32_t enemy_health = 100;
      // static bool enemy_godmode = false;

      ImGui::InputInt("Enemy x", &enemy_x_pos);
      ImGui::InputInt("Enemy y", &enemy_y_pos);

      // TODO: Everything else besides pos
      // Scale, rotation.
      // Velocity for enemy
      // Dropdown list (combo box) for sprite texture ID
      // Angle and speed of projectiles
      // Repeat freq and duration of projectiles (in seconds)

      if (ImGui::Button("Spawn enemy")) {
        Entity new_enemy = registry->create_entity();
        new_enemy.group("enemy");
        new_enemy.add_component<TransformComponent>(glm::vec2(enemy_x_pos, enemy_y_pos), glm::vec2(2.0, 2.0), 0.0);
        new_enemy.add_component<RigidBodyComponent>(glm::vec2(0.0, 0.0));
        new_enemy.add_component<SpriteComponent>("tank-image", 32, 32, 0, 0, 2);
        new_enemy.add_component<BoxColliderComponent>(60, 60);
        new_enemy.add_component<CollisionComponent>();
        new_enemy.add_component<HealthComponent>(enemy_health);
        new_enemy.add_component<ProjectileEmitterComponent>(glm::vec2(100, 0), 5000, 10000, 10, false);
        new_enemy.add_component<GodModeComponent>(false);
        new_enemy.add_component<MovingTextComponent>(7, -10, "Tank", "arial-font", SDL_Color {255, 0, 0});
      }
    }
    ImGui::End();

    ImGui::Render();
    ImGui_ImplSDLRenderer2_RenderDrawData(ImGui::GetDrawData(), renderer);
  }
};
