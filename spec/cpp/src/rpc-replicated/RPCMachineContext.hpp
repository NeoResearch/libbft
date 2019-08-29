#pragma once
#ifndef LIBBFT_SRC_CPP_RPC_MACHINE_CONTEXT_HPP
#define LIBBFT_SRC_CPP_RPC_MACHINE_CONTEXT_HPP

// system includes
#include <iostream> // TODO: remove
#include <vector>

#include <assert.h> // TODO: remove
#include <unistd.h> // TODO: remove busy sleep

// libbft includes

// Prototype?
#include "../bftevents-grpc/BFTEventsClient.hpp"
#include "../events/Event.hpp"
#include "../machine/TimedStateMachine.hpp"
#include "../single/SingleTimerStateMachine.hpp"

using namespace std; // TODO: remove

namespace libbft {

template<class Param = nullptr_t>
struct RPCMachineContext
{
   // my params
   Param* params;
   // my id
   int me;
   // the world I can connect to
   vector<BFTEventsClient*> world;
   // my events
   vector<Event*> events;

   // Different from MultiContext... in this one, I can only access my own events
   bool hasEvent(string name, vector<string> eventParams)
   {
      for (unsigned i = 0; i < events.size(); i++) {
         if (events[i]->isActivated(name, eventParams))
            return true;
      }
      return false;
   }

   RPCMachineContext(Param* _params, int _me, vector<BFTEventsClient*> _world)
     : params(_params)
     , me(_me)
     , world(_world)
   {
   }
};

} // libbft

#endif // LIBBFT_SRC_CPP_RPC_MACHINE_CONTEXT_HPP