// SPDX-License-Identifier:  MIT
// Copyright (C) 2019-2022 - LibBFT developers
// https://github.com/neoresearch/libbft

#ifndef INCLUDE_LIBBFT_MACHINE_TIMEDSTATEMACHINE_HPP_
#define INCLUDE_LIBBFT_MACHINE_TIMEDSTATEMACHINE_HPP_

// C
#include <unistd.h>

// C++
#include <cstddef>
#include <memory>
#include <string>
#include <utility>

// default clock
#include <libbft/machine/MachineId.hpp>
#include <libbft/timing/Clock.hpp>
#include <libbft/utils/IPrintable.hpp>

namespace libbft {

/**
 * requires a StateType (perhaps TransitionType) and a machine-specific Param
 * (context type)
 * @tparam StateType
 * @tparam Param
 */
template <class StateType, class Param = std::nullptr_t>
class TimedStateMachine : public IFPrintable {
 public:
  using TParam = std::shared_ptr<Param>;
  using TStateType = std::shared_ptr<StateType>;

  /** state machine clock */
  TClock clock;
  /** an identifier for itself */
  MachineId me{0};
  /** string name */
  std::string name{""};

  /** a Timed State Machine requires a global clock, and a unique personal
   * identifier */
  explicit TimedStateMachine(TClock _clock = nullptr,
                             MachineId _me = MachineId{0},
                             std::string _name = "")
      : clock(std::move(_clock)), me(std::move(_me)), name(std::move(_name)) {
    // clock must exist
    if (!clock) {
      clock = std::unique_ptr<Clock>(new Clock());
    }
  }

  /**
   * TODO: delete lot's of stuff
   * unique_ptr the clock perhaps?
   */
  // virtual ~TimedStateMachine() = default;
  // virtual ~TimedStateMachine() {}

  /** triggers this, after state machine enters this state */
  virtual void onEnterState(StateType& current, TParam p) = 0;

  /**
   * do global processing (return 'true' means 'break' process)
   * TODO: may use a global processing FLAG here, to break
   */
  virtual bool beforeUpdateState(StateType& current, TParam p) = 0;

  /**
   * do global after-processing (return 'true' means 'break' process)
   * TODO: may use a global processing FLAG here, to break
   * @param current
   * @param p
   * @param updated
   * @return
   */
  virtual bool afterUpdateState(StateType& current, TParam p, bool updated) {
    // sleeping a little bit if not updated (avoid wasting resources)
    if (!updated) usleep(1000 * 100);  // 100 milli (in microsecs)
    return false;                      // do not 'break'
  }

  /**
   * get next state (null state is not allowed)
   * may return the same state, if nothing happened
   * @param current
   * @param p
   * @return
   */
  virtual bool updateState(TStateType& current, TParam p) = 0;

  virtual bool isFinal(const StateType& current, TParam p) = 0;

  /**
   * initialize runtime states and timers, etc
   * if initial state is not given, it must provide one here
   * @param current
   * @param p
   * @return
   */
  virtual TStateType initialize(TStateType current, TParam p) = 0;

  /**
   * launch when machine is finished
   * @param final
   * @param p
   */
  virtual void OnFinished(const StateType& final, TParam p) = 0;

  /**
   * execute the state machine (returns final state, or nullptr if on failure)
   * @param initial
   * @param p
   * @return
   */
  // virtual TStateType run(TStateType initial = nullptr, TParam p = nullptr) {
  virtual TStateType run(TStateType initial, TParam p) {
    //
    auto current = this->initialize(initial, p);
    // if no state given, abort
    if (!current) return nullptr;

    onEnterState(*current, p);

    // while current is  not final
    while (!isFinal(*current, p)) {
      // preprocess stuff (watchdogs? global transitions?)
      if (beforeUpdateState(*current, p)) return nullptr;

      bool updated = updateState(current, p);
      if (updated) {
        onEnterState(*current, p);
      }

      // post-process stuff (sleep if not evolving, etc)
      if (afterUpdateState(*current, p, updated)) return nullptr;
    }

    OnFinished(*current, p);

    return current;
  }

  std::string toStringFormat(StringFormat format) override {
    std::stringstream ss;
    if (format == StringFormat::Graphviz) {
    } else {
      // standard text

      ss << "TSM {";
      ss << "#id = " << me.id << ";";
      ss << "clock = " << clock->toString() << ";";
      ss << "}";
    }
    return ss.str();
  }
};

}  // namespace libbft

#endif  // INCLUDE_LIBBFT_MACHINE_TIMEDSTATEMACHINE_HPP_
