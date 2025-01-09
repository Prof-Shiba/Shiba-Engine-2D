#pragma once
///////////////////////////////////////////////////////////////
class Component {
public:
  Component();
  Component(const Component &) = default;
  ~Component();

private:
  
};
///////////////////////////////////////////////////////////////
class Entity {
public:
  Entity();
  Entity(const Entity &) = default;
  ~Entity();

private:
  
};
///////////////////////////////////////////////////////////////
class System {
public:
  System();
  System(const System &) = default;
  ~System();

private:
  
};
///////////////////////////////////////////////////////////////
class Registry {
public:
  Registry();
  Registry(const Registry &) = default;
  ~Registry();

private:
  
};
