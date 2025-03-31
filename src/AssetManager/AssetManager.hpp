#pragma once

#include <map>
#include <string>
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

class AssetManager {
public:
  AssetManager();
  ~AssetManager();

  void clear_assets();

  void add_texture(SDL_Renderer* renderer, const std::string& asset_id, const std::string& file_path);
  SDL_Texture* get_texture(const std::string& asset_id) const;

  void add_font(const std::string& asset_id, const std::string& file_path, uint16_t font_size);
  TTF_Font* get_font(const std::string& asset_id);

private:
  std::map<std::string, SDL_Texture*> textures;
  std::map<std::string, TTF_Font*> fonts;
  // TODO: Audio map  
};
