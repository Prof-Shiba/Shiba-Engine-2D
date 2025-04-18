#pragma once
#include <cstdint>
#include <memory>
#include "../ECS/ECS.hpp"
#include "../AssetManager/AssetManager.hpp"
#include <SDL2/SDL_render.h>
#include "../../libs/sol/sol.hpp"

class LevelLoader {
public:
  LevelLoader();
  ~LevelLoader();

  void LoadLevel(sol::state& lua, uint8_t level, const std::unique_ptr<Registry>& registry, const std::unique_ptr<AssetManager>& asset_manager, SDL_Renderer* renderer);
};
