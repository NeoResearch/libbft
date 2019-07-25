
// system
#include <iostream>
#include <vector>

// lib

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
   alwaysTrue->add(Condition<Data>("true", [](const Timer& t, Data*) -> bool { return true; }));
   //initial->addTransition(alwaysTrue); // (unused)

   initial->addTransition(
     (new Transition<Data>(final, "after1sec"))->add(Condition<Data>("C >= 1", [](const Timer& t, Data* d) -> bool {
        return t.elapsedTime() >= 1.0;
     })));

   //cout << "initial state: " << initial->toString() << endl;
   //cout << "final state: " << final->toString() << endl;

   SingleTimerStateMachine<Data> machine(new Timer("C"));

   machine.registerState(initial);
   machine.registerState(final);

   cout << "Machine => " << machine.toString() << endl;

   machine.run(*initial);
}

struct dBFTData
{
   // view number
   int v;
   // blockchain height
   int H;
   // block time (in seconds)
   int T;
   // number of nodes (TODO: better solution for this?)
   int R;
   // current node (TODO: better solution for this?)
   int i;

   dBFTData(int _v, int _H, int _T, int _R, int _i)
     : v(_v)
     , H(_H)
     , T(_T)
     , R(_R)
     , i(_i)
   {
   }
};

void
dbft()
{
   // ---------------------
   // declaring dBFT states
   // ---------------------

   State<dBFTData> initial(false, "Initial");
   State<dBFTData> backup(false, "Backup");
   State<dBFTData> primary(false, "Primary");
   State<dBFTData> reqSentOrRecv(false, "RequestSentOrReceived");
   State<dBFTData> commitSent(false, "CommitSent");
   State<dBFTData> viewChanging(false, "ViewChanging");
   State<dBFTData> blockSent(true, "BlockSent");

   // -------------------------
   // creating dBFT transitions
   // -------------------------

   // initial -> backup
   initial.addTransition(
     (new Transition<dBFTData>(&backup))->add(Condition<dBFTData>("not (H+v) mod R = i", [](const Timer& t, dBFTData* d) -> bool {
        cout << "lambda1" << endl;
        return !((d->H + d->v) % d->R == d->i);
     })));

   // initial -> primary
   initial.addTransition(
     (new Transition<dBFTData>(&primary))->add(Condition<dBFTData>("(H+v) mod R = i", [](const Timer& t, dBFTData* d) -> bool {
        cout << "lambda2" << endl;
        return (d->H + d->v) % d->R == d->i;
     })));

   // backup ->reqSentOrRecv
   auto toReqSentOrRecv1 = 
   initial.addTransition(
     (new Transition<dBFTData>(&primary))->add(Condition<dBFTData>("(H+v) mod R = i", [](const Timer& t, dBFTData* d) -> bool {
        return (d->H + d->v) % d->R == d->i;
     })));

   SingleTimerStateMachine<dBFTData> machine(new Timer("C"));

   machine.registerState(&initial);
   machine.registerState(&blockSent);

   cout << "Machine => " << machine.toString() << endl;

   // v = 0, H = 1000, T = 3 (secs), R = 1 (one node network), i = 0 (first id.. should move to lambdas?)
   dBFTData data(0, 1000, 3, 1, 0);

   machine.run(initial, &data);
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