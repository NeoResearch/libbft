
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
   //// Transition<Data>* alwaysTrue = new Transition<Data>(final, "always true");

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

// helper method: will send OnStart after 1 second
void
sendOnStart(BFTEventsClient* myClient, int delayMS)
{
   std::this_thread::sleep_for(std::chrono::milliseconds(delayMS)); // 1000 ms -> 1 sec
   std::cout << " -x-x-> Will deliver message 'OnStart'" << std::endl;
   std::vector<std::string> eventArgs;
   bool output = myClient->informEvent(-1, "OnStart", eventArgs);
   if (output)
      std::cout << " -x-x-> Event message 'OnStart' delivered!" << std::endl;
}

void
RPC_dbft_test_real_dbft2(int me, int N, int f, int H, int T, int DelayMS)
{
   cout << "will create RPC machine context!" << endl;
   // v = 0, H = 1501, T = 3 (secs), R = N (multi-node network)
   int v = 0;
   dBFT2Context data(v, H, T, N); // 1500 -> primary (R=1)
   // dBFT2Context(int _v, int _H, int _T, int _R)

   // initialize my world: one RPC Client for every other node (including myself)
   vector<BFTEventsClient*> worldCom(N, nullptr);
   for (unsigned i = 0; i < worldCom.size(); i++)
      worldCom[i] = new BFTEventsClient(i);

   // my own context: including my data, my name and my world
   RPCMachineContext<dBFT2Context> ctx(&data, me, worldCom);

   cout << "will create RPC machine!" << endl;
   // ctx is passed here just to initialize my server... ctx is not stored.
   auto machine = new dBFT2RPCMachine(f, N, MachineId(me), &ctx);
   // run for 5.0 seconds max
   machine->setWatchdog(5.0);
   cout << "RPC Machine => " << machine->toString() << endl;

   cout << "BEFORE RUN, WILL PRINT AS GRAPHVIZ!" << endl;
   string graphviz = machine->toString("graphviz");
   cout << graphviz << endl;

   // send OnStart event, after 1 second
   std::thread threadOnStart(sendOnStart, worldCom[me], DelayMS);

   // run dBFT on main thread and start RPC on background
   // TODO: 'runWithEventsServer' should become default 'run', as RPC is required here, not optional
   machine->runWithEventsServer(nullptr, &ctx);

   cout << "FINISHED WORK ON MAIN THREAD... JUST JOIN OnStart THREAD" << endl;

   // finishes thread OnStart
   threadOnStart.join();

   // show
   FILE* fgraph = fopen("fgraph.dot", "w");
   fprintf(fgraph, "%s\n", graphviz.c_str());
   fclose(fgraph);
   cout << "Generating image 'fgraph.png'" << endl;
   system("dot -Tpng fgraph.dot -o fgraph.png");
   //system("dot -Tpng fgraph.dot -o fgraph.png && eog fgraph.png");
}

int
main(int argc, char* argv[])
{
   if (argc != 7) {
      std::cout << "missing parameters! argc=" << argc << " and should be 7" << std::endl;
      std::cout << "requires: my_index N f H T DelayMS" << std::endl;
      return 1;
   }

   int my_index = stoi(std::string(argv[1])); // GET FROM MAIN()
   int N = stoi(std::string(argv[2]));        // total number of nodes
   int f = stoi(std::string(argv[3]));        // number of max faulty nodes
   int H = stoi(std::string(argv[4]));        // initial height
   int T = stoi(std::string(argv[5]));        // block time (3 secs)
   int DelayMS = stoi(std::string(argv[6]));  // initial delay for OnStart (in MS)

   RPC_dbft_test_real_dbft2(my_index, N, f, H, T, DelayMS);

   cout << "finished successfully!" << endl;

   return 0;
}