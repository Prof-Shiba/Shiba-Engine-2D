#pragma once
#include <SDL2/SDL_render.h>
#include "../ECS/ECS.hpp"
#include "../../libs/imgui/imgui.h"
#include "../../libs/imgui/backends/imgui_impl_sdl2.h"
#include "../../libs/imgui/backends/imgui_impl_sdlrenderer2.h"

class RenderGUISystem : public System {
public:
  RenderGUISystem() = default;
  ~RenderGUISystem() = default;

  void Update(SDL_Renderer* renderer) {
    ImGui_ImplSDLRenderer2_NewFrame();
    ImGui_ImplSDL2_NewFrame();
    ImGui::NewFrame();

    if (ImGui::Begin("Spawn Enemies")) {
      ImGui::Text("Spawn enemies here!");
      ImGui::SeparatorText("This is a SeparatorText!");
      ImGui::BulletText("This is some bullet text!");
    }
    ImGui::End();

    ImGui::Render();
    ImGui_ImplSDLRenderer2_RenderDrawData(ImGui::GetDrawData(), renderer);
  }
};
