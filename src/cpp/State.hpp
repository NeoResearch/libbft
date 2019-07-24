#ifndef LIBBFT_SRC_CPP_STATE_HPP
#define LIBBFT_SRC_CPP_STATE_HPP

// system includes
#include <vector>
#include <sstream>

// standard Transition
#include "Transition.hpp"
#include "Timer.hpp"

// standard State

// every state is a Timed state (states that allow timed transitions)

using namespace std; // TODO: remove

namespace libbft {

class State
{
public:
   vector<Transition*> transitions;

   bool isFinal;

   State(bool _isFinal)
     : isFinal(_isFinal)
   {
   }

   Transition* tryGetTransition(Timer& timer)
   {
      for(unsigned i=0; i<transitions.size(); i++)
      {
         if(transitions[i]->isValid())
            return transitions[i];
      }
      return nullptr;
   }

   string toString(bool recursive = true)
   {
      stringstream ss;
      ss << "state:{";
      ss << "final=" << isFinal << ";";
      if(recursive)
      {
      ss << "transitions=[";
      for(unsigned i=0; i<transitions.size(); i++)
         ss << transitions[i]->toString() << ";";
      ss << "]";
      }
      ss << "}";
      return ss.str();
   }
};

} // libbft

#endif // LIBBFT_SRC_CPP_STATE_HPP