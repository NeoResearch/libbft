#pragma once
#ifndef LIBBFT_SRC_CPP_DBFT2_DBFT2MACHINE_HPP
#define LIBBFT_SRC_CPP_DBFT2_DBFT2MACHINE_HPP

// system includes
#include <iostream> // TODO: remove
#include <sstream>
#include <vector>
// simulate non-deterministic nature
#include <algorithm>
#include <assert.h>
#include <random>

// standard Transition
#include "../replicated/ReplicatedSTSM.hpp"
#include "../single/Transition.hpp"
#include "../timing/Timer.hpp"

#include "dBFT2Context.hpp"

using namespace std; // TODO: remove

namespace libbft {

class dBFT2Machine : public ReplicatedSTSM<dBFT2Context>
{
public:
   int f; // max number of faulty nodes

   // it is recommended to have N = 3f+1 (e.g., f=0 -> N=1; f=1 -> N=4; f=2 -> N=7; ...)
   dBFT2Machine(int _f = 0, int N = 1, Clock* _clock = nullptr, MachineId _me = MachineId(), string _name = "replicated_dBFT")
     : ReplicatedSTSM<dBFT2Context>(_clock, _me, _name)
     , f(_f)
   {
      assert(f >= 0);
      assert(N >= 1);
      assert(f <= N);
      // create N machines
      this->machines = vector<SingleTimerStateMachine<MultiContext<dBFT2Context>>*>(N, nullptr);

      // initialize independent machines (each one with its Timer, sharing same global Clock)
      // should never share Timers here, otherwise strange things may happen (TODO: protect from this... unique_ptr?)
      for (int i = 0; i < N; i++) {
         this->machines[i] = new SingleTimerStateMachine<MultiContext<dBFT2Context>>(new Timer("C", this->clock), i, this->clock, "dBFT");
      }

      // fill states and transitions on each machine
      for (int i = 0; i < N; i++)
         fillStatesForMachine(i);
   }

   // already pre-initialized machines (including states, I suppose...)
   // Each one has its clock and timer
   dBFT2Machine(int _f, vector<SingleTimerStateMachine<MultiContext<dBFT2Context>>*> _machines, Clock* _clock = nullptr, int _me = 0, string _name = "replicated_dBFT")
     : ReplicatedSTSM<dBFT2Context>(_clock, _me, _name)
     , f(_f)
   {
      this->machines = _machines;
      assert(f >= 0);
      assert(machines.size() >= 1);
      assert(f <= (int)machines.size());
   }

   virtual ~dBFT2Machine()
   {
      // TODO: delete lot's of stuff
      // unique_ptr the clock perhaps? shared_ptr?
      // very dangerous to delete like this... clock may be shared.
   }

