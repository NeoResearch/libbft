
// system
#include <iostream>
#include <thread>
#include <vector>

// lib

#include "events/Event.hpp"
#include "replicated/ReplicatedSTSM.hpp"
#include "single/SingleTimerStateMachine.hpp"
#include "single/State.hpp"

#include "dbft2/dBFT2Machine.hpp"
#include "dbft2/dBFT2RPCMachine.hpp"

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
     (new Transition<Data>(final, "after1sec"))->add(Condition<Data>("C >= 1", [](const Timer& c, Data* d, MachineId) -> bool {
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

SingleTimerStateMachine<MultiContext<dBFT2Context>>*
simpleMultiMachineExample(int id)
{
   // ---------------------
   // declaring dBFT states
   // ---------------------

   auto started = new State<MultiContext<dBFT2Context>>(false, "Started");
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
   started->addTransition(
     (new Transition<MultiContext<dBFT2Context>>(backup))->add(Condition<MultiContext<dBFT2Context>>("not (H+v) mod R = i", [](const Timer& t, MultiContext<dBFT2Context>* d, MachineId me) -> bool {
        cout << "lambda1" << endl;
        return !((d->vm[me.id].params->H + d->vm[me.id].params->v) % d->vm[me.id].params->R == me.id);
     })));

   // initial -> primary
   started->addTransition(
     (new Transition<MultiContext<dBFT2Context>>(primary))->add(Condition<MultiContext<dBFT2Context>>("(H+v) mod R = i", [](const Timer& t, MultiContext<dBFT2Context>* d, MachineId me) -> bool {
        cout << "lambda2 H=" << d->vm[me.id].params->H << " v=" << d->vm[me.id].params->v << " me=" << me.id << endl;
        return (d->vm[me.id].params->H + d->vm[me.id].params->v) % d->vm[me.id].params->R == me.id;
     })));

   // backup -> reqSentOrRecv
   auto toReqSentOrRecv1 = new Transition<MultiContext<dBFT2Context>>(reqSentOrRecv);
   backup->addTransition(
     toReqSentOrRecv1->add(Condition<MultiContext<dBFT2Context>>("OnPrepareRequest", [](const Timer& t, MultiContext<dBFT2Context>* d, MachineId me) -> bool {
        cout << "waiting for event OnPrepareRequest at " << me.id << endl;
        return d->hasEvent("OnPrepareRequest", me.id, vector<string>(0)); // no parameters on event
     })));

   // reqSentOrRecv -> commitSent
   reqSentOrRecv->addTransition(
     (new Transition<MultiContext<dBFT2Context>>(commitSent))->add(Condition<MultiContext<dBFT2Context>>("(H+v) mod R = i", [](const Timer& t, MultiContext<dBFT2Context>* d, MachineId me) -> bool {
        cout << "nothing to do... assuming all preparations were received!" << endl;
        return true;
     })));

   // commitSent -> blockSent
   commitSent->addTransition(
     (new Transition<MultiContext<dBFT2Context>>(blockSent))->add(Condition<MultiContext<dBFT2Context>>("(H+v) mod R = i", [](const Timer& t, MultiContext<dBFT2Context>* d, MachineId me) -> bool {
        cout << "nothing to do... assuming all commits were received!" << endl;
        return true;
     })));

   machine->registerState(started);
   machine->registerState(blockSent);

   return machine;
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
     1.0,          // 1 second to expire: after initialize()
     MachineId(0), // machine 0
     "OnStart",
     vector<string>(0)); // no parameters

   // event scheduled to raise "OnPrepareRequest" machine 0, after 3 seconds
   machine->scheduleEvent(
     3.0,          // 3 second to expire: after initialize()
     MachineId(0), // machine 0
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

void
dbft_test_backup_multi()
{
   auto machine0 = simpleMultiMachineExample(0);

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
   /*
   multiMachine.scheduleGlobalTransition(
     (new Timer())->init(1.0), // 1 second to expire
     0,                        // machine 0
     // no other conditions, always 'true'
     (new Transition<MultiContext<dBFT2Context>>(machine0->getStateByName("Initial")))
       ->add(Action<MultiContext<dBFT2Context>>(
         "C := 0 | v := 0",
         [](Timer& C, MultiContext<dBFT2Context>* d, MachineId me) -> void {
            cout << " => action: C := 0" << endl;
            C.reset();
            cout << " => action: v := 0" << endl;
            d->vm[me.id].params->v = 0;
         })));
   */

   // transition for PreInitial to Started
   // preinitial -> started
   preinitial->addTransition(
     (new Transition<MultiContext<dBFT2Context>>(machine0->getStateByName("Started")))->add(Condition<MultiContext<dBFT2Context>>("OnStart", [](const Timer& t, MultiContext<dBFT2Context>* d, MachineId me) -> bool {
        cout << "Waiting for OnStart..." << endl;
        return d->hasEvent("OnStart", me.id, vector<string>(0));
     })));

   // event scheduled to raise "OnStart" machine 0, after 1 seconds
   multiMachine.scheduleEvent(
     1.0,          // 1 second to expire: after initialize()
     MachineId(0), // machine 0
     "OnStart",
     vector<string>(0)); // no parameters

   // event scheduled to raise "OnPrepareRequest" machine 0, after 3 seconds
   multiMachine.scheduleEvent(
     3.0,          // 3 second to expire: after initialize()
     MachineId(0), // machine 0
     "OnPrepareRequest",
     vector<string>(0)); // no parameters

   MultiState<dBFT2Context> minitial(1, nullptr);
   minitial[0] = machine0->getStateByName("PreInitial");

   // run for 5.0 seconds max (watchdog limit)
   multiMachine.setWatchdog(5.0);
   multiMachine.run(&minitial, &ctx);
}

void
dbft_test_primary()
{
   auto machine0 = simpleMultiMachineExample(0);

   cout << "Machine => " << machine0->toString() << endl;

   // v = 0, H = 1500, T = 3 (secs), R = 1 (one node network)
   dBFT2Context data(0, 1500, 3, 1); // 1500 -> primary (R=1)

   MultiContext<dBFT2Context> ctx;
   ctx.vm.push_back(MachineContext<dBFT2Context>(&data, machine0));

   // run for 5.0 seconds max
   machine0->setWatchdog(5.0);
   machine0->run(machine0->states[0], &ctx); // explicitly passing first state as default
}


// TODO: working with threads on global scope... improve this.

dBFT2RPCMachine* global_dBFT_machine = nullptr;

void globalRunRPCServer()
{
   if(global_dBFT_machine)
   {
      cout << "will launch RPC events server on machine: " << global_dBFT_machine->me.id << endl;
      global_dBFT_machine->runEventsServer();
   }
   else
      cerr << "PLEASE SET 'global_dBFT_machine'" << endl;
}

void
RPC_dbft_test_real_dbft2_primary()
{
   cout << "will create RPC machine!" << endl;
   auto machine = new dBFT2RPCMachine(0, 1); // f=0, N=1

   //This machine doesn't have all stuff for testing... real RPC one

   /*
   // ==============================
   // prepare two events for testing
   // ==============================
   // event scheduled to raise "OnStart" machine 0, after 1 seconds
   machine->scheduleEvent(
     1.0,          // 1 second to expire: after initialize()
     MachineId(0), // machine 0
     "OnStart",
     vector<string>(0)); // no parameters

   // event scheduled to raise "OnPrepareRequest" machine 0, after 3 seconds
   machine->scheduleEvent(
     3.0,          // 3 second to expire: after initialize()
     MachineId(0), // machine 0
     "OnPrepareRequest",
     vector<string>(1, "0")); // view = 0
   // ==============================
   */

   cout << "RPC Machine => " << machine->toString() << endl;

   // v = 0, H = 1500, T = 3 (secs), R = 1 (one node network)
   dBFT2Context data(0, 1501, 3, 2); // 1500 -> primary (R=1)

   // TODO: should initialize world here... how many of us exist? get from main?
   vector<BFTEventsClient*> world;
   // should get from main // TODO:
   int me = 0;

   RPCMachineContext<dBFT2Context> ctx(&data, me, world);
   // create world here? // TODO:

   //ctx.vm.push_back(MachineContext<dBFT2Context>(&data, machine->machines[0]));

   // run for 5.0 seconds max
   machine->setWatchdog(5.0);

   cout << "BEFORE RUN, WILL PRINT AS GRAPHVIZ!" << endl;

   string graphviz = machine->toString("graphviz");

   cout << graphviz << endl;

   // TODO: this is where we need two threads at least...
   // one to hear external RPC events
   // another to execute our independent machine

   cout << "Starting thread to handle RPC messages:" << endl;
   global_dBFT_machine = machine;
   std::thread threadRPC(std::bind(globalRunRPCServer)); //machine->runEventsServer();
   // run dBFT on main thread (RPC is running on background)  
   machine->run(nullptr, &ctx); // cannot do both here

   cout << "FINISHED WORK ON MAIN THREAD... JUST WAITING FOR RPC TO FINISH NOW." << endl;
   // do we need to join? or let it expire?
   threadRPC.join();

   // show
   FILE* fgraph = fopen("fgraph.dot", "w");
   fprintf(fgraph, "%s\n", graphviz.c_str());
   fclose(fgraph);
   cout << "Generating image 'fgraph.png'" << endl;
   system("dot -Tpng fgraph.dot -o fgraph.png");
   //system("dot -Tpng fgraph.dot -o fgraph.png && eog fgraph.png");
}

void nothing()
{
   std::cout << "OI" << std::endl;
}

int
main()
{
   /*
      cout << "binding thread" << endl;
   std::thread t(std::bind(nothing));
   sleep(1);
   cout << "will join thread" << endl;
   t.join();
   cout << "after join thread" << endl;

   return 1;
   */

   cout << "begin test state machines!" << endl;

   // simple example: wait one second and quit
   //simpleExample();

   // Neo dbft modeling as example
   //dbft_test_primary();

   // warm-up
   //dbft_test_backup_multi();


   // real thing starting to happen here
   RPC_dbft_test_real_dbft2_primary();

   //std::thread t([](bool b){return true;});
   //t.join();

   cout << "finished successfully!" << endl;

   return 0;
}