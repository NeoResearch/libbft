
// system
#include <iostream>
#include <vector>

// lib

#include "SingleTimerStateMachine.hpp"
#include "State.h"

using namespace std;
using namespace libbft;

// main function for base testing purposes

int
main()
{
   cout << "begin test state machines!" << endl;

   State* initial = new State(false, "Initial");
   State* final = new State(true, "Final");

   Transition* alwaysTrue = new Transition(final, "always true");
   alwaysTrue->timedFunction = [](Timer& t) -> bool { return true; };

   Transition* after1sec = new Transition(final, "after1sec");
   after1sec->timedFunction = [](Timer& t) -> bool { return t.elapsedTime() >= 1.0; };

   //initial->transitions.push_back(alwaysTrue);
   initial->transitions.push_back(after1sec);

   //cout << "initial state: " << initial->toString() << endl;
   //cout << "final state: " << final->toString() << endl;

   SingleTimerStateMachine machine(new Timer("C"));

   machine.registerState(initial);
   machine.registerState(final);

   cout << "Machine => " << machine.toString() << endl;

   machine.run(initial);

   cout << "finished successfully!" << endl;

   return 0;
}