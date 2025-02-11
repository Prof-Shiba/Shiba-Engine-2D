#pragma once
#include "../Logger/Logger.hpp"
#include "../EventManager/Event.hpp"
#include <functional>
#include <list>
#include <map>
#include <memory>
#include <typeindex>
#include <utility>

class I_EventCallback {
public:
  virtual ~I_EventCallback() = default;
  void execute(Event& e) { Call(e); }

private:
  virtual void Call(Event& e) = 0;
};

template <typename T_Owner, typename T_Event>
class EventCallback : public I_EventCallback {
private:
  typedef void (T_Owner::*CallbackFunction)(T_Event&);

  T_Owner* owner_instance;
  CallbackFunction call_back_function;

  virtual void Call(Event& e) override {
    std::invoke(call_back_function, owner_instance, static_cast<T_Event&>(e));
  }

public:
  EventCallback(T_Owner* owner_instance, CallbackFunction call_back_function)
  : owner_instance{owner_instance}, call_back_function{call_back_function} {}

  virtual ~EventCallback() override = default;
};

typedef std::list<std::unique_ptr<I_EventCallback>> HandlerList;

class EventManager {
private:
  std::map<std::type_index, std::unique_ptr<HandlerList>> listeners;

public:
  EventManager() { Logger::Log("EventManager Constructor Called!"); }
  ~EventManager() { Logger::Log("EventManager Destructor Called!"); }

  template <typename T_Owner, typename T_Event>
  void listen_for_event(T_Owner* owner_instance, void (T_Owner::*call_back_function)(T_Event&)) {
    if (!listeners[typeid(T_Event)].get())
      listeners[typeid(T_Event)] = std::make_unique<HandlerList>();

    auto listener = std::make_unique<EventCallback<T_Owner, T_Event>>(owner_instance, call_back_function);
    listeners[typeid(T_Event)]->push_back(std::move(listener));
  }

  template <typename T_Event, typename ...T_Args>
  void emit_event(T_Args&& ...args) {
    auto handlers = listeners[typeid(T_Event)].get();
    if (handlers) {
      for (auto it = handlers->begin(); it != handlers->end(); it++) {
        auto handler = it->get();
        T_Event event(std::forward<T_Args>(args)...);
        handler->execute(event);
      }
    }
  }

};
