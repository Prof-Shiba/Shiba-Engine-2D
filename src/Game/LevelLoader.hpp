#pragma once
#include <cstdint>
#include <memory>
#include "../ECS/ECS.hpp"
#include "../AssetManager/AssetManager.hpp"
#include <SDL2/SDL_render.h>

class LevelLoader {
public:
  LevelLoader();
  ~LevelLoader();

  void LoadLevel(uint8_t level, const std::unique_ptr<Registry>& registry, const std::unique_ptr<AssetManager>& asset_manager, SDL_Renderer* renderer);
};
