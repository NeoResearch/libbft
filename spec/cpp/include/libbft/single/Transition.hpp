#pragma once
#ifndef LIBBFT_SRC_CPP_TRANSITION_HPP
#define LIBBFT_SRC_CPP_TRANSITION_HPP

// system includes
#include <memory>
#include <cstddef>
#include <cassert>
#include <functional>
#include <sstream>

// libbft
//#include "State.h" // forward declaration
#include "machine/MachineId.hpp"
#include "timing/Timer.hpp"
////#include "Event.hpp"

// standard Transition
// Every transition may or may not be a timed transition

namespace libbft {

/**
 * forward declaration
 * @tparam Param
 */
template<class Param>
class State;

/**
 * Condition has a name and boolean function (should not affect any param or Timer)
 * @tparam Param
 */
template<class Param = std::nullptr_t>
struct Condition
{
   using TParam = std::shared_ptr<Param>;
   using TimedFunctionType = std::function<bool(const Timer&, TParam, MachineId)>;

   std::string name = "true";
   /** TODO Should not we avoid exposing it like this? */
   TimedFunctionType timedFunction = [](const Timer& t, TParam p, const MachineId &) -> bool { return true; };

   Condition(std::string _name, TimedFunctionType _timedFunction)
     : name(std::move(_name))
     , timedFunction(_timedFunction)
   {
   }

   std::string toString() const
   {
      return name;
   }
};

/**
 * Action is a Condition that always returns true (can affect params and Timer)
 * @tparam Param
 */
template<class Param = std::nullptr_t>
struct Action
{
   using TParam = std::shared_ptr<Param>;
   using TimedActionType = std::function<void(Timer&, TParam, MachineId)>;

   std::string name = "nop";
   /** TODO Should not we avoid exposing it like this? */
   TimedActionType timedAction = [](Timer&, TParam, const MachineId &) -> void {};

   Action(std::string _name, TimedActionType _timedAction)
     : name(std::move(_name))
     , timedAction(_timedAction)
   {
   }

   std::string toString() const
   {
      return name;
   }
};

template<class Param = std::nullptr_t>
class Transition
{
public:
   using TState = std::shared_ptr<State<Param>>;
   using TParam = std::shared_ptr<Param>;
private:
   /** state to go after executing this transition */
   TState to;
   /** transition name (not really useful) */
   std::string name;
   /** boolean conditions (if all are valid, transition is valid) */
   std::vector<Condition<Param>> conditions;
   /** actions to be performed during Transition execution (reset timers, etc) */
   std::vector<Action<Param>> actions;
public:
   /**
    * a Transition goes to some state 'to'
    * @param _to
    * @param _name
    */
   explicit Transition(TState _to, std::string _name = "")
     : to(_to)
     , name(std::move(_name))
   {
      assert(to != nullptr);
   }

   /**
    * add a new boolean Condition (returns pointer to itself, to allow cascading effect)
    * @param c
    * @return
    */
   Transition* add(Condition<Param> c)
   {
      conditions.push_back(c);
      return this; // allow chaining effect
   }

   /**
    * add a new Action (returns pointer to itself, to allow cascading effect)
    * @param a
    * @return
    */
   Transition* add(Action<Param> a)
   {
      actions.push_back(a);
      return this; // allow chaining effect
   }

   /**
    * returns 'true' if all conditions are valid (or no conditions are required)
    * @param timer
    * @param p
    * @param me
    * @return
    */
   virtual bool isValid(const Timer& timer, TParam p, MachineId me)
   {
      for (auto & condition : conditions) {
         if (!condition.timedFunction(timer, p, me)) {
            return false;
         }
      }
      return true;
   }

   /**
    * execute transition and returns the next State
    * @param timer
    * @param p
    * @param me
    * @return
    */
   virtual TState execute(Timer& timer, TParam p, MachineId me)
   {
      for (auto & action : actions) {
         action.timedAction(timer, p, me);
      }
      return to;
   }

   /**
    * converts to string
    * @param format
    * @return
    */
   std::string toString(std::string format = "") const
   {
      std::stringstream ss;
      if (format == "graphviz") {
         ss << " -> " << this->to->name;
         ss << " [ label = \"";

         bool first = true;
         for (unsigned i = 0; i < conditions.size(); i++) {
            if (!first)
               ss << " \\n ";
            ss << conditions[i].name << " ";
            first = false;
         }
         for (unsigned i = 0; i < actions.size(); i++) {
            if (!first)
               ss << " \\n ";

            ss << actions[i].name << " ";
            first = false;
         }
         ss << "\"];";
         return ss.str();
      } else {
         // default print
         ss << "t() => {name = '" << name << "',";
         ss << "to='" << to->toString(false) << "',";
         ss << "conditions=[";
         for (unsigned i = 0; i < conditions.size(); i++)
            ss << conditions[i].toString() << ";";
         ss << "], ";
         ss << "actions=[";
         for (unsigned i = 0; i < actions.size(); i++)
            ss << actions[i].toString() << ";";
         ss << "], ";
         ss << "'}";
      }
      return ss.str();
   }
};

} // libbft

// forward declaration
#include "State.hpp"

#endif // LIBBFT_SRC_CPP_TRANSITION_HPP
