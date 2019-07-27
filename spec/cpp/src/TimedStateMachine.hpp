#pragma once
#ifndef LIBBFT_SRC_CPP_TIMEDSTATEMACHINE_HPP
#define LIBBFT_SRC_CPP_TIMEDSTATEMACHINE_HPP

// default clock
#include "timing/Clock.hpp"

// default timer (this timer is only used for watchdog logic, not machine logic)
#include "timing/Timer.hpp"

using namespace std; // TODO: remove

namespace libbft {

// requires a StateType (perhaps TransitionType) and a machine-specific Param (context type)
template<class StateType, class Param = nullptr_t>
class TimedStateMachine
{
public:
   // state machine clock
   Clock* clock;
   // an identifier for itself
   int me{ 0 };

   // states for the state machine
   //vector<StateType*> states;
   // global transitions: may come from any state (including a null state)
   //vector<TransitionType*> globalTransitions;

   // Is it worth dealing with transitions here??

   TimedStateMachine(Clock* _clock = nullptr, int _me = 0)
     : clock(_clock)
     , me(_me)
   {
      if (!clock)
         clock = new Clock();
   }

   //void registerState(StateType* s)
   //{
   // something else?
   //   states.push_back(s);
   //}

   //void registerGlobal(TransitionType* t)
   //{
   // something else?
   //   globalTransitions.push_back(t);
   //}

   /*
   // TODO: keep it here?
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
   */

   virtual void onEnterState(StateType& current, Param* p) = 0;

   // do global processing
   virtual void beforeUpdateState(StateType& current, Param* p) = 0;

   // do global after-processing (what exactly?)
   virtual void afterUpdateState(StateType& current, Param* p) = 0;

   // get next state (null state is not allowed)
   // may return the same state, if nothing happened
   virtual bool updateState(StateType*& current, Param* p) = 0;

   virtual bool isFinal(const StateType& current, Param* p) = 0;

   // initialize runtime states and timers, etc
   virtual void initialize() = 0;

   // execute the state machine (should be asynchonous for the future)
   // TODO: should be non-null?
   virtual void run(StateType& initial, double MaxTime = -1.0, Param* p = nullptr)
   {
      StateType* current = &initial;

      cout << endl;
      cout << "===========" << endl;
      cout << "begin run()" << endl;
      cout << "===========" << endl;

      this->initialize();

      // TODO: remove watchdog here and MaxTime parameter!
      // it should be implementation specific using initialize()
      Timer watchdog;
      // -1.0 means infinite time here
      watchdog.init(MaxTime);
      watchdog.reset();

      onEnterState(*current, p);

      // while current is  not final
      while (!isFinal(*current, p)) {

         beforeUpdateState(*current, p);

         // check watchdog timer
         if (watchdog.expired()) {
            cout << "TimedStateMachine FAILED MAXTIME = " << MaxTime << endl;
            break;
         }

         bool updated = updateState(current, p);
         if (updated) {
            //cout << "moved to state: " << current->toString() << endl;
            // TODO: try this using operator<<
            watchdog.reset();
            onEnterState(*current, p);
            //current->onEnter(p); // really useful?
            //current = next;
         }

         afterUpdateState(*current, p);

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
      ss << "TSM {";
      ss << "#id = " << me << ";";
      ss << "}";
      return ss.str();
   }
};

} // libbft

#endif // LIBBFT_SRC_CPP_TIMEDSTATEMACHINE_HPP