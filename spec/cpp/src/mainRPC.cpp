
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
RPC_dbft_test_real_dbft2(int me, int N, int f, int H, int T, int W, int InitDelayMS, int RegularDelayMS,
		double dropRate, std::string scenario, std::string dbft_type, int RANDOM)
{
   std::stringstream ssbasefile;
   ssbasefile << scenario << "-r" << RANDOM << "-id" << me << "_N" << N;

   std::stringstream sslog;
   sslog << "log-" << ssbasefile.str() << ".txt";
   FILE* flog = fopen(sslog.str().c_str(), "w");

   std::stringstream ssimage;
   ssimage << "graph-" << ssbasefile.str() << ".dot";

   std::stringstream ssgraphviz;
   ssgraphviz << "dot -Tpng " << ssimage.str() << " -o " << ssimage.str() << ".png";

   cout << "will create RPC machine context!" << endl;
   // v = 0, H = 1501, T = 3 (secs), R = N (multi-node network)
   int v = 0;
   dBFT2Context data(v, H, T, N, f); // 1500 -> primary (R=1)
   // dBFT2Context(int _v, int _H, int _T, int _R)

   // initialize my world: one RPC Client for every other node (including myself)
   vector<BFTEventsClient*> worldCom(N, nullptr);
   for (unsigned i = 0; i < worldCom.size(); i++)
      worldCom[i] = new BFTEventsClient(i);

   // my own context: including my data, my name and my world
   // random 'seed' is added to '_me' identifier, to get independent (but deterministic) values on different nodes
   RPCMachineContext<dBFT2Context> ctx(&data, me, worldCom, RANDOM + me);
   std::cout << " delayMS = " << RegularDelayMS << std::endl;
   ctx.testSetRegularDelay(RegularDelayMS);
   std::cout << " dropRate = " << dropRate << std::endl;
   ctx.testSetDropRate(dropRate);

   cout << "will create RPC machine!" << endl;
   // ctx is passed here just to initialize my server... ctx is not stored.
   auto machine = new dBFT2RPCMachine(f, N, MachineId(me), &ctx);
   // limit execution time to W secs
   std::cout << "Setting watchdog to " << W << " seconds" << std::endl;
   machine->setWatchdog(W);

   cout << "RPC Machine => " << machine->toString() << endl;

   cout << "BEFORE RUN, WILL PRINT AS GRAPHVIZ!" << endl;
   string graphviz = machine->toString("graphviz");
   cout << graphviz << endl;

   // send OnStart event, after 1 second
   std::thread threadOnStart(sendOnStart, worldCom[me], InitDelayMS);

   // run dBFT on main thread and start RPC on background
   // TODO: 'runWithEventsServer' should become default 'run', as RPC is required here, not optional
   machine->runWithEventsServer(nullptr, &ctx);

   cout << "FINISHED WORK ON MAIN THREAD... JUST JOIN OnStart THREAD" << endl;

   // finishes thread OnStart
   threadOnStart.join();

   // show
   FILE* fgraph = fopen(ssimage.str().c_str(), "w");
   fprintf(fgraph, "%s\n", graphviz.c_str());
   fclose(fgraph);
   cout << "Generating image '" << ssimage.str() << ".png'" << endl;
   if (!system(ssgraphviz.str().c_str())) {
      cout << "Problem generating files" << endl;
   }
   //system("dot -Tpng fgraph.dot -o fgraph.png");
   //system("dot -Tpng fgraph.dot -o fgraph.png && eog fgraph.png");
   fclose(flog);
}

int
main(int argc, char* argv[])
{
   if (argc < 5) {
      std::cout << "missing parameters! argc=" << argc << " and should be 5 or 6" << std::endl;
      std::cout << "requires: my_index N f scenario RANDOM" << std::endl;
      return 1;
   }

   std::cout << std::endl
             << "=======================================" << std::endl
             << "Welcome to libbft RPC testing for dbft2" << std::endl
             << "=======================================" << std::endl
             << std::endl;

   // parameter 0 is program itself
   int my_index = stoi(std::string(argv[1])); // 1. GET FROM MAIN()
   int N = stoi(std::string(argv[2]));        // 2. total number of nodes
   int f = stoi(std::string(argv[3]));        // 3. number of max faulty nodes
   std::string scenario(argv[4]);             // 4. test scenario
   int RANDOM = 99;
   if (argc == 6)
      RANDOM = stoi(std::string(argv[5])); // 5. random (usually 2-bytes)

   // show random seed (shared among all instances)
   // this is good to reproduce nearly-deterministic behavior (some small race conditions may still affect execution)
   std::cout << " ---------- SHARED RANDOM SEED IS: " << RANDOM << " ---------- " << std::endl;

   // default
   int T = 3;                    // block time (3 secs)
   int W = 10;                   // watchdog (10 secs)
   int H = 100;                  // initial height
   int InitDelayMS = 1000;       // initial delay for OnStart (in MS)
   int RegularDelayMS = 0;       // regular delay (in MS)
   std::string type = "Commit1"; // dbft type
   double dropRate = 0.0;        // 0% drop rate

   std::cout << "Loading test scenario: " << scenario << std::endl;
   if (scenario == "S3_1000_0_Commit1") {
      // single cycle (single block relay commit phase)
      T = 3;              // block time (3 secs)
      W = 10;             // watchdog - 10 secs
      H = 100;            // initial height
      InitDelayMS = 1000; // initial delay for OnStart (in MS)
      RegularDelayMS = 0; // regular delay (in MS)
   } else if (scenario == "S3_2000_0_Commit1") {
      // single cycle (single block relay commit phase)
      T = 3;              // block time (3 secs)
      W = 10;             // watchdog - 10 secs
      H = 100;            // initial height
      InitDelayMS = 2000; // initial delay for OnStart (in MS)
      RegularDelayMS = 0; // regular delay (in MS)
   } else if (scenario == "S3_1000_500_Commit1") {
      // single cycle (single block relay commit phase)
      T = 3;                // block time (3 secs)
      W = 10;               // watchdog - 10 secs
      H = 100;              // initial height
      InitDelayMS = 1000;   // initial delay for OnStart (in MS)
      RegularDelayMS = 500; // regular delay (in MS)
   } else if (scenario == "S3_2000_500_Commit1") {
      // single cycle (single block relay commit phase)
      T = 3;                // block time (3 secs)
      W = 10;               // watchdog - 10 secs
      H = 100;              // initial height
      InitDelayMS = 2000;   // initial delay for OnStart (in MS)
      RegularDelayMS = 500; // regular delay (in MS)
   } else if (scenario == "S_drop_3_1000_500_Commit1") {
      // single cycle (single block relay commit phase)
      T = 3;                // block time (3 secs)
      W = 10;               // watchdog - 10 secs
      H = 100;              // initial height
      InitDelayMS = 2000;   // initial delay for OnStart (in MS)
      RegularDelayMS = 500; // regular delay (in MS)
      dropRate = 0.2;       // 20% drop rate (permanent failure of message!)
   } else {
      std::cerr << "NO SCENARIO PASSED! USING DEFAULT!" << std::endl;
   }

   RPC_dbft_test_real_dbft2(my_index, N, f, H, T, W, InitDelayMS, RegularDelayMS, dropRate, scenario, type, RANDOM);

   std::cout << "finished successfully!" << std::endl;

   return 0;
}