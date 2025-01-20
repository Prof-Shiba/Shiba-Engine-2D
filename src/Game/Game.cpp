#include <SDL2/SDL.h>
#include <SDL2/SDL_stdinc.h>
#include <SDL2/SDL_timer.h>
#include <memory>
#include "../ECS/ECS.hpp"
#include "../../libs/glm/glm.hpp"
#include "../Logger/Logger.hpp"
#include "Game.hpp"
#include "../Components/TransformComponent.hpp"
#include "../Components/RigidBodyComponent.hpp"

Game::Game() {
  is_running = false;
  registry = std::make_unique<Registry>();
  Logger::Log("Game Constructor Called");
}

Game::~Game() {
  Logger::Log("Game Destructor Called");
}

void Game::Setup() {
  Entity tank = registry->create_entity();

  registry->add_component<TransformComponent>(tank, glm::vec2(10, 10), glm::vec2(1.0, 1.0), 0.0);
  registry->add_component<RigidBodyComponent>(tank, glm::vec2(10.0, 0.0));
}

void Game::Update() {
  // Yield resources to OS
  Uint32 time_to_wait = MS_PER_FRAME - (SDL_GetTicks() - ms_previous_frame);
  if (time_to_wait > 0 && time_to_wait <= MS_PER_FRAME)
    SDL_Delay(time_to_wait);
 
  // DT is diff in ticks since last frame, converted to seconds
  double delta_time = (SDL_GetTicks() - ms_previous_frame) / 1000.0;

  // Store current frame time
  ms_previous_frame = SDL_GetTicks();

  // TODO: 
  // Movement_System.update();
  // Collision_System.update();
  // etc
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
  /*SDL_SetWindowFullscreen(window, SDL_WINDOW_FULLSCREEN);*/
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
        }
        break;
    }
  }
};


void Game::Render() {
  SDL_SetRenderDrawColor(renderer, 21, 21, 21, 255);
  SDL_RenderClear(renderer);
  
  // TODO: Render game objects

  // Double buffer
  SDL_RenderPresent(renderer);
};

void Game::Destroy() {
  SDL_DestroyWindow(window);
  SDL_DestroyRenderer(renderer);
  SDL_Quit();
};
