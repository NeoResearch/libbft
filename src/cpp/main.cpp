
// system
#include <iostream>
#include <vector>

// lib

#include "SingleTimerStateMachine.hpp"
#include "State.h"
#include "MultiSTSM.hpp"

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

   SingleTimerStateMachine<Data> machine(new Timer("C"));

   machine.registerState(initial);
   machine.registerState(final);

   cout << "Machine => " << machine.toString() << endl;

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

   // add ConsensusContext information here

   dBFTContext(int _v, int _H, int _T, int _R)
     : v(_v)
     , H(_H)
     , T(_T)
     , R(_R)
   {
   }
};

SingleTimerStateMachine<MultiContext<dBFTContext>>* getdBFTMachine(int id)
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

   auto machine = new SingleTimerStateMachine<MultiContext<dBFTContext>>(new Timer("C"));
   machine->me = id;

   // initial -> backup
   initial->addTransition(
     (new Transition<MultiContext<dBFTContext>>(backup))->add(Condition<MultiContext<dBFTContext>>("not (H+v) mod R = i", [](const Timer& t, MultiContext<dBFTContext>* d, int me) -> bool {
        cout << "lambda1" << endl;
        return !((d->params[me]->H + d->params[me]->v) % d->params[me]->R == me);
     })));

   // initial -> primary
   initial->addTransition(
     (new Transition<MultiContext<dBFTContext>>(primary))->add(Condition<MultiContext<dBFTContext>>("(H+v) mod R = i", [](const Timer& t, MultiContext<dBFTContext>* d, int me) -> bool {
        cout << "lambda2" << endl;
        return (d->params[me]->H + d->params[me]->v) % d->params[me]->R == me;
     })));

   // backup -> reqSentOrRecv
   /*
   auto toReqSentOrRecv1 = new Transition<dBFTContext>(&primary);
   initial.addTransition(
     toReqSentOrRecv1->add(Event<dBFTContext>("OnPrepareRequest", "OnPrepareRequest", [](const Timer& t, dBFTContext* d) -> bool {
        return (d->H + d->v) % d->R == d->i;
     })));
   */

   

   machine->registerState(initial);
   machine->registerState(blockSent);

   return machine;
}


void
dbft()
{
   auto machine = getdBFTMachine(0);

   cout << "Machine => " << machine->toString() << endl;

   // v = 0, H = 1000, T = 3 (secs), R = 1 (one node network)
   dBFTContext data(0, 1000, 3, 1);

   MultiContext<dBFTContext> ctx;
   ctx.params.push_back(&data);
   ctx.machines.push_back(machine);

   auto initial = machine->states[0];

   machine->run(initial, &ctx);
}

int
main()
{
   cout << "begin test state machines!" << endl;

   // simple example: wait one second and quit
   simpleExample();

   // Neo dbft modeling as example
   dbft();

   cout << "finished successfully!" << endl;

   return 0;
}