#pragma once
#ifndef LIBBFT_SRC_CPP_MULTI_CONTEXT_HPP
#define LIBBFT_SRC_CPP_MULTI_CONTEXT_HPP

// system includes
#include <iostream> // TODO: remove
#include <vector>

#include <assert.h> // TODO: remove
#include <unistd.h> // TODO: remove busy sleep

// libbft includes

// Prototype?
#include "../events/Event.hpp"
#include "../machine/TimedStateMachine.hpp"
#include "../single/SingleTimerStateMachine.hpp"

using namespace std; // TODO: remove

namespace libbft {

template<class Param>
struct MachineContext;

template<class Param = nullptr_t>
struct MultiContext
{
   // vector of machines
   vector<MachineContext<Param>> vm;

   // from may be -1, if broadcasted from system
   void broadcast(string event, int from, vector<string> eventParams)
   {
      broadcast(new Event(event, from, eventParams), from);
   }

   // from may be -1, if broadcasted from system
   void broadcast(Event* event, int from)
   {
      for (unsigned i = 0; vm.size(); i++)
         if (i != from)
            sendTo(event, i); // this may break with memory leaks (TODO: use shared_ptr, or copy-based final class)
   }

   // 'to' should be valid (0 <= to <= R)
   void sendTo(Event* event, int to)
   {
      assert((to >= 0) && (to < vm.size()));
      vm[to].events.push_back(event);
   }

   // 'from' may be -1, if broadcasted from system
   // 'to' should be valid (0 <= to <= R)
   void sendTo(string event, int from, int to, vector<string> eventParams)
   {
      sendTo(new Event(event, from, eventParams), to);
   }

   bool hasEvent(string name, int at, vector<string> eventParams)
   {
      for (unsigned i = 0; i < vm[at].events.size(); i++) {
         //cout << "comparing " << name << "(" << parameters << ") with: " << vm[at].events[i]->toString() << endl;
         if (vm[at].events[i]->isActivated(name, eventParams))
            return true;
      }
      return false;
   }

   void consumeEvent(string name, int at, vector<string> eventParams)
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