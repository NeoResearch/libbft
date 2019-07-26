#pragma once
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
#include "timing/Timer.hpp"
// default state
#include "State.hpp"

using namespace std; // TODO: remove

namespace libbft {

template<class Param = nullptr_t>
class SingleTimerStateMachine
{
public:
   // state machine timer // TODO: really keep it global?
   Timer* timer;
   int me{ 0 };

   // states for the state machine
   vector<State<Param>*> states;
   // global transitions: may come from any state (including a null state)
   vector<Transition<Param>*> globalTransitions;

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

   void registerGlobal(Transition<Param>* t)
   {
      // something else?
      globalTransitions.push_back(t);
   }

   virtual Transition<Param>* findGlobalTransition(Param* p)
   {
      // TODO: shuffle global?
      vector<Transition<Param>*> _transitions = globalTransitions;
      for (unsigned i = 0; i < _transitions.size(); i++) {
         if (_transitions[i]->isValid(*timer, p, me))
            return _transitions[i];
      }
      return nullptr;
   }

   // get next state (current may be null, waiting for global transition)
   // may return the same state, if nothing happened
   virtual State<Param>* getNextState(State<Param>* current, Param* p)
   {
      // find global transition
      Transition<Param>* gt = findGlobalTransition(p);
      if (gt) {
         // found global transition
         cout << "-> found valid global transition! " << gt->toString() << endl;
         current = gt->execute(*timer, p, me);
      }

      //cout << "finding transition! ...";
      if (current) {
         Transition<Param>* go = current->tryGetTransition(*timer, p, me);
         if (go) {
            cout << "-> found valid transition! " << go->toString() << endl;
            current = go->execute(*timer, p, me);
         }
      }
      return current;
   }

   // initialize timer, etc
   virtual void initialize()
   {
      cout << "will initialize timer" << endl;
      timer->init();

      cout << "will reset timer" << endl;
      timer->reset();
   }

   // execute the state machine (should be asynchonous for the future)
   // TODO: should be non-null?
   virtual void run(State<Param>* current, double MaxTime = 5.0, Param* p = nullptr)
   {
      // current state may be null, waiting for a globalTransition

      cout << endl;
      cout << "===========" << endl;
      cout << "begin run()" << endl;
      cout << "===========" << endl;

      this->initialize();

      Timer watchdog;
      watchdog.init(MaxTime);
      watchdog.reset();

      // while current is null, or not final
      while (!current || !current->isFinal) {
         // check watchdog timer
         if (watchdog.expired()) {
            cout << "StateMachine FAILED MAXTIME = " << MaxTime << endl;
            break;
         }

         State<Param>* next = getNextState(current, p);
         if (next != current) {
            cout << "moved to state: " << next->toString() << endl;
            watchdog.reset();
            next->onEnter(p); // really useful?
            current = next;
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
      ss << "#id = " << me << ";";
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