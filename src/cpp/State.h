#ifndef LIBBFT_SRC_CPP_STATE_HPP
#define LIBBFT_SRC_CPP_STATE_HPP

// system includes
#include <sstream>
#include <vector>

// standard Transition
#include "Timer.hpp"
#include "Transition.h"

// standard State

// every state is a Timed state (states that allow timed transitions)

using namespace std; // TODO: remove

namespace libbft {

class State
{
public:
   vector<Transition*> transitions;

   string name;
   bool isFinal;

   State(bool _isFinal = false, string _name = "")
     : name(_name)
     , isFinal(_isFinal)
   {
   }

   Transition* tryGetTransition(Timer& timer)
   {
      for (unsigned i = 0; i < transitions.size(); i++) {
         if (transitions[i]->isValid(timer))
            return transitions[i];
      }
      return nullptr;
   }

   string toString(bool recursive = true)
   {
      stringstream ss;
      ss << "state:{";
      ss << "name='" << name << "';";
      if (isFinal)
         ss << "final";
      ss << ";";
      if (recursive) {
         ss << "transitions=[";
         for (unsigned i = 0; i < transitions.size(); i++)
            ss << transitions[i]->toString() << ";";
         ss << "]";
      } else
         ss << "...";
      ss << "}";
      return ss.str();
   }
};

} // libbft

#endif // LIBBFT_SRC_CPP_STATE_HPP