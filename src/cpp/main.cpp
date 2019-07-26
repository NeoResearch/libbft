
// system
#include <iostream>
#include <vector>

// lib

#include "Event.hpp"
#include "MultiSTSM.hpp"
#include "SingleTimerStateMachine.hpp"
#include "State.h"

using namespace std;
using namespace libbft;

// main function for base testing purposes

// empty data
struct Data
{
};

void
simpleExample()
{
   State<Data>* initial = new State<Data>(false, "Initial");
   State<Data>* final = new State<Data>(true, "Final");

   // unused (demonstration)
   Transition<Data>* alwaysTrue = new Transition<Data>(final, "always true");
   alwaysTrue->add(Condition<Data>("true", [](const Timer& t, Data*, int) -> bool { return true; }));
   //initial->addTransition(alwaysTrue); // (unused)

   initial->addTransition(
     (new Transition<Data>(final, "after1sec"))->add(Condition<Data>("C >= 1", [](const Timer& t, Data* d, int) -> bool {
        return t.elapsedTime() >= 1.0;
     })));

   //cout << "initial state: " << initial->toString() << endl;
   //cout << "final state: " << final->toString() << endl;

   SingleTimerStateMachine<Data> machine(new Timer(-1.0, "C"));

   machine.registerState(initial);
   machine.registerState(final);

   cout << "Machine => " << machine.toString() << endl;

   // run for 5.0 seconds max (default)
   machine.run(initial);
}

struct dBFTContext
{
   // view number
   int v;
   // blockchain height
   int H;
   // block time (in seconds)
   int T;
   // number of nodes (TODO: better solution for this?)
   int R;

   // cached events

   // add ConsensusContext information here

   dBFTContext(int _v, int _H, int _T, int _R)
     : v(_v)
     , H(_H)
     , T(_T)
     , R(_R)
   {
   }
};

SingleTimerStateMachine<MultiContext<dBFTContext>>*
create_dBFTMachine(int id)
{
   // ---------------------
   // declaring dBFT states
   // ---------------------

   auto initial = new State<MultiContext<dBFTContext>>(false, "Initial");
   auto backup = new State<MultiContext<dBFTContext>>(false, "Backup");
   auto primary = new State<MultiContext<dBFTContext>>(false, "Primary");
   auto reqSentOrRecv = new State<MultiContext<dBFTContext>>(false, "RequestSentOrReceived");
   auto commitSent = new State<MultiContext<dBFTContext>>(false, "CommitSent");
   auto viewChanging = new State<MultiContext<dBFTContext>>(false, "ViewChanging");
   auto blockSent = new State<MultiContext<dBFTContext>>(true, "BlockSent");

   // -------------------------
   // creating dBFT transitions
   // -------------------------

   auto machine = new SingleTimerStateMachine<MultiContext<dBFTContext>>(new Timer(-1.0, "C"));
   machine->me = id;

   // initial -> backup
   initial->addTransition(
     (new Transition<MultiContext<dBFTContext>>(backup))->add(Condition<MultiContext<dBFTContext>>("not (H+v) mod R = i", [](const Timer& t, MultiContext<dBFTContext>* d, int me) -> bool {
        cout << "lambda1" << endl;
        return !((d->vm[me].params->H + d->vm[me].params->v) % d->vm[me].params->R == me);
     })));

   // initial -> primary
   initial->addTransition(
     (new Transition<MultiContext<dBFTContext>>(primary))->add(Condition<MultiContext<dBFTContext>>("(H+v) mod R = i", [](const Timer& t, MultiContext<dBFTContext>* d, int me) -> bool {
        cout << "lambda2 H=" << d->vm[me].params->H << " v=" << d->vm[me].params->v << " me=" << me << endl;
        return (d->vm[me].params->H + d->vm[me].params->v) % d->vm[me].params->R == me;
     })));

   // backup -> reqSentOrRecv
   auto toReqSentOrRecv1 = new Transition<MultiContext<dBFTContext>>(reqSentOrRecv);
   backup->addTransition(
     toReqSentOrRecv1->add(Condition<MultiContext<dBFTContext>>("OnPrepareRequest", [](const Timer& t, MultiContext<dBFTContext>* d, int me) -> bool {
        cout << "waiting for event OnPrepareRequest at " << me << endl;
        bool e = d->hasEvent("OnPrepareRequest", me);
        cout << "e=" << e << endl;
        return e;
     })));

   machine->registerState(initial);
   machine->registerState(blockSent);

   return machine;
}

void
dbft_backup()
{
   auto machine0 = create_dBFTMachine(0);

   cout << "Machine => " << machine0->toString() << endl;

   // v = 0, H = 1501, T = 3 (secs), R = 1 (one node network)
   dBFTContext data(0, 1501, 3, 2); // 1501 -> backup (R=2)

   MultiContext<dBFTContext> ctx;
   ctx.vm.push_back(MachineContext<dBFTContext>(&data, machine0));

   // run for 5.0 seconds max
   machine0->run(machine0->states[0], 5.0, &ctx); 
}

void
dbft_primary()
{
   auto machine0 = create_dBFTMachine(0);

   cout << "Machine => " << machine0->toString() << endl;

   // v = 0, H = 1500, T = 3 (secs), R = 1 (one node network)
   dBFTContext data(0, 1500, 3, 1); // 1500 -> primary (R=1)

   MultiContext<dBFTContext> ctx;
   ctx.vm.push_back(MachineContext<dBFTContext>(&data, machine0));

   // run for 5.0 seconds max
   machine0->run(machine0->states[0], 5.0, &ctx);
}

int
main()
{
   cout << "begin test state machines!" << endl;

   // simple example: wait one second and quit
   simpleExample();

   // Neo dbft modeling as example
   dbft_primary();

   dbft_backup();

   cout << "finished successfully!" << endl;

   return 0;
}