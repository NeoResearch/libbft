#pragma once
#ifndef LIBBFT_SRC_CPP_DBFT2_DBFT2_RPC_MACHINE_HPP
#define LIBBFT_SRC_CPP_DBFT2_DBFT2_RPC_MACHINE_HPP

// system includes
#include <sstream>
#include <vector>
// simulate non-deterministic nature
#include <algorithm>
#include <cassert>
#include <random>
#include <thread>

// standard Transition
//#include "replicated/ReplicatedSTSM.hpp"
#include "single/SingleTimerStateMachine.hpp"
#include "single/Transition.hpp"
#include "timing/Timer.hpp"

// dbft specific
#include "dBFT2Context.hpp"

// rpc part
#include "bftevents-grpc/BFTEventsServer.hpp"
#include "rpc-replicated/RPCMachineContext.hpp"

#include "events/ScheduledEvent.hpp"

namespace libbft {

/**
 * This an RPC-based dBFT2 machine
 */
class dBFT2RPCMachine : public SingleTimerStateMachine<RPCMachineContext<dBFT2Context>>
{
public:
   /** max number of faulty nodes */
   int f;
   /** events server to receive info from other nodes */
   BFTEventsServer<dBFT2Context> eventsServer;

   // trying to avoid scheduled events here... this is a real machine, not for testing.
   ///std::vector<ScheduledEvent> schedEvents;

   /** it is recommended to have N = 3f+1 (e.g., f=0 -> N=1; f=1 -> N=4; f=2 -> N=7; ...) */
   explicit dBFT2RPCMachine(int _f = 0, int N = 1, MachineId _me = MachineId(),
   		RPCMachineContext<dBFT2Context>* myCtx = nullptr, std::string _name = "dBFT2_RPC_machine",
         std::string dbft_type = "Commit1", std::unique_ptr<Clock> _clock = nullptr)
     : SingleTimerStateMachine<RPCMachineContext<dBFT2Context>>(
          new Timer("C", std::move(_clock)), _me, std::move(_clock), std::move(_name)
       )
     , f(_f)
     , eventsServer(_me.id, myCtx)
   //Timer* t = nullptr, int me = 0, Clock* _clock = nullptr, string name = "STSM"
   {
      assert(f >= 0);
      assert(N >= 1);
      assert(f <= N);
      // create 'world' here? TODO:

      // create N machines
      //this->machines = vector<SingleTimerStateMachine<RPCMachineContext<dBFT2Context>>*>(N, nullptr);

      // initialize independent machines (each one with its Timer, sharing same global Clock)
      // should never share Timers here, otherwise strange things may happen (TODO: protect from this... unique_ptr?)
      //for (unsigned i = 0; i < N; i++) {
      //   this->machines[i] = new SingleTimerStateMachine<RPCMachineContext<dBFT2Context>>(new Timer("C", this->clock), i, this->clock, "dBFT");
      //}

      // fill states and transitions on each machine
      ////for (unsigned i = 0; i < N; i++)
      // now not using i, single machine

      fillStatesForMachine(dbft_type);
   }

   /**
    * TODO: delete lot's of stuff
    * unique_ptr the clock perhaps? shared_ptr?
    * very dangerous to delete like this... clock may be shared.
    */
   virtual ~dBFT2RPCMachine() = default;

