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
#include "../events/ScheduledEvent.hpp"
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
   // regular delay (in MS): for testing purposes only (fork simulation)
   int testRegularDelayMS{ 0 };

public:
   RPCMachineContext(Param* _params, int _me, vector<BFTEventsClient*> _world)
     : params(_params)
     , me(_me)
     , world(_world)
   {
   }

   // just to test purposes: force a delay on message passing
   void testSetRegularDelay(int _testRegularDelayMS)
   {
      this->testRegularDelayMS = _testRegularDelayMS;
   }

   // Different from MultiContext... in this one, I can only access my own events
   bool hasEvent(string name, vector<string> eventArgs)
   {
      for (unsigned i = 0; i < events.size(); i++) {
         if (events[i]->isActivated(name, eventArgs))
            return true;
      }
      return false;
   }

   vector<Event*> getEvents()
   {
      return events;
   }

   void registerEvent(Event* event)
   {
      std::cout << "RPCMachineContext registering event '" << event->toString() << "'" << std::endl;
      events.push_back(event);
   }

   // this is used to add events that come from any other sources, and get pending. TODO(@igormcoelho): is this the best design?
   void addEvents(vector<Event*> pendingEvents)
   {
      // do manual insertion of events, because of print messages
      for (unsigned i = 0; i < pendingEvents.size(); i++)
         registerEvent(pendingEvents[i]);
      //events.insert(events.begin() + 0, pendingEvents.begin(), pendingEvents.end());
   }

   void broadcast(std::string event, std::vector<std::string> eventArgs)
   {
      std::cout << "  -~-~:broadcasting event '" << event << "' with params = " << eventArgs.size() << std::endl;
      for (unsigned i = 0; i < world.size(); i++) {
         std::cout << "  -~-~:sending to " << i << " event '" << event << "' with params = " << eventArgs.size() << std::endl;
         world[i]->informEvent(me, event, eventArgs);
      }
   }

   /*
   bool launchTimedEvent(ScheduledEvent se)
   {
      if (se.machineTo.id != me)
         return false; // not for me

      // launch TimedEvent, from -1 (broadcast/system style..). Could be from 'me' too.
      events.push_back(new TimedEvent(se.countdown, se.name, MachineId(-1), se.eventParams));
      return true;
   }
*/

   void addEventFromRPC(string _name, MachineId _from, vector<string> _parameters)
   {
      registerEvent(new Event(_name, _from, _parameters));
   }
};

} // libbft

#endif // LIBBFT_SRC_CPP_RPC_MACHINE_CONTEXT_HPP