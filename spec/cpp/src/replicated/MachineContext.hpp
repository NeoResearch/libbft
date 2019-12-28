#pragma once
#ifndef LIBBFT_SRC_CPP_MACHINE_CONTEXT_HPP
#define LIBBFT_SRC_CPP_MACHINE_CONTEXT_HPP

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
struct MultiContext;

template<class Param = std::nullptr_t>
struct MachineContext
{
   using Events = std::vector<Event*>;

   Param* params;
   SingleTimerStateMachine<MultiContext<Param>>* machine;
   Events events;

   //void addEvent(Event<MultiContext<Param>>* e)
   //{
   //   events.push_back(e);
   //}

   MachineContext(Param* _params, SingleTimerStateMachine<MultiContext<Param>>* _machine)
     : params(_params)
     , machine(_machine)
   {
   }
};

} // libbft

// forward declaration
#include "MultiContext.hpp"

#endif // LIBBFT_SRC_CPP_MACHINE_CONTEXT_HPP