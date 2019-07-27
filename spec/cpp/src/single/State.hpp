#pragma once
#ifndef LIBBFT_SRC_CPP_STATE_HPP
#define LIBBFT_SRC_CPP_STATE_HPP

// system includes
#include <iostream> // TODO: remove
#include <sstream>
#include <vector>
// simulate non-deterministic nature
#include <algorithm>
#include <random>

// standard Transition
#include "../timing/Timer.hpp"
#include "Transition.hpp"

// standard State

// every state is a Timed state (states that allow timed transitions)

using namespace std; // TODO: remove

namespace libbft {

template<class Param = nullptr_t>
class State
{
private:
   vector<Transition<Param>*> transitions;

public:
   string name;
   bool isFinal;

   State(bool _isFinal = false, string _name = "")
     : name(_name)
     , isFinal(_isFinal)
   {
   }

   void addTransition(Transition<Param>* t)
   {
      transitions.push_back(t);
   }

   Transition<Param>* tryGetTransition(Timer& timer, Param* p, int me)
   {
      //cout << "Trying to Get Transition" << endl;
      // should be non-deterministic and asynchronous...
      // TODO: simulate this with random, at least, to avoid getting stuck on tests by chance
      vector<Transition<Param>*> _transitions = transitions;

      auto rng = std::default_random_engine{};
      std::shuffle(std::begin(_transitions), std::end(_transitions), rng);

      for (unsigned i = 0; i < _transitions.size(); i++) {
         if (_transitions[i]->isValid(timer, p, me))
            return _transitions[i];
      }
      return nullptr;
   }

   string toString(bool recursive = true) const
   {
      stringstream ss;
      ss << "state:{";
      ss << "name='" << name << "';";
      if (isFinal)
         ss << "FINAL";
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