#pragma once
#ifndef LIBBFT_SRC_CPP_TIMEDSTATEMACHINE_HPP
#define LIBBFT_SRC_CPP_TIMEDSTATEMACHINE_HPP

// default clock
#include "../timing/Clock.hpp"

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

   // a Timed State Machine requires a global clock, and a unique personal identifier
   TimedStateMachine(Clock* _clock = nullptr, int _me = 0)
     : clock(_clock)
     , me(_me)
   {
      // clock must exist
      if (!clock)
         clock = new Clock();
   }

   virtual ~TimedStateMachine()
   {
      // TODO: delete lot's of stuff
      // unique_ptr the clock perhaps?
   }

   // triggers this, after state machine enters this state
   virtual void onEnterState(StateType& current, Param* p) = 0;

   // do global processing (return 'true' means 'break' process)
   // TODO: may use a global processing FLAG here, to break
   virtual bool beforeUpdateState(StateType& current, Param* p) = 0;

   // do global after-processing (return 'true' means 'break' process)
   // TODO: may use a global processing FLAG here, to break
   virtual bool afterUpdateState(StateType& current, Param* p, bool updated)
   {
      // sleeping a little bit if not updated (avoid wasting resources)
      if (!updated)
         usleep(1000 * 100); // 100 milli (in microsecs)
      return false;          // do not 'break'
   }

   // get next state (null state is not allowed)
   // may return the same state, if nothing happened
   virtual bool updateState(StateType*& current, Param* p) = 0;

   virtual bool isFinal(const StateType& current, Param* p) = 0;

   // initialize runtime states and timers, etc
   // if initial state is not given, it must provide one here
   virtual StateType* initialize(StateType* current, Param* p) = 0;

   // launch when machine is finished
   virtual void OnFinished(const StateType& final, Param* p) = 0;

   // execute the state machine (returns final state, or nullptr if on failure)
   virtual StateType* run(StateType* initial = nullptr, Param* p = nullptr)
   {
      StateType* current = this->initialize(initial, p);
      // if no state given, abort
      if (!current)
         return nullptr;

      onEnterState(*current, p);

      // while current is  not final
      while (!isFinal(*current, p)) {

         // preprocess stuff (watchdogs? global transitions?)
         if (beforeUpdateState(*current, p))
            return nullptr;

         bool updated = updateState(current, p);
         if (updated) {
            onEnterState(*current, p);
         }

         // post-process stuff (sleep if not evolving, etc)
         if (afterUpdateState(*current, p, updated))
            return nullptr;
      }

      OnFinished(*current, p);

      return current;
   }

   virtual string toString()
   {
      stringstream ss;
      ss << "TSM {";
      ss << "#id = " << me << ";";
      ss << "clock = " << clock->toString() << ";";
      ss << "}";
      return ss.str();
   }
};

} // libbft

#endif // LIBBFT_SRC_CPP_TIMEDSTATEMACHINE_HPP