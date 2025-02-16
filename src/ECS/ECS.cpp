#include "ECS.hpp"
#include "../Logger/Logger.hpp"
#include <string>

uint8_t I_component::next_id {0};

uint32_t Entity::get_entity_id() const { return entity_id; }
void Entity::remove() { registry->remove_entity(*this); }

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
  uint32_t entity_id;

  if (free_ids.empty()) {
    entity_id = total_num_of_entities++;
    if (entity_id >= entity_component_signatures.size())
      entity_component_signatures.resize(entity_id + 1);
  }
  else {
    entity_id = free_ids.front();
    free_ids.pop_front();
  }

  Entity new_entity(entity_id);
  new_entity.registry = this;
  entities_to_add.insert(new_entity);

  Logger::Log("Entity with ID [" + std::to_string(entity_id) + "] created!");
  return new_entity;
}

void Registry::remove_entity(Entity entity) {
  entities_to_remove.insert(entity);
  Logger::Warn("Removing Entity with ID [" + std::to_string(entity.get_entity_id()) + "]!");
}

void Registry::remove_entity_from_system(Entity entity) {
  for (auto& system: systems)
    system.second->remove_entity_from_system(entity);
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

void Registry::update() {
  for (auto& entity: entities_to_add)
    add_entity_to_system(entity);

  entities_to_add.clear();

  for (auto& entity: entities_to_remove) {
    remove_entity_from_system(entity);
    entity_component_signatures[entity.get_entity_id()].reset();
    free_ids.push_back(entity.get_entity_id()); 
    // NOTE: Probably a better way of doing this ^
    // might end up hogging more heap memory as it doubles in size if we
    // delete many elements at once
  }
  entities_to_remove.clear();
}
