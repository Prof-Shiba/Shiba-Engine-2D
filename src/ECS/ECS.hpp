#pragma once

#include <cstdint>
#include <bitset>
#include <typeindex>
#include <unordered_map>
#include <utility>
#include <vector>
#include <set>

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

  uint8_t get_component_id() const;

private:
  uint8_t component_id;
  // returns unique id of comp <T>
  static uint8_t get_id() {
    static auto id = next_id++;
    return id;
  }
};

///////////////////////////////////////////////////////////////
// Entity is an ID representing an object in the world
// it's just a wrapper for a number so we can track things
///////////////////////////////////////////////////////////////
class Entity {
public:
  Entity(uint32_t id) : entity_id{id} {}
  ~Entity() = default;
  Entity(const Entity&) = default;

  uint32_t get_entity_id() const;

private:
  uint32_t entity_id;
};

///////////////////////////////////////////////////////////////
// System processes entities with a certain Signature
//
// It represents game logic that operates on entities and their
// components. 
//
// It will declare what components it needs through the component
// signature, only process an entity with the required components
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
// are stored here in one area, making it easier to access
// (as well as a lot faster!)
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
  Registry() = default;
  ~Registry() = default;
  Registry(const Registry&) = default;
  // Entity management
  Entity create_entity();
  void add_entity_to_system(Entity entity);
  // Component management
  template <typename T_component, typename ...TArgs> void add_component(Entity entity, TArgs&& ...args);

  void update();
  // TODO:
  // Remove entity
  // add and remove components
  // check if entity has component
  // Add & Remove system

private:
  size_t total_num_of_entities {0};
  // Each pool contains all the data of a certain comp type
  // Vector index is component type ID
  // Pool index is entity id
  std::vector<I_Pool*> component_pool;
  // Vector index = entity id
  std::vector<Signature> entity_component_signatures;
  std::unordered_map<std::type_index, System*> systems;
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
    Pool<T_component>* new_comp_pool = new Pool<T_component>();
    component_pool[component_id] = new_comp_pool;
  }

  // get the pool of comp values for that comp type
  Pool<T_component>* current_comp_pool = component_pool[component_id];

  if (entity_id >= current_comp_pool->get_size())
    current_comp_pool->resize(total_num_of_entities);

  // Create new comp obj or type T_comp, and fwrd the various
  // params to the constructor
  T_component new_component(std::forward<TArgs>(args)...);
  current_comp_pool->set_new_index(entity_id, new_component);

  // Update the comp sig of the entity and set comp id on bitset to 1
  entity_component_signatures[entity_id].set(component_id);
}
