
// system
#include <iostream>
#include <vector>

// lib

#include "replicated/Event.hpp"
#include "replicated/ReplicatedSTSM.hpp"
#include "single/SingleTimerStateMachine.hpp"
#include "single/State.hpp"

#include "dbft2/dBFT2Machine.hpp"

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

   // Timer never expires
   SingleTimerStateMachine<Data> machine(new Timer("C"));

   machine.registerState(initial);
   machine.registerState(final);

   cout << "Machine => " << machine.toString() << endl;

   // run for 5.0 seconds max (default)
   machine.run(); // initial state is given by default first
}

void
dbft_test_backup_multi()
{
   auto machine0 = create_dBFTMachine(0);

   // insert pre-initial state
   auto preinitial = new State<MultiContext<dBFT2Context>>(false, "PreInitial");

   machine0->states.insert(machine0->states.begin() + 0, preinitial);

   cout << "Machine => " << machine0->toString() << endl;

   // v = 0, H = 1501, T = 3 (secs), R = 1 (one node network)
   dBFT2Context data(0, 1501, 3, 2); // 1501 -> backup (R=2)

   MultiContext<dBFT2Context> ctx;
   ctx.vm.push_back(MachineContext<dBFT2Context>(&data, machine0));

   ReplicatedSTSM<dBFT2Context> multiMachine;
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
   multiMachine.run(&minitial, &ctx);
}

void
dbft_test_primary()
{
   auto machine0 = create_dBFTMachine(0);

   cout << "Machine => " << machine0->toString() << endl;

   // v = 0, H = 1500, T = 3 (secs), R = 1 (one node network)
   dBFT2Context data(0, 1500, 3, 1); // 1500 -> primary (R=1)

   MultiContext<dBFT2Context> ctx;
   ctx.vm.push_back(MachineContext<dBFT2Context>(&data, machine0));

   // run for 5.0 seconds max
   machine0->setWatchdog(5.0);
   machine0->run(machine0->states[0], &ctx); // explicitly passing first state as default
}

void
dbft_test_real_dbft2_primary()
{
   auto machine = new dBFT2Machine(0, 1); // f=0, N=1

   cout << "Machine => " << machine->toString() << endl;

   // v = 0, H = 1500, T = 3 (secs), R = 1 (one node network)
   dBFT2Context data(0, 1500, 3, 1); // 1500 -> primary (R=1)

   MultiContext<dBFT2Context> ctx;
   ctx.vm.push_back(MachineContext<dBFT2Context>(&data, machine->machines[0]));

   // run for 5.0 seconds max
   machine->setWatchdog(5.0);
   machine->run(nullptr, &ctx);
}

int
main()
{
   cout << "begin test state machines!" << endl;

   // simple example: wait one second and quit
   simpleExample();

   // Neo dbft modeling as example
   dbft_test_primary();

   // warm-up
   dbft_test_backup_multi();

   // real thing starting to happen here
   dbft_test_real_dbft2_primary();

   cout << "finished successfully!" << endl;

   return 0;
}