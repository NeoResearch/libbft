#pragma once
#ifndef LIBBFT_SRC_CPP_DBFT2_DBFT2_RPC_MACHINE_HPP
#define LIBBFT_SRC_CPP_DBFT2_DBFT2_RPC_MACHINE_HPP

// system includes
#include <iostream> // TODO: remove
#include <sstream>
#include <vector>
// simulate non-deterministic nature
#include <algorithm>
#include <assert.h>
#include <random>

// standard Transition
//#include "../replicated/ReplicatedSTSM.hpp"
#include "../single/SingleTimerStateMachine.hpp"
#include "../single/Transition.hpp"
#include "../timing/Timer.hpp"

// dbft specific
#include "dBFT2Context.hpp"

// rpc part
#include "../bftevents-grpc/BFTEventsServer.hpp"
#include "../rpc-replicated/RPCMachineContext.hpp"

#include "../events/ScheduledEvent.hpp"

using namespace std; // TODO: remove

namespace libbft {

// This an RPC-based dBFT2 machine

class dBFT2RPCMachine : public SingleTimerStateMachine<RPCMachineContext<dBFT2Context>>
{
public:
   // max number of faulty nodes
   int f;
   // events server to receive info from other nodes
   BFTEventsServer<dBFT2Context> eventsServer;

   // trying to avoid scheduled events here... this is a real machine, not for testing.
   ///std::vector<ScheduledEvent> schedEvents;

   // it is recommended to have N = 3f+1 (e.g., f=0 -> N=1; f=1 -> N=4; f=2 -> N=7; ...)
   dBFT2RPCMachine(int _f = 0, int N = 1, MachineId _me = MachineId(), RPCMachineContext<dBFT2Context>* myCtx = nullptr, string _name = "dBFT2_RPC_machine", Clock* _clock = nullptr)
     : SingleTimerStateMachine<RPCMachineContext<dBFT2Context>>(new Timer("C", _clock), _me, _clock, _name)
     , f(_f)
     , eventsServer(_me.id, myCtx)
   //Timer* t = nullptr, int me = 0, Clock* _clock = nullptr, string name = "STSM"
   {
      assert(f >= 0);
      assert(N >= 1);
      assert(f <= N);
      // create 'world' here? TODO:

      // create N machines
      //this->machines = vector<SingleTimerStateMachine<RPCMachineContext<dBFT2Context>>*>(N, nullptr);

      // initialize independent machines (each one with its Timer, sharing same global Clock)
      // should never share Timers here, otherwise strange things may happen (TODO: protect from this... unique_ptr?)
      //for (unsigned i = 0; i < N; i++) {
      //   this->machines[i] = new SingleTimerStateMachine<RPCMachineContext<dBFT2Context>>(new Timer("C", this->clock), i, this->clock, "dBFT");
      //}

      // fill states and transitions on each machine
      ////for (unsigned i = 0; i < N; i++)
      // now not using i, single machine

      fillStatesForMachine();
   }

   virtual ~dBFT2RPCMachine()
   {
      // TODO: delete lot's of stuff
      // unique_ptr the clock perhaps? shared_ptr?
      // very dangerous to delete like this... clock may be shared.
   }

   void fillSimpleCycle()
   {
      // ---------------------
      // declaring dBFT states
      // ---------------------

      auto preinitial = new State<RPCMachineContext<dBFT2Context>>(false, "PreInitial");
      this->registerState(preinitial);
      auto started = new State<RPCMachineContext<dBFT2Context>>(false, "Started");
      this->registerState(started);
      auto backup = new State<RPCMachineContext<dBFT2Context>>(false, "Backup");
      this->registerState(backup);
      auto primary = new State<RPCMachineContext<dBFT2Context>>(false, "Primary");
      this->registerState(primary);
      auto reqSentOrRecv = new State<RPCMachineContext<dBFT2Context>>(false, "RequestSentOrReceived");
      this->registerState(reqSentOrRecv);
      auto commitSent = new State<RPCMachineContext<dBFT2Context>>(false, "CommitSent");
      this->registerState(commitSent);
      auto viewChanging = new State<RPCMachineContext<dBFT2Context>>(false, "ViewChanging");
      this->registerState(viewChanging);
      auto blockSent = new State<RPCMachineContext<dBFT2Context>>(true, "BlockSent");
      this->registerState(blockSent);

      // -------------------------
      // creating dBFT transitions
      // -------------------------

      //auto machine = this;
      //int id = this->me.id;

      // preinitial -> started
      preinitial->addTransition(
        (new Transition<RPCMachineContext<dBFT2Context>>(started))->add(Condition<RPCMachineContext<dBFT2Context>>("OnStart()", [](const Timer& t, RPCMachineContext<dBFT2Context>* d, MachineId me) -> bool {
           cout << "Waiting for OnStart..." << endl;
           return d->hasEvent("OnStart", vector<string>(0)); // no parameters on event
        })));

      // initial -> backup
      started->addTransition(
        (new Transition<RPCMachineContext<dBFT2Context>>(backup))->add(Condition<RPCMachineContext<dBFT2Context>>("not (H+v) mod R = i", [](const Timer& t, RPCMachineContext<dBFT2Context>* d, MachineId me) -> bool {
           cout << "lambda1" << endl;
           return !((d->params->H + d->params->v) % d->params->R == me.id);
        })));

      // initial -> primary
      started->addTransition(
        (new Transition<RPCMachineContext<dBFT2Context>>(primary))->add(Condition<RPCMachineContext<dBFT2Context>>("(H+v) mod R = i", [](const Timer& t, RPCMachineContext<dBFT2Context>* d, MachineId me) -> bool {
           cout << "lambda2 H=" << d->params->H << " v=" << d->params->v << " me=" << me.id << endl;
           return (d->params->H + d->params->v) % d->params->R == me.id;
        })));

      // backup -> reqSentOrRecv
      auto toReqSentOrRecv1 = new Transition<RPCMachineContext<dBFT2Context>>(reqSentOrRecv);
      backup->addTransition(
        toReqSentOrRecv1->add(Condition<RPCMachineContext<dBFT2Context>>("OnPrepareRequest(v)", [](const Timer& t, RPCMachineContext<dBFT2Context>* d, MachineId me) -> bool {
           cout << "waiting for event OnPrepareRequest at " << me.id << " for view " << d->params->v << endl;
           stringstream ss;
           ss << d->params->v;
           vector<string> params(1, ss.str());
           //cout << "for " << ss.str() << endl;
           return d->hasEvent("OnPrepareRequest", params);
        })));

      // reqSentOrRecv -> commitSent
      reqSentOrRecv->addTransition(
        (new Transition<RPCMachineContext<dBFT2Context>>(commitSent))->add(Condition<RPCMachineContext<dBFT2Context>>("(H+v) mod R = i", [](const Timer& t, RPCMachineContext<dBFT2Context>* d, MachineId me) -> bool {
           cout << "nothing to do... assuming all preparations were received!" << endl;
           return true;
        })));

      // commitSent -> blockSent
      commitSent->addTransition(
        (new Transition<RPCMachineContext<dBFT2Context>>(blockSent))->add(Condition<RPCMachineContext<dBFT2Context>>("(H+v) mod R = i", [](const Timer& t, RPCMachineContext<dBFT2Context>* d, MachineId me) -> bool {
           cout << "nothing to do... assuming all commits were received!" << endl;
           return true;
        })));
   }

public: // real public
   using Events = std::vector<Event*>;

   // pending events (external queue may affect this one)
   Events pendingEvents; // TODO: this should be concurrent safe (some std::concurrent_vector?)

   // override beforeUpdateState to include pendingEvents
   virtual bool beforeUpdateState(State<RPCMachineContext<dBFT2Context>>& current, RPCMachineContext<dBFT2Context>* p) override
   {
      if (this->watchdog && this->watchdog->expired()) {
         cout << "dBFT2 RPC StateMachine FAILED: MAXTIME = " << this->watchdog->getCountdown() << endl;
         return true;
      }

      if (pendingEvents.size() > 0) {
         cout << "Has some pending events to process! size = " << pendingEvents.size() << endl;
         // update states (TODO: update to do in concurrent) .. make some lock here?
         p->addEvents(pendingEvents);
         pendingEvents.clear();
      }

      // nothing to do?
      return false;
   }

private:
   std::thread rpcThread;

   static void startEventsServer(dBFT2RPCMachine* machine)
   {
      machine->eventsServer.RunForever();
   }

public:
   // TODO(@igormcoelho): 'runWithEventsServer' should become default 'run', as RPC is required here, not optional
   virtual void runWithEventsServer(State<RPCMachineContext<dBFT2Context>>* initial, RPCMachineContext<dBFT2Context>* ctx)
   {
      cout << "Starting thread to handle RPC messages:" << endl;
      runEventsServer(); // this will run on a dettached (background) thread
      // will wait few ms, just for RPC to start. TODO(@igormcoelho): should sync on some condition_variable from RPC (indicating 'started')
      std::this_thread::sleep_for(std::chrono::milliseconds(100)); // 100 ms
      // run dBFT on main thread (RPC is already running on background)
      this->run(initial, ctx);
      // stops events server and join its thread
      killEventsServer();
   }

private:
   virtual void runEventsServer()
   {
      // starts rpc thread in background
      rpcThread = std::thread(startEventsServer, this);
   }

   virtual void killEventsServer()
   {
      eventsServer.Stop();
      rpcThread.join();
   }

   // official method
   virtual void fillStatesForMachine()
   {
      // method for a simple dbft cycle (single height considered)
      fillSimpleCycle();
   }

public:
   /*

   virtual State<dBFT2Context>* initialize(State<dBFT2Context>* current, RPCMachineContext<dBFT2Context>* p) override
   {
      if (!current)
         current = new State<dBFT2Context>(machines.size(), nullptr);
      for (unsigned i = 0; i < machines.size(); i++)
         current->at(i) = machines[i]->getDefaultState(); // first state is initial (default)

      cout << endl;
      cout << "=================" << endl;
      cout << "begin run() dBFT2" << endl;
      cout << "=================" << endl;

      cout << "initializing multimachine" << endl;
      if (watchdog)
         watchdog->reset();
      else
         cout << "No watchdog configured!" << endl;
      for (unsigned i = 0; i < machines.size(); i++)
         machines[i]->initialize(current->at(i), p);

      // from ReplicatedSTSM (scheduled events for simulation)
      this->launchScheduledEvents(p);

      return current;
   }

   */

   void OnInitialize(RPCMachineContext<dBFT2Context>* p) override
   {
      //launchSchedEvents(p);
   }

   // no scheduled events on a real machine (try to avoid this, and only put on testing with mocking)
   /*
private:
   void launchSchedEvents(RPCMachineContext<dBFT2Context>* p)
   {
      cout << "launching scheduled events!" << endl;
      // launch all scheduled events
      for (unsigned i = 0; i < schedEvents.size(); i++) {
         p->launchTimedEvent(schedEvents[i]);
      }
      // clear all scheduled events
      schedEvents.clear();
   }
*/

public:
   string toString(string format = "") override
   {
      stringstream ss;

      if (format == "graphviz") {
         // will do this only for machine 0 (the others should be replicated)
         ss << "digraph " << this->name << " {" << endl;
         ss << "//graph [bgcolor=lightgoldenrodyellow]" << endl;
         ss << "//rankdir=LR;" << endl;
         ss << "size=\"11\"" << endl;
         // add states
         ss << "Empty [ label=\"\", width=0, height=0, style = invis ];" << endl;
         for (unsigned i = 0; i < this->states.size(); i++)
            ss << "node [shape = " << (this->states[i]->isFinal ? "doublecircle" : "circle") << "]; " << this->states[i]->name << ";" << endl;
         // default initial state transition
         State<RPCMachineContext<dBFT2Context>>* defState = this->getDefaultState();
         // will happen in an empty transition
         ss << "Empty -> " << defState->name << " [label = \"\"];" << endl;
         // begin regular transitions
         //Initial -> Primary [ label = "(H + v) mod R = i" ];
         //      getDefaultState
         for (unsigned i = 0; i < this->states.size(); i++) {
            State<RPCMachineContext<dBFT2Context>>* state = this->states[i];
            for (unsigned t = 0; t < state->transitions.size(); t++) {
               ss << state->name << " ";
               ss << state->transitions[t]->toString("graphviz") << endl;
            }
         }

         ss << "}" << endl;
      } else {
         // standard text
         ss << "Welcome to dBFT2RPCMachine : ";
         ss << "{ name = " << this->name << "}";
      }

      return ss.str();
   }
}; // class dBFT2RPCMachine

} // libbft

#endif // LIBBFT_SRC_CPP_DBFT2_DBFT2_RPC_MACHINE_HPP