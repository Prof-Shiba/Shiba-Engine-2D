#pragma once
#include <SDL2/SDL_render.h>
#include <array>
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

      ImGui::SeparatorText("Position");
      static int32_t enemy_x_pos = 0;
      static int32_t enemy_y_pos = 0;
      ImGui::InputInt("Enemy x", &enemy_x_pos);
      ImGui::InputInt("Enemy y", &enemy_y_pos);

      ImGui::SeparatorText("Health");
      static int32_t enemy_health = 100;
      static bool enemy_godmode = false;
      ImGui::SliderInt("Enemy health", &enemy_health, 1, 100);
      ImGui::Checkbox("Enemy Godmode", &enemy_godmode);

      const static char* sprites[] = { "tank-image", "truck-image"};
      static int32_t current_sprite = 0;
      static std::string enemy_name = "";
      (current_sprite == 0) ? enemy_name = "Tank" : enemy_name = "Truck";
      ImGui::SeparatorText("Sprite image");
      ImGui::Combo("Sprite selection", &current_sprite, sprites, 2);

      // TODO: Everything else besides pos
      // Scale, rotation.
      // Velocity for enemy
      // Angle and speed of projectiles
      // Repeat freq and duration of projectiles (in seconds)

      // creates invis rect for space
      ImGui::Dummy(ImVec2(0, 35));
      
      if (ImGui::Button("Spawn enemy")) {
        Entity new_enemy = registry->create_entity();
        new_enemy.group("enemy");
        new_enemy.add_component<TransformComponent>(glm::vec2(enemy_x_pos, enemy_y_pos), glm::vec2(2.0, 2.0), 0.0);
        new_enemy.add_component<RigidBodyComponent>(glm::vec2(0.0, 0.0));
        new_enemy.add_component<SpriteComponent>(sprites[current_sprite], 32, 32, 0, 0, 2);
        new_enemy.add_component<BoxColliderComponent>(60, 60);
        new_enemy.add_component<CollisionComponent>();
        new_enemy.add_component<HealthComponent>(enemy_health);
        new_enemy.add_component<ProjectileEmitterComponent>(glm::vec2(100, 0), 5000, 10000, 10, false);
        new_enemy.add_component<GodModeComponent>(enemy_godmode);
        new_enemy.add_component<MovingTextComponent>(7, -10, enemy_name.c_str(), "arial-font", SDL_Color {255, 0, 0});
      }
    }
    ImGui::End();

    ImGui::Render();
    ImGui_ImplSDLRenderer2_RenderDrawData(ImGui::GetDrawData(), renderer);
  }
};