   void fillSimpleCycle(int m)
   {
      // ---------------------
      // declaring dBFT states
      // ---------------------

      auto preinitial = new State<MultiContext<dBFT2Context>>(false, "PreInitial");
      this->machines[m]->registerState(preinitial);
      auto started = new State<MultiContext<dBFT2Context>>(false, "Started");
      this->machines[m]->registerState(started);
      auto backup = new State<MultiContext<dBFT2Context>>(false, "Backup");
      this->machines[m]->registerState(backup);
      auto primary = new State<MultiContext<dBFT2Context>>(false, "Primary");
      this->machines[m]->registerState(primary);
      auto reqSentOrRecv = new State<MultiContext<dBFT2Context>>(false, "RequestSentOrReceived");
      this->machines[m]->registerState(reqSentOrRecv);
      auto commitSent = new State<MultiContext<dBFT2Context>>(false, "CommitSent");
      this->machines[m]->registerState(commitSent);
      auto viewChanging = new State<MultiContext<dBFT2Context>>(false, "ViewChanging");
      this->machines[m]->registerState(viewChanging);
      auto blockSent = new State<MultiContext<dBFT2Context>>(true, "BlockSent");
      this->machines[m]->registerState(blockSent);

      // -------------------------
      // creating dBFT transitions
      // -------------------------

      //auto machine = this->machines[m];
      //int id = this->machines[m]->me.id;

      // preinitial -> started
      preinitial->addTransition(
        (new Transition<MultiContext<dBFT2Context>>(started))->add(Condition<MultiContext<dBFT2Context>>("OnStart()", [](const Timer& t, MultiContext<dBFT2Context>* d, MachineId me) -> bool {
           cout << "Waiting for OnStart..." << endl;
           return d->hasEvent("OnStart", me.id, vector<string>(0)); // no parameters on event
        })));

      // initial -> backup
      started->addTransition(
        (new Transition<MultiContext<dBFT2Context>>(backup))->add(Condition<MultiContext<dBFT2Context>>("not (H+v) mod R = i", [](const Timer& t, MultiContext<dBFT2Context>* d, MachineId me) -> bool {
           cout << "lambda1" << endl;
           return !((d->vm[me.id].params->H + d->vm[me.id].params->v) % d->vm[me.id].params->R == me.id);
        })));

      // initial -> primary
      started->addTransition(
        (new Transition<MultiContext<dBFT2Context>>(primary))->add(Condition<MultiContext<dBFT2Context>>("(H+v) mod R = i", [](const Timer& t, MultiContext<dBFT2Context>* d, MachineId me) -> bool {
           cout << "lambda2 H=" << d->vm[me.id].params->H << " v=" << d->vm[me.id].params->v << " me=" << me.id << endl;
           return (d->vm[me.id].params->H + d->vm[me.id].params->v) % d->vm[me.id].params->R == me.id;
        })));

      // backup -> reqSentOrRecv
      auto toReqSentOrRecv1 = new Transition<MultiContext<dBFT2Context>>(reqSentOrRecv);
      backup->addTransition(
        toReqSentOrRecv1->add(Condition<MultiContext<dBFT2Context>>("OnPrepareRequest(v)", [](const Timer& t, MultiContext<dBFT2Context>* d, MachineId me) -> bool {
           cout << "waiting for event OnPrepareRequest at " << me.id << " for view " << d->vm[me.id].params->v << endl;
           stringstream ss;
           ss << d->vm[me.id].params->v;
           vector<string> params(1, ss.str());
           //cout << "for " << ss.str() << endl;
           return d->hasEvent("OnPrepareRequest", me.id, params);
        })));

      // reqSentOrRecv -> commitSent
      reqSentOrRecv->addTransition(
        (new Transition<MultiContext<dBFT2Context>>(commitSent))->add(Condition<MultiContext<dBFT2Context>>("(H+v) mod R = i", [](const Timer& t, MultiContext<dBFT2Context>* d, MachineId me) -> bool {
           cout << "nothing to do... assuming all preparations were received!" << endl;
           return true;
        })));

      // commitSent -> blockSent
      commitSent->addTransition(
        (new Transition<MultiContext<dBFT2Context>>(blockSent))->add(Condition<MultiContext<dBFT2Context>>("(H+v) mod R = i", [](const Timer& t, MultiContext<dBFT2Context>* d, MachineId me) -> bool {
           cout << "nothing to do... assuming all commits were received!" << endl;
           return true;
        })));
   }

   // official method
   virtual void fillStatesForMachine(int m)
   {
      // method for a simple dbft cycle (single height considered)
      fillSimpleCycle(m);
   }

   virtual MultiState<dBFT2Context>* initialize(MultiState<dBFT2Context>* current, MultiContext<dBFT2Context>* p) override
   {
      if (!current)
         current = new MultiState<dBFT2Context>(machines.size(), nullptr);
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

   string toString(string format = "") override
   {
      stringstream ss;

      if (format == "graphviz") {
         // will do this only for machine 0 (the others should be replicated)
         if (this->machines.size() == 0)
            return "";
         ss << "digraph " << this->machines[0]->name << " {" << endl;
         ss << "//graph [bgcolor=lightgoldenrodyellow]" << endl;
         ss << "//rankdir=LR;" << endl;
         ss << "size=\"11\"" << endl;
         // add states
         ss << "Empty [ label=\"\", width=0, height=0, style = invis ];" << endl;
         for (unsigned i = 0; i < this->machines[0]->states.size(); i++)
            ss << "node [shape = " << (this->machines[0]->states[i]->isFinal ? "doublecircle" : "circle") << "]; " << this->machines[0]->states[i]->name << ";" << endl;
         // default initial state transition
         State<MultiContext<dBFT2Context>>* defState = this->machines[0]->getDefaultState();
         // will happen in an empty transition
         ss << "Empty -> " << defState->name << " [label = \"\"];" << endl;
         // begin regular transitions
         //Initial -> Primary [ label = "(H + v) mod R = i" ];
         //      getDefaultState
         for (unsigned i = 0; i < this->machines[0]->states.size(); i++) {
            State<MultiContext<dBFT2Context>>* state = this->machines[0]->states[i];
            for (unsigned t = 0; t < state->transitions.size(); t++) {
               ss << state->name << " ";
               ss << state->transitions[t]->toString("graphviz") << endl;
            }
         }

         ss << "}" << endl;
      } else {
         // standard text
         ss << "Welcome to dBFT2Machine : ";
         ss << "{ name = " << this->name << "}";
      }

      return ss.str();
   }
}; // class dBFT2Machine

} // libbft

#endif // LIBBFT_SRC_CPP_DBFT2_DBFT2MACHINE_HPP