// SPDX-License-Identifier:  MIT
// Copyright (C) 2019-2022 - LibBFT developers
// https://github.com/neoresearch/libbft

#ifndef INCLUDE_LIBBFT_REPLICATED_MULTICONTEXT_HPP_
#define INCLUDE_LIBBFT_REPLICATED_MULTICONTEXT_HPP_

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
struct MachineContext;

template <class Param = std::nullptr_t>
struct MultiContext {
  using TParam = std::shared_ptr<Param>;

  /** vector of machines */
  std::vector<MachineContext<Param>> vm;

  TParam getParams(int id_me) { return vm[id_me].params; }

  std::vector<TEvent> getEvents(int id_me) { return vm[id_me].events; }

  /** from may be -1, if broadcasted from system */
  void broadcast(std::string event, MachineId from,
                 std::vector<std::string> eventParams) {
    std::cout << " -> BROADCASTING EVENT '" << event << "' from " << from.id
              << std::endl;
    broadcast(
        std::shared_ptr<Event>(new Event(event, from, std::move(eventParams))),
        from);
  }

  /**
   * from may be -1, if broadcasted from system
   * @param event
   * @param from
   */
  void broadcast(TEvent event, MachineId from) {
    for (int i = 0; i < static_cast<int>(vm.size()); i++) {
      if (i != from.id) {
        // this may break with memory leaks (TODO: use shared_ptr, or copy-based
        // final class)
        sendTo(event, MachineId(i));
      }
    }
  }

  /**
   * @param event
   * @param to should be valid (0 <= to <= R)
   */
  void sendTo(TEvent event, MachineId to) {
    std::cout << " => SEND TO " << to.id << std::endl;
    assert((to.id >= 0) && (to.id < static_cast<int>(vm.size())));
    vm[to.id].events.push_back(event);
  }

  /**
   *
   * @param event
   * @param from may be -1, if broadcasted from system
   * @param to should be valid (0 <= to <= R)
   * @param eventParams
   */
  void sendTo(std::string event, MachineId from, MachineId to,
              std::vector<std::string> eventParams) {
    sendTo(new Event(std::move(event), std::move(from), std::move(eventParams)),
           to);
  }

  bool hasEvent(std::string name, int at,
                std::vector<std::string> eventParams) {
    for (unsigned i = 0; i < vm[at].events.size(); i++) {
      // cout << "comparing " << name << "(" << parameters << ") with: " <<
      // vm[at].events[i]->toString() << endl;
      if (vm[at].events[i]->isActivated(name, eventParams)) return true;
    }
    return false;
  }

  void consumeEvent(std::string name, int at,
                    std::vector<std::string> eventParams) {
    for (unsigned i = 0; i < vm[at].events.size(); i++)
      if (vm[at].events[i]->isActivated(name, eventParams)) {
        vm[at].events.erase(vm[at].events.begin() + i);
        return;
      }
  }

  /*
  void processEvent(string name, int at)
  {
     for (unsigned i = 0; i < vm[at].events.size(); i++)
        if (vm[at].events[i]->getType() == name) {
           vm[at].events.erase(vm[at].events.begin() + i);
        }
  }
*/
};

}  // namespace libbft

// forward declaration
#include <libbft/replicated/MachineContext.hpp>

#endif  // INCLUDE_LIBBFT_REPLICATED_MULTICONTEXT_HPP_
