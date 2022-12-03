#pragma once
#ifndef LIBBFT_SRC_CPP_MACHINE_CONTEXT_HPP
#define LIBBFT_SRC_CPP_MACHINE_CONTEXT_HPP

// system includes
#include <memory>
#include <cstddef>
#include <vector>

// libbft includes

// Prototype?
#include "events/Event.hpp"
#include "machine/TimedStateMachine.hpp"
#include "single/SingleTimerStateMachine.hpp"

namespace libbft {

template<class Param>
struct MultiContext;

template<class Param = std::nullptr_t>
struct MachineContext
{
   using TParam = std::shared_ptr<Param>;
   using TSingleTimerStateMachine = std::shared_ptr<SingleTimerStateMachine<MultiContext<Param>>>;

   TParam params;
   TSingleTimerStateMachine machine;
   Events events;

   //void addEvent(Event<MultiContext<Param>>* e)
   //{
   //   events.push_back(e);
   //}

   MachineContext(TParam _params, TSingleTimerStateMachine _machine)
     : params(_params)
     , machine(_machine)
   {
   }
};

} // libbft

// forward declaration
#include "MultiContext.hpp"

#endif // LIBBFT_SRC_CPP_MACHINE_CONTEXT_HPP
