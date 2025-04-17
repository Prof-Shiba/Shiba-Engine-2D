#pragma once
#include "../ECS/ECS.hpp"
#include "../Components/TransformComponent.hpp"
#include "../Components/SpriteComponent.hpp"
#include "../AssetManager/AssetManager.hpp"
#include <SDL2/SDL.h>
#include <SDL2/SDL_rect.h>
#include <SDL2/SDL_render.h>
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

  // TODO: In future have layers, background layer, foreground, HUD, etc 
  // if entity in layer HUD -> render first, background 2nd, etc.
  // Pass in the layers to SpriteComponent instead of each individual z_index
 
  // NOTE: under what conditions would std::sort actually need to be called?
  // how about only sorting when a new entity is added?
  void Update(SDL_Renderer* renderer, std::unique_ptr<AssetManager>& asset_manager, SDL_Rect& camera) {
    auto entities = get_system_entities();

    std::sort(entities.begin(), entities.end(), [](const Entity& lhs, const Entity& rhs) {
       return lhs.get_component<SpriteComponent>().z_index < rhs.get_component<SpriteComponent>().z_index;
    });

    for (auto& entity: entities) {
      const auto& transform = entity.get_component<TransformComponent>();
      const auto& sprite = entity.get_component<SpriteComponent>();

      bool entity_outside_camera_view = (
        transform.position.x + (transform.scale.x * sprite.width) < camera.x ||
        transform.position.x > camera.x + camera.w ||
        transform.position.y + (transform.scale.y * sprite.height) < camera.y ||
        transform.position.y > camera.y + camera.h
      );

      if (entity_outside_camera_view && !sprite.is_fixed) continue;

      // Set source rectangle of OG sprite texture, needed for RenderCopy
      SDL_Rect source_rect = sprite.src_rect;

      SDL_Rect destination_rect = {
        static_cast<int>(transform.position.x - (sprite.is_fixed ? 0 : camera.x)),
        static_cast<int>(transform.position.y - (sprite.is_fixed ? 0 : camera.y)),
        static_cast<int>(sprite.width * transform.scale.x),
        static_cast<int>(sprite.height * transform.scale.y)
      };

      SDL_RenderCopyEx(renderer, asset_manager->get_texture(sprite.asset_id), 
                     &source_rect, &destination_rect, transform.rotation,
                     NULL, sprite.flip);
    }
  }

private:
  
};
