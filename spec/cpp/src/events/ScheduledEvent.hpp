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

using namespace std; // TODO: remove

namespace libbft {

// Scheduled class: launches a 'thing' (type T) after clock has expired
// simple class to hold information, could be an std::array perhaps, if named
struct ScheduledEvent
{
   string name;                // event name
   vector<string> eventParams; // event parameters
   double countdown;
   MachineId machine;

   ScheduledEvent(string _name, double _countdown, MachineId _machine, vector<string> _eventParams)
     : name(_name)
     , eventParams(_eventParams)
     , countdown(_countdown)
     , machine(_machine)
   {
   }
};

} // libbft

#endif // LIBBFT_SRC_CPP_SCHEDULED_EVENT_HPP