#pragma once
#include "../ECS/ECS.hpp"
#include "../AssetManager/AssetManager.hpp"
#include "../Components/TextComponent.hpp"
#include <SDL2/SDL.h>
#include <SDL2/SDL_render.h>
#include <memory>

class RenderTextSystem : public System {
public:
  RenderTextSystem() {
    require_component<TextComponent>();
  }

  void Update(std::unique_ptr<AssetManager>& asset_manager, SDL_Renderer* renderer, const SDL_Rect& camera) {
    for (auto& entity: get_system_entities()) {
      const auto& text = entity.get_component<TextComponent>();

      SDL_Surface* surface = TTF_RenderText_Blended(asset_manager->get_font(text.asset_id), text.text.c_str(), text.color);
      SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
      SDL_FreeSurface(surface);

      int32_t text_width = 0;
      int32_t text_height = 0;

      SDL_QueryTexture(texture, NULL, NULL, &text_width, &text_height);

      SDL_Rect dst_rect {
        static_cast<int>(text.position.x - (text.is_fixed ? 0 : camera.x)),
        static_cast<int>(text.position.y - (text.is_fixed ? 0 : camera.y)),
        text_width,
        text_height
      };

      SDL_RenderCopy(renderer, texture, NULL, &dst_rect);
    }
  }

};
