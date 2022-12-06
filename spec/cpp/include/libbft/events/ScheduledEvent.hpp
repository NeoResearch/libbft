// SPDX-License-Identifier:  MIT
// Copyright (C) 2019-2022 - LibBFT developers
// https://github.com/neoresearch/libbft

#ifndef INCLUDE_LIBBFT_EVENTS_SCHEDULEDEVENT_HPP_
#define INCLUDE_LIBBFT_EVENTS_SCHEDULEDEVENT_HPP_

// C++
#include <vector>

// libbft includes
#include <libbft/events/Event.hpp>
#include <libbft/machine/TimedStateMachine.hpp>
#include <libbft/single/SingleTimerStateMachine.hpp>

namespace libbft {

/**
 * Scheduled class: launches a 'thing' (type T) after clock has expired
 * simple class to hold information, could be an std::array perhaps, if named
 */
struct ScheduledEvent {
  /** event name */
  std::string name;
  /** event parameters */
  std::vector<std::string> eventParams;
  /** countdown in seconds for event */
  double countdown;
  /** machine to send event */
  MachineId machineTo;
  // machine 'from' is not needed, as this is usually seen as a system event,
  // but if necessary, we can add this option here, to "simulate" a timed
  // message from other nodes

  ScheduledEvent(
      std::string _name, double _countdown, MachineId _machineTo,
      std::vector<std::string> _eventParams = std::vector<std::string>(0))
      : name(std::move(_name)),
        eventParams(std::move(_eventParams)),
        countdown(_countdown),
        machineTo(std::move(_machineTo)) {}
};

}  // namespace libbft

#endif  // INCLUDE_LIBBFT_EVENTS_SCHEDULEDEVENT_HPP_
