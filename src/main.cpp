#include <iostream>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_mixer.h>
#include "../libs/glm/glm.hpp"
#include "../libs/imgui/imgui.h"
#include <sol/sol.hpp>
#include <sol/types.hpp>

int main()
{
  // Test that everything works
  SDL_Init(SDL_INIT_EVERYTHING);
  sol::state lua;
  lua.open_libraries(sol::lib::base);

  auto velocity = glm::vec2(5.0, -2.0);

  std::cout << "buh" << std::endl;

  return 0;
}
