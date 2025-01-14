#include "ECS.hpp"

size_t Entity::get_entity_id() const { return entity_id; }

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