   void fillSimpleCycle()
   {
      // ---------------------
      // declaring dBFT states
      // ---------------------

      auto preinitial = new State<RPCMachineContext<dBFT2Context>>(false, "PreInitial");
      this->registerState(preinitial);
      auto started = new State<RPCMachineContext<dBFT2Context>>(false, "Started");
      this->registerState(started);
      auto backup = new State<RPCMachineContext<dBFT2Context>>(false, "Backup");
      this->registerState(backup);
      auto primary = new State<RPCMachineContext<dBFT2Context>>(false, "Primary");
      this->registerState(primary);
      auto reqSentOrRecv = new State<RPCMachineContext<dBFT2Context>>(false, "RequestSentOrReceived");
      this->registerState(reqSentOrRecv);
      auto commitSent = new State<RPCMachineContext<dBFT2Context>>(false, "CommitSent");
      this->registerState(commitSent);
      auto viewChanging = new State<RPCMachineContext<dBFT2Context>>(false, "ViewChanging");
      this->registerState(viewChanging);
      auto blockSent = new State<RPCMachineContext<dBFT2Context>>(true, "BlockSent");
      this->registerState(blockSent);

      // -------------------------
      // creating dBFT transitions
      // -------------------------

      //auto machine = this;
      //int id = this->me.id;

      // preinitial -> started
      preinitial->addTransition(
        (new Transition<RPCMachineContext<dBFT2Context>>(started))->add(Condition<RPCMachineContext<dBFT2Context>>(
        		"OnStart()", [](const Timer& t, RPCMachineContext<dBFT2Context>* d, MachineId _me) -> bool {
           std::cout << "Waiting for OnStart..." << std::endl;
           return d->hasEvent("OnStart", std::vector<std::string>(0)); // no parameters on event
        })));

      // initial -> backup
      started->addTransition(
        (new Transition<RPCMachineContext<dBFT2Context>>(backup))->add(Condition<RPCMachineContext<dBFT2Context>>(
        		"not (H+v) mod R = i", [](const Timer& t, RPCMachineContext<dBFT2Context>* d, MachineId _me) -> bool {
           std::cout << "lambda1" << std::endl;
           return (d->params->H + d->params->v) % d->params->R != _me.id;
        })));

      // initial -> primary
      started->addTransition(
        (new Transition<RPCMachineContext<dBFT2Context>>(primary))->add(Condition<RPCMachineContext<dBFT2Context>>(
        		"(H+v) mod R = i", [](const Timer& t, RPCMachineContext<dBFT2Context>* d, MachineId _me) -> bool {
           std::cout << "lambda2 H=" << d->params->H << " v=" << d->params->v << " _me=" << _me.id << std::endl;
           return (d->params->H + d->params->v) % d->params->R == _me.id;
        })));

      // backup -> reqSentOrRecv
      auto toReqSentOrRecv1 = new Transition<RPCMachineContext<dBFT2Context>>(reqSentOrRecv);
      backup->addTransition(
        toReqSentOrRecv1->add(Condition<RPCMachineContext<dBFT2Context>>("OnPrepareRequest(v,H)", [](
        		const Timer& t, RPCMachineContext<dBFT2Context>* d, MachineId _me) -> bool {
                      std::cout << "waiting for event OnPrepareRequest at " << _me.id << " for view " << d->params->v
                           << std::endl;
							 // args: view and height
                      std::vector<std::string> evArgs = { std::to_string(d->params->v), std::to_string(d->params->H) };
							 return d->hasEvent("PrepareRequest", evArgs);
						})
				)->add(Condition<RPCMachineContext<dBFT2Context>>("ValidBlock", [](
						const Timer& t, RPCMachineContext<dBFT2Context>* d, MachineId _me) -> bool {
             // always good block right now
             return true;
          	})
				)->add(Action<RPCMachineContext<dBFT2Context>>("send: PrepareResponse(v, H)",
						[](Timer& C, RPCMachineContext<dBFT2Context>* d, MachineId _me) -> void {
                     std::cout << "sending PrepareResponse from " << _me.id << " for view " << d->params->v << std::endl;
							// TODO: attach  block hash as well?
                     std::vector<std::string> evArgs = { std::to_string(d->params->v), std::to_string(d->params->H) };
							d->broadcast("PrepareResponse", evArgs);
          	})
				)
			);

      // primary -> reqSentOrRecv
      auto primToReqSentOrRecv1 = new Transition<RPCMachineContext<dBFT2Context>>(reqSentOrRecv);
      primary->addTransition(
        primToReqSentOrRecv1->add(Condition<RPCMachineContext<dBFT2Context>>("C >= T?", [](
        		const Timer& C, RPCMachineContext<dBFT2Context>* d, MachineId _me) -> bool {
							 // C >= T?
							 return C.elapsedTime() >= d->params->T;
						})
				)->add(Action<RPCMachineContext<dBFT2Context>>("send: PrepareRequest(v, H)",
						[](Timer& C, RPCMachineContext<dBFT2Context>* d, MachineId _me) -> void {
                     std::cout << "sending PrepareRequest from " << _me.id << " for view " << d->params->v << std::endl;
							// TODO: attach  block hash as well?
                     std::vector<std::string> evArgs = { std::to_string(d->params->v), std::to_string(d->params->H) };
							d->broadcast("PrepareRequest", evArgs);
          	})
				)->add(Action<RPCMachineContext<dBFT2Context>>("C := 0",
						[](Timer& C, RPCMachineContext<dBFT2Context>* d, MachineId _me) -> void {
            	C.reset();
          	})
				)
			);

      // primary -> viewChanging
      auto primToViewChanging = new Transition<RPCMachineContext<dBFT2Context>>(viewChanging);
      primary->addTransition(
        primToViewChanging->add(Condition<RPCMachineContext<dBFT2Context>>("(C >= T exp(v+1))?", [](const Timer& C,
        		RPCMachineContext<dBFT2Context>* d, MachineId _me) -> bool {
							 // C >= T?
							 //cout << "evaluate primary view change... C = " << C.elapsedTime() << " ~ T_limit = " << (d->params->T * std::pow(2, d->params->v+1)) << std::endl;
							 return C.elapsedTime() >= (d->params->T * std::pow(2, d->params->v + 1));
						})
				)->add(Action<RPCMachineContext<dBFT2Context>>("send: ChangeView(v+1, H)", [](Timer& C,
						RPCMachineContext<dBFT2Context>* d, MachineId _me) -> void {
                     std::cout << "sending ChangeView from " << _me.id << " for view " << d->params->v << std::endl;
							// TODO: attach  block hash as well?
                     std::vector<std::string> evArgs = { std::to_string(d->params->v + 1), std::to_string(d->params->H) };
							d->broadcast("ChangeView", evArgs);
          	})
				)->add(Action<RPCMachineContext<dBFT2Context>>("C := 0", [](Timer& C, RPCMachineContext<dBFT2Context>* d,
						MachineId _me) -> void {
            	C.reset();
						})
				)
			);

      // backup -> viewChanging
      auto backpToViewChanging = new Transition<RPCMachineContext<dBFT2Context>>(viewChanging);
      backup->addTransition(backpToViewChanging->add(Condition<RPCMachineContext<dBFT2Context>>("(C >= T exp(v+1))?",
      		[](const Timer& C, RPCMachineContext<dBFT2Context>* d, MachineId _me) -> bool {
						// C >= T?
						//cout << "evaluate primary view change... C = " << C.elapsedTime() << " ~ T_limit = " << (d->params->T * std::pow(2, d->params->v+1)) << std::endl;
						return C.elapsedTime() >= (d->params->T * std::pow(2, d->params->v + 1));
					}
			))->add(Action<RPCMachineContext<dBFT2Context>>("send: ChangeView(v+1, H)", [](Timer& C,
					RPCMachineContext<dBFT2Context>* d, MachineId _me) -> void {
             std::cout << "sending ChangeView from " << _me.id << " for view " << d->params->v << std::endl;
             // TODO: attach  block hash as well?
             std::vector<std::string> evArgs = { std::to_string(d->params->v + 1), std::to_string(d->params->H) };
             d->broadcast("ChangeView", evArgs);
          }
			))->add(Action<RPCMachineContext<dBFT2Context>>("C := 0", [](Timer& C, RPCMachineContext<dBFT2Context>* d,
					MachineId _me) -> void {
             C.reset();
          }
			)));

      // reqSentOrRecv -> viewChanging
      auto reqsrToViewChanging = new Transition<RPCMachineContext<dBFT2Context>>(viewChanging);
      reqSentOrRecv->addTransition(reqsrToViewChanging->add(Condition<RPCMachineContext<dBFT2Context>>(
      		"(C >= T exp(v+1))?", [](const Timer& C, RPCMachineContext<dBFT2Context>* d, MachineId _me) -> bool {
						// C >= T?
						//cout << "evaluate primary view change... C = " << C.elapsedTime() << " ~ T_limit = " << (d->params->T * std::pow(2, d->params->v+1)) << std::endl;
						return C.elapsedTime() >= (d->params->T * std::pow(2, d->params->v + 1));
					}
			))->add(Action<RPCMachineContext<dBFT2Context>>("send: ChangeView(v+1, H)", [](Timer& C,
					RPCMachineContext<dBFT2Context>* d, MachineId _me) -> void {
             std::cout << "sending ChangeView from " << _me.id << " for view " << d->params->v << std::endl;
             // TODO: attach  block hash as well?
             std::vector<std::string> evArgs = { std::to_string(d->params->v + 1), std::to_string(d->params->H) };
             d->broadcast("ChangeView", evArgs);
          }
			))->add(Action<RPCMachineContext<dBFT2Context>>("C := 0", [](Timer& C, RPCMachineContext<dBFT2Context>* d,
					MachineId _me) -> void {
             C.reset();
          }
			)));

      // reqSentOrRecv -> commitSent
      auto reqsrToCommitSent = new Transition<RPCMachineContext<dBFT2Context>>(commitSent);
      reqSentOrRecv->addTransition(reqsrToCommitSent->add(Condition<RPCMachineContext<dBFT2Context>>(
      		"EnoughPreparations", [](const Timer& C, RPCMachineContext<dBFT2Context>* d, MachineId _me) -> bool {
                     std::vector<Event*> events = d->getEvents();
                     std::cout << "waiting for 2f+1 PrepareResponse" << std::endl;
							// waiting for 2f+1 PrepareResponse(v,H)
                     std::vector<std::string> evArgs = { std::to_string(d->params->v), std::to_string(d->params->H) };
							int countPrepResp = 0;
							for (int id = 0; id < d->params->R; id++) {
								 for (auto & event : events) {
										if (event->getFrom().id == id)
											 if (event->isActivated("PrepareResponse", evArgs))
													countPrepResp++;
								 }
							}
                     std::cout << "count PrepareResponse = " << countPrepResp << " / " << (d->params->M() - 1) << std::endl;
							// count >= 2f+1 (or M) -1 (because Prepare Request also counts)
							return countPrepResp >= (d->params->M() - 1);
					}
			))->add(Action<RPCMachineContext<dBFT2Context>>("send: Commit(v, H)", [](Timer& C,
					RPCMachineContext<dBFT2Context>* d, MachineId _me) -> void {
             std::cout << "sending Commit from " << _me.id << " for view " << d->params->v << std::endl;
             // TODO: attach  block hash as well?
             std::vector<std::string> evArgs = { std::to_string(d->params->v), std::to_string(d->params->H) };
             d->broadcast("Commit", evArgs);
          }
			))->add(Action<RPCMachineContext<dBFT2Context>>("C := 0", [](Timer& C, RPCMachineContext<dBFT2Context>* d,
					MachineId _me) -> void {
             C.reset();
          }
			)));

      // commitSent -> blockSent
      auto commitSentToBlockSent = new Transition<RPCMachineContext<dBFT2Context>>(blockSent);
      commitSent->addTransition(commitSentToBlockSent->add(Condition<RPCMachineContext<dBFT2Context>>("EnoughCommits",
      		[](const Timer& C, RPCMachineContext<dBFT2Context>* d, MachineId _me) -> bool {
							std::vector<Event*> events = d->getEvents();
                     std::cout << "waiting for 2f+1 Commits" << std::endl;
							// waiting for 2f+1 Commit(v,H)
                     std::vector<std::string> evArgs = { std::to_string(d->params->v), std::to_string(d->params->H) };
							int count = 0;
							for (int id = 0; id < d->params->R; id++) {
								 for (auto & event : events) {
										if (event->getFrom().id == id)
											 if (event->isActivated("Commit", evArgs))
													count++;
								 }
							}
                     std::cout << "count Commit = " << count << " / " << d->params->M() << std::endl;
							// count >= 2f+1 (or M)
							return count >= d->params->M();
					}
			))->add(Action<RPCMachineContext<dBFT2Context>>("send: BlockRelay(v, H)",
					[](Timer& C, RPCMachineContext<dBFT2Context>* d, MachineId _me) -> void {
             std::cout << "sending BlockRelay for H=" << d->params->H << std::endl;
             // TODO: attach  block hash as well?
             std::vector<std::string> evArgs = { std::to_string(d->params->v), std::to_string(d->params->H) };
             d->broadcast("BlockRelay", evArgs);
          }
			))->add(Action<RPCMachineContext<dBFT2Context>>("C := 0", [](Timer& C, RPCMachineContext<dBFT2Context>* d,
					MachineId _me) -> void {
             C.reset();
          }
			)));

      // viewChanging -> started
      auto viewChToStarted = new Transition<RPCMachineContext<dBFT2Context>>(started);
      viewChanging->addTransition(viewChToStarted->add(Condition<RPCMachineContext<dBFT2Context>>("EnoughViewChanges",
      		[](const Timer& C, RPCMachineContext<dBFT2Context>* d, MachineId _me) -> bool {
							std::vector<Event*> events = d->getEvents();
                     std::cout << "waiting for 2f+1 View Changes" << std::endl;
							// waiting for 2f+1 ChangeView(v+1,H)
                     std::vector<std::string> evArgs = { std::to_string(d->params->v + 1), std::to_string(d->params->H) };
							int count = 0;
							for (int id = 0; id < d->params->R; id++) {
								 for (auto & event : events) {
										if (event->getFrom().id == id)
											 if (event->isActivated("ChangeView", evArgs))
													count++;
								 }
							}
                     std::cout << "count ChangeView = " << count << " / " << d->params->M() << std::endl;
							// count >= 2f+1 (or M)
							return count >= d->params->M();
					 }
			 ))->add(Action<RPCMachineContext<dBFT2Context>>("v := v + 1", [](Timer& C, RPCMachineContext<dBFT2Context>* d,
			 		MachineId _me) -> void {
             d->params->v = d->params->v + 1;
             std::cout << " ***** MOVING TO NEXT VIEW: " << d->params->v << std::endl;
          }
			))->add(Action<RPCMachineContext<dBFT2Context>>("C := 0", [](Timer& C, RPCMachineContext<dBFT2Context>* d,
					MachineId _me) -> void {
             C.reset();
          }
			)));
   }

public: // real public
   using Events = std::vector<Event*>;

   /** pending events (external queue may affect this one) */
   Events pendingEvents; // TODO: this should be concurrent safe (some std::concurrent_vector?)

   /** override beforeUpdateState to include pendingEvents */
   bool beforeUpdateState(State<RPCMachineContext<dBFT2Context>>& current, RPCMachineContext<dBFT2Context>* p) override
   {
      if (this->watchdog && this->watchdog->expired()) {
         std::cout << "dBFT2 RPC StateMachine FAILED: MAXTIME = " << this->watchdog->getCountdown() << std::endl;
         return true;
      }

      if (!pendingEvents.empty()) {
         std::cout << "Has some pending events to process! size = " << pendingEvents.size() << std::endl;
         // update states (TODO: update to do in concurrent) .. make some lock here?
         p->addEvents(pendingEvents);
         pendingEvents.clear();
      }

      // nothing to do?
      return false;
   }

private:
   std::thread rpcThread;

   static void startEventsServer(dBFT2RPCMachine* machine)
   {
      machine->eventsServer.RunForever();
   }

public:
   /** TODO(@igormcoelho): 'runWithEventsServer' should become default 'run', as RPC is required here, not optional */
   virtual void runWithEventsServer(State<RPCMachineContext<dBFT2Context>>* initial, RPCMachineContext<dBFT2Context>* ctx)
   {
      std::cout << "Starting thread to handle RPC messages:" << std::endl;
      runEventsServer(); // this will run on a dettached (background) thread
      // will wait few ms, just for RPC to start. TODO(@igormcoelho): should sync on some condition_variable from RPC (indicating 'started')
      std::this_thread::sleep_for(std::chrono::milliseconds(100)); // 100 ms
      // run dBFT on main thread (RPC is already running on background)
      this->run(initial, ctx);
      // stops events server and join its thread
      killEventsServer();
   }

private:
   virtual void runEventsServer()
   {
      // starts rpc thread in background
      rpcThread = std::thread(startEventsServer, this);
   }

   virtual void killEventsServer()
   {
      eventsServer.Stop();
      rpcThread.join();
   }

   /** official method */
   virtual void fillStatesForMachine(const std::string &dbft_type)
   {
      if (dbft_type == "Commit1") {
         // method for a simple dbft cycle (single height considered)
         fillSimpleCycle();
      } else {
         std::cout << "ERROR! NOT RECOGNIZED DBFT TYPE = '" << dbft_type << "'" << std::endl;
         exit(1);
      }
   }

public:
   /*

   virtual State<dBFT2Context>* initialize(State<dBFT2Context>* current, RPCMachineContext<dBFT2Context>* p) override
   {
      if (!current)
         current = new State<dBFT2Context>(machines.size(), nullptr);
      for (unsigned i = 0; i < machines.size(); i++)
         current->at(i) = machines[i]->getDefaultState(); // first state is initial (default)

      cout << endl;
      cout << "=================" << endl;
      cout << "begin run() dBFT2" << endl;
      cout << "=================" << endl;

      cout << "initializing multimachine" << endl;
      if (watchdog)
         watchdog->reset();
      else
         cout << "No watchdog configured!" << endl;
      for (unsigned i = 0; i < machines.size(); i++)
         machines[i]->initialize(current->at(i), p);

      // from ReplicatedSTSM (scheduled events for simulation)
      this->launchScheduledEvents(p);

      return current;
   }

   */

