#include "./LevelLoader.hpp"
#include <fstream>
#include "../Components/TransformComponent.hpp"
#include "../Components/RigidBodyComponent.hpp"
#include "../Components/SpriteComponent.hpp"
#include "../Components/AnimationComponent.hpp"
#include "../Components/BoxColliderComponent.hpp"
#include "../Components/KeyboardControlComponent.hpp"
#include "../Components/CollisionComponent.hpp"
#include "../Components/CameraComponent.hpp"
#include "../Components/HealthComponent.hpp"
#include "../Components/ProjectileEmitterComponent.hpp"
#include "../Components/TextComponent.hpp"
#include "../Components/MovingTextComponent.hpp"
#include "../Components/GodModeComponent.hpp"
#include "./Game.hpp"
#include "../../libs/sol/sol.hpp"

LevelLoader::LevelLoader() {
  Logger::Log("LevelLoader constructor called!");
}

LevelLoader::~LevelLoader() {
  Logger::Log("LevelLoader destructor called!");
}

void LevelLoader::LoadLevel(uint8_t level, const std::unique_ptr<Registry>& registry, const std::unique_ptr<AssetManager>& asset_manager, SDL_Renderer* renderer) {
  sol::state lua;
  lua.open_libraries(sol::lib::base);


  // // The linker will find #includes properly, however, when using images etc you must do it from the
  // // makefiles perspective. It lives in the main dir, outside this /src/Game dir
  // asset_manager->add_texture(renderer, "tank-image", "./assets/images/tank-tiger-right.png");
  // asset_manager->add_texture(renderer, "truck-image", "./assets/images/truck-ford-right.png");
  // asset_manager->add_texture(renderer, "helicopter-image", "./assets/images/chopper-spritesheet.png");
  // asset_manager->add_texture(renderer, "radar-image", "./assets/images/radar.png");
  // asset_manager->add_texture(renderer, "jungle-tilemap", "./assets/tilemaps/jungle.png");
  // asset_manager->add_texture(renderer, "bullet-image", "./assets/images/bullet.png");
  // asset_manager->add_texture(renderer, "tree-image", "./assets/images/tree.png");
  // asset_manager->add_font("charriot-font", "./assets/fonts/charriot.ttf", 16);
  // asset_manager->add_font("arial-font", "./assets/fonts/arial.ttf", 16);
  //
  // const uint8_t TILE_SIZE = 32;
  // uint8_t number_of_map_cols = 25;
  // uint8_t number_of_map_rows = 20;
  // float tile_scale = 3.5;
  //
  // std::ifstream in_file {"./assets/tilemaps/jungle.map"};
  // if (in_file) {
  //   for (int y = 0; y < number_of_map_rows; y++) {
  //     for (int x = 0; x < number_of_map_cols; x++) {
  //       char ch;
  //
  //       in_file.get(ch);
  //       uint16_t src_rect_y = std::atoi(&ch) * TILE_SIZE;
  //
  //       in_file.get(ch);
  //       uint16_t src_rect_x = std::atoi(&ch) * TILE_SIZE;
  //
  //       in_file.ignore();
  //
  //       Entity map_tile = registry->create_entity();
  //       map_tile.group("tile");
  //       map_tile.add_component<TransformComponent>(glm::vec2(x * (tile_scale * TILE_SIZE), y * (tile_scale * TILE_SIZE)), glm::vec2(tile_scale, tile_scale), 0.0);
  //       map_tile.add_component<SpriteComponent>("jungle-tilemap", TILE_SIZE, TILE_SIZE, src_rect_x, src_rect_y, 0, false);
  //     }
  //   }
  // } else {
  //   Logger::Err("Failed opening jungle.map file. Should be in assets/tilemaps/jungle.map");
  // }
  //
  // in_file.close();
  // Game::map_width = number_of_map_cols * TILE_SIZE * tile_scale; 
  // Game::map_height = number_of_map_rows * TILE_SIZE * tile_scale;
  //
  // const SDL_Color COLOR_RED = {255, 0, 0};
  // const SDL_Color COLOR_YELLOW = {255, 255, 0};
  // const SDL_Color COLOR_GREEN = {0, 255, 0};
  //
  // // Entities & Components
  // Entity helicopter = registry->create_entity(); // 500
  // helicopter.tag("player");
  // helicopter.add_component<TransformComponent>(glm::vec2(50, 90), glm::vec2(2.0, 2.0), 0.0);
  // helicopter.add_component<RigidBodyComponent>(glm::vec2(0.0, 0.0));
  // helicopter.add_component<SpriteComponent>("helicopter-image", 32, 32, 0, 0, 3);
  // helicopter.add_component<AnimationComponent>(2, 10, true);
  // helicopter.add_component<KeyboardControlComponent>(glm::vec2(0, -320), glm::vec2(320, 0), glm::vec2(0, 320), glm::vec2(-320, 0));
  // helicopter.add_component<BoxColliderComponent>(32, 32);
  // helicopter.add_component<CollisionComponent>();
  // helicopter.add_component<CameraComponent>();
  // helicopter.add_component<HealthComponent>(100);
  // helicopter.add_component<ProjectileEmitterComponent>(glm::vec2(500, 500), 0, 2000, 10, true);
  // helicopter.add_component<GodModeComponent>(false);
  // helicopter.add_component<MovingTextComponent>(0, -15, "Helicopter", "arial-font", COLOR_GREEN);
  //
  // Entity radar = registry->create_entity();
  // radar.add_component<TransformComponent>(glm::vec2(10, 50), glm::vec2(2.0, 2.0), 0.0);
  // radar.add_component<RigidBodyComponent>(glm::vec2(0.0, 0.0));
  // radar.add_component<SpriteComponent>("radar-image", 64, 64, 0, 0, 4, true);
  // radar.add_component<AnimationComponent>(8, 5, true);
  //
  // Entity tank = registry->create_entity(); // 502
  // tank.group("enemy");
  // tank.add_component<TransformComponent>(glm::vec2(450, 860), glm::vec2(2.0, 2.0), 0.0);
  // tank.add_component<RigidBodyComponent>(glm::vec2(90.0, 0.0));
  // tank.add_component<SpriteComponent>("tank-image", 32, 32, 0, 0, 2); // imgs are 32px, width and height, src rect x, src rect y, then z-index
  // tank.add_component<BoxColliderComponent>(32, 32);
  // tank.add_component<CollisionComponent>();
  // tank.add_component<HealthComponent>(100);
  // tank.add_component<ProjectileEmitterComponent>(glm::vec2(250, 0), 2000, 10000, 10, false);
  // tank.add_component<GodModeComponent>(false);
  // tank.add_component<MovingTextComponent>(7, -10, "Tank", "arial-font", COLOR_RED);
  //
  // Entity truck = registry->create_entity(); // 503
  // truck.group("enemy");
  // truck.add_component<TransformComponent>(glm::vec2(180, 860), glm::vec2(2.0, 2.0), 0.0);
  // truck.add_component<RigidBodyComponent>(glm::vec2(0.0, 00.0));
  // truck.add_component<SpriteComponent>("truck-image", 32, 32, 0, 0, 1);
  // truck.add_component<BoxColliderComponent>(32, 32);
  // truck.add_component<CollisionComponent>();
  // truck.add_component<HealthComponent>(100);
  // truck.add_component<GodModeComponent>(true);
  // truck.add_component<MovingTextComponent>(10, -10, "Truck", "arial-font", COLOR_YELLOW);
  //
  // Entity tree0 = registry->create_entity();
  // tree0.group("object");
  // tree0.add_component<TransformComponent>(glm::vec2(400, 860), glm::vec2(2.0, 2.0), 0.0);
  // tree0.add_component<SpriteComponent>("tree-image", 16, 32, 0, 0, 3);
  // tree0.add_component<BoxColliderComponent>(16, 32);
  // tree0.add_component<CollisionComponent>();
  //
  // Entity tree1 = registry->create_entity();
  // tree1.group("object");
  // tree1.add_component<TransformComponent>(glm::vec2(700, 860), glm::vec2(2.0, 2.0), 0.0);
  // tree1.add_component<SpriteComponent>("tree-image", 16, 32, 0, 0, 3);
  // tree1.add_component<BoxColliderComponent>(16, 32);
  // tree1.add_component<CollisionComponent>();
  //
  // Entity text = registry->create_entity();
  // SDL_Color COLOR_WHITE = {255, 255, 255};
  // text.add_component<TextComponent>(true, glm::vec2(Game::WINDOW_WIDTH / 2 - 60, 0), "Shiba Engine 2D!", "arial-font", COLOR_WHITE);
  //
  // Entity display_fps = registry->create_entity();
  // display_fps.tag("fps");
  // display_fps.add_component<TextComponent>(true, glm::vec2(0, 500), "", "arial-font", COLOR_WHITE);
}
