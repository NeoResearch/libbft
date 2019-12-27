#pragma once
#ifndef LIBBFT_SRC_CPP_TRANSITION_HPP
#define LIBBFT_SRC_CPP_TRANSITION_HPP

// system includes
#include <assert.h>
#include <functional>
#include <sstream>

// libbft
//#include "State.h" // forward declaration
#include "machine/MachineId.hpp"
#include "timing/Timer.hpp"
////#include "Event.hpp"

// standard Transition
// Every transition may or may not be a timed transition

using namespace std; // TODO: remove

namespace libbft {

// forward declaration
template<class Param>
class State;

// Condition has a name and boolean function (should not affect any param or Timer)
template<class Param = nullptr_t>
struct Condition
{
   std::string name = "true";
   std::function<bool(const Timer&, Param*, MachineId)> timedFunction = [](const Timer& t, Param* p, MachineId) -> bool { return true; };

   Condition(std::string _name, std::function<bool(const Timer&, Param*, MachineId)> _timedFunction)
     : name(_name)
     , timedFunction(_timedFunction)
   {
   }

   string toString() const
   {
      return name;
   }
};

// Action is a Condition that always returns true (can affect params and Timer)
template<class Param = nullptr_t>
struct Action
{
   std::string name = "nop";
   std::function<void(Timer&, Param*, MachineId)> timedAction = [](Timer& t, Param* p, MachineId) -> void {};

   Action(std::string _name, std::function<void(Timer&, Param*, MachineId)> _timedAction)
     : name(_name)
     , timedAction(_timedAction)
   {
   }

   string toString() const
   {
      return name;
   }
};

template<class Param = nullptr_t>
class Transition
{
private:
   // state to go after executing this transition
   State<Param>* to;
   // transition name (not really useful)
   string name;
   // boolean conditions (if all are valid, transition is valid)
   std::vector<Condition<Param>> conditions;
   // actions to be performed during Transition execution (reset timers, etc)
   std::vector<Action<Param>> actions;

public:
   // a Transition goes to some state 'to'
   Transition(State<Param>* _to, string _name = "")
     : to(_to)
     , name(_name)
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
   string toString(string format = "") const
   {
      stringstream ss;
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
