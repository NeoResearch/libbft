// SPDX-License-Identifier:  MIT
// Copyright (C) 2019-2022 - LibBFT developers
// https://github.com/neoresearch/libbft

#ifndef INCLUDE_LIBBFT_RPC_REPLICATED_RPCMACHINECONTEXT_HPP_
#define INCLUDE_LIBBFT_RPC_REPLICATED_RPCMACHINECONTEXT_HPP_

// system includes
#include <cstddef>
#include <memory>
#include <vector>

// libbft includes

// Prototype?
#include <bftevents-grpc/BFTEventsClient.hpp>
#include <libbft/events/Event.hpp>
#include <libbft/events/ScheduledEvent.hpp>
#include <libbft/machine/TimedStateMachine.hpp>
#include <libbft/single/SingleTimerStateMachine.hpp>

namespace libbft {

template <class Param = std::nullptr_t>
struct RPCMachineContext {
  using TParam = std::unique_ptr<Param>;

  /** my params */
  TParam params;
  /** my id */
  int me;
  /** the world I can connect to */
  std::vector<TBFTEventsClient> world;

 private:
  /** my events */
  Events events;
  /** regular delay (in MS): for testing purposes only (fork simulation) */
  int testRegularDelayMS{0};
  /** regular drop rate: for testing purposes only (fork simulation) */
  double testDropRate{0.0};
  /** machine random */
  std::mt19937 generator;
  // std::uniform_real_distribution<double> dis(0.0, 1.0);
  // double randomRealBetweenZeroAndOne = dis(generator);

 public:
  RPCMachineContext(TParam _params, int _me,
                    std::vector<TBFTEventsClient> _world, int seed = 99)
      : params(std::move(_params)),
        me(_me),
        world(std::move(_world)),
        generator(seed) {}

  /**
   * just to test purposes: force a delay on message passing
   * @param _testRegularDelayMS
   */
  void testSetRegularDelay(int _testRegularDelayMS) {
    this->testRegularDelayMS = _testRegularDelayMS;
  }

  /**
   * just to test purposes: force a drop rate on message passing
   * @param _dropRate
   */
  void testSetDropRate(double _dropRate) { this->testDropRate = _dropRate; }

  /**
   * Different from MultiContext... in this one, I can only access my own events
   * @param name
   * @param eventArgs
   * @return
   */
  bool hasEvent(std::string name, std::vector<std::string> eventArgs) {
    for (unsigned i = 0; i < events.size(); i++) {
      if (events[i]->isActivated(name, eventArgs)) return true;
    }
    return false;
  }

  std::vector<TEvent> getEvents() { return events; }

  void registerEvent(TEvent event) {
    std::cout << "RPCMachineContext registering event '" << event->toString()
              << "'" << std::endl;
    events.push_back(event);
  }

  /**
   * this is used to add events that come from any other sources, and get
   * pending. TODO(@igormcoelho): is this the best design?
   * @param pendingEvents
   */
  void addEvents(Events pendingEvents) {
    // do manual insertion of events, because of print messages
    for (auto& pendingEvent : pendingEvents) {
      registerEvent(pendingEvent);
    }
    // events.insert(events.begin() + 0, pendingEvents.begin(),
    // pendingEvents.end());
  }

  void broadcast(std::string event, std::vector<std::string> eventArgs) {
    std::cout << "  -~-~:broadcasting event '" << event
              << "' with params = " << eventArgs.size() << std::endl;
    for (unsigned i = 0; i < world.size(); i++) {
      std::cout << "  -~-~:sending to " << i << " event '" << event
                << "' with params = " << eventArgs.size() << std::endl;
      // if send with delay
      int delay = 0;
      if (testRegularDelayMS > 0) {
        std::uniform_int_distribution<> dis(0, testRegularDelayMS);
        delay = dis(this->generator);
      }

      if (testDropRate > 0.0) {
        std::uniform_real_distribution<double> dis(0.0, 1.0);
        double x = dis(this->generator);
        if (x < testDropRate) {
          std::cout << "LOST EVENT '" << event << "' for " << i << std::endl;
          continue;  // do not send message
        }
      }

      world[i]->informEvent(me, event, eventArgs, delay);
    }
  }

  /*
  bool launchTimedEvent(ScheduledEvent se)
  {
     if (se.machineTo.id != me)
        return false; // not for me

     // launch TimedEvent, from -1 (broadcast/system style..). Could be from
  'me' too. events.push_back(new TimedEvent(se.countdown, se.name,
  MachineId(-1), se.eventParams)); return true;
  }
*/

  void addEventFromRPC(std::string _name, MachineId _from,
                       std::vector<std::string> _parameters, int delay = 0) {
    if (delay == 0) {
      registerEvent(
          std::shared_ptr<Event>(new Event(_name, _from, _parameters)));
    } else {
      registerEvent(std::shared_ptr<Event>(
          new TimedEvent(delay / 1000.0, _name, _from, _parameters)));
    }
  }
};

}  // namespace libbft

#endif  // INCLUDE_LIBBFT_RPC_REPLICATED_RPCMACHINECONTEXT_HPP_
