#pragma once

#include <cstdint>
#include <bitset>
#include <memory>
#include <string>
#include <typeindex>
#include <unordered_map>
#include <utility>
#include <vector>
#include <set>
#include "../Logger/Logger.hpp"

///////////////////////////////////////////////////////////////
// bitset tracks which components an entity has, and helps
// track which entities a system is interested in
///////////////////////////////////////////////////////////////
const uint8_t MAX_COMPONENTS = 32;
typedef std::bitset<MAX_COMPONENTS> Signature;

///////////////////////////////////////////////////////////////
// Components store pure data that can be manipulated by the
// registry. Ensures diff comps (health, position, sprite etc)
// will be assigned a unique id.
///////////////////////////////////////////////////////////////
struct I_component {
protected:
  static uint8_t next_id;
};

// assign unique id to a comp type
template <typename T>
class Component : public I_component {
public:
  Component() = default;
  ~Component() = default;
  Component(const Component&) = default;

  static uint8_t get_component_id() {
    static auto id = next_id++;
    return id;
  }
};

///////////////////////////////////////////////////////////////
// Entity is an ID representing an object in the world
// (actors in UE)
//
// it's just a wrapper for a number and for accessing what
// components are grouped together, so we can track things.
//
// (if renderer needs to render a SpriteComponent it needs 
// to know what transform component is connected to that etc)
///////////////////////////////////////////////////////////////
class Entity {
public:
  Entity(uint32_t id) : entity_id{id} {}
  ~Entity() = default;
  Entity(const Entity&) = default;

  Entity& operator=(const Entity& other) = default;
  bool operator<(const Entity& other) const { return entity_id < other.get_entity_id(); }
  bool operator>(const Entity& other) const { return entity_id > other.get_entity_id(); }
  bool operator==(const Entity& other) const { return entity_id == other.get_entity_id(); }
  bool operator!=(const Entity& other) const { return entity_id != other.get_entity_id(); }

  // Each entity can hold a pointer to it's registry owner
  // giving access to registry methods directly
  class Registry* registry;

  template <typename T_component, typename ...T_args> void add_component(T_args&& ...args);
  template <typename T_component> void remove_component();
  template <typename T_component> bool has_component() const;
  template <typename T_component> T_component& get_component() const;

  uint32_t get_entity_id() const;

private:
  uint32_t entity_id;
};

///////////////////////////////////////////////////////////////
// System processes entities with a certain Signature
//
// MovementSystem might require a MovementComponent,
// CollisionSystem might require ColliderComponent, etc.
//
// RendererMeshSystem might find all meshes in the scene and
// submit them to the Renderer to be rendered.
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
// The pool is a storage container. Components of the same type
// are stored here in one area, which utilizes CPU cache making
// it a ton faster as opposed to random cold pointers in RAM.
//
// Must use an interface class as we don't know the types yet
// for the registry class!
///////////////////////////////////////////////////////////////
class I_Pool {
public:
  virtual ~I_Pool() = default;
};

template <typename T>
class Pool : public I_Pool {
public:
  Pool(size_t size = 100) { data.resize(size); }
  virtual ~Pool() = default;
  bool is_empty() { return data.empty(); }
  size_t get_size() { return data.size(); }
  void resize(size_t size) { data.resize(size); }
  void clear() {data.clear(); }
  void add(T obj) { data.push_back(obj); }
  void set_new_index(size_t index, T obj) { data[index] = obj; }
  T& get_at_index(size_t index) { return static_cast<T&>( data[index] ); }
  T& operator[](size_t index) { return data[index]; }

private:
  std::vector<T> data;
};

///////////////////////////////////////////////////////////////
// The registry can manipulate an entity and its components
// it's what the game code will interact with to do things
//
// It coordinates all the other parts of the ECS, and is the
// main interface everything will go through
///////////////////////////////////////////////////////////////
class Registry {
public:
  Registry() { Logger::Log("Registry Constructor called!"); }
  ~Registry() { Logger::Log("Registry Destructor called!"); }
  Registry(const Registry&) = default;

  // Entity management
  Entity create_entity();
  void add_entity_to_system(Entity entity);

  // Component management
  template <typename T_component, typename ...T_Args> void add_component(Entity entity, T_Args&& ...T_args);
  template <typename T_component> void remove_component(Entity entity);
  template <typename T_component> bool has_component(Entity entity) const;
  template <typename T_component> T_component& get_component(Entity entity) const;

  // System management
  template <typename T_system, typename ...T_Args> void add_system(T_Args&& ...T_args);
  template <typename T_system> void remove_system();
  template <typename T_system> bool has_system() const;
  template <typename T_system> T_system& get_system() const;

  void update();

private:
  uint32_t total_num_of_entities {0};
  // Each pool contains all the data of a certain comp type
  // Vector index is component type ID
  // Pool index is entity id
  std::vector<std::shared_ptr<I_Pool>> component_pool;

  // Vector index = entity id
  std::vector<Signature> entity_component_signatures;

  std::unordered_map<std::type_index, std::shared_ptr<System>> systems;
  std::set<Entity> entities_to_add;
  std::set<Entity> entities_to_remove;
};

/////////////////////////////////////////////////////////////
// Templates below 
///////////////////////////////////////////////////////////////
template <typename T_component>
void System::require_component() {
  const auto component_id = Component<T_component>::get_component_id();
  component_signature.set(component_id);
}


template <typename T_component, typename ...TArgs>
void Registry::add_component(Entity entity, TArgs&& ...args) {
  const auto component_id = Component<T_component>::get_component_id();
  const auto entity_id = entity.get_entity_id();

  if (component_id >= component_pool.size())
    component_pool.resize(component_id + 1, nullptr);
  
  // If we don't have a pool for that comp type, make it
  if (!component_pool[component_id]) {
    auto new_comp_pool = std::make_shared<Pool<T_component>>();
    component_pool[component_id] = new_comp_pool;
  }

  // get the pool of comp values for that comp type
  auto current_comp_pool = std::static_pointer_cast<Pool<T_component>>(component_pool[component_id]);

  if (entity_id >= current_comp_pool->get_size())
    current_comp_pool->resize(total_num_of_entities);

  // Create new comp obj of type T_comp, and fwrd the various
  // params to the constructor
  T_component new_component(std::forward<TArgs>(args)...);
  current_comp_pool->set_new_index(entity_id, new_component);

  // Update the comp sig of the entity and set comp id on bitset to 1
  entity_component_signatures[entity_id].set(component_id);

  Logger::Log("Component with ID [" + std::to_string(component_id) + "] added to Entity ID [" + std::to_string(entity_id) + "]");
}

template <typename T_component>
void Registry::remove_component(Entity entity) {
  const auto component_id = Component<T_component>::get_component_id();
  const auto entity_id = entity.get_entity_id();

  if (has_component<T_component>(entity)) {

    entity_component_signatures[entity_id].set(component_id, false);
    Logger::Log("Removed component [" + std::to_string(component_id) + "] successfully from Entity ID [" + std::to_string(entity_id) + "]!");
    } 
  else {
    Logger::Err("Failed removing component with ID [" + std::to_string(component_id) + "] from Entity with ID [" + std::to_string(entity_id) + "]. Entity is missing component!");
  }
}

template <typename T_component>
bool Registry::has_component(Entity entity) const {
  const auto component_id = Component<T_component>::get_component_id();
  const auto entity_id = entity.get_entity_id();

  return entity_component_signatures[entity_id].test(component_id);
}

template <typename T_component>
T_component& Registry::get_component(Entity entity) const {
  const auto component_id = Component<T_component>::get_component_id();
  const auto entity_id = entity.get_entity_id();

  auto comp_pool = std::static_pointer_cast<Pool<T_component>>(component_pool[component_id]);
  return comp_pool->get_at_index(entity_id);
}

template <typename T_system, typename ...T_Args>
void Registry::add_system(T_Args&& ...T_args) {
  auto new_system = std::make_shared<T_system>(std::forward<T_Args>(T_args)...);
  systems.insert(std::make_pair(std::type_index(typeid(T_system)), new_system));
}

template <typename T_system>
void Registry::remove_system() {
  auto system = systems.find(std::type_index(typeid(T_system)));
  systems.erase(system);
}

template <typename T_system>
bool Registry::has_system() const {
  return systems.find(std::type_index(typeid(T_system))) != systems.end();
}

template <typename T_system>
T_system& Registry::get_system() const {
  auto system = systems.find(std::type_index(typeid(T_system)));
  return *(std::static_pointer_cast<T_system>(system->second));
}

template <typename T_component, typename ...T_Args>
void Entity::add_component(T_Args&& ...args) {
  registry->add_component<T_component>(*this, std::forward<T_Args>(args)...);
}

template <typename T_component>
void Entity::remove_component() {
    registry->remove_component<T_component>(*this);
}

template <typename T_component>
bool Entity::has_component() const {
  return registry->has_component<T_component>(*this);
}

template <typename T_component>
T_component& Entity::get_component() const {
  return registry->get_component<T_component>(*this);
}
