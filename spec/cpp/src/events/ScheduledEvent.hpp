#pragma once
#ifndef LIBBFT_SRC_CPP_SCHEDULED_EVENT_HPP
#define LIBBFT_SRC_CPP_SCHEDULED_EVENT_HPP

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

namespace libbft {

// Scheduled class: launches a 'thing' (type T) after clock has expired
// simple class to hold information, could be an std::array perhaps, if named
struct ScheduledEvent
{
   string name;                // event name
   vector<string> eventParams; // event parameters
   double countdown;           // countdown in seconds for event
   MachineId machineTo;        // machine to send event
   // machine 'from' is not needed, as this is usually seen as a system event,
   // but if necessary, we can add this option here, to "simulate" a timed message from other nodes

   ScheduledEvent(string _name, double _countdown, MachineId _machineTo, vector<string> _eventParams = vector<string>(0))
     : name(_name)
     , eventParams(_eventParams)
     , countdown(_countdown)
     , machineTo(_machineTo)
   {
   }
};

} // libbft

#endif // LIBBFT_SRC_CPP_SCHEDULED_EVENT_HPP
