#pragma once
#ifndef LIBBFT_SRC_CPP_DBFT2_DBFT2MACHINE_HPP
#define LIBBFT_SRC_CPP_DBFT2_DBFT2MACHINE_HPP

// system includes
#include <memory>
#include <sstream>
#include <vector>
// simulate non-deterministic nature
#include <algorithm>
#include <cassert>
#include <random>

// standard Transition
#include "replicated/ReplicatedSTSM.hpp"
#include "single/Transition.hpp"
#include "timing/Timer.hpp"

#include "dBFT2Context.hpp"

namespace libbft {

class dBFT2Machine : public ReplicatedSTSM<dBFT2Context>
{
public:
   using TSingleTimerStateMachine = std::shared_ptr<SingleTimerStateMachine<MultiContext<dBFT2Context>>>;

   /** max number of faulty nodes */
   int f;

   /** it is recommended to have N = 3f+1 (e.g., f=0 -> N=1; f=1 -> N=4; f=2 -> N=7; ...) */
   explicit dBFT2Machine(int _f = 0, int N = 1, TClock _clock = nullptr, MachineId _me = MachineId(),
                std::string _name = "replicated_dBFT")
     : ReplicatedSTSM<dBFT2Context>(std::move(_clock), std::move(_me), std::move(_name))
     , f(_f)
   {
      assert(f >= 0);
      assert(N >= 1);
      assert(f <= N);
      // create N machines
      this->machines = std::vector<TSingleTimerStateMachine>(N, nullptr);

      // initialize independent machines (each one with its Timer, sharing same global Clock)
      // should never share Timers here, otherwise strange things may happen (TODO: protect from this... unique_ptr?)
      for (int i = 0; i < N; i++) {
         this->machines[i] = std::shared_ptr<SingleTimerStateMachine<MultiContext<dBFT2Context>>>(
               new SingleTimerStateMachine<MultiContext<dBFT2Context>>(
               std::unique_ptr<Timer>(new Timer("C", std::unique_ptr<Clock>(new Clock(*this->clock)))), MachineId(i),
               std::unique_ptr<Clock>(new Clock(*this->clock)), "dBFT"));
      }

      // fill states and transitions on each machine
      for (int i = 0; i < N; i++) {
         fillStatesForMachine(i);
      }
   }

   /**
    * already pre-initialized machines (including states, I suppose...)
    * Each one has its clock and timer
    * @param _f
    * @param _machines
    * @param _clock
    * @param _me
    * @param _name
    */
   dBFT2Machine(int _f, std::vector<TSingleTimerStateMachine> _machines,
                TClock _clock = nullptr, int _me = 0, std::string _name = "replicated_dBFT")
     : ReplicatedSTSM<dBFT2Context>(std::move(_clock), MachineId(_me), _name)
     , f(_f)
   {
      this->machines = std::move(_machines);
      assert(f >= 0);
      assert(!this->machines.empty());
      assert(f <= static_cast<int>(this->machines.size()));
   }

   /**
    * TODO: delete lot's of stuff
    * unique_ptr the clock perhaps? shared_ptr?
    * very dangerous to delete like this... clock may be shared.
    */
   virtual ~dBFT2Machine() = default;

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
        (new Transition<MultiContext<dBFT2Context>>(started))->add(Condition<MultiContext<dBFT2Context>>("OnStart()",
        		[](const Timer& t, MultiContext<dBFT2Context>* d, MachineId _me) -> bool {
           std::cout << "Waiting for OnStart..." << std::endl;
           return d->hasEvent("OnStart", _me.id, std::vector<std::string>(0)); // no parameters on event
        })));

      // initial -> backup
      started->addTransition(
        (new Transition<MultiContext<dBFT2Context>>(backup))->add(Condition<MultiContext<dBFT2Context>>(
        		"not (H+v) mod R = i", [](const Timer& t, MultiContext<dBFT2Context>* d, MachineId _me) -> bool {
           std::cout << "lambda1" << std::endl;
           return (d->vm[_me.id].params->H + d->vm[_me.id].params->v) % d->vm[_me.id].params->R != _me.id;
        })));

      // initial -> primary
      started->addTransition(
        (new Transition<MultiContext<dBFT2Context>>(primary))->add(Condition<MultiContext<dBFT2Context>>(
        		"(H+v) mod R = i", [](const Timer& t, MultiContext<dBFT2Context>* d, MachineId _me) -> bool {
           std::cout << "lambda2 H=" << d->vm[_me.id].params->H << " v=" << d->vm[_me.id].params->v << " _me="
               << _me.id << std::endl;
           return (d->vm[_me.id].params->H + d->vm[_me.id].params->v) % d->vm[_me.id].params->R == _me.id;
        })));

      // backup -> reqSentOrRecv
      auto toReqSentOrRecv1 = new Transition<MultiContext<dBFT2Context>>(reqSentOrRecv);
      backup->addTransition(
        toReqSentOrRecv1->add(Condition<MultiContext<dBFT2Context>>("OnPrepareRequest(v)",
        		[](const Timer& t, MultiContext<dBFT2Context>* d, MachineId _me) -> bool {
           std::cout << "waiting for event OnPrepareRequest at " << _me.id << " for view " << d->vm[_me.id].params->v
               << std::endl;
               std::stringstream ss;
           ss << d->vm[_me.id].params->v;
               std::vector<std::string> params(1, ss.str());
           //cout << "for " << ss.str() << endl;
           return d->hasEvent("OnPrepareRequest", _me.id, params);
        })));

      // reqSentOrRecv -> commitSent
      reqSentOrRecv->addTransition(
        (new Transition<MultiContext<dBFT2Context>>(commitSent))->add(Condition<MultiContext<dBFT2Context>>(
        		"(H+v) mod R = i", [](const Timer& t, MultiContext<dBFT2Context>* d, MachineId _me) -> bool {
           std::cout << "nothing to do... assuming all preparations were received!" << std::endl;
           return true;
        })));

      // commitSent -> blockSent
      commitSent->addTransition(
        (new Transition<MultiContext<dBFT2Context>>(blockSent))->add(Condition<MultiContext<dBFT2Context>>(
        		"(H+v) mod R = i", [](const Timer& t, MultiContext<dBFT2Context>* d, MachineId _me) -> bool {
           std::cout << "nothing to do... assuming all commits were received!" << std::endl;
           return true;
        })));
   }

   /** official method */
   virtual void fillStatesForMachine(int m)
   {
      // method for a simple dbft cycle (single height considered)
      fillSimpleCycle(m);
   }

   MultiState<dBFT2Context>* initialize(MultiState<dBFT2Context>* current, MultiContext<dBFT2Context>* p) override
   {
      if (!current)
         current = new MultiState<dBFT2Context>(machines.size(), nullptr);
      for (unsigned i = 0; i < machines.size(); i++)
         current->at(i) = machines[i]->getDefaultState(); // first state is initial (default)

      std::cout << std::endl;
      std::cout << "=================" << std::endl;
      std::cout << "begin run() dBFT2" << std::endl;
      std::cout << "=================" << std::endl;

      std::cout << "initializing multimachine" << std::endl;
      if (watchdog)
         watchdog->reset();
      else
         std::cout << "No watchdog configured!" << std::endl;
      for (unsigned i = 0; i < machines.size(); i++)
         machines[i]->initialize(current->at(i), p);

      // from ReplicatedSTSM (scheduled events for simulation)
      this->launchScheduledEvents(p);

      return current;
   }

   std::string toString(std::string format = "") override
   {
      std::stringstream ss;

      if (format == "graphviz") {
         // will do this only for machine 0 (the others should be replicated)
         if (this->machines.empty())
            return "";
         ss << "digraph " << this->machines[0]->name << " {" << std::endl;
         ss << "//graph [bgcolor=lightgoldenrodyellow]" << std::endl;
         ss << "//rankdir=LR;" << std::endl;
         ss << "size=\"11\"" << std::endl;
         // add states
         ss << "Empty [ label=\"\", width=0, height=0, style = invis ];" << std::endl;
         for (auto & state : this->machines[0]->states) {
            ss << "node [shape = " << (state->isFinal ? "doublecircle" : "circle") << "]; "
               << state->name << ";" << std::endl;
         }
         // default initial state transition
         State<MultiContext<dBFT2Context>>* defState = this->machines[0]->getDefaultState();
         // will happen in an empty transition
         ss << "Empty -> " << defState->name << " [label = \"\"];" << std::endl;
         // begin regular transitions
         //Initial -> Primary [ label = "(H + v) mod R = i" ];
         //      getDefaultState
         for (auto & state : this->machines[0]->states) {
            for (auto & transition : state->transitions) {
               ss << state->name << " ";
               ss << transition->toString("graphviz") << std::endl;
            }
         }

         ss << "}" << std::endl;
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
