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
struct Condition
{
   std::string name = "true";
   std::function<bool(const Timer&)> timedFunction = [](const Timer& t) -> bool { return true; };

   Condition(std::string _name, std::function<bool(const Timer&)> _timedFunction)
     : name(_name)
     , timedFunction(_timedFunction)
   {
   }
};

// Action is a Condition that always returns true (can affect params and Timer)
struct Action
{
   std::string name = "true";
   std::function<void(Timer&)> timedAction = [](Timer& t) -> void {};

   Action(std::string _name, std::function<void(Timer&)> _timedAction)
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
   std::vector<Condition> conditions;
   std::vector<Action> actions;

public:
   //std::function<bool(Timer&)> timedFunction = [](Timer& t) -> bool { return true; };

   // TODO: add a "when" function here? (make this class "final"?)
   Transition(State<Param>* _to, string _name = "")
     : to(_to)
     , name(_name)
   {
   }

   void add(Condition c)
   {
      conditions.push_back(c);
   }

   void add(Action a)
   {
      actions.push_back(a);
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

   virtual bool isValid(const Timer& timer, const Param* p = nullptr)
   {
      for (unsigned i = 0; i < conditions.size(); i++)
         if (!conditions[i].timedFunction(timer))
            return false;
      return true;
   }

   // execute transition and return next State
   virtual State<Param>* execute(Timer& timer)
   {
      for (unsigned i = 0; i < actions.size(); i++)
         actions[i].timedAction(timer);
      return to;
   }
};

} // libbft

// forward declaration
#include "State.h"

#endif // LIBBFT_SRC_CPP_TRANSITION_HPP