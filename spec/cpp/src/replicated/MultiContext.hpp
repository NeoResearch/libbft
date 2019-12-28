#pragma once
#ifndef LIBBFT_SRC_CPP_MULTI_CONTEXT_HPP
#define LIBBFT_SRC_CPP_MULTI_CONTEXT_HPP

// system includes
#include <cstddef>
#include <iostream> // TODO: remove
#include <vector>

#include <assert.h> // TODO: remove
#include <unistd.h> // TODO: remove busy sleep

// libbft includes

// Prototype?
#include "../events/Event.hpp"
#include "../machine/TimedStateMachine.hpp"
#include "../single/SingleTimerStateMachine.hpp"

namespace libbft {

template<class Param>
struct MachineContext;

template<class Param = std::nullptr_t>
struct MultiContext
{
   // vector of machines
   std::vector<MachineContext<Param>> vm;

   Param* getParams(int id_me)
   {
      return vm[id_me].params;
   }

   std::vector<Event*> getEvents(int id_me)
   {
      return vm[id_me].events;
   }

   // from may be -1, if broadcasted from system
   void broadcast(std::string event, MachineId from, std::vector<std::string> eventParams)
   {
      std::cout << " -> BROADCASTING EVENT '" << event << "' from " << from.id << std::endl;
      broadcast(new Event(event, from, eventParams), from);
   }

   // from may be -1, if broadcasted from system
   void broadcast(Event* event, MachineId from)
   {
      for (int i = 0; i < static_cast<int>(vm.size()); i++)
         if (i != from.id)
            sendTo(event, i); // this may break with memory leaks (TODO: use shared_ptr, or copy-based final class)
   }

   // 'to' should be valid (0 <= to <= R)
   void sendTo(Event* event, MachineId to)
   {
      std::cout << " => SEND TO " << to.id << std::endl;
      assert((to.id >= 0) && (to.id < static_cast<int>(vm.size())));
      vm[to.id].events.push_back(event);
   }

   // 'from' may be -1, if broadcasted from system
   // 'to' should be valid (0 <= to <= R)
   void sendTo(std::string event, MachineId from, MachineId to, std::vector<std::string> eventParams)
   {
      sendTo(new Event(event, from, eventParams), to);
   }

   bool hasEvent(std::string name, int at, std::vector<std::string> eventParams)
   {
      for (unsigned i = 0; i < vm[at].events.size(); i++) {
         //cout << "comparing " << name << "(" << parameters << ") with: " << vm[at].events[i]->toString() << endl;
         if (vm[at].events[i]->isActivated(name, eventParams))
            return true;
      }
      return false;
   }

   void consumeEvent(std::string name, int at, std::vector<std::string> eventParams)
   {
      for (unsigned i = 0; i < vm[at].events.size(); i++)
         if (vm[at].events[i]->isActivated(name, eventParams)) {
            vm[at].events.erase(vm[at].events.begin() + i);
            return;
         }
   }

   /*
   void processEvent(string name, int at)
   {
      for (unsigned i = 0; i < vm[at].events.size(); i++)
         if (vm[at].events[i]->getType() == name) {
            vm[at].events.erase(vm[at].events.begin() + i);
         }
   }
*/
};

} // libbft

// forward declaration
#include "MachineContext.hpp"

#endif // LIBBFT_SRC_CPP_MULTI_CONTEXT_HPP
