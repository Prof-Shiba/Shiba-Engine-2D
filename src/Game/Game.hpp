#pragma once

#include <cstdint>
#include <SDL2/SDL_render.h>
#include <SDL2/SDL_video.h>
#include <SDL2/SDL_image.h>
#include "../ECS/ECS.hpp"
#include "../AssetManager/AssetManager.hpp"
#include "../EventManager/EventManager.hpp"

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
  void LoadLevel(int level);
  void ProcessInput();
  void Update();
  void Render();
  void Destroy();

  static uint16_t WINDOW_WIDTH;
  static uint16_t WINDOW_HEIGHT;
  static uint16_t map_width;
  static uint16_t map_height;
  bool is_running;
  bool debug_enabled;

private:
  SDL_Window* window;
  SDL_Renderer* renderer;
  SDL_Rect camera;
  uint32_t ms_previous_frame = 0;
  std::unique_ptr<Registry> registry;
  std::unique_ptr<AssetManager> asset_manager;
  std::unique_ptr<EventManager> event_manager;
};
