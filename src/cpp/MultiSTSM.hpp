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

// TODO: inherits from single or from prototype? prototype would be much better...
template<class Param = nullptr_t>
class MultiSTSM : public SingleTimerStateMachine<MultiContext<Param>>
{
public:
   // includes several internal machines
   vector<SingleTimerStateMachine<MultiContext<Param>>*> machines;

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

   bool multiIsFinal(vector<State<MultiContext<Param>>*> states)
   {
      for (unsigned i = 0; i < states.size(); i++) {
         if (!states[i] || !states[i]->isFinal)
            return false;
      }
      return true;
   }

   // execute the state machine (should be asynchonous for the future)
   // TODO: should be non-null?
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
      watchdog.init(this->MaxTime);
      watchdog.reset();

      vector<State<MultiContext<Param>>*> states(machines.size(), nullptr);

      // to simulate non-deterministic behavior
      //auto rng = std::default_random_engine{};

      // while current is null, or not final
      while (!this->multiIsFinal(states)) {
         // check watchdog timer
         if (watchdog.expired()) {
            cout << "Multi StateMachine FAILED MAXTIME = " << this->MaxTime << endl;
            return;
         }

         //auto _machines = this->machines; // copy to shuffle
         // simulate non-deterministic behavior
         //std::shuffle(std::begin(_machines), std::end(_machines), rng);

         // for now is fully deterministic, due to states (TODO: make random)
         for (unsigned i = 0; i < machines.size(); i++) {
            // evaluate situation on each machine
            State<Param>* next_i = machines[i]->getNextState(states[i], p);
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