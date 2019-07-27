#pragma once
#ifndef LIBBFT_SRC_CPP_SINGLETIMERSTATEMACHINE_HPP
#define LIBBFT_SRC_CPP_SINGLETIMERSTATEMACHINE_HPP

// system includes
#include <iostream> // TODO: remove
#include <vector>

#include <assert.h> // TODO: remove
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
class SingleTimerStateMachine : public TimedStateMachine<State<Param>, Param>
{
public:
   // state machine timer // TODO: really keep it global?
   Timer* timer;

   // watchdog timer
   Timer* watchdog{ nullptr };

   // states for the state machine
   vector<State<Param>*> states;
   // global transitions: may come from any state (including a null state)
   vector<Transition<Param>*> globalTransitions;

   // specific timer
   SingleTimerStateMachine(Timer* t = nullptr, int me = 0, Clock* _clock = nullptr)
     : timer(t)
     , TimedStateMachine<State<Param>, Param>(_clock, me)
   {
      // timer must exist
      if (!timer)
         timer = new Timer("", this->clock);
   }

   State<Param>* getStateByName(string name)
   {
      for (unsigned i = 0; i < states.size(); i++)
         if (states[i]->name == name)
            return states[i];
      return nullptr; // not found
   }

   void registerState(State<Param>* s)
   {
      assert(s != nullptr);
      states.push_back(s);
   }

   void registerGlobal(Transition<Param>* t)
   {
      assert(t != nullptr);
      globalTransitions.push_back(t);
   }

   virtual Transition<Param>* findGlobalTransition(Param* p)
   {
      // TODO: shuffle global?
      vector<Transition<Param>*> _transitions = globalTransitions;
      for (unsigned i = 0; i < _transitions.size(); i++) {
         if (_transitions[i]->isValid(*timer, p, this->me))
            return _transitions[i];
      }
      return nullptr;
   }

   virtual void onEnterState(State<Param>& current, Param* p) override
   {
      cout << "entering state: " << current.toString() << endl;

      if (watchdog)
         watchdog->reset();
   }

   virtual bool isFinal(const State<Param>& current, Param* p) override
   {
      return current.isFinal;
   }

   // get next state (current may be null, waiting for global transition)
   // may return the same state, if nothing happened
   virtual bool updateState(State<Param>*& outcurrent, Param* p) override
   {
      State<Param>* current = outcurrent;
      bool r = false;
      // find global transition
      Transition<Param>* gt = findGlobalTransition(p);
      if (gt) {
         // found global transition
         cout << "-> found valid global transition! " << gt->toString() << endl;
         current = gt->execute(*timer, p, this->me);
         r = true;
      }

      //cout << "finding transition! ...";
      if (current) {
         Transition<Param>* go = current->tryGetTransition(*timer, p, this->me);
         if (go) {
            cout << "-> found valid transition! " << go->toString() << endl;
            current = go->execute(*timer, p, this->me);
            r = true;
         }
      }
      outcurrent = current;
      return r;
   }

   // MaxTime -1.0 means infinite time
   // positive time is real expiration time
   void setWatchdog(double MaxTime)
   {
      watchdog = (new Timer())->init(MaxTime);
   }

   // initialize timer, etc
   virtual void initialize() override
   {
      cout << "OnInitialize() Single MACHINE!" << endl;

      if (watchdog)
         watchdog->reset();
      else
         cout << "No watchdog configured!" << endl;

      cout << "will initialize timer" << endl;
      timer->init();

      cout << "will reset timer" << endl;
      timer->reset();
   }

   virtual bool beforeUpdateState(State<Param>& current, Param* p) override
   {
      if (watchdog && watchdog->expired()) {
         cout << "StateMachine FAILED MAXTIME" << watchdog->getCountdown() << endl;
         return true;
      }

      // nothing to do?
      return false;
   }

   string toString()
   {
      stringstream ss;
      ss << "STSM {";
      ss << "#id = " << this->me << ";";
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