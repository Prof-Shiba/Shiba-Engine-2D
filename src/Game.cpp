#include "Game.hpp"
#include <SDL2/SDL.h>
#include <iostream>

const Uint16 WIDTH = 1280;
const Uint16 HEIGHT = 720;
const int8_t DEFAULT_MONITOR_NUMBER = -1;

Game::Game() {
  is_running = false;
  std::cout << "Game Constructor Called" << std::endl;
}

Game::~Game() {
  std::cout << "Game Destructor Called" << std::endl;
}

void Game::Initialize() {
  if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
    std::cerr << "SDL failed to Initialize! Line 20 Game.cpp" << std::endl;
    return;
  }

  window = SDL_CreateWindow(
    "Shibe Engine",
    SDL_WINDOWPOS_CENTERED,
    SDL_WINDOWPOS_CENTERED,
    WIDTH,
    HEIGHT,
    SDL_WINDOW_ALWAYS_ON_TOP
  );
  if (!window) {
    std::cerr << "Failed creating SDL window! Line 25 Game.cpp" << std::endl;
    return;
  }

  renderer = SDL_CreateRenderer(window, DEFAULT_MONITOR_NUMBER, 0);
  if (!renderer) {
    std::cerr << "Failed to create SDL renderer! Line 39 Game.cpp" << std::endl;
    return;
  }

  is_running = true;
};

void Game::Run() {
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

void Game::Update() {

};

void Game::Render() {
  SDL_SetRenderDrawColor(renderer, 0, 0, 255, 255);
  SDL_RenderClear(renderer);

  // Double buffer
  SDL_RenderPresent(renderer);
};

void Game::Destroy() {
  SDL_DestroyWindow(window);
  SDL_DestroyRenderer(renderer);
  SDL_Quit();
};
