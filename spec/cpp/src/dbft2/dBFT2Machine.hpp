#pragma once
#ifndef LIBBFT_SRC_CPP_DBFT2_DBFT2MACHINE_HPP
#define LIBBFT_SRC_CPP_DBFT2_DBFT2MACHINE_HPP

// system includes
#include <iostream> // TODO: remove
#include <sstream>
#include <vector>
// simulate non-deterministic nature
#include <algorithm>
#include <random>

// standard Transition
#include "../replicated/ReplicatedSTSM.hpp"
#include "../single/Transition.hpp"
#include "../timing/Timer.hpp"

#include "dBFT2Context.hpp"

using namespace std; // TODO: remove

namespace libbft {

/*
      MultiContext<dBFT2Context> ctx;
   ctx.vm.push_back(MachineContext<dBFT2Context>(&data, machine0));

   MultiSTSM<dBFT2Context> multiMachine;
   multiMachine.registerMachine(machine0);

   // global transition scheduled to start machine 0 ("OnStart") after 1 second
   multiMachine.scheduleGlobalTransition(
     (new Timer())->init(1.0), // 1 second to expire
     0,                        // machine 0
     // no other conditions, always 'true'
     (new Transition<MultiContext<dBFT2Context>>(machine0->getStateByName("Initial")))
       ->add(Action<MultiContext<dBFT2Context>>(
         "C := 0 | v := 0",
         [](Timer& C, MultiContext<dBFT2Context>* d, int me) -> void {
            cout << " => action: C := 0" << endl;
            C.reset();
            cout << " => action: v := 0" << endl;
            d->vm[me].params->v = 0;
         })));

   // event scheduled to raise "OnPrepareRequest" machine 0, after 3 seconds
   multiMachine.scheduleEvent(
     (new Timer())->init(3.0), // 3 second to expire
     0,                        // machine 0
     new Event<MultiContext<dBFT2Context>>("OnPrepareRequest", "OnPrepareRequest"));

   MultiState<dBFT2Context> minitial(1, nullptr);
   minitial[0] = machine0->getStateByName("PreInitial");

   // run for 5.0 seconds max (watchdog limit)
   multiMachine.setWatchdog(5.0);
   multiMachine.run(minitial, &ctx);

*/

class dBFT2Machine : public ReplicatedSTSM<dBFT2Context>
{
public:
   virtual MultiState<dBFT2Context>* initialize(MultiState<dBFT2Context>* current, MultiContext<dBFT2Context>* p) override
   {
      if (!current)
         current = new MultiState<dBFT2Context>(machines.size(), nullptr);

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

      return current;
   }
};

SingleTimerStateMachine<MultiContext<dBFT2Context>>*
create_dBFTMachine(int id)
{
   // ---------------------
   // declaring dBFT states
   // ---------------------

   auto initial = new State<MultiContext<dBFT2Context>>(false, "Initial");
   auto backup = new State<MultiContext<dBFT2Context>>(false, "Backup");
   auto primary = new State<MultiContext<dBFT2Context>>(false, "Primary");
   auto reqSentOrRecv = new State<MultiContext<dBFT2Context>>(false, "RequestSentOrReceived");
   auto commitSent = new State<MultiContext<dBFT2Context>>(false, "CommitSent");
   auto viewChanging = new State<MultiContext<dBFT2Context>>(false, "ViewChanging");
   auto blockSent = new State<MultiContext<dBFT2Context>>(true, "BlockSent");

   // -------------------------
   // creating dBFT transitions
   // -------------------------

   auto machine = new SingleTimerStateMachine<MultiContext<dBFT2Context>>(new Timer("C"));
   machine->me = id;

   // initial -> backup
   initial->addTransition(
     (new Transition<MultiContext<dBFT2Context>>(backup))->add(Condition<MultiContext<dBFT2Context>>("not (H+v) mod R = i", [](const Timer& t, MultiContext<dBFT2Context>* d, int me) -> bool {
        cout << "lambda1" << endl;
        return !((d->vm[me].params->H + d->vm[me].params->v) % d->vm[me].params->R == me);
     })));

   // initial -> primary
   initial->addTransition(
     (new Transition<MultiContext<dBFT2Context>>(primary))->add(Condition<MultiContext<dBFT2Context>>("(H+v) mod R = i", [](const Timer& t, MultiContext<dBFT2Context>* d, int me) -> bool {
        cout << "lambda2 H=" << d->vm[me].params->H << " v=" << d->vm[me].params->v << " me=" << me << endl;
        return (d->vm[me].params->H + d->vm[me].params->v) % d->vm[me].params->R == me;
     })));

   // backup -> reqSentOrRecv
   auto toReqSentOrRecv1 = new Transition<MultiContext<dBFT2Context>>(reqSentOrRecv);
   backup->addTransition(
     toReqSentOrRecv1->add(Condition<MultiContext<dBFT2Context>>("OnPrepareRequest", [](const Timer& t, MultiContext<dBFT2Context>* d, int me) -> bool {
        cout << "waiting for event OnPrepareRequest at " << me << endl;
        return d->hasEvent("OnPrepareRequest", me);
     })));

   // reqSentOrRecv -> commitSent
   reqSentOrRecv->addTransition(
     (new Transition<MultiContext<dBFT2Context>>(commitSent))->add(Condition<MultiContext<dBFT2Context>>("(H+v) mod R = i", [](const Timer& t, MultiContext<dBFT2Context>* d, int me) -> bool {
        cout << "nothing to do... assuming all preparations were received!" << endl;
        return true;
     })));

   // commitSent -> blockSent
   commitSent->addTransition(
     (new Transition<MultiContext<dBFT2Context>>(blockSent))->add(Condition<MultiContext<dBFT2Context>>("(H+v) mod R = i", [](const Timer& t, MultiContext<dBFT2Context>* d, int me) -> bool {
        cout << "nothing to do... assuming all commits were received!" << endl;
        return true;
     })));

   machine->registerState(initial);
   machine->registerState(blockSent);

   return machine;
}

} // libbft

#endif // LIBBFT_SRC_CPP_DBFT2_DBFT2MACHINE_HPP