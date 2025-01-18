#include "ECS.hpp"
#include "../Logger/Logger.hpp"

uint8_t I_component::next_id {0};

uint32_t Entity::get_entity_id() const { return entity_id; }

void System::add_entity_to_system(Entity entity) { entities.push_back(entity); }

void System::remove_entity_from_system(Entity entity) {
  for (auto iterator = entities.begin(); iterator != entities.end(); iterator++) {
    if (iterator->get_entity_id() == entity.get_entity_id()) {
      entities.erase(iterator);
      break;
    }
  }
}

std::vector<Entity> System::get_system_entities() const { return entities; }
const Signature& System::get_component_signature() const { return component_signature; }

Entity Registry::create_entity() {
  uint32_t entity_id = total_num_of_entities++;
  Entity new_entity(entity_id);
  
  entities_to_add.insert(new_entity);
  Logger::Log("Entity with ID [" + std::to_string(entity_id) + "] created!");

  return new_entity;
}

void Registry::update() {

}

void Registry::add_entity_to_system(Entity entity) {
  const auto& entity_id = entity.get_entity_id();
  const auto& entity_component_signature = entity_component_signatures[entity_id];

  for (auto& sys: systems) {
    const auto& system_component_signature = sys.second->get_component_signature();
    bool will_accept = (entity_component_signature & system_component_signature) == system_component_signature;
    
    if (will_accept)
      sys.second->add_entity_to_system(entity);
  }
}
