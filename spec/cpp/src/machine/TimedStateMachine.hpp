#pragma once
#ifndef LIBBFT_SRC_CPP_TIMEDSTATEMACHINE_HPP
#define LIBBFT_SRC_CPP_TIMEDSTATEMACHINE_HPP

#include <memory>
#include <cstddef>
#include <unistd.h>

// default clock
#include "MachineId.hpp"
#include "timing/Clock.hpp"

namespace libbft {

/**
 * requires a StateType (perhaps TransitionType) and a machine-specific Param (context type)
 * @tparam StateType
 * @tparam Param
 */
template<class StateType, class Param = std::nullptr_t>
class TimedStateMachine
{
public:
   /** state machine clock */
   std::shared_ptr<Clock> clock;
   /** an identifier for itself */
   MachineId me{ 0 };
   /** string name */
   std::string name{ "" };

   /** a Timed State Machine requires a global clock, and a unique personal identifier */
   explicit TimedStateMachine(std::shared_ptr<Clock> _clock = nullptr, MachineId _me = 0, std::string _name = "")
     : clock(_clock)
     , me(std::move(_me))
     , name(std::move(_name))
   {
      // clock must exist
      if (!clock)
         clock = std::shared_ptr<Clock>(new Clock());
   }

   /**
    * TODO: delete lot's of stuff
    * unique_ptr the clock perhaps?
    */
   virtual ~TimedStateMachine() = default;

   /** triggers this, after state machine enters this state */
   virtual void onEnterState(StateType& current, Param* p) = 0;

   /**
    * do global processing (return 'true' means 'break' process)
    * TODO: may use a global processing FLAG here, to break
    */
   virtual bool beforeUpdateState(StateType& current, Param* p) = 0;

   /**
    * do global after-processing (return 'true' means 'break' process)
    * TODO: may use a global processing FLAG here, to break
    * @param current
    * @param p
    * @param updated
    * @return
    */
   virtual bool afterUpdateState(StateType& current, Param* p, bool updated)
   {
      // sleeping a little bit if not updated (avoid wasting resources)
      if (!updated)
         usleep(1000 * 100); // 100 milli (in microsecs)
      return false;          // do not 'break'
   }

   /**
    * get next state (null state is not allowed)
    * may return the same state, if nothing happened
    * @param current
    * @param p
    * @return
    */
   virtual bool updateState(StateType*& current, Param* p) = 0;

   virtual bool isFinal(const StateType& current, Param* p) = 0;

   /**
    * initialize runtime states and timers, etc
    * if initial state is not given, it must provide one here
    * @param current
    * @param p
    * @return
    */
   virtual StateType* initialize(StateType* current, Param* p) = 0;

   /**
    * launch when machine is finished
    * @param final
    * @param p
    */
   virtual void OnFinished(const StateType& final, Param* p) = 0;

   /**
    * execute the state machine (returns final state, or nullptr if on failure)
    * @param initial
    * @param p
    * @return
    */
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

   virtual std::string toString(std::string format = "")
   {
      std::stringstream ss;
      if (format == "graphviz") {

      } else {
         // standard text

         ss << "TSM {";
         ss << "#id = " << me.id << ";";
         ss << "clock = " << clock->toString() << ";";
         ss << "}";
      }
      return ss.str();
   }
};

} // libbft

#endif // LIBBFT_SRC_CPP_TIMEDSTATEMACHINE_HPP
