#ifndef LIBBFT_SRC_CPP_TRANSITION_HPP
#define LIBBFT_SRC_CPP_TRANSITION_HPP

// system includes
#include <functional>
#include <sstream>

// libbft
//#include "State.h" // forward declaration
#include "Timer.hpp"

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
   std::function<bool(const Timer&, Param*)> timedFunction = [](const Timer& t, Param* p) -> bool { return true; };

   Condition(std::string _name, std::function<bool(const Timer&, Param*)> _timedFunction)
     : name(_name)
     , timedFunction(_timedFunction)
   {
   }
};

// Action is a Condition that always returns true (can affect params and Timer)
template<class Param = nullptr_t>
struct Action
{
   std::string name = "true";
   std::function<void(Timer&, Param*)> timedAction = [](Timer& t, Param* p) -> void {};

   Action(std::string _name, std::function<void(Timer&, Param*)> _timedAction)
     : name(_name)
     , timedAction(_timedAction)
   {
   }
};

template<class Param = nullptr_t>
class Transition
{
private:
   State<Param>* to;
   string name;
   std::vector<Condition<Param>> conditions;
   std::vector<Action<Param>> actions;

public:
   //std::function<bool(Timer&)> timedFunction = [](Timer& t) -> bool { return true; };

   // TODO: add a "when" function here? (make this class "final"?)
   Transition(State<Param>* _to, string _name = "")
     : to(_to)
     , name(_name)
   {
   }

   Transition* add(Condition<Param> c)
   {
      conditions.push_back(c);
      return this; // allow chaining effect
   }

   Transition* add(Action<Param> a)
   {
      actions.push_back(a);
      return this; // allow chaining effect
   }

   string
   toString()
   {
      stringstream ss;
      ss << "t() => {name = '" << name << "',";
      ss << "to='" << to->toString(false) << "',";
      ss << "conditions=[";
      for (unsigned i = 0; i < conditions.size(); i++)
         ss << conditions[i].name << ";"; // TODO: toString() ?
      ss << "], ";
      ss << "actions=[";
      for (unsigned i = 0; i < actions.size(); i++)
         ss << actions[i].name << ";"; // TODO: toString() ?
      ss << "], ";
      ss << "'}";
      return ss.str();
   }

   virtual bool isValid(const Timer& timer, Param* p)
   {
      for (unsigned i = 0; i < conditions.size(); i++)
         if (!conditions[i].timedFunction(timer, p))
            return false;
      return true;
   }

   // execute transition and return next State
   virtual State<Param>* execute(Timer& timer, Param* p)
   {
      for (unsigned i = 0; i < actions.size(); i++)
         actions[i].timedAction(timer, p);
      return to;
   }
};

} // libbft

// forward declaration
#include "State.h"

#endif // LIBBFT_SRC_CPP_TRANSITION_HPP