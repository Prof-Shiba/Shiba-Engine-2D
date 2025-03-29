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
#include <deque>
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
  void remove();

  void tag(const std::string& tag);
  bool has_tag(const std::string& tag) const;
  void group(const std::string& group);
  bool belongs_to_group(const std::string& group) const;

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
  virtual void remove_entity_from_pool(uint32_t entity_id) = 0;
};

template <typename T>
class Pool : public I_Pool {
public:
  Pool(uint32_t capacity = 100, uint32_t size = 0) : size(size) { data.resize(capacity); }
  virtual ~Pool() = default;
  bool is_empty() const { return size == 0; }
  uint32_t get_size() { return size; }
  void resize(uint32_t new_size) { data.resize(new_size); }

  void clear() { 
    data.clear();
    size = 0;
  }

  void add(T obj) { data.push_back(obj); }

  void set_new_index(uint32_t entity_id, T obj) {
    if (entity_id_to_index.find(entity_id) != entity_id_to_index.end()) {
      uint32_t index = entity_id_to_index[entity_id];
      data[index] = obj;
    } else {
      uint32_t index = size;

      entity_id_to_index.emplace(entity_id, index);
      index_to_entity_id.emplace(index, entity_id);

      if (index >= data.capacity()) data.resize(size * 2);
      data[index] = obj;
      size++;
    }
  }

  void remove(uint32_t entity_id) {
    if (entity_id_to_index.find(entity_id) == entity_id_to_index.end())
      return;

    uint32_t removal_index = entity_id_to_index[entity_id];
    uint32_t last_index = size - 1;
    data[removal_index] = data[last_index];

    uint32_t entity_id_last_element = index_to_entity_id[last_index];
    entity_id_to_index[entity_id_last_element] = removal_index;
    index_to_entity_id[removal_index] = entity_id_last_element;

    entity_id_to_index.erase(entity_id);
    index_to_entity_id.erase(last_index);

    size--;
  }

  void remove_entity_from_pool(uint32_t entity_id) override {
    if (entity_id_to_index.find(entity_id) != entity_id_to_index.end())
      remove(entity_id);
  }

  T& get_at_index(uint32_t entity_id) {
    uint32_t index = entity_id_to_index[entity_id];
    return static_cast<T&>(data[index]); 
  }

  T& operator[](uint32_t index) { return data[index]; }

private:
  std::vector<T> data;
  uint32_t size;
  // helper maps for tracking entity IDs per index, so vector is always packed
  std::unordered_map<uint32_t, uint32_t> entity_id_to_index;
  std::unordered_map<uint32_t, uint32_t> index_to_entity_id;
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
  void remove_entity(Entity entity);

  void add_entity_to_system(Entity entity);
  void remove_entity_from_system(Entity entity);

  void add_tag_to_entity(Entity& entity, const std::string& tag);
  bool entity_has_tag(const Entity& entity, const std::string& tag) const;
  Entity get_entity_by_tag(const std::string& tag) const;
  void remove_tag_from_entity(Entity entity);

  void add_group_to_entity(Entity& entity, const std::string& group);
  bool entity_in_group(const Entity& entity, const std::string& group) const;
  std::vector<Entity> get_entities_by_group(const std::string& group) const;
  void remove_group_from_entity(Entity entity);

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
  std::deque<uint32_t> free_ids;

  std::unordered_map<std::string, Entity> entity_per_tag;
  std::unordered_map<uint16_t, std::string> tag_per_entity;

  std::unordered_map<std::string, std::set<Entity>> entities_per_group;
  std::unordered_map<uint16_t, std::string> group_per_entity;
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

  // Create new comp obj of type T_comp, and fwrd the various
  // params to the constructor
  T_component new_component(std::forward<TArgs>(args)...);
  current_comp_pool->set_new_index(entity_id, new_component);

  // Update the comp sig of the entity and set comp id on bitset to 1
  entity_component_signatures[entity_id].set(component_id);

  Logger::Log("Component with ID [" + std::to_string(component_id) + "] added to Entity ID [" + std::to_string(entity_id) + "]");
  Logger::Err("COMPONENT ID = " + std::to_string(component_id) + " --> POOL SIZE: " + std::to_string(current_comp_pool->get_size()));
}

template <typename T_component>
void Registry::remove_component(Entity entity) {
  const auto& component_id = Component<T_component>::get_component_id();
  const auto& entity_id = entity.get_entity_id();

  if (has_component<T_component>(entity)) {
    auto current_comp_pool = std::static_pointer_cast<Pool<T_component>>(component_pool[component_id]);
    current_comp_pool->remove(entity_id);

    entity_component_signatures[entity_id].set(component_id, false);

    Logger::Log("Removed component [" + std::to_string(component_id) + "] successfully from Entity ID [" + std::to_string(entity_id) + "]!");
    } else {
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
