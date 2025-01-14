#pragma once

#include <cstdint>
#include <bitset>
#include <typeindex>
#include <unordered_map>
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
///////////////////////////////////////////////////////////////
class Registry {
public:
  Registry() = default;
  ~Registry() = default;
  Registry(const Registry&) = default;
  // TODO:
  // Create & Remove entity
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
};

template <typename T_component>
void System::require_component() {
  const auto component_id = Component<T_component>::get_component_id();
  component_signature.set(component_id);
}
