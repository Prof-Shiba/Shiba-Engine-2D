#pragma once

#include <cstdint>
#include <bitset>
#include <vector>

///////////////////////////////////////////////////////////////
// bitset tracks which components an entity has
// also helps track which entities a system is interested in
///////////////////////////////////////////////////////////////
const uint8_t MAX_COMPONENTS = 32;
typedef std::bitset<MAX_COMPONENTS> Signature;

///////////////////////////////////////////////////////////////
// Components store pure data that can be manipulated
// by the registry
///////////////////////////////////////////////////////////////
struct I_component {
protected:
  static size_t next_id;
};

// assign unique id to a comp type
template <typename T>
class Component : public I_component {
public:
  Component() = default;
  ~Component() = default;
  Component(const Component&) = default;

  size_t get_component_id() const;

private:
  size_t component_id;
  // returns unique id of comp <T>
  static size_t get_id() {
    static auto id = next_id++;
    return id;
  }
};

///////////////////////////////////////////////////////////////
// Entity is an ID representing an object in the world 
///////////////////////////////////////////////////////////////
class Entity {
public:
  Entity(size_t id) : entity_id{id} {}
  ~Entity() = default;
  Entity(const Entity&) = default;

  size_t get_entity_id() const;

private:
  size_t entity_id;
};

///////////////////////////////////////////////////////////////
// System processes entities with a certain Signature
///////////////////////////////////////////////////////////////
class System {
public:
  System() = default;
  ~System() = default;
  System(const System&) = default;
  
  void add_entity_to_system(Entity entity);
  void remove_entity_from_system(Entity entity);
  std::vector<Entity> get_system_entities() const;
  const Signature& get_component_signature() const;
  template<typename T_component> void require_component();

private:
  Signature component_signature;
  std::vector<Entity> entities;
};

///////////////////////////////////////////////////////////////
// The registry can manipulate an entity and its components
///////////////////////////////////////////////////////////////
class Registry {
public:
  Registry() = default;
  ~Registry() = default;
  Registry(const Registry&) = default;

private:
  size_t total_num_of_entities {0};
  
};

template <typename T_component>
void System::require_component() {
  const auto component_id = Component<T_component>::get_component_id();
  component_signature.set(component_id);
}
