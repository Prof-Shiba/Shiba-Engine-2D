#include "Game.hpp"
#include <SDL2/SDL.h>
#include <iostream>


Game::Game() {
  is_running = false;
  std::cout << "Game Constructor Called" << std::endl;
}

Game::~Game() {
  std::cout << "Game Destructor Called" << std::endl;
}

void Game::Setup() {
  // TODO: Initialize game objects
}

void Game::Initialize() {
  if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
    std::cerr << "SDL failed to Initialize! Line 20 Game.cpp" << std::endl;
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
    std::cerr << "Failed creating SDL window! Line 25 Game.cpp" << std::endl;
    return;
  }

  renderer = SDL_CreateRenderer(window, DEFAULT_MONITOR_NUMBER, 0);
  if (!renderer) {
    std::cerr << "Failed to create SDL renderer! Line 39 Game.cpp" << std::endl;
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

void Game::Update() {
  // TODO: Update game objects
};

void Game::Render() {
  SDL_SetRenderDrawColor(renderer, 21, 21, 21, 255);
  SDL_RenderClear(renderer);
  
  // Loads PNG texture, surface only needed to make texture using it
  // Loading is relative to executable dir, not Game.cpp dir
  SDL_Surface* surface = IMG_Load("./assets/images/tank-tiger-right.png");
  SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
  SDL_FreeSurface(surface);
  
  // Destination rect to place our texture at
  SDL_Rect dstRect {10, 10, 32, 32};
  SDL_RenderCopy(renderer, texture, NULL, &dstRect);
  SDL_DestroyTexture(texture);
  
  // Double buffer
  SDL_RenderPresent(renderer);
};

void Game::Destroy() {
  SDL_DestroyWindow(window);
  SDL_DestroyRenderer(renderer);
  SDL_Quit();
};
