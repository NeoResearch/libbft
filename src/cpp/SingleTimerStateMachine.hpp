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

template<class Param = nullptr_t>
class SingleTimerStateMachine
{
public:
   // state machine timer
   Timer* timer;

   vector<State<Param>*> states;

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

   void registerState(State<Param>* s)
   {
      // something else?
      states.push_back(s);
   }

   // execute the state machine (should be asynchonous for the future)
   // TODO: should be non-null?
   virtual void run(State<Param>& first, Param* p = nullptr)
   {
      State<Param>* current = &first;

      cout << endl;
      cout << "===========" << endl;
      cout << "begin run()" << endl;
      cout << "===========" << endl;

      cout << "will initialize timer" << endl;
      timer->initialize();

      cout << "will reset timer" << endl;
      timer->reset();

      // visit first state (really useful?)
      current->onEnter(p);

      // begin loop
      cout << "begin loop at state: " << current->toString() << endl;
      while (!current->isFinal) {
         //cout << "finding transition! ...";
         Transition<Param>* go = current->tryGetTransition(*timer);
         if (go) {
            cout << "-> found valid transition! " << go->toString() << endl;
            current = go->execute(*timer);
            cout << "moved to state: " << current->toString() << endl;
            current->onEnter(p);
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