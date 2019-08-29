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

private:
   // my events
   vector<Event*> events;

public:
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

   void addEvents(vector<Event*> pendingEvents)
   {
      events.insert(events.begin() + 0, pendingEvents.begin(), pendingEvents.end());
   }

   bool launchTimedEvent(ScheduledEvent se)
   {
      if (se.machineTo.id != me)
         return false; // not for me

      // launch TimedEvent, from -1 (broadcast/system style..). Could be from 'me' too.
      events.push_back(new TimedEvent(se.countdown, se.name, MachineId(-1), se.eventParams));
      return true;
   }

   bool launchEventRPC()
   {
      return false;
   }
};

} // libbft

#endif // LIBBFT_SRC_CPP_RPC_MACHINE_CONTEXT_HPP