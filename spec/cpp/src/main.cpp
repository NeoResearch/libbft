
// system
#include <iostream>
#include <vector>

// lib

#include "events/Event.hpp"
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
   //alwaysTrue->add(Condition<Data>("true", [](const Timer& t, Data*, int) -> bool { return true; }));
   //initial->addTransition(alwaysTrue); // (unused)

   initial->addTransition(
     (new Transition<Data>(final, "after1sec"))->add(Condition<Data>("C >= 1", [](const Timer& c, Data* d, int) -> bool {
        cout << "WAITING TRANSITION TO HAPPEN" << c.elapsedTime() << "s" << endl;
        return c.elapsedTime() >= 3.0;
     })));

   //cout << "initial state: " << initial->toString() << endl;
   //cout << "final state: " << final->toString() << endl;

   // Timer never expires
   SingleTimerStateMachine<Data> machine(new Timer("C"));

   machine.registerState(initial);
   machine.registerState(final);

   cout << "Machine => " << machine.toString() << endl;

   cout << "BEFORE RUN, WILL PRINT AS GRAPHVIZ!" << endl;
   string graphviz = machine.toString("graphviz");
   cout << graphviz << endl;

   // set set machine watchdog - 5 seconds - Default disabled
   machine.setWatchdog(5);

   machine.run(); // initial state is given by default first

   FILE* fgraph = fopen("fgraph_STSM.dot", "w");
   fprintf(fgraph, "%s\n", graphviz.c_str());
   fclose(fgraph);
   cout << "Generating image 'fgraph_STSM.png'" << endl;
   system("dot -Tpng fgraph_STSM.dot -o fgraph_STSM.png");
}

void
dbft_test_real_dbft2_primary()
{
   cout << "will create machine!" << endl;
   auto machine = new dBFT2Machine(0, 1); // f=0, N=1

   // ==============================
   // prepare two events for testing
   // ==============================
   // event scheduled to raise "OnStart" machine 0, after 1 seconds
   machine->scheduleEvent(
     1.0, // 1 second to expire: after initialize()
     0,   // machine 0
     "OnStart",
     vector<string>(0)); // no parameters

   // event scheduled to raise "OnPrepareRequest" machine 0, after 3 seconds
   machine->scheduleEvent(
     3.0, // 3 second to expire: after initialize()
     0,   // machine 0
     "OnPrepareRequest",
     vector<string>(1, "0")); // view = 0
   // ==============================

   cout << "Machine => " << machine->toString() << endl;

   // v = 0, H = 1500, T = 3 (secs), R = 1 (one node network)
   dBFT2Context data(0, 1501, 3, 2); // 1500 -> primary (R=1)

   MultiContext<dBFT2Context> ctx;
   ctx.vm.push_back(MachineContext<dBFT2Context>(&data, machine->machines[0]));

   // run for 5.0 seconds max
   machine->setWatchdog(5.0);

   cout << "BEFORE RUN, WILL PRINT AS GRAPHVIZ!" << endl;

   string graphviz = machine->toString("graphviz");

   cout << graphviz << endl;

   machine->run(nullptr, &ctx);

   // show
   FILE* fgraph = fopen("fgraph.dot", "w");
   fprintf(fgraph, "%s\n", graphviz.c_str());
   fclose(fgraph);
   cout << "Generating image 'fgraph.png'" << endl;
   system("dot -Tpng fgraph.dot -o fgraph.png");
   //system("dot -Tpng fgraph.dot -o fgraph.png && eog fgraph.png");
}

int
main()
{
   cout << "begin test state machines!" << endl;

   // simple example: wait one second and quit
   simpleExample();

   // Neo dbft modeling as example
   //dbft_test_primary();

   // warm-up
   //dbft_test_backup_multi();

   // real thing starting to happen here
   dbft_test_real_dbft2_primary();

   cout << "finished successfully!" << endl;

   return 0;
}