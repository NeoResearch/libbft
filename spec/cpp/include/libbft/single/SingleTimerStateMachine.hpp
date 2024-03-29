// SPDX-License-Identifier:  MIT
// Copyright (C) 2019-2022 - LibBFT developers
// https://github.com/neoresearch/libbft

#ifndef INCLUDE_LIBBFT_SINGLE_SINGLETIMERSTATEMACHINE_HPP_  // NOLINT
#define INCLUDE_LIBBFT_SINGLE_SINGLETIMERSTATEMACHINE_HPP_

// system includes
#include <cstddef>
#include <iostream>
#include <memory>
#include <string>
#include <utility>
#include <vector>

// libbft includes

#include <libbft/machine/TimedStateMachine.hpp>
#include <libbft/single/State.hpp>
#include <libbft/timing/Timer.hpp>
#include <libbft/utils/Pointer.hpp>

namespace libbft {

template <class Param = std::nullptr_t>
class SingleTimerStateMachine : public TimedStateMachine<State<Param>, Param> {
 public:  // perhaps protect
  using TParam = std::shared_ptr<Param>;
  using TState = std::shared_ptr<State<Param>>;
  using TTransition = std::shared_ptr<Transition<Param>>;

  /** state machine timer */
  TTimer timer;
  /** states for the state machine */
  std::vector<TState> states;
  /** global transitions: may come from any state */
  std::vector<TTransition> globalTransitions;

 protected:
  /** watchdog timer */
  TTimer watchdog{nullptr};

 public:
  /**
   *
   * @param MaxTime -1.0 means infinite time, positive time is real expiration
   * time
   */
  void setWatchdog(double MaxTime) {
    watchdog = std::unique_ptr<Timer>((new Timer())->init(MaxTime));
  }

  /**
   * specific timer
   * @param t
   * @param _me
   * @param _clock
   * @param _name
   */
  explicit SingleTimerStateMachine(TTimer t = nullptr,
                                   MachineId _me = MachineId(0),
                                   TClock _clock = nullptr,
                                   std::string _name = "STSM")
      : TimedStateMachine<State<Param>, Param>(std::move(_clock), _me, _name),
        timer(std::move(t)) {
    // timer must exist
    if (!timer) {
      // timer = std::unique_ptr<Timer>(new Timer("", clonePtr(*this->clock)));
      // ABOVE: Is this Casting?
      timer = std::unique_ptr<Timer>{new Timer("", clonePtr(*this->clock))};
    }
  }

  /**
   * TODO: delete lot's of stuff
   * unique_ptr the clock perhaps?
   */
  virtual ~SingleTimerStateMachine() = default;

  TState getStateByName(std::string _name) {
    for (unsigned i = 0; i < states.size(); i++)
      if (states[i]->name == _name) return states[i];
    return nullptr;  // not found
  }

  /**
   * @return default state is 0, or null
   */
  TState getDefaultState() {
    if (states.size() == 0) return nullptr;
    return states[0];
  }

  void registerState(TState s) {
    assert(s != nullptr);
    states.push_back(s);
  }

  void registerGlobal(TTransition t) {
    assert(t != nullptr);
    globalTransitions.push_back(t);
  }

  /** unused method... TODO: put somewhere */
  virtual TTransition findGlobalTransition(TParam p) {
    // TODO: shuffle global?
    auto _transitions = globalTransitions;
    for (unsigned i = 0; i < _transitions.size(); i++) {
      if (_transitions[i]->isValid(*timer, p, MachineId(this->me.id)))
        return _transitions[i];
    }
    return nullptr;
  }

  void onEnterState(State<Param>& current, TParam p) override {
    std::cout << "entering state: " << current.toStringR(true) << std::endl;

    if (watchdog) watchdog->reset();
  }

  bool isFinal(const State<Param>& current, TParam p) override {
    return current.isFinal;
  }

  /**
   * get next state (current may be null, waiting for global transition)
   * may return the same state, if nothing happened
   * @param outcurrent
   * @param p
   * @return
   */
  bool updateState(TState& outcurrent, TParam p) override {
    auto current = outcurrent;
    bool r = false;
    // find global transition
    auto gt = findGlobalTransition(p);
    if (gt) {
      // found global transition
      std::cout << "-> found valid global transition! " << gt->toString()
                << std::endl;
      current = gt->execute(*timer, p, MachineId(this->me.id));
      r = true;
    }

    // cout << "finding transition! ...";
    if (current) {
      auto go = current->tryGetTransition(*timer, p, MachineId(this->me.id));
      if (go) {
        std::cout << "-> found valid transition! " << go->toString()
                  << std::endl;
        current = go->execute(*timer, p, MachineId(this->me.id));
        r = true;
      }
    }
    outcurrent = current;
    return r;
  }

  /**
   * just for inherited classes
   * @param p
   */
  virtual void OnInitialize(TParam p) {}

  /**
   * initialize timer, etc, and also, setup first state (if not given)
   * @param current
   * @param p
   * @return
   */
  TState initialize(TState current, TParam p) override {
    // check if there's initial state available
    if (!current && states.size() == 0) return nullptr;

    std::cout << std::endl;
    std::cout << "===========" << std::endl;
    std::cout << "begin run()" << std::endl;
    std::cout << "===========" << std::endl;

    std::cout << "OnInitialize() Single MACHINE!" << std::endl;
    this->OnInitialize(p);  // some inherited methods perhaps need this

    if (watchdog) {
      watchdog->reset();
    } else {
      std::cout << "No watchdog configured!" << std::endl;
    }

    std::cout << "will initialize timer" << std::endl;
    timer->init();

    std::cout << "will reset timer" << std::endl;
    timer->reset();

    if (!current) {
      current = getDefaultState();
    }

    return current;
  }

  /**
   * launch when machine is finished
   * @param final
   * @param p
   */
  void OnFinished(const State<Param>& final, TParam p) override {
    std::cout << std::endl;
    std::cout << "=================" << std::endl;
    std::cout << "finished machine!" << std::endl;
    std::cout << "=================" << std::endl;
  }

  bool beforeUpdateState(State<Param>& current, TParam p) override {
    if (watchdog && watchdog->expired()) {
      std::cout << "StateMachine FAILED: MAXTIME = " << watchdog->getCountdown()
                << std::endl;
      return true;
    }

    // nothing to do?
    return false;
  }

  std::string toStringFormat(StringFormat format) override {
    std::stringstream ss;
    if (format == StringFormat::Graphviz) {
      ss << "digraph " << this->name << " {" << std::endl;
      ss << "//graph [bgcolor=lightgoldenrodyellow]" << std::endl;
      ss << "//rankdir=LR;" << std::endl;
      ss << "size=\"11\"" << std::endl;
      // add states
      ss << "Empty [ label=\"\", width=0, height=0, style = invis ];"
         << std::endl;
      for (unsigned i = 0; i < this->states.size(); i++)
        ss << "node [shape = "
           << (this->states[i]->isFinal ? "doublecircle" : "circle") << "]; "
           << this->states[i]->name << ";" << std::endl;
      // default initial state transition
      auto defState = this->getDefaultState();
      // will happen in an empty transition
      ss << "Empty -> " << defState->name << " [label = \"\"];" << std::endl;
      // begin regular transitions
      // Initial -> Primary [ label = "(H + v) mod R = i" ];
      //      getDefaultState
      for (unsigned i = 0; i < this->states.size(); i++) {
        auto state = this->states[i];
        for (unsigned t = 0; t < state->transitions.size(); t++) {
          ss << state->name << " ";
          ss << state->transitions[t]->toStringFormat(StringFormat::Graphviz)
             << std::endl;
        }
      }

      ss << "}" << std::endl;
    } else {
      // standard text

      ss << "STSM {";
      ss << "#id = " << this->me.id << ";";
      ss << "Timer='" << timer->toString() << "';";
      ss << "States=[";
      for (unsigned i = 0; i < states.size(); i++)
        ss << states[i]->toStringR(true) << ";";
      ss << "]";
      ss << "}";
    }
    return ss.str();
  }
};

}  // namespace libbft

#endif  // INCLUDE_LIBBFT_SINGLE_SINGLETIMERSTATEMACHINE_HPP_
