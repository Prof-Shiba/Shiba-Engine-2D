#pragma once

#include <cstdint>
#include <SDL2/SDL_render.h>
#include <SDL2/SDL_video.h>
#include <SDL2/SDL_image.h>

const Uint16 FPS = 144;
// 1000ms -> 1 second. Each frame should take 16.6 repeating ms
const Uint16 MS_PER_FRAME = 1000 / FPS;
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

  Uint16 WIDTH;
  Uint16 HEIGHT;
  bool is_running;

private:
  SDL_Window* window;
  SDL_Renderer* renderer;
  uint32_t ms_previous_frame = 0;
};
