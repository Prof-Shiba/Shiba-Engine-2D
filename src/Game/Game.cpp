#include <SDL2/SDL.h>
#include <SDL2/SDL_stdinc.h>
#include <SDL2/SDL_timer.h>
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
#include "../Systems/MovementSystem.hpp"
#include "../Systems/RenderSystem.hpp"
#include "../Systems/AnimationSystem.hpp"
#include "../Systems/CollisionSystem.hpp"
#include "../Systems/RenderCollisionSystem.hpp"


Game::Game() {
  is_running = false;

  registry = std::make_unique<Registry>();
  asset_manager = std::make_unique<AssetManager>();

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

  // The linker will find #includes properly, however, when using images etc you must do it from the
  // makefiles perspective. It lives in the main dir, outside this /src/Game dir
  asset_manager->add_texture(renderer, "tank-image", "./assets/images/tank-tiger-right.png");
  asset_manager->add_texture(renderer, "truck-image", "./assets/images/truck-ford-right.png");
  asset_manager->add_texture(renderer, "helicopter-image", "./assets/images/chopper.png");
  asset_manager->add_texture(renderer, "radar-image", "./assets/images/radar.png");
  asset_manager->add_texture(renderer, "jungle-tilemap", "./assets/tilemaps/jungle.png");

  const uint8_t TILE_SIZE = 32;
  uint8_t number_of_map_cols = 25;
  uint8_t number_of_map_rows = 20;
  float tile_scale = 3.5;

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
        map_tile.add_component<TransformComponent>(glm::vec2(x * (tile_scale * TILE_SIZE), y * (tile_scale * TILE_SIZE)), glm::vec2(tile_scale, tile_scale), 0.0);
        map_tile.add_component<SpriteComponent>("jungle-tilemap", TILE_SIZE, TILE_SIZE, src_rect_x, src_rect_y, 0);
      }
    }
  } else {
    Logger::Err("Failed opening jungle.map file. Should be in assets/tilemaps/jungle.map");
  }

  in_file.close();

  // Entities & Components
  Entity helicopter = registry->create_entity();
  helicopter.add_component<TransformComponent>(glm::vec2(50, 90), glm::vec2(2.0, 2.0), 0.0);
  helicopter.add_component<RigidBodyComponent>(glm::vec2(100.0, 0.0));
  helicopter.add_component<SpriteComponent>("helicopter-image", 32, 32, 0, 0, 3);
  helicopter.add_component<AnimationComponent>(2, 10, true);

  Entity radar = registry->create_entity();
  radar.add_component<TransformComponent>(glm::vec2(50, 100), glm::vec2(2.0, 2.0), 0.0);
  radar.add_component<RigidBodyComponent>(glm::vec2(0.0, 0.0));
  radar.add_component<SpriteComponent>("radar-image", 64, 64, 0, 0, 4);
  radar.add_component<AnimationComponent>(8, 5, true);

  Entity tank = registry->create_entity();
  tank.add_component<TransformComponent>(glm::vec2(100, 10), glm::vec2(2.0, 2.0), 0.0);
  tank.add_component<RigidBodyComponent>(glm::vec2(50.0, 0.0));
  tank.add_component<SpriteComponent>("tank-image", 32, 32, 0, 0, 2); // imgs are 32px, width and height, src rect x, src rect y, then z-index
  tank.add_component<BoxColliderComponent>(60, 60);

  Entity truck = registry->create_entity();
  truck.add_component<TransformComponent>(glm::vec2(300, 10), glm::vec2(2.0, 2.0), 0.0);
  truck.add_component<RigidBodyComponent>(glm::vec2(-50.0, 00.0));
  truck.add_component<SpriteComponent>("truck-image", 32, 32, 0, 0, 1);
  truck.add_component<BoxColliderComponent>(60, 50);
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

  // Store current frame time
  ms_previous_frame = SDL_GetTicks();

  registry->get_system<MovementSystem>().Update(delta_time);
  registry->get_system<AnimationSystem>().Update();
  registry->get_system<CollisionSystem>().Update(is_colliding);

  // Process entities that are waiting to be created/destroyed
  registry->update();
};

void Game::Render() {
  SDL_SetRenderDrawColor(renderer, 21, 21, 21, 255);
  SDL_RenderClear(renderer);
  
  registry->get_system<RenderSystem>().Update(renderer, asset_manager);

  if (debug_enabled)
    registry->get_system<RenderCollisionSystem>().Update(renderer, is_colliding);

  // Double buffer
  SDL_RenderPresent(renderer);
};

void Game::Initialize() {
  if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
    Logger::Err("SDL failed to Initialize!");
    return;
  }

  SDL_DisplayMode display_mode;
  SDL_GetCurrentDisplayMode(0, &display_mode);

  // This will be the total area the player can view
  WIDTH = 1280;
  HEIGHT = 720;

  window = SDL_CreateWindow(
    "Shibe Engine",
    SDL_WINDOWPOS_CENTERED,
    SDL_WINDOWPOS_CENTERED,
    WIDTH,
    HEIGHT,
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
    switch (sdl_event.type) {
      case SDL_QUIT:
        is_running = false; 
        break;

      case SDL_KEYDOWN:
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
  SDL_DestroyWindow(window);
  SDL_DestroyRenderer(renderer);
  SDL_Quit();
};
