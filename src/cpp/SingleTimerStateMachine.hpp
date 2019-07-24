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
   Timer timer;

   vector<State*> states;

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

      cout << "begin run()" << endl;

      cout << "will start timer" << endl;
      timer.start();

      cout << "will get first state" << endl;
      State* current = states[index];

      cout << "will loop" << endl;
      while (!current->isFinal) {
         cout << "finding transition! ...";
         Transition* go = current->tryGetTransition(timer);
         if (go) {
            cout << "found valid transition! " << go->toString() << endl;
            current = go->to;
         }
         cout << "sleeping a little bit... (TODO: improve busy sleep)" << endl;
         usleep(1000*100); // 100 milli (in microsecs)
      }

      cout << "finished machine!" << endl;
   }
};

} // libbft

#endif // LIBBFT_SRC_CPP_SINGLETIMERSTATEMACHINE_HPP