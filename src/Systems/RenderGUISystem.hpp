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

      ImGui::SeparatorText("Position");
      static int32_t enemy_x_pos = 0;
      static int32_t enemy_y_pos = 0;
      ImGui::InputInt("Enemy x", &enemy_x_pos);
      ImGui::InputInt("Enemy y", &enemy_y_pos);
      static int32_t enemy_z_index = 2;
      ImGui::InputInt("Enemy z-index", &enemy_z_index);

      const static char* sprites[] = { "tank-image", "truck-image"};
      static int32_t current_sprite = 0;
      static std::string enemy_name = "";
      // NOTE: needs to be re-written for when other assets are added
      (current_sprite == 0) ? enemy_name = "Tank" : enemy_name = "Truck";
      ImGui::SeparatorText("Sprite image");
      ImGui::Combo("Sprite selection", &current_sprite, sprites, 2);

      ImGui::SeparatorText("Enemy Transform");
      static float enemy_scale_x = 2.0;
      static float enemy_scale_y = 2.0;
      ImGui::SliderFloat("Enemy X scale", &enemy_scale_x, 0.0, 10.0);
      ImGui::SliderFloat("Enemy Y scale", &enemy_scale_y, 0.0, 10.0);
      static float enemy_rotation = 0;
      ImGui::SliderFloat("Enemy rotation", &enemy_rotation, 0.0, 360);

      ImGui::SeparatorText("Enemy Velocity");
      static float enemy_velocity_x = 0;
      static float enemy_velocity_y = 0;
      ImGui::InputFloat("Enemy Velocity X", &enemy_velocity_x);
      ImGui::InputFloat("Enemy Velocity Y", &enemy_velocity_y);

      ImGui::SeparatorText("Health");
      static int32_t enemy_health = 100;
      static bool enemy_godmode = false;
      ImGui::SliderInt("Enemy health", &enemy_health, 1, 100);
      ImGui::Checkbox("Enemy Godmode", &enemy_godmode);

      // TODO: Angle and speed of projectiles
      // Repeat freq and duration of projectiles (in seconds)

      // creates invis rect for space
      ImGui::Dummy(ImVec2(0, 15));
      
      if (ImGui::Button("Spawn enemy")) {
        Entity new_enemy = registry->create_entity();
        new_enemy.group("enemy");
        new_enemy.add_component<TransformComponent>(glm::vec2(enemy_x_pos, enemy_y_pos), glm::vec2(enemy_scale_x, enemy_scale_y), enemy_rotation);
        new_enemy.add_component<RigidBodyComponent>(glm::vec2(enemy_velocity_x, enemy_velocity_y));
        new_enemy.add_component<SpriteComponent>(sprites[current_sprite], 32, 32, 0, 0, enemy_z_index);
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
