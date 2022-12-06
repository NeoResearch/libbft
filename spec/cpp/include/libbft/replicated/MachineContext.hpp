// SPDX-License-Identifier:  MIT
// Copyright (C) 2019-2022 - LibBFT developers
// https://github.com/neoresearch/libbft

#ifndef INCLUDE_LIBBFT_REPLICATED_MACHINECONTEXT_HPP_
#define INCLUDE_LIBBFT_REPLICATED_MACHINECONTEXT_HPP_

// system includes
#include <cstddef>
#include <memory>
#include <vector>

// libbft includes

// Prototype?
#include <libbft/events/Event.hpp>
#include <libbft/machine/TimedStateMachine.hpp>
#include <libbft/single/SingleTimerStateMachine.hpp>

namespace libbft {

template <class Param>
struct MultiContext;

template <class Param = std::nullptr_t>
struct MachineContext {
  using TParam = std::shared_ptr<Param>;
  using TSingleTimerStateMachine =
      std::shared_ptr<SingleTimerStateMachine<MultiContext<Param>>>;

  TParam params;
  TSingleTimerStateMachine machine;
  Events events;

  // void addEvent(Event<MultiContext<Param>>* e)
  //{
  //    events.push_back(e);
  // }

  MachineContext(TParam _params, TSingleTimerStateMachine _machine)
      : params(_params), machine(_machine) {}
};

}  // namespace libbft

// forward declaration
#include <libbft/replicated/MultiContext.hpp>

#endif  // INCLUDE_LIBBFT_REPLICATED_MACHINECONTEXT_HPP_
