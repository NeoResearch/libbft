#ifndef LIBBFT_SRC_CPP_TRANSITION_HPP
#define LIBBFT_SRC_CPP_TRANSITION_HPP

// system includes
#include <sstream>
#include <functional>

// libbft
//#include "State.h" // forward declaration
#include "Timer.hpp"

// standard Transition
// Every transition may or may not be a timed transition

using namespace std; // TODO: remove

namespace libbft {

// forward declaration
class State;

class Transition
{
public:
   State* to;
   string name;
   std::function<bool(Timer&)> timedFunction = [](Timer& t) -> bool { return true; };

   // TODO: add a "when" function here? (make this class "final"?)
   Transition(State* _to, string _name = "")
     : to(_to)
     , name(_name)
   {
   }

   // see .cpp
   string toString();

   virtual bool isValid(Timer& timer)
   {
      // always valid
      // TODO: replace by lambda?
      return timedFunction(timer);
   }
};

} // libbft

// forward declaration
#include "State.h"

#endif // LIBBFT_SRC_CPP_TRANSITION_HPP