#include <utility>

#include <utility>

#pragma once
#ifndef LIBBFT_SRC_CPP_SCHEDULED_EVENT_HPP
#define LIBBFT_SRC_CPP_SCHEDULED_EVENT_HPP

// system includes
#include <vector>

// libbft includes

// Prototype?
#include "events/Event.hpp"
#include "machine/TimedStateMachine.hpp"
#include "single/SingleTimerStateMachine.hpp"

namespace libbft {

/**
 * Scheduled class: launches a 'thing' (type T) after clock has expired
 * simple class to hold information, could be an std::array perhaps, if named
 */
struct ScheduledEvent
{
   /** event name */
   std::string name;
   /** event parameters */
   std::vector<std::string> eventParams;
   /** countdown in seconds for event */
   double countdown;
   /** machine to send event */
   MachineId machineTo;
   // machine 'from' is not needed, as this is usually seen as a system event,
   // but if necessary, we can add this option here, to "simulate" a timed message from other nodes

   ScheduledEvent(std::string _name, double _countdown, MachineId _machineTo,
         std::vector<std::string> _eventParams = std::vector<std::string>(0))
     : name(std::move(_name))
     , eventParams(std::move(_eventParams))
     , countdown(_countdown)
     , machineTo(std::move(_machineTo))
   {
   }
};

} // libbft

#endif // LIBBFT_SRC_CPP_SCHEDULED_EVENT_HPP
