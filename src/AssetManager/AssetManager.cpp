#include "./AssetManager.hpp"
#include "../Logger/Logger.hpp"
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_render.h>
#include <SDL2/SDL_surface.h>

AssetManager::AssetManager() { Logger::Log("AssetManager Constructor called!"); }

AssetManager::~AssetManager() { 
  clear_assets();
  Logger::Log("AssetManager Destructor called!"); 
}

void AssetManager::clear_assets() {
  for (auto texture: textures)
    SDL_DestroyTexture(texture.second);

  // This only clears the map, not the SDL
  // heap memory! Hence above^
  textures.clear();
}

void AssetManager::add_texture(SDL_Renderer* renderer, const std::string& asset_id, const std::string& file_path) {
  SDL_Surface* surface = IMG_Load(file_path.c_str());
  SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
  SDL_FreeSurface(surface);

  textures.emplace(asset_id, texture);

  Logger::Log("New texture added to asset manager with ID [" + asset_id + "]!");
}

// TODO: Add error checking later
SDL_Texture* AssetManager::get_texture(const std::string& asset_id) const { return textures.at(asset_id); }
