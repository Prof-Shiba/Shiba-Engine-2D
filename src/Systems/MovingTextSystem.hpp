#pragma once
#include "../ECS/ECS.hpp"
#include "../AssetManager/AssetManager.hpp"
#include "../Components/MovingTextComponent.hpp"
#include "../Components/TransformComponent.hpp"
#include <SDL2/SDL.h>
#include <SDL2/SDL_render.h>

class MovingTextSystem : public System {
public:
  MovingTextSystem() {
    require_component<MovingTextComponent>();
    require_component<TransformComponent>();
  }

  void Update(std::unique_ptr<AssetManager>& asset_manager, SDL_Renderer* renderer, const SDL_Rect& camera) {
    for (auto& entity: get_system_entities()) {
      const auto& text = entity.get_component<MovingTextComponent>();
      const auto& transform = entity.get_component<TransformComponent>();

      SDL_Surface* surface = TTF_RenderText_Blended(asset_manager->get_font(text.asset_id), text.text.c_str(), text.color);
      SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
      SDL_FreeSurface(surface);

      int32_t text_width = 0;
      int32_t text_height = 0;

      SDL_QueryTexture(texture, NULL, NULL, &text_width, &text_height);

      SDL_Rect dst_rect {
        static_cast<int>((transform.position.x + text.offset_x) - camera.x),
        static_cast<int>((transform.position.y + text.offset_y) - camera.y),
        text_width,
        text_height
      };

      SDL_RenderCopy(renderer, texture, NULL, &dst_rect);
    }
  }

};
