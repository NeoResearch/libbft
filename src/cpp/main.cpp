
// system
#include <iostream>
#include <vector>

// lib

#include "SingleTimerStateMachine.hpp"
#include "State.h"

using namespace std;
using namespace libbft;

// main function for base testing purposes

struct Data
{
};

int
main()
{
   cout << "begin test state machines!" << endl;

   State<Data>* initial = new State<Data>(false, "Initial");
   State<Data>* final = new State<Data>(true, "Final");

   Transition<Data>* alwaysTrue = new Transition<Data>(final, "always true");
   alwaysTrue->timedFunction = [](Timer& t) -> bool { return true; };

   Transition<Data>* after1sec = new Transition<Data>(final, "after1sec");
   after1sec->timedFunction = [](Timer& t) -> bool { return t.elapsedTime() >= 1.0; };

   //initial->transitions.push_back(alwaysTrue);
   initial->transitions.push_back(after1sec);

   //cout << "initial state: " << initial->toString() << endl;
   //cout << "final state: " << final->toString() << endl;

   SingleTimerStateMachine<Data> machine(new Timer("C"));

   machine.registerState(initial);
   machine.registerState(final);

   cout << "Machine => " << machine.toString() << endl;

   machine.run(initial);

   cout << "finished successfully!" << endl;

   return 0;
}