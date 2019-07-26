#pragma once
#ifndef LIBBFT_SRC_CPP_MULTI_STSM_HPP
#define LIBBFT_SRC_CPP_MULTI_STSM_HPP

// system includes
#include <iostream> // TODO: remove
#include <vector>

#include <unistd.h> // TODO: remove busy sleep

// libbft includes

// Prototype?
#include "Event.hpp"
#include "SingleTimerStateMachine.hpp"

using namespace std; // TODO: remove

namespace libbft {

template<class Param>
struct MultiContext;

template<class Param = nullptr_t>
struct MachineContext
{
   Param* params;
   SingleTimerStateMachine<MultiContext<Param>>* machine;
   vector<Event<Param>*> events;

   MachineContext(Param* _params, SingleTimerStateMachine<MultiContext<Param>>* _machine)
     : params(_params)
     , machine(_machine)
   {
   }
};

template<class Param = nullptr_t>
struct MultiContext
{
   // vector of machines
   vector<MachineContext<Param>> vm;

   void broadcast(string event, int from)
   {
      for (unsigned i = 0; vm.size(); i++)
         if (i != from)
            sendTo(event, from, i);
   }

   void sendTo(string event, int from, int to)
   {
      vm[to].events.push_back(new Event<MachineContext<Param>>(event, event, from));
   }

   bool hasEvent(string type, int at)
   {
      for (unsigned i = 0; i < vm[at].events.size(); i++)
         if (vm[at].events[i]->getType() == type)
            return true;
      return false;
   }

   void processEvent(string type, int at)
   {
      for (unsigned i = 0; i < vm[at].events.size(); i++)
         if (vm[at].events[i]->getType() == type) {
            vm[at].events.erase(vm[at].events.begin() + i);
         }
   }
};

template<class MParam = nullptr_t>
struct ScheduledEvent
{
   Event<MParam>* event;
   Timer* timer;
   int from;
   int to;
};

template<class MParam = nullptr_t>
struct ScheduledTransition
{
   Transition<MParam>* transition;
   Timer* timer;
   int machine;

   // from = -1, if really global
   ScheduledTransition(Transition<MParam>* _transition, Timer* _timer, int _machine)
     : transition(_transition)
     , timer(_timer)
     , machine(_machine)
   {
   }
};

// TODO: inherits from single or from prototype? prototype would be much better...
template<class Param = nullptr_t>
class MultiSTSM : public SingleTimerStateMachine<MultiContext<Param>>
{
public:
   // includes several internal machines
   vector<SingleTimerStateMachine<MultiContext<Param>>*> machines;

   // includes several internal machines
   vector<ScheduledEvent<MultiContext<Param>>> scheduledEvents;

   // requires global transitions here... from inheritance. "Inherit or not inherit, that's the question"
   // create again with other name...
   vector<ScheduledTransition<MultiContext<Param>>> scheduledTransitions;
   // scheduled transitions may perhaps launch events on Action... must see if both are necessary

   void scheduleGlobal(ScheduledTransition<MultiContext<Param>> sch)
   {
      scheduledTransitions.push_back(sch);
   }

   void scheduleGlobal(Timer* when, int machine, Transition<MultiContext<Param>>* t)
   {
      scheduledTransitions.push_back(ScheduledTransition<MultiContext<Param>>(t, when, machine));
   }

public:
   MultiSTSM()
   {
   }

   void registerMachine(SingleTimerStateMachine<MultiContext<Param>>* m)
   {
      // something else?
      machines.push_back(m);
   }

   // initialize timer, etc
   virtual void initialize()
   {
      cout << "initializing multimachine" << endl;
      for (unsigned i = 0; i < machines.size(); i++)
         machines[i]->initialize();
   }

   bool allFinal(vector<State<MultiContext<Param>>*> states)
   {
      for (unsigned i = 0; i < states.size(); i++) {
         if (!states[i] || !states[i]->isFinal)
            return false;
      }
      return true;
   }

   bool processScheduledTransitions(vector<State<MultiContext<Param>>*>& states, MultiContext<Param>* p)
   {
      bool r = false;
      for (unsigned i = 0; i < scheduledTransitions.size(); i++) {
         if (scheduledTransitions[i].timer->expired()) // expired timer
         {
            int m = scheduledTransitions[i].machine; // get target machine
            if (scheduledTransitions[i].transition->isValid(*(machines[m]->timer), p, m)) {
               // isValid() transition
               cout << "processing scheduled transition on Machine " << m << ": " << scheduledTransitions[i].transition->toString() << endl;
               State<MultiContext<Param>>* next = scheduledTransitions[i].transition->execute(*(machines[m]->timer), p, m);
               cout << "updating state on machine " << m << " to " << next->toString() << endl;
               states[m] = next;
               // removing processed transition (or keep it?)
               scheduledTransitions.erase(scheduledTransitions.begin() + i);
               i--; // bad practice... use iterators on loop, instead
               r = true;
            }
         }
      }
      return r;
   }

   // execute the state machine (should be asynchonous for the future)
   // TODO: should be non-null?
   // this class should not inherit directly from Single, but from some prototype.. parameter 'mst' should not be here
   virtual void run(State<MultiContext<Param>>* mst, double MaxTime = 5.0, MultiContext<Param>* p = nullptr)
   {
      if (mst) {
         cout << "ERROR! CANNOT HAVE AN INITIAL MULTISTATE" << endl;
         return;
      }

      // 'mst' multistate will always be empty (only keeping eye on local states)

      cout << endl;
      cout << "========================" << endl;
      cout << "begin multimachine run()" << endl;
      cout << "========================" << endl;

      // initialize all machines
      this->initialize();

      // begin loop
      Timer watchdog;
      watchdog.init(MaxTime);
      watchdog.reset();

      vector<State<MultiContext<Param>>*> states(machines.size(), nullptr);

      // to simulate non-deterministic behavior
      //auto rng = std::default_random_engine{};

      // while current is null, or not final
      while (!allFinal(states)) {
         // check watchdog timer
         if (watchdog.expired()) {
            cout << "Multi StateMachine FAILED MAXTIME = " << MaxTime << endl;
            return;
         }

         // look for a scheduled / global transition (or event)
         bool b = processScheduledTransitions(states, p);
         if (b) {
            cout << "SOME GLOBAL TRANSITION HAPPENED!" << endl;
            watchdog.reset();
         }

         //auto _machines = this->machines; // copy to shuffle
         // simulate non-deterministic behavior
         //std::shuffle(std::begin(_machines), std::end(_machines), rng);

         // for now is fully deterministic, due to states (TODO: make random)
         for (unsigned i = 0; i < machines.size(); i++) {
            // evaluate situation on each machine
            State<MultiContext<Param>>* next_i = machines[i]->getNextState(states[i], p);
            if (next_i != states[i]) {
               cout << "machine " << i << " moved to state: " << next_i->toString() << endl;
               watchdog.reset();
               states[i]->onEnter(p); // really useful?
               states[i] = next_i;
            }
         }

         //cout << "sleeping a little bit... (TODO: improve busy sleep)" << endl;
         usleep(1000 * 100); // 100 milli (in microsecs)
      }

      cout << endl;
      cout << "======================" << endl;
      cout << "finished multimachine!" << endl;
      cout << "======================" << endl;
   }

   string toString()
   {
      stringstream ss;
      ss << "MultiSTSM [";
      for (unsigned i = 0; i < machines.size(); i++)
         ss << machines[i]->toString() << ";";
      ss << "]";
      return ss.str();
   }
};

} // libbft

#endif // LIBBFT_SRC_CPP_MULTI_STSM_HPP