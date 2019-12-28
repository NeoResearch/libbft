#pragma once
#ifndef LIBBFT_SRC_CPP_TRANSITION_HPP
#define LIBBFT_SRC_CPP_TRANSITION_HPP

// system includes
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

// forward declaration
template<class Param>
class State;

// Condition has a name and boolean function (should not affect any param or Timer)
template<class Param = std::nullptr_t>
struct Condition
{
   using TimedFunctionType = std::function<bool(const Timer&, Param*, MachineId)>;
   std::string name = "true";
   TimedFunctionType timedFunction = [](const Timer& t, Param* p, const MachineId &) -> bool { return true; };

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

// Action is a Condition that always returns true (can affect params and Timer)
template<class Param = std::nullptr_t>
struct Action
{
   using TimedActionType = std::function<void(Timer&, Param*, MachineId)>;
   std::string name = "nop";
   TimedActionType timedAction = [](Timer&, Param*, const MachineId &) -> void {};

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
private:
   // state to go after executing this transition
   State<Param>* to;
   // transition name (not really useful)
   std::string name;
   // boolean conditions (if all are valid, transition is valid)
   std::vector<Condition<Param>> conditions;
   // actions to be performed during Transition execution (reset timers, etc)
   std::vector<Action<Param>> actions;

public:
   // a Transition goes to some state 'to'
   explicit Transition(State<Param>* _to, std::string _name = "")
     : to(_to)
     , name(std::move(_name))
   {
      assert(to != nullptr);
   }

   // add a new boolean Condition (returns pointer to itself, to allow cascading effect)
   Transition* add(Condition<Param> c)
   {
      conditions.push_back(c);
      return this; // allow chaining effect
   }

   // add a new Action (returns pointer to itself, to allow cascading effect)
   Transition* add(Action<Param> a)
   {
      actions.push_back(a);
      return this; // allow chaining effect
   }

   // returns 'true' if all conditions are valid (or no conditions are required)
   virtual bool isValid(const Timer& timer, Param* p, MachineId me)
   {
      for (unsigned i = 0; i < conditions.size(); i++)
         if (!conditions[i].timedFunction(timer, p, me))
            return false;
      return true;
   }

   // execute transition and returns the next State
   virtual State<Param>* execute(Timer& timer, Param* p, MachineId me)
   {
      for (unsigned i = 0; i < actions.size(); i++)
         actions[i].timedAction(timer, p, me);
      return to;
   }

   // converts to string
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
