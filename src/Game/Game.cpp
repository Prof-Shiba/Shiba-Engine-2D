#include <SDL2/SDL_mouse.h>
#include <SDL2/SDL_ttf.h>
#include <cstdint>
#include <memory>
#include <string>
#include "./LevelLoader.hpp"
#include "../ECS/ECS.hpp"
#include "../../libs/glm/glm.hpp"
#include "../Logger/Logger.hpp"
#include "Game.hpp"
#include "../Systems/MovementSystem.hpp"
#include "../Systems/CameraMovementSystem.hpp"
#include "../Systems/RenderSystem.hpp"
#include "../Systems/AnimationSystem.hpp"
#include "../Systems/CollisionSystem.hpp"
#include "../Systems/RenderCollisionSystem.hpp"
#include "../Systems/DamageSystem.hpp"
#include "../Systems/KeyboardMovementSystem.hpp"
#include "../Systems/ProjectileEmitterSystem.hpp"
#include "../Systems/ProjectileDurationSystem.hpp"
#include "../Systems/RenderTextSystem.hpp"
#include "../Systems/MovingTextSystem.hpp"
#include "../Systems/RenderHealthSystem.hpp"
#include "../Systems/RenderGUISystem.hpp"
#include "../../libs/imgui/imgui.h"
#include "../../libs/imgui/backends/imgui_impl_sdl2.h"
#include "../../libs/imgui/backends/imgui_impl_sdlrenderer2.h"

uint16_t Game::WINDOW_HEIGHT;
uint16_t Game::WINDOW_WIDTH;
uint16_t Game::map_height;
uint16_t Game::map_width;

Game::Game() {
  is_running = false;
  debug_enabled = false;

  registry = std::make_unique<Registry>();
  asset_manager = std::make_unique<AssetManager>();
  event_manager = std::make_unique<EventManager>();

  Logger::Log("Game Constructor Called");
}

Game::~Game() {
  Logger::Log("Game Destructor Called");
}

void Game::Initialize() {
  if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
    Logger::Err("SDL failed to Initialize!");
    return;
  }

  if (TTF_Init() != 0) {
    Logger::Err("TTF failed to initialize!");
    return;
  }

  SDL_DisplayMode display_mode;
  SDL_GetCurrentDisplayMode(0, &display_mode);

  // This will be the total area the player can view
  WINDOW_WIDTH = 2560;
  WINDOW_HEIGHT = 1440;

  window = SDL_CreateWindow(
    "Shiba Engine",
    SDL_WINDOWPOS_CENTERED,
    SDL_WINDOWPOS_CENTERED,
    WINDOW_WIDTH,
    WINDOW_HEIGHT,
    SDL_WINDOW_ALWAYS_ON_TOP
  );
  if (!window) {
    Logger::Err("Failed creating SDL window!");
    return;
  }

  renderer = SDL_CreateRenderer(window, DEFAULT_MONITOR_NUMBER, 0);
  if (!renderer) {
    Logger::Err("Failed to create SDL renderer!");
    return;
  }

  IMGUI_CHECKVERSION();
  ImGui::CreateContext();
  ImGuiIO& io = ImGui::GetIO(); (void)io;

  ImGui_ImplSDL2_InitForSDLRenderer(window, renderer);
  ImGui_ImplSDLRenderer2_Init(renderer);

  // ImGui Nord styling
  ImGuiStyle* style = &ImGui::GetStyle();
  ImVec4* colors = style->Colors;
  ImGui::StyleColorsDark(style);          // Reset to base/dark theme
  colors[ImGuiCol_Text]                   = ImVec4(0.85f, 0.87f, 0.91f, 0.88f);
  colors[ImGuiCol_TextDisabled]           = ImVec4(0.49f, 0.50f, 0.53f, 1.00f);
  colors[ImGuiCol_WindowBg]               = ImVec4(0.18f, 0.20f, 0.25f, 1.00f);
  colors[ImGuiCol_ChildBg]                = ImVec4(0.16f, 0.17f, 0.20f, 1.00f);
  colors[ImGuiCol_PopupBg]                = ImVec4(0.23f, 0.26f, 0.32f, 1.00f);
  colors[ImGuiCol_Border]                 = ImVec4(0.14f, 0.16f, 0.19f, 1.00f);
  colors[ImGuiCol_BorderShadow]           = ImVec4(0.09f, 0.09f, 0.09f, 0.00f);
  colors[ImGuiCol_FrameBg]                = ImVec4(0.23f, 0.26f, 0.32f, 1.00f);
  colors[ImGuiCol_FrameBgHovered]         = ImVec4(0.56f, 0.74f, 0.73f, 1.00f);
  colors[ImGuiCol_FrameBgActive]          = ImVec4(0.53f, 0.75f, 0.82f, 1.00f);
  colors[ImGuiCol_TitleBg]                = ImVec4(0.16f, 0.16f, 0.20f, 1.00f);
  colors[ImGuiCol_TitleBgActive]          = ImVec4(0.16f, 0.16f, 0.20f, 1.00f);
  colors[ImGuiCol_TitleBgCollapsed]       = ImVec4(0.16f, 0.16f, 0.20f, 1.00f);
  colors[ImGuiCol_MenuBarBg]              = ImVec4(0.16f, 0.16f, 0.20f, 1.00f);
  colors[ImGuiCol_ScrollbarBg]            = ImVec4(0.18f, 0.20f, 0.25f, 1.00f);
  colors[ImGuiCol_ScrollbarGrab]          = ImVec4(0.23f, 0.26f, 0.32f, 0.60f);
  colors[ImGuiCol_ScrollbarGrabHovered]   = ImVec4(0.23f, 0.26f, 0.32f, 1.00f);
  colors[ImGuiCol_ScrollbarGrabActive]    = ImVec4(0.23f, 0.26f, 0.32f, 1.00f);
  colors[ImGuiCol_CheckMark]              = ImVec4(0.37f, 0.51f, 0.67f, 1.00f);
  colors[ImGuiCol_SliderGrab]             = ImVec4(0.51f, 0.63f, 0.76f, 1.00f);
  colors[ImGuiCol_SliderGrabActive]       = ImVec4(0.37f, 0.51f, 0.67f, 1.00f);
  colors[ImGuiCol_Button]                 = ImVec4(0.18f, 0.20f, 0.25f, 1.00f);
  colors[ImGuiCol_ButtonHovered]          = ImVec4(0.51f, 0.63f, 0.76f, 1.00f);
  colors[ImGuiCol_ButtonActive]           = ImVec4(0.37f, 0.51f, 0.67f, 1.00f);
  colors[ImGuiCol_Header]                 = ImVec4(0.51f, 0.63f, 0.76f, 1.00f);
  colors[ImGuiCol_HeaderHovered]          = ImVec4(0.53f, 0.75f, 0.82f, 1.00f);
  colors[ImGuiCol_HeaderActive]           = ImVec4(0.37f, 0.51f, 0.67f, 1.00f);
  colors[ImGuiCol_SeparatorHovered]       = ImVec4(0.56f, 0.74f, 0.73f, 1.00f);
  colors[ImGuiCol_SeparatorActive]        = ImVec4(0.53f, 0.75f, 0.82f, 1.00f);
  colors[ImGuiCol_ResizeGrip]             = ImVec4(0.53f, 0.75f, 0.82f, 0.86f);
  colors[ImGuiCol_ResizeGripHovered]      = ImVec4(0.61f, 0.74f, 0.87f, 1.00f);
  colors[ImGuiCol_ResizeGripActive]       = ImVec4(0.37f, 0.51f, 0.67f, 1.00f);
  colors[ImGuiCol_Tab]                    = ImVec4(0.18f, 0.20f, 0.25f, 1.00f);
  colors[ImGuiCol_TabHovered]             = ImVec4(0.22f, 0.24f, 0.31f, 1.00f);
  colors[ImGuiCol_TabActive]              = ImVec4(0.23f, 0.26f, 0.32f, 1.00f);
  colors[ImGuiCol_TabUnfocused]           = ImVec4(0.13f, 0.15f, 0.18f, 1.00f);
  colors[ImGuiCol_TabUnfocusedActive]     = ImVec4(0.17f, 0.19f, 0.23f, 1.00f);
  colors[ImGuiCol_PlotHistogram]          = ImVec4(0.56f, 0.74f, 0.73f, 1.00f);
  colors[ImGuiCol_PlotHistogramHovered]   = ImVec4(0.53f, 0.75f, 0.82f, 1.00f);
  colors[ImGuiCol_TextSelectedBg]         = ImVec4(0.37f, 0.51f, 0.67f, 1.00f);
  colors[ImGuiCol_NavHighlight]           = ImVec4(0.53f, 0.75f, 0.82f, 0.86f);
  style->WindowBorderSize                 = 1.00f;
  style->ChildBorderSize                  = 1.00f;
  style->PopupBorderSize                  = 1.00f;
  style->FrameBorderSize                  = 1.00f;

  // Initialize camera view with entire screen area
  camera.x = 0;
  camera.y = 0;
  camera.w = WINDOW_WIDTH;
  camera.h = WINDOW_HEIGHT;

  // Sets the actual video mode to fullscreen, keeping that width from earlier
  // avoids large and smaller monitors/resolutions seeing more or less
  SDL_SetWindowFullscreen(window, SDL_WINDOW_FULLSCREEN);
  is_running = true;
};

