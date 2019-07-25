
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

   Transition<Data>* alwaysTrue = new Transition<Data>(final, "always true");
   alwaysTrue->add(Condition<Data>("true", [](const Timer& t, Data*) -> bool { return true; }));

   Transition<Data>* after1sec = new Transition<Data>(final, "after1sec");
   after1sec->add(Condition<Data>("C >= 1", [](const Timer& t, Data*) -> bool { return t.elapsedTime() >= 1.0; }));
   //after1sec->timedFunction = [](Timer& t) -> bool { return t.elapsedTime() >= 1.0; };

   //initial->transitions.push_back(alwaysTrue);
   initial->addTransition(after1sec);

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

   dBFTData(int _v, int _H, int _T)
     : v(_v)
     , H(_H)
     , T(_T)
   {
   }
};

void
dbft()
{
   // declaring dBFT states

   State<dBFTData> initial(false, "Initial");
   State<dBFTData> backup(false, "Backup");
   State<dBFTData> primary(false, "Primary");
   State<dBFTData> reqSentOrRecv(false, "RequestSentOrReceived");
   State<dBFTData> commitSent(false, "CommitSent");
   State<dBFTData> viewChanging(false, "ViewChanging");
   State<dBFTData> blockSent(true, "BlockSent");

   // creating dBFT transitions

   initial.addTransition((new Transition<dBFTData>(&backup))->add(
     Condition<dBFTData>("not (H+v) mod R = i", [](const Timer& t, dBFTData*) -> bool {
        return true;
     })));

   Transition<dBFTData> alwaysTrue(&blockSent, "always true");
   alwaysTrue.add(Condition<dBFTData>("true", [](const Timer& t, dBFTData*) -> bool {
      return true;
   }));

   Transition<dBFTData> after1sec(&blockSent, "after1sec");
   after1sec.add(Condition<dBFTData>("C >= 1", [](const Timer& t, dBFTData*) -> bool {
      return t.elapsedTime() >= 1.0;
   }));

   //initial->transitions.push_back(alwaysTrue);
   initial.addTransition(&after1sec);

   //cout << "initial state: " << initial->toString() << endl;
   //cout << "final state: " << final->toString() << endl;

   SingleTimerStateMachine<dBFTData> machine(new Timer("C"));

   machine.registerState(&initial);
   machine.registerState(&blockSent);

   cout << "Machine => " << machine.toString() << endl;

   // v = 0, H = 1000, T = 3 (secs)
   dBFTData data(0, 1000, 15);

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