#pragma once

#include <SDL2/SDL_render.h>
#include <SDL2/SDL_video.h>

class Game {
public:
  Game();
  ~Game();
  void Initialize();
  void Run();
  void ProcessInput();
  void Update();
  void Render();
  void Destroy();

  Uint16 WIDTH;
  Uint16 HEIGHT;
  const int8_t DEFAULT_MONITOR_NUMBER = -1;
  bool is_running;

private:
  SDL_Window* window;
  SDL_Renderer* renderer;
};
