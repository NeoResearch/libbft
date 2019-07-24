
// system
#include <iostream>
#include <vector>

// lib

#include "SingleTimerStateMachine.hpp"
#include "State.hpp"

using namespace std;
using namespace libbft;

// main function for base testing purposes

int
main()
{
   cout << "begin test state machines!" << endl;

   SingleTimerStateMachine machine;

   State* initial = new State(false);
   State* final = new State(true);
   initial->transitions.push_back(new Transition(final, "always true"));

   cout << "initial state: " << initial->toString() << endl;
   cout << "final state: " << final->toString() << endl;

   machine.registerState(initial);
   machine.registerState(final);

   machine.run();

   cout << "finished successfully!" << endl;

   return 0;
}