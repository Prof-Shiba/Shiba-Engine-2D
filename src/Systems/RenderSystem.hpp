#pragma once
#include "../ECS/ECS.hpp"
#include "../Components/TransformComponent.hpp"
#include "../Components/SpriteComponent.hpp"
#include <SDL2/SDL.h>
#include <SDL2/SDL_rect.h>
#include <SDL2/SDL_render.h>
#include "../AssetManager/AssetManager.hpp"
#include <algorithm>
#include <memory>

class RenderSystem : public System {
public:
  RenderSystem() {
    require_component<SpriteComponent>();
    require_component<TransformComponent>();
  }

  RenderSystem(const RenderSystem&) = default;
  ~RenderSystem() = default;

  // I hate sorting here. I will change it later, but I just want to get a feel
  // for how doing it works first.
  void Update(SDL_Renderer* renderer, std::unique_ptr<AssetManager>& asset_manager) {
    // for every entity in get_system_entities ?
    // sort by the lowest z_index
    // NOTE: Can i even do that? The question is:
    // how can i get all the entity IDs in get_system_entities
    // and sort by the lowest, or can i even do that to begin with?
    for (auto& entity: get_system_entities()) {
      uint8_t z = entity.get_component<SpriteComponent>().z_index;
    }

    for (auto entity: get_system_entities()) {
      const auto transform = entity.get_component<TransformComponent>();
      const auto sprite = entity.get_component<SpriteComponent>();

      // Set source rectangle of OG sprite texture, needed for RenderCopy
      SDL_Rect source_rect = sprite.src_rect;

      SDL_Rect destination_rect = {
        static_cast<int>(transform.position.x),
        static_cast<int>(transform.position.y),
        static_cast<int>(sprite.width * transform.scale.x),
        static_cast<int>(sprite.height * transform.scale.y)
      };

      SDL_RenderCopyEx(renderer, asset_manager->get_texture(sprite.asset_id), 
                     &source_rect, &destination_rect, transform.rotation,
                     NULL, SDL_FLIP_NONE);
    }
  }

private:
  
};
