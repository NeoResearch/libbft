#pragma once
#ifndef LIBBFT_SRC_CPP_MULTI_STSM_HPP
#define LIBBFT_SRC_CPP_MULTI_STSM_HPP

// system includes
#include <iostream> // TODO: remove
#include <vector>

#include <assert.h> // TODO: remove
#include <unistd.h> // TODO: remove busy sleep

// libbft includes

// Prototype?
#include "Event.hpp"
#include "../SingleTimerStateMachine.hpp"

using namespace std; // TODO: remove

namespace libbft {

template<class Param>
struct MultiContext;

template<class Param = nullptr_t>
struct MachineContext
{
   Param* params;
   SingleTimerStateMachine<MultiContext<Param>>* machine;
   vector<Event<MultiContext<Param>>*> events;

   //void addEvent(Event<MultiContext<Param>>* e)
   //{
   //   events.push_back(e);
   //}

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

   // from may be -1, if broadcasted from system
   void broadcast(string event, int from)
   {
      broadcast(new Event<MultiContext<Param>>(event, event, from), from);
   }

   // from may be -1, if broadcasted from system
   void broadcast(Event<MultiContext<Param>>* event, int from)
   {
      for (unsigned i = 0; vm.size(); i++)
         if (i != from)
            sendTo(event, i); // this may break with memory leaks (TODO: use shared_ptr, or copy-based final class)
   }

   // 'to' should be valid (0 <= to <= R)
   void sendTo(Event<MultiContext<Param>>* event, int to)
   {
      assert((to >= 0) && (to < vm.size()));
      vm[to].events.push_back(event);
   }

   // 'from' may be -1, if broadcasted from system
   // 'to' should be valid (0 <= to <= R)
   void sendTo(string event, int from, int to)
   {
      sendTo(new Event<MultiContext<Param>>(event, event, from), to);
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

// Scheduled class: launches a 'thing' (type T) after clock has expired
// simple class to hold information, could be an std::array perhaps, if named
template<class T>
struct Scheduled
{
   T* thing;
   Timer* timer;
   int machine;

   Scheduled(T* _thing, Timer* _timer, int _machine)
     : thing(_thing)
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
   vector<Scheduled<Event<MultiContext<Param>>>> scheduledEvents;

   // requires global transitions here... from inheritance. "Inherit or not inherit, that's the question"
   // create again with other name...
   vector<Scheduled<Transition<MultiContext<Param>>>> scheduledTransitions;
   // scheduled transitions may perhaps launch events on Action... must see if both are necessary

   void scheduleGlobalTransition(Scheduled<Transition<MultiContext<Param>>> sch)
   {
      scheduledTransitions.push_back(sch);
   }

   void scheduleGlobalTransition(Timer* when, int machine, Transition<MultiContext<Param>>* t)
   {
      scheduledTransitions.push_back(Scheduled(t, when, machine));
   }

   void scheduleEvent(Timer* when, int machine, Event<MultiContext<Param>>* e)
   {
      scheduledEvents.push_back(Scheduled(e, when, machine));
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

   // perhaps just processGlobalTransitions here (both scheduled and non-scheduled)
   bool processScheduledGlobalTransitions(vector<State<MultiContext<Param>>*>& states, MultiContext<Param>* p)
   {
      bool r = false;
      for (unsigned i = 0; i < scheduledTransitions.size(); i++) {
         if (scheduledTransitions[i].timer->expired()) // expired timer
         {
            int m = scheduledTransitions[i].machine; // get target machine
            if (scheduledTransitions[i].thing->isValid(*(machines[m]->timer), p, m)) {
               // isValid() transition
               cout << "processing scheduled transition on Machine " << m << ": " << scheduledTransitions[i].thing->toString() << endl;
               State<MultiContext<Param>>* next = scheduledTransitions[i].thing->execute(*(machines[m]->timer), p, m);
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

   bool processScheduledEvents(MultiContext<Param>* p)
   {
      bool r = false;
      for (unsigned i = 0; i < scheduledEvents.size(); i++) {
         if (scheduledEvents[i].timer->expired()) // expired timer
         {
            int to = scheduledEvents[i].machine;           // get target machine
            if (to == -1)                                  // broadcast
               p->broadcast(scheduledEvents[i].thing, -1); // from -1 (from system)
            else
               p->sendTo(scheduledEvents[i].thing, to);
            // removing processed event
            scheduledEvents.erase(scheduledEvents.begin() + i);
            i--; // bad practice... use iterators on loop, instead
            r = true;
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

         // process events
         bool re = processScheduledEvents(p);
         if (re) {
            cout << "SOME EVENT HAPPENED!" << endl;
            watchdog.reset();
         }

         // look for a scheduled global transition (or event)
         bool b = processScheduledGlobalTransitions(states, p);
         if (b) {
            cout << "SOME GLOBAL TRANSITION HAPPENED!" << endl;
            watchdog.reset();
         }

         //auto _machines = this->machines; // copy to shuffle
         // simulate non-deterministic behavior
         //std::shuffle(std::begin(_machines), std::end(_machines), rng);

         // for now is fully deterministic, due to states (TODO: make random, but remember to preserve order between 'states' and 'machines')
         for (unsigned i = 0; i < machines.size(); i++) {
            // evaluate situation on each machine
            State<MultiContext<Param>>* next_i = machines[i]->getNextState(states[i], p);
            if (next_i != states[i]) {
               cout << "machine " << i << " moved to state: " << next_i->toString() << endl;
               watchdog.reset();
               states[i] = next_i;
               states[i]->onEnter(p); // really useful?
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