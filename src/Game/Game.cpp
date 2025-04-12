#include <SDL2/SDL_mouse.h>
#include <SDL2/SDL_ttf.h>
#include <cstdint>
#include <memory>
#include <fstream>
#include <string>
#include "../ECS/ECS.hpp"
#include "../../libs/glm/glm.hpp"
#include "../Logger/Logger.hpp"
#include "Game.hpp"
#include "../Components/TransformComponent.hpp"
#include "../Components/RigidBodyComponent.hpp"
#include "../Components/SpriteComponent.hpp"
#include "../Components/AnimationComponent.hpp"
#include "../Components/BoxColliderComponent.hpp"
#include "../Components/KeyboardControlComponent.hpp"
#include "../Components/CollisionComponent.hpp"
#include "../Components/CameraComponent.hpp"
#include "../Components/HealthComponent.hpp"
#include "../Components/ProjectileEmitterComponent.hpp"
#include "../Components/TextComponent.hpp"
#include "../Components/MovingTextComponent.hpp"
#include "../Components/GodModeComponent.hpp"
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

void Game::LoadLevel(int level) {
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

  // The linker will find #includes properly, however, when using images etc you must do it from the
  // makefiles perspective. It lives in the main dir, outside this /src/Game dir
  asset_manager->add_texture(renderer, "tank-image", "./assets/images/tank-tiger-right.png");
  asset_manager->add_texture(renderer, "truck-image", "./assets/images/truck-ford-right.png");
  asset_manager->add_texture(renderer, "helicopter-image", "./assets/images/chopper-spritesheet.png");
  asset_manager->add_texture(renderer, "radar-image", "./assets/images/radar.png");
  asset_manager->add_texture(renderer, "jungle-tilemap", "./assets/tilemaps/jungle.png");
  asset_manager->add_texture(renderer, "bullet-image", "./assets/images/bullet.png");
  asset_manager->add_font("charriot-font", "./assets/fonts/charriot.ttf", 16);
  asset_manager->add_font("arial-font", "./assets/fonts/arial.ttf", 16);

  const uint8_t TILE_SIZE = 32;
  uint8_t number_of_map_cols = 25;
  uint8_t number_of_map_rows = 20;
  float tile_scale = 3.5;

  // FIXME: Some race condition or smth similar here. Occasionally
  // will not fully load the background, it's just black.
  // everything else always loads though
  std::ifstream in_file {"./assets/tilemaps/jungle.map"};
  if (in_file) {
    for (int y = 0; y < number_of_map_rows; y++) {
      for (int x = 0; x < number_of_map_cols; x++) {
        char ch;

        in_file.get(ch);
        uint16_t src_rect_y = std::atoi(&ch) * TILE_SIZE;

        in_file.get(ch);
        uint16_t src_rect_x = std::atoi(&ch) * TILE_SIZE;

        in_file.ignore();

        Entity map_tile = registry->create_entity();
        map_tile.group("tile");
        map_tile.add_component<TransformComponent>(glm::vec2(x * (tile_scale * TILE_SIZE), y * (tile_scale * TILE_SIZE)), glm::vec2(tile_scale, tile_scale), 0.0);
        map_tile.add_component<SpriteComponent>("jungle-tilemap", TILE_SIZE, TILE_SIZE, src_rect_x, src_rect_y, 0, false);
      }
    }
  } else {
    Logger::Err("Failed opening jungle.map file. Should be in assets/tilemaps/jungle.map");
  }

  in_file.close();
  map_width = number_of_map_cols * TILE_SIZE * tile_scale; 
  map_height = number_of_map_rows * TILE_SIZE * tile_scale;
  
  const SDL_Color COLOR_RED = {255, 0, 0};
  const SDL_Color COLOR_YELLOW = {255, 255, 0};
  const SDL_Color COLOR_GREEN = {0, 255, 0};

  // Entities & Components
  Entity helicopter = registry->create_entity(); // 500
  helicopter.tag("player");
  helicopter.add_component<TransformComponent>(glm::vec2(50, 90), glm::vec2(2.0, 2.0), 0.0);
  helicopter.add_component<RigidBodyComponent>(glm::vec2(0.0, 0.0));
  helicopter.add_component<SpriteComponent>("helicopter-image", 32, 32, 0, 0, 3);
  helicopter.add_component<AnimationComponent>(2, 10, true);
  helicopter.add_component<KeyboardControlComponent>(glm::vec2(0, -360), glm::vec2(360, 0), glm::vec2(0, 360), glm::vec2(-360, 0));
  helicopter.add_component<BoxColliderComponent>(60, 60);
  helicopter.add_component<CollisionComponent>();
  helicopter.add_component<CameraComponent>();
  helicopter.add_component<HealthComponent>(100);
  helicopter.add_component<ProjectileEmitterComponent>(glm::vec2(500, 500), 0, 2000, 10, true);
  helicopter.add_component<GodModeComponent>(false);
  helicopter.add_component<MovingTextComponent>(0, -15, "Helicopter", "arial-font", COLOR_GREEN);

  Entity radar = registry->create_entity();
  radar.add_component<TransformComponent>(glm::vec2(0, 80), glm::vec2(2.0, 2.0), 0.0);
  radar.add_component<RigidBodyComponent>(glm::vec2(0.0, 0.0));
  radar.add_component<SpriteComponent>("radar-image", 64, 64, 0, 0, 4, true);
  radar.add_component<AnimationComponent>(8, 5, true);

  Entity tank = registry->create_entity(); // 502
  tank.group("enemy");
  tank.add_component<TransformComponent>(glm::vec2(100, 30), glm::vec2(2.0, 2.0), 0.0);
  tank.add_component<RigidBodyComponent>(glm::vec2(0.0, 0.0));
  tank.add_component<SpriteComponent>("tank-image", 32, 32, 0, 0, 2); // imgs are 32px, width and height, src rect x, src rect y, then z-index
  tank.add_component<BoxColliderComponent>(60, 60);
  tank.add_component<CollisionComponent>();
  tank.add_component<HealthComponent>(100);
  tank.add_component<ProjectileEmitterComponent>(glm::vec2(100, 0), 5000, 10000, 10, false);
  tank.add_component<GodModeComponent>(false);
  tank.add_component<MovingTextComponent>(7, -10, "Tank", "arial-font", COLOR_RED);

  Entity truck = registry->create_entity(); // 503
  truck.group("enemy");
  truck.add_component<TransformComponent>(glm::vec2(300, 30), glm::vec2(2.0, 2.0), 0.0);
  truck.add_component<RigidBodyComponent>(glm::vec2(0.0, 00.0));
  truck.add_component<SpriteComponent>("truck-image", 32, 32, 0, 0, 1);
  truck.add_component<BoxColliderComponent>(60, 50);
  truck.add_component<CollisionComponent>();
  truck.add_component<HealthComponent>(100);
  truck.add_component<ProjectileEmitterComponent>(glm::vec2(0, 100), 2000, 5000, 10, false);
  truck.add_component<GodModeComponent>(true);
  truck.add_component<MovingTextComponent>(10, -10, "Truck", "arial-font", COLOR_YELLOW);

  Entity text = registry->create_entity();
  SDL_Color COLOR_WHITE = {255, 255, 255};
  text.add_component<TextComponent>(true, glm::vec2(WINDOW_WIDTH / 2 - 60, 0), "Shibe Engine 2D!", "arial-font", COLOR_WHITE);

  Entity display_fps = registry->create_entity();
  display_fps.tag("fps");
  display_fps.add_component<TextComponent>(true, glm::vec2(0, 500), "", "arial-font", COLOR_WHITE);
}

void Game::Setup() {
  LoadLevel(1);
}

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
  
    ImGui_ImplSDLRenderer2_NewFrame();
    ImGui_ImplSDL2_NewFrame();

    ImGui::NewFrame();
    ImGui::ShowDemoWindow();
    ImGui::Render();

    ImGui_ImplSDLRenderer2_RenderDrawData(ImGui::GetDrawData(), renderer);
  }

  // Double buffer
  SDL_RenderPresent(renderer);
};

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
    "Shibe Engine",
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

void Game::Run() {
  Setup();
  while (is_running) {
    ProcessInput();
    Update();
    Render();
  }
};

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

void Game::Destroy() {
  ImGui_ImplSDLRenderer2_Shutdown();
  ImGui_ImplSDL2_Shutdown();
  ImGui::DestroyContext();

  SDL_DestroyWindow(window);
  SDL_DestroyRenderer(renderer);
  SDL_Quit();
};
