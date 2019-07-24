#ifndef LIBBFT_SRC_CPP_SINGLETIMERSTATEMACHINE_HPP
#define LIBBFT_SRC_CPP_SINGLETIMERSTATEMACHINE_HPP

// system includes
#include <iostream> // TODO: remove
#include <vector>

#include <unistd.h> // TODO: remove busy sleep

// libbft includes

// Prototype?
#include "TimedStateMachine.hpp"
// default timer
#include "Timer.hpp"
// default state
#include "State.h"
// default timer
#include "Timer.hpp"

using namespace std; // TODO: remove

namespace libbft {

class SingleTimerStateMachine
{
public:
   // state machine timer
   Timer* timer;

   vector<State*> states;

   SingleTimerStateMachine()
   {
      // generic Timer
      timer = new Timer();
   }

   // specific timer
   SingleTimerStateMachine(Timer* t)
     : timer(t)
   {
   }

   void registerState(State* s)
   {
      // something else?
      states.push_back(s);
   }

   // execute the state machine (should be asynchonous for the future)
   // index is the index of initial state
   virtual void run(int index = 0)
   {
      if (index >= states.size()) {
         cout << "not enough states! size=" << states.size() << endl;
         return;
      }

      cout << endl;
      cout << "===========" << endl;
      cout << "begin run()" << endl;
      cout << "===========" << endl;

      cout << "will initialize timer" << endl;
      timer->initialize();

      cout << "will reset timer" << endl;
      timer->reset();

      // get first state
      State* current = states[index];

      // begin loop
      cout << "begin loop at state: " << current->toString() << endl;
      while (!current->isFinal) {
         //cout << "finding transition! ...";
         Transition* go = current->tryGetTransition(*timer);
         if (go) {
            cout << "-> found valid transition! " << go->toString() << endl;
            current = go->to;
            cout << "moved to state: " << current->toString() << endl;
         }
         //cout << "sleeping a little bit... (TODO: improve busy sleep)" << endl;
         usleep(1000 * 100); // 100 milli (in microsecs)
      }

      cout << endl;
      cout << "=================" << endl;
      cout << "finished machine!" << endl;
      cout << "=================" << endl;
   }

   string toString()
   {
      stringstream ss;
      ss << "STSM {";
      ss << "Timer='" << timer->toString() << "';";
      ss << "States=[";
      for (unsigned i = 0; i < states.size(); i++)
         ss << states[i]->toString() << ";";
      ss << "]";
      ss << "}";
      return ss.str();
   }
};

} // libbft

#endif // LIBBFT_SRC_CPP_SINGLETIMERSTATEMACHINE_HPP