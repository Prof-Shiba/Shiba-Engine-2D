#pragma once

#include <map>
#include <string>
#include <SDL2/SDL.h>

class AssetManager {
  public:
  AssetManager();
  ~AssetManager();

  void clear_assets();
  void add_texture(SDL_Renderer* renderer, const std::string& asset_id, const std::string& file_path);
  SDL_Texture* get_texture(const std::string& asset_id) const;

  private:
    std::map<std::string, SDL_Texture*> textures;
    // TODO: Fonts map  
    // TODO: Audio map  
};