   void OnInitialize(RPCMachineContext<dBFT2Context>* p) override
   {
      //launchSchedEvents(p);
   }

   // no scheduled events on a real machine (try to avoid this, and only put on testing with mocking)
   /*
private:
   void launchSchedEvents(RPCMachineContext<dBFT2Context>* p)
   {
      cout << "launching scheduled events!" << endl;
      // launch all scheduled events
      for (unsigned i = 0; i < schedEvents.size(); i++) {
         p->launchTimedEvent(schedEvents[i]);
      }
      // clear all scheduled events
      schedEvents.clear();
   }
*/

public:
   std::string toString(std::string format = "") override
   {
      std::stringstream ss;

      if (format == "graphviz") {
         // will do this only for machine 0 (the others should be replicated)
         ss << "digraph " << this->name << " {" << std::endl;
         ss << "//graph [bgcolor=lightgoldenrodyellow]" << std::endl;
         ss << "//rankdir=LR;" << std::endl;
         ss << "size=\"11\"" << std::endl;
         // add states
         ss << "Empty [ label=\"\", width=0, height=0, style = invis ];" << std::endl;
         for (auto & state : this->states) {
            ss << "node [shape = " << (state->isFinal ? "doublecircle" : "circle") << "]; "
               << state->name << ";" << std::endl;
         }
         // default initial state transition
         State<RPCMachineContext<dBFT2Context>>* defState = this->getDefaultState();
         // will happen in an empty transition
         ss << "Empty -> " << defState->name << " [label = \"\"];" << std::endl;
         // begin regular transitions
         //Initial -> Primary [ label = "(H + v) mod R = i" ];
         //      getDefaultState
         for (auto & state : this->states) {
            for (auto & transition: state->transitions) {
               ss << state->name << " ";
               ss << transition->toString("graphviz") << std::endl;
            }
         }

         ss << "}" << std::endl;
      } else {
         // standard text
         ss << "Welcome to dBFT2RPCMachine : ";
         ss << "{ name = " << this->name << "}";
      }

      return ss.str();
   }
}; // class dBFT2RPCMachine

} // libbft

#endif // LIBBFT_SRC_CPP_DBFT2_DBFT2_RPC_MACHINE_HPP
