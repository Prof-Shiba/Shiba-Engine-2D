#pragma once

#include <cstdint>
#include <SDL2/SDL_render.h>
#include <SDL2/SDL_video.h>
#include <SDL2/SDL_image.h>
#include "../ECS/ECS.hpp"
#include "../AssetManager/AssetManager.hpp"

const uint16_t FPS = 144;
// 1000ms -> 1 second. Each frame should take 16.6 repeating ms
const uint16_t MS_PER_FRAME = 1000 / FPS;
const int8_t DEFAULT_MONITOR_NUMBER = -1;

class Game {
public:
  Game();
  ~Game();
  void Initialize();
  void Run();
  void Setup();
  void ProcessInput();
  void Update();
  void Render();
  void Destroy();

  uint16_t WIDTH;
  uint16_t HEIGHT;
  bool is_running;

private:
  SDL_Window* window;
  SDL_Renderer* renderer;
  uint32_t ms_previous_frame = 0;
  std::unique_ptr<Registry> registry;
  std::unique_ptr<AssetManager> asset_manager;
};