void Game::Setup() {
  // Systems
  registry->add_system<MovementSystem>();
  registry->add_system<RenderSystem>();
  registry->add_system<AnimationSystem>();
  registry->add_system<CollisionSystem>();
  registry->add_system<RenderCollisionSystem>();
  registry->add_system<DamageSystem>();
  registry->add_system<KeyboardMovementSystem>();
  registry->add_system<CameraMovementSystem>();
  registry->add_system<ProjectileEmitterSystem>();
  registry->add_system<ProjectileDurationSystem>();
  registry->add_system<RenderTextSystem>();
  registry->add_system<MovingTextSystem>();
  registry->add_system<RenderHealthSystem>();
  registry->add_system<RenderGUISystem>();
  
  LevelLoader loader;
  loader.LoadLevel(1, registry, asset_manager, renderer);
}

void Game::ProcessInput() {
  SDL_Event sdl_event;

  while (SDL_PollEvent(&sdl_event)) {
    ImGui_ImplSDL2_ProcessEvent(&sdl_event);

    switch (sdl_event.type) {
      case SDL_QUIT:
        is_running = false; 
        break;

      case SDL_KEYDOWN:
        event_manager->emit_event<KeyPressedEvent>(sdl_event);

        if (sdl_event.key.keysym.sym == SDLK_ESCAPE) {
          is_running = false;
          break;
        }

        if (sdl_event.key.keysym.sym == SDLK_d) {
          (!debug_enabled) ? debug_enabled = true : debug_enabled = false;
          break;
        }
    }
  }
};

void Game::Update() {
  // Yield resources to OS
  uint32_t time_to_wait = MS_PER_FRAME - (SDL_GetTicks() - ms_previous_frame);
  if (time_to_wait > 0 && time_to_wait <= MS_PER_FRAME)
    SDL_Delay(time_to_wait);
 
  // DT is diff in ticks since last frame, converted to seconds
  double delta_time = (SDL_GetTicks() - ms_previous_frame) / 1000.0;

  current_fps = static_cast<int>(1 / delta_time);

  // Store current frame time
  ms_previous_frame = SDL_GetTicks();

  // Reset event handlers for current frame
  event_manager->reset();

  // Only valid for this current frame
  registry->get_system<MovementSystem>().ListenForEvents(event_manager);
  registry->get_system<DamageSystem>().ListenForEvents(event_manager);
  registry->get_system<KeyboardMovementSystem>().ListenForEvents(event_manager);
  registry->get_system<ProjectileEmitterSystem>().ListenForEvents(event_manager);

  registry->get_system<MovementSystem>().Update(delta_time);
  registry->get_system<AnimationSystem>().Update();
  registry->get_system<CollisionSystem>().Update(event_manager);
  registry->get_system<CameraMovementSystem>().Update(camera);
  registry->get_system<ProjectileEmitterSystem>().Update(registry);
  registry->get_system<ProjectileDurationSystem>().Update();

  // Process entities that are waiting to be created/destroyed
  registry->update();
}

void Game::Render() {
  SDL_SetRenderDrawColor(renderer, 21, 21, 21, 255);
  SDL_RenderClear(renderer);

  registry->get_system<RenderSystem>().Update(renderer, asset_manager, camera);
  registry->get_system<RenderTextSystem>().Update(asset_manager, renderer, camera, current_fps);
  registry->get_system<MovingTextSystem>().Update(asset_manager, renderer, camera);
  registry->get_system<RenderHealthSystem>().Update(renderer, camera);

  if (debug_enabled) {
    registry->get_system<RenderCollisionSystem>().Update(renderer, camera);
    registry->get_system<RenderGUISystem>().Update(renderer, registry);
  }

  // Double buffer
  SDL_RenderPresent(renderer);
};

void Game::Run() {
  Setup();
  while (is_running) {
    ProcessInput();
    Update();
    Render();
  }
};

void Game::Destroy() {
  ImGui_ImplSDLRenderer2_Shutdown();
  ImGui_ImplSDL2_Shutdown();
  ImGui::DestroyContext();

  SDL_DestroyWindow(window);
  SDL_DestroyRenderer(renderer);
  SDL_Quit();
};
