#pragma once
#ifndef LIBBFT_SRC_CPP_REPLICATED_STSM_HPP
#define LIBBFT_SRC_CPP_REPLICATED_STSM_HPP

// system includes
#include <iostream> // TODO: remove
#include <vector>

#include <assert.h> // TODO: remove
#include <unistd.h> // TODO: remove busy sleep

// libbft includes

// Prototype?
#include "../events/Event.hpp"
#include "../machine/TimedStateMachine.hpp"
#include "../single/SingleTimerStateMachine.hpp"

using namespace std; // TODO: remove

namespace libbft {

template<class Param>
struct MultiContext;

template<class Param = nullptr_t>
struct MachineContext
{
   Param* params;
   SingleTimerStateMachine<MultiContext<Param>>* machine;
   vector<Event*> events;

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
   void broadcast(string event, int from, string parameters)
   {
      broadcast(new Event(event, from, parameters), from);
   }

   // from may be -1, if broadcasted from system
   void broadcast(Event* event, int from)
   {
      for (unsigned i = 0; vm.size(); i++)
         if (i != from)
            sendTo(event, i); // this may break with memory leaks (TODO: use shared_ptr, or copy-based final class)
   }

   // 'to' should be valid (0 <= to <= R)
   void sendTo(Event* event, int to)
   {
      assert((to >= 0) && (to < vm.size()));
      vm[to].events.push_back(event);
   }

   // 'from' may be -1, if broadcasted from system
   // 'to' should be valid (0 <= to <= R)
   void sendTo(string event, int from, int to, string parameters)
   {
      sendTo(new Event(event, from, parameters), to);
   }

   bool hasEvent(string name, int at, string parameters)
   {
      for (unsigned i = 0; i < vm[at].events.size(); i++)
      {
         //cout << "comparing " << name << "(" << parameters << ") with: " << vm[at].events[i]->toString() << endl;
         if (vm[at].events[i]->isActivated(name, parameters))
            return true;
      }
      return false;
   }

   void consumeEvent(string name, int at, string parameters)
   {
      for (unsigned i = 0; i < vm[at].events.size(); i++)
         if (vm[at].events[i]->isActivated(name, parameters)) {
            vm[at].events.erase(vm[at].events.begin() + i);
            return;
         }
   }

   /*
   void processEvent(string name, int at)
   {
      for (unsigned i = 0; i < vm[at].events.size(); i++)
         if (vm[at].events[i]->getType() == name) {
            vm[at].events.erase(vm[at].events.begin() + i);
         }
   }
*/
};

// Scheduled class: launches a 'thing' (type T) after clock has expired
// simple class to hold information, could be an std::array perhaps, if named
struct ScheduledEvent
{
   string name;       // event name
   string parameters; // event parameters
   double countdown;
   int machine;

   ScheduledEvent(string _name, double _countdown, int _machine, string _parameters)
     : name(_name)
     , parameters(_parameters)
     , countdown(_countdown)
     , machine(_machine)
   {
   }
};

template<class Param>
using MultiState = vector<State<MultiContext<Param>>*>;

template<class Param = nullptr_t>
class ReplicatedSTSM : public TimedStateMachine<MultiState<Param>, MultiContext<Param>>
{
public:
   // includes several internal machines
   vector<SingleTimerStateMachine<MultiContext<Param>>*> machines;

   // includes several internal machines
   vector<ScheduledEvent> scheduledEvents;

   // requires global transitions here... from inheritance. "Inherit or not inherit, that's the question"
   // create again with other name...
   //vector<Scheduled<Transition<MultiContext<Param>>>> scheduledTransitions;
   // scheduled transitions may perhaps launch events on Action... must see if both are necessary

   // watchdog timer
   Timer* watchdog{ nullptr };

   // MaxTime -1.0 means infinite time
   // positive time is real expiration time
   void setWatchdog(double MaxTime)
   {
      watchdog = (new Timer())->init(MaxTime);
   }

   //void scheduleGlobalTransition(Scheduled<Transition<MultiContext<Param>>> sch)
   //{
   //   scheduledTransitions.push_back(sch);
   //}

   //void scheduleGlobalTransition(Timer* when, int machine, Transition<MultiContext<Param>>* t)
   //{
   //   scheduledTransitions.push_back(Scheduled(t, when, machine));
   //}

   //void scheduleEvent(Timer* when, int machine, Event<MultiContext<Param>>* e)
   void scheduleEvent(double countdown, int machine, string name, string parameters)
   {
      scheduledEvents.push_back(ScheduledEvent(name, countdown, machine, parameters));
   }

public:
   ReplicatedSTSM(Clock* _clock = nullptr, int _me = 0, string _name = "")
     : TimedStateMachine<MultiState<Param>, MultiContext<Param>>(_clock, _me, _name)
   {
   }

   virtual ~ReplicatedSTSM()
   {
      // TODO: delete lot's of stuff
      // unique_ptr the clock perhaps?
   }

   void registerMachine(SingleTimerStateMachine<MultiContext<Param>>* m)
   {
      // something else?
      machines.push_back(m);
   }

   void launchScheduledEvents(MultiContext<Param>* p)
   {
      cout << "launching scheduled events!" << endl;
      // launch all scheduled events
      for (unsigned i = 0; i < scheduledEvents.size(); i++) {
         ScheduledEvent e = scheduledEvents[i];
         if (e.machine == -1) {
            // broadcast event
            p->broadcast(new TimedEvent(e.countdown, e.name, -1, e.parameters), -1);
         } else {
            // target machine event
            p->sendTo(new TimedEvent(e.countdown, e.name, -1, e.parameters), e.machine);
         }
      }
   }

   // initialize timer, etc, and also, setup first state (if not given)
   virtual MultiState<Param>* initialize(MultiState<Param>* current, MultiContext<Param>* p) override
   {
      // check if there's initial state available
      if (!current)
         current = new MultiState<Param>(machines.size(), nullptr);

      cout << endl;
      cout << "===========" << endl;
      cout << "begin run()" << endl;
      cout << "===========" << endl;

      cout << "initializing multimachine" << endl;
      if (watchdog)
         watchdog->reset();
      else
         cout << "No watchdog configured!" << endl;
      for (unsigned i = 0; i < machines.size(); i++)
         machines[i]->initialize(current->at(i), p);

      launchScheduledEvents(p);

      return current;
   }

   // launch when machine is finished
   virtual void OnFinished(const MultiState<Param>& states, MultiContext<Param>* p) override
   {
      cout << endl;
      cout << "=================" << endl;
      cout << "finished machine!" << endl;
      cout << "=================" << endl;
   }

   virtual bool isFinal(const MultiState<Param>& states, MultiContext<Param>* p) override
   {
      for (unsigned i = 0; i < states.size(); i++) {
         if (!states[i] || !states[i]->isFinal)
            return false;
      }
      return true;
   }

   /*
   // perhaps just processGlobalTransitions here (both scheduled and non-scheduled)
   bool processScheduledGlobalTransitions(MultiState<Param>& states, MultiContext<Param>* p)
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
*/

   virtual bool updateState(MultiState<Param>*& states, MultiContext<Param>* p) override
   {
      bool ret = false;
      for (unsigned i = 0; i < machines.size(); i++) {
         // evaluate situation on each machine
         bool r = machines[i]->updateState(states->at(i), p);
         if (r) {
            cout << "machine " << i << " moved to state: " << states->at(i)->toString() << endl;
            //states->at(i)->onEnter(p); // really useful?
            ret = true;
         }
      }
      return ret;
   }

   virtual void onEnterState(MultiState<Param>& current, MultiContext<Param>* p) override
   {
      cout << "updating multi state! STATES:" << endl;
      for (unsigned i = 0; i < current.size(); i++) {
         cout << "Machine " << i << " => " << current[i]->toString() << endl;
      }

      if (watchdog)
         watchdog->reset();
   }

   virtual bool beforeUpdateState(MultiState<Param>& states, MultiContext<Param>* p) override
   {
      // check watchdog
      if (watchdog && watchdog->expired()) {
         cout << "StateMachine FAILED MAXTIME" << watchdog->getCountdown() << endl;
         return true;
      }
      /*
      // process events
      bool re = processScheduledEvents(p);
      if (re) {
         cout << "SOME EVENT HAPPENED!" << endl;
         //watchdog.reset(); // TODO: make watchdog part of this specific class
      }

      // look for a scheduled global transition (or event)
      bool b = processScheduledGlobalTransitions(states, p);
      if (b) {
         cout << "SOME GLOBAL TRANSITION HAPPENED!" << endl;
         //watchdog.reset(); // TODO: make watchdog part of this specific class
      }
*/
      return false;
   }

   virtual string toString(string format = "") override
   {
      stringstream ss;
      if (format == "graphviz") {

      } else {
         // standard text

         ss << "ReplicatedSTSM [";
         for (unsigned i = 0; i < machines.size(); i++)
            ss << machines[i]->toString() << ";";
         ss << "]";
      }
      return ss.str();
   }
};

} // libbft

#endif // LIBBFT_SRC_CPP_REPLICATED_STSM_HPP