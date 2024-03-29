// SPDX-License-Identifier:  MIT
// Copyright (C) 2019-2022 - LibBFT developers
// https://github.com/neoresearch/libbft

// C++
#include <iostream>
#include <memory>
#include <thread>  // NOLINT
#include <vector>

// lib

#include <libbft/dbft2/dBFT2Machine.hpp>
#include <libbft/events/Event.hpp>
#include <libbft/replicated/ReplicatedSTSM.hpp>
#include <libbft/single/SingleTimerStateMachine.hpp>
#include <libbft/single/State.hpp>
#include <libbft/utils/Shortcuts.hpp>

using namespace std;     // NOLINT
using namespace libbft;  // NOLINT

// main function for base testing purposes

// empty data
struct Data {};

using TData = std::shared_ptr<Data>;

void simpleExample() {
  auto initial =
      std::shared_ptr<State<Data>>(new State<Data>(false, "Initial"));
  auto final = std::shared_ptr<State<Data>>(new State<Data>(true, "Final"));

  // unused (demonstration)
  //// Transition<Data>* alwaysTrue = new Transition<Data>(final, "always
  /// true");

  // alwaysTrue->add(Condition<Data>("true", [](const Timer& t, Data*, int) ->
  // bool { return true; })); initial->addTransition(alwaysTrue); // (unused)

  initial->addTransition(std::shared_ptr<Transition<Data>>(
      (new Transition<Data>(final, "after1sec"))
          ->add(Condition<Data>(
              "C >= 3", [](const Timer& c, TData d, MachineId) -> bool {
                // cout << "WAITING TRANSITION TO HAPPEN: " << c.elapsedTime()
                // << "s" << endl;
                cout << "time: " << c.elapsedTime() << "s" << endl;
                return c.elapsedTime() >= 3.0;
              }))));

  // cout << "initial state: " << initial->toString() << endl;
  // cout << "final state: " << final->toString() << endl;

  // Timer never expires
  SingleTimerStateMachine<Data> machine(std::unique_ptr<Timer>(new Timer("C")));

  machine.registerState(initial);
  machine.registerState(final);

  cout << "Machine => " << machine.toString() << endl;

  cout << "BEFORE RUN, WILL PRINT AS GRAPHVIZ!" << endl;
  string graphviz = machine.toStringFormat(StringFormat::Graphviz);
  cout << graphviz << endl;

  // set set machine watchdog - 5 seconds - Default disabled
  machine.setWatchdog(5);

  machine.run(nullptr, nullptr);  // initial state is given by default first

  FILE* fgraph = fopen("fgraph_STSM.dot", "w");  // NOLINT
  fprintf(fgraph, "%s\n", graphviz.c_str());     // NOLINT
  fclose(fgraph);                                // NOLINT
  cout << "Generating image 'fgraph_STSM.png'" << endl;
  if (!system("dot -Tpng fgraph_STSM.dot -o fgraph_STSM.png")) {
    cout << "Problem generating graph" << endl;
  }
}

using McDBFT = MultiContext<dBFT2Context>;
using TMcDBFT = std::shared_ptr<McDBFT>;

SingleTimerStateMachine<McDBFT>* simpleMultiMachineExample(int id) {
  // ---------------------
  // declaring dBFT states
  // ---------------------

  auto started =
      std::shared_ptr<State<McDBFT>>(new State<McDBFT>(false, "Started"));
  auto backup =
      std::shared_ptr<State<McDBFT>>(new State<McDBFT>(false, "Backup"));
  auto primary =
      std::shared_ptr<State<McDBFT>>(new State<McDBFT>(false, "Primary"));
  auto reqSentOrRecv = std::shared_ptr<State<McDBFT>>(
      new State<McDBFT>(false, "RequestSentOrReceived"));
  auto commitSent =
      std::shared_ptr<State<McDBFT>>(new State<McDBFT>(false, "CommitSent"));
  //   auto viewChanging = std::shared_ptr<State<McDBFT>>(new
  //   State<McDBFT>(false, "ViewChanging"));
  auto blockSent =
      std::shared_ptr<State<McDBFT>>(new State<McDBFT>(true, "BlockSent"));

  // -------------------------
  // creating dBFT transitions
  // -------------------------

  auto machine = new SingleTimerStateMachine<McDBFT>(
      std::unique_ptr<Timer>(new Timer("C")));
  machine->me = MachineId(id);

  // initial -> backup
  started->addTransition(std::shared_ptr<Transition<McDBFT>>(
      (new Transition<McDBFT>(backup))
          ->add(Condition<McDBFT>(
              "not (H+v) mod R = i",
              [](const Timer& t, TMcDBFT d, MachineId me) -> bool {
                cout << "lambda1" << endl;
                return (d->vm[me.id].params->H + d->vm[me.id].params->v) %
                           d->vm[me.id].params->R !=
                       me.id;
              }))));

  // initial -> primary
  started->addTransition(std::shared_ptr<Transition<McDBFT>>(
      (new Transition<McDBFT>(primary))
          ->add(Condition<McDBFT>(
              "(H+v) mod R = i",
              [](const Timer& t, TMcDBFT d, MachineId me) -> bool {
                cout << "lambda2 H=" << d->vm[me.id].params->H
                     << " v=" << d->vm[me.id].params->v << " me=" << me.id
                     << endl;
                return (d->vm[me.id].params->H + d->vm[me.id].params->v) %
                           d->vm[me.id].params->R ==
                       me.id;
              }))));

  // backup -> reqSentOrRecv
  auto toReqSentOrRecv1 = new Transition<McDBFT>(reqSentOrRecv);
  backup->addTransition(std::shared_ptr<Transition<McDBFT>>(
      toReqSentOrRecv1->add(Condition<McDBFT>(
          "OnPrepareRequest",
          [](const Timer& t, TMcDBFT d, MachineId me) -> bool {
            cout << "waiting for event OnPrepareRequest at " << me.id << endl;
            return d->hasEvent("OnPrepareRequest", me.id,
                               vector<string>(0));  // no parameters on event
          }))));

  // reqSentOrRecv -> commitSent
  reqSentOrRecv->addTransition(std::shared_ptr<Transition<McDBFT>>(
      (new Transition<McDBFT>(commitSent))
          ->add(Condition<McDBFT>(
              "(H+v) mod R = i",
              [](const Timer& t, TMcDBFT d, MachineId me) -> bool {
                cout << "nothing to do... assuming all preparations were "
                        "received!"
                     << endl;
                return true;
              }))));

  // commitSent -> blockSent
  commitSent->addTransition(std::shared_ptr<Transition<McDBFT>>(
      (new Transition<McDBFT>(blockSent))
          ->add(Condition<McDBFT>(
              "(H+v) mod R = i",
              [](const Timer& t, TMcDBFT d, MachineId me) -> bool {
                cout << "nothing to do... assuming all commits were received!"
                     << endl;
                return true;
              }))));

  machine->registerState(started);
  machine->registerState(blockSent);

  return machine;
}

void dbft_test_real_dbft2_primary() {
  cout << "will create machine!" << endl;
  auto machine =
      std::shared_ptr<dBFT2Machine>(new dBFT2Machine(0, 1));  // f=0, N=1

  // ==============================
  // prepare two events for testing
  // ==============================
  // event scheduled to raise "OnStart" machine 0, after 1 seconds
  machine->scheduleEvent(1.0,  // 1 second to expire: after initialize()
                         MachineId(0),  // machine 0
                         "OnStart",
                         vector<string>(0));  // no parameters

  // event scheduled to raise "OnPrepareRequest" machine 0, after 3 seconds
  machine->scheduleEvent(3.0,  // 3 second to expire: after initialize()
                         MachineId(0),  // machine 0
                         "OnPrepareRequest",
                         vector<string>(1, "0"));  // view = 0
  // ==============================

  cout << "Machine => " << machine->toString() << endl;

  // v = 0, H = 1500, T = 3 (secs), R = 1 (one node network), f=0
  // 1500 -> primary (R=1)
  auto data = std::shared_ptr<dBFT2Context>(new dBFT2Context(0, 1501, 3, 2, 0));

  auto ctx = std::shared_ptr<McDBFT>();
  ctx->vm.emplace_back(data, machine->machines[0]);

  // run for 5.0 seconds max
  machine->setWatchdog(5.0);

  cout << "BEFORE RUN, WILL PRINT AS GRAPHVIZ!" << endl;

  string graphviz = machine->toStringFormat(StringFormat::Graphviz);

  cout << graphviz << endl;

  machine->run(nullptr, ctx);

  // show
  FILE* fgraph = fopen("fgraph.dot", "w");    // NOLINT
  fprintf(fgraph, "%s\n", graphviz.c_str());  // NOLINT
  fclose(fgraph);                             // NOLINT
  cout << "Generating image 'fgraph.png'" << endl;
  if (!system("dot -Tpng fgraph.dot -o fgraph.png")) {
    cout << "Problem generating graph" << endl;
  }
  // system("dot -Tpng fgraph.dot -o fgraph.png && eog fgraph.png");
}

void dbft_test_backup_multi() {
  auto machine0 = std::shared_ptr<SingleTimerStateMachine<McDBFT>>(
      simpleMultiMachineExample(0));

  // insert pre-initial state
  auto preinitial =
      std::shared_ptr<State<McDBFT>>(new State<McDBFT>(false, "PreInitial"));

  machine0->states.insert(machine0->states.begin() + 0, preinitial);

  cout << "Machine => " << machine0->toString() << endl;

  // v = 0, H = 1501, T = 3 (secs), R = 1 (one node network), f=0
  auto data = std::shared_ptr<dBFT2Context>(
      new dBFT2Context(0, 1501, 3, 2, 0));  // 1501 -> backup (R=2)

  auto ctx = std::shared_ptr<McDBFT>();
  ctx->vm.emplace_back(data, machine0);

  ReplicatedSTSM<dBFT2Context> multiMachine;
  multiMachine.registerMachine(machine0);

  // global transition scheduled to start machine 0 ("OnStart") after 1 second
  /*
  multiMachine.scheduleGlobalTransition(
    (new Timer())->init(1.0), // 1 second to expire
    0,                        // machine 0
    // no other conditions, always 'true'
    (new Transition<McDBFT>(machine0->getStateByName("Initial")))
      ->add(Action<McDBFT>(
        "C := 0 | v := 0",
        [](Timer& C, McDBFT* d, MachineId me) -> void {
           cout << " => action: C := 0" << endl;
           C.reset();
           cout << " => action: v := 0" << endl;
           d->vm[me.id].params->v = 0;
        })));
  */

  // transition for PreInitial to Started
  // preinitial -> started
  preinitial->addTransition(std::shared_ptr<Transition<McDBFT>>(
      (new Transition<McDBFT>(machine0->getStateByName("Started")))
          ->add(Condition<McDBFT>(
              "OnStart", [](const Timer& t, TMcDBFT d, MachineId me) -> bool {
                cout << "Waiting for OnStart..." << endl;
                return d->hasEvent("OnStart", me.id, vector<string>(0));
              }))));

  // event scheduled to raise "OnStart" machine 0, after 1 seconds
  multiMachine.scheduleEvent(1.0,  // 1 second to expire: after initialize()
                             MachineId(0),  // machine 0
                             "OnStart",
                             vector<string>(0));  // no parameters

  // event scheduled to raise "OnPrepareRequest" machine 0, after 3 seconds
  multiMachine.scheduleEvent(3.0,  // 3 second to expire: after initialize()
                             MachineId(0),  // machine 0
                             "OnPrepareRequest",
                             vector<string>(0));  // no parameters

  auto minitial = std::shared_ptr<MultiState<dBFT2Context>>(
      new MultiState<dBFT2Context>(1, nullptr));
  (*minitial)[0] = machine0->getStateByName("PreInitial");

  // run for 5.0 seconds max (watchdog limit)
  multiMachine.setWatchdog(5.0);
  multiMachine.run(minitial, ctx);
}

void dbft_test_primary() {
  auto machine0 = std::shared_ptr<SingleTimerStateMachine<McDBFT>>(
      simpleMultiMachineExample(0));

  cout << "Machine => " << machine0->toString() << endl;

  // v = 0, H = 1500, T = 3 (secs), R = 1 (one node network), f=0
  auto data = std::shared_ptr<dBFT2Context>(
      new dBFT2Context(0, 1500, 3, 1, 0));  // 1500 -> primary (R=1)

  auto ctx = std::shared_ptr<McDBFT>();
  ctx->vm.emplace_back(data, machine0);

  // run for 5.0 seconds max
  machine0->setWatchdog(5.0);
  machine0->run(machine0->states[0],
                ctx);  // explicitly passing first state as default
}

SingleTimerStateMachine<McDBFT>* commit_phase_dbft2(int id_me) {
  auto machine = new SingleTimerStateMachine<McDBFT>(
      std::unique_ptr<Timer>(new Timer("C")));
  machine->me = MachineId(id_me);

  // ---------------------
  // declaring dBFT states
  // ---------------------

  auto preinitial =
      std::shared_ptr<State<McDBFT>>(new State<McDBFT>(false, "PreInitial"));
  machine->registerState(preinitial);
  auto started =
      std::shared_ptr<State<McDBFT>>(new State<McDBFT>(false, "Started"));
  machine->registerState(started);
  auto backup =
      std::shared_ptr<State<McDBFT>>(new State<McDBFT>(false, "Backup"));
  machine->registerState(backup);
  auto primary =
      std::shared_ptr<State<McDBFT>>(new State<McDBFT>(false, "Primary"));
  machine->registerState(primary);
  auto reqSentOrRecv = std::shared_ptr<State<McDBFT>>(
      new State<McDBFT>(false, "RequestSentOrReceived"));
  machine->registerState(reqSentOrRecv);
  auto commitSent =
      std::shared_ptr<State<McDBFT>>(new State<McDBFT>(false, "CommitSent"));
  machine->registerState(commitSent);
  auto viewChanging =
      std::shared_ptr<State<McDBFT>>(new State<McDBFT>(false, "ViewChanging"));
  machine->registerState(viewChanging);
  auto blockSent =
      std::shared_ptr<State<McDBFT>>(new State<McDBFT>(true, "BlockSent"));
  machine->registerState(blockSent);

  // -------------------------
  // creating dBFT transitions
  // -------------------------

  // auto machine = this;
  // int id = this->me.id;

  // preinitial -> started
  preinitial->addTransition(std::shared_ptr<Transition<McDBFT>>(
      (new Transition<McDBFT>(started))
          ->add(Condition<McDBFT>(
              "OnStart()", [](const Timer& t, TMcDBFT d, MachineId me) -> bool {
                // cout << "Waiting for OnStart..." << endl;
                return d->hasEvent(
                    "OnStart", me.id,
                    vector<string>(0));  // no parameters on event
              }))));

  // initial -> backup
  started->addTransition(std::shared_ptr<Transition<McDBFT>>(
      (new Transition<McDBFT>(backup))
          ->add(Condition<McDBFT>(
              "not (H+v) mod R = i",
              [](const Timer& t, TMcDBFT d, MachineId me) -> bool {
                // cout << "lambda1" << endl;
                return (d->getParams(me.id)->H + d->getParams(me.id)->v) %
                           d->getParams(me.id)->R !=
                       me.id;
              }))));

  // initial -> primary
  started->addTransition(std::shared_ptr<Transition<McDBFT>>(
      (new Transition<McDBFT>(primary))
          ->add(Condition<McDBFT>(
              "(H+v) mod R = i",
              [](const Timer& t, TMcDBFT d, MachineId me) -> bool {
                // cout << "lambda2 H=" << d->getParams(me.id)->H << " v=" <<
                // d->getParams(me.id)->v << " me=" << me.id << endl;
                return (d->getParams(me.id)->H + d->getParams(me.id)->v) %
                           d->getParams(me.id)->R ==
                       me.id;
              }))));

  // backup -> reqSentOrRecv
  auto toReqSentOrRecv1 = new Transition<McDBFT>(reqSentOrRecv);
  backup->addTransition(std::shared_ptr<Transition<McDBFT>>(
      toReqSentOrRecv1
          ->add(Condition<McDBFT>(
              "OnPrepareRequest(v,H)",
              [](const Timer& t, TMcDBFT d, MachineId me) -> bool {
                cout << "waiting for event OnPrepareRequest at " << me.id
                     << " for view " << d->getParams(me.id)->v << endl;
                // args: view and height
                vector<string> evArgs = {
                    std::to_string(d->getParams(me.id)->v),
                    std::to_string(d->getParams(me.id)->H)};
                return d->hasEvent("PrepareRequest", me.id, evArgs);
              }))
          ->add(Condition<McDBFT>(
              "ValidBlock",
              [](const Timer& t, TMcDBFT d, MachineId me) -> bool {
                // always good block right now
                return true;
              }))
          ->add(Action<McDBFT>("send: PrepareResponse(v, H)",
                               [](Timer& C, TMcDBFT d, MachineId me) -> void {
                                 cout << "sending PrepareResponse from "
                                      << me.id << " for view "
                                      << d->getParams(me.id)->v << endl;
                                 // TODO: attach  block hash as well?
                                 vector<string> evArgs = {
                                     std::to_string(d->getParams(me.id)->v),
                                     std::to_string(d->getParams(me.id)->H)};
                                 d->broadcast("PrepareResponse", me, evArgs);
                               }))));

  // primary -> reqSentOrRecv
  auto primToReqSentOrRecv1 = new Transition<McDBFT>(reqSentOrRecv);
  primary->addTransition(std::shared_ptr<Transition<McDBFT>>(
      primToReqSentOrRecv1
          ->add(Condition<McDBFT>(
              "C >= T?",
              [](const Timer& C, TMcDBFT d, MachineId me) -> bool {
                // C >= T?
                return C.elapsedTime() >= d->getParams(me.id)->T;
              }))
          ->add(Action<McDBFT>("send: PrepareRequest(v, H)",
                               [](Timer& C, TMcDBFT d, MachineId me) -> void {
                                 cout << "sending PrepareRequest from " << me.id
                                      << " for view " << d->getParams(me.id)->v
                                      << endl;
                                 // TODO: attach  block hash as well?
                                 vector<string> evArgs = {
                                     std::to_string(d->getParams(me.id)->v),
                                     std::to_string(d->getParams(me.id)->H)};
                                 d->broadcast("PrepareRequest", me, evArgs);
                               }))
          ->add(Action<McDBFT>(
              "C := 0",
              [](Timer& C, TMcDBFT d, MachineId me) -> void { C.reset(); }))));

  // primary -> viewChanging
  auto primToViewChanging = new Transition<McDBFT>(viewChanging);
  primary->addTransition(std::shared_ptr<Transition<McDBFT>>(
      primToViewChanging
          ->add(Condition<McDBFT>(
              "(C >= T exp(v+1))?",
              [](const Timer& C, TMcDBFT d, MachineId me) -> bool {
                // C >= T?
                // cout << "evaluate primary view change... C = " <<
                // C.elapsedTime() << " ~ T_limit = " << (d->params->T *
                // std::pow(2, d->params->v+1)) << std::endl;
                return C.elapsedTime() >=
                       (d->getParams(me.id)->T *
                        std::pow(2, d->getParams(me.id)->v + 1));
              }))
          ->add(Action<McDBFT>("send: ChangeView(v+1, H)",
                               [](Timer& C, TMcDBFT d, MachineId me) -> void {
                                 cout << "sending ChangeView from " << me.id
                                      << " for view " << d->getParams(me.id)->v
                                      << endl;
                                 // TODO: attach  block hash as well?
                                 vector<string> evArgs = {
                                     std::to_string(d->getParams(me.id)->v + 1),
                                     std::to_string(d->getParams(me.id)->H)};
                                 d->broadcast("ChangeView", me, evArgs);
                               }))
          ->add(Action<McDBFT>(
              "C := 0",
              [](Timer& C, TMcDBFT d, MachineId me) -> void { C.reset(); }))));

  // backup -> viewChanging
  auto backpToViewChanging = new Transition<McDBFT>(viewChanging);
  backup->addTransition(std::shared_ptr<Transition<McDBFT>>(
      backpToViewChanging
          ->add(Condition<McDBFT>(
              "(C >= T exp(v+1))?",
              [](const Timer& C, TMcDBFT d, MachineId me) -> bool {
                // C >= T?
                // cout << "evaluate primary view change... C = " <<
                // C.elapsedTime() << " ~ T_limit = " << (d->params->T *
                // std::pow(2, d->params->v+1)) << std::endl;
                return C.elapsedTime() >=
                       (d->getParams(me.id)->T *
                        std::pow(2, d->getParams(me.id)->v + 1));
              }))
          ->add(Action<McDBFT>("send: ChangeView(v+1, H)",
                               [](Timer& C, TMcDBFT d, MachineId me) -> void {
                                 cout << "sending ChangeView from " << me.id
                                      << " for view " << d->getParams(me.id)->v
                                      << endl;
                                 // TODO: attach  block hash as well?
                                 vector<string> evArgs = {
                                     std::to_string(d->getParams(me.id)->v + 1),
                                     std::to_string(d->getParams(me.id)->H)};
                                 d->broadcast("ChangeView", me, evArgs);
                               }))
          ->add(Action<McDBFT>(
              "C := 0",
              [](Timer& C, TMcDBFT d, MachineId me) -> void { C.reset(); }))));

  // reqSentOrRecv -> viewChanging
  auto reqsrToViewChanging = new Transition<McDBFT>(viewChanging);
  reqSentOrRecv->addTransition(std::shared_ptr<Transition<McDBFT>>(
      reqsrToViewChanging
          ->add(Condition<McDBFT>(
              "(C >= T exp(v+1))?",
              [](const Timer& C, TMcDBFT d, MachineId me) -> bool {
                // C >= T?
                // cout << "evaluate primary view change... C = " <<
                // C.elapsedTime() << " ~ T_limit = " << (d->params->T *
                // std::pow(2, d->params->v+1)) << std::endl;
                return C.elapsedTime() >=
                       (d->getParams(me.id)->T *
                        std::pow(2, d->getParams(me.id)->v + 1));
              }))
          ->add(Action<McDBFT>("send: ChangeView(v+1, H)",
                               [](Timer& C, TMcDBFT d, MachineId me) -> void {
                                 cout << "sending ChangeView from " << me.id
                                      << " for view " << d->getParams(me.id)->v
                                      << endl;
                                 // TODO: attach  block hash as well?
                                 vector<string> evArgs = {
                                     std::to_string(d->getParams(me.id)->v + 1),
                                     std::to_string(d->getParams(me.id)->H)};
                                 d->broadcast("ChangeView", me, evArgs);
                               }))
          ->add(Action<McDBFT>(
              "C := 0",
              [](Timer& C, TMcDBFT d, MachineId me) -> void { C.reset(); }))));

  // reqSentOrRecv -> commitSent
  auto reqsrToCommitSent = new Transition<McDBFT>(commitSent);
  reqSentOrRecv->addTransition(std::shared_ptr<Transition<McDBFT>>(
      reqsrToCommitSent
          ->add(Condition<McDBFT>(
              "EnoughPreparations",
              [](const Timer& C, TMcDBFT d, MachineId me) -> bool {
                vector<std::shared_ptr<Event>> events = d->getEvents(me.id);
                cout << "waiting for 2f+1 PrepareResponse" << endl;
                // waiting for 2f+1 PrepareResponse(v,H)
                vector<string> evArgs = {
                    std::to_string(d->getParams(me.id)->v),
                    std::to_string(d->getParams(me.id)->H)};
                int countPrepResp = 0;
                for (int id = 0; id < d->getParams(me.id)->R; id++) {
                  for (auto& event : events) {
                    if (event->getFrom().id == id)
                      if (event->isActivated("PrepareResponse", evArgs))
                        countPrepResp++;
                  }
                }
                cout << "count PrepareResponse = " << countPrepResp << " / "
                     << (d->getParams(me.id)->M() - 1) << endl;
                // count >= 2f+1 (or M) -1 (because Prepare Request also counts)
                return countPrepResp >= (d->getParams(me.id)->M() - 1);
              }))
          ->add(Action<McDBFT>("send: Commit(v, H)",
                               [](Timer& C, TMcDBFT d, MachineId me) -> void {
                                 cout << "sending Commit from " << me.id
                                      << " for view " << d->getParams(me.id)->v
                                      << endl;
                                 // TODO: attach  block hash as well?
                                 vector<string> evArgs = {
                                     std::to_string(d->getParams(me.id)->v),
                                     std::to_string(d->getParams(me.id)->H)};
                                 d->broadcast("Commit", me, evArgs);
                               }))
          ->add(Action<McDBFT>(
              "C := 0",
              [](Timer& C, TMcDBFT d, MachineId me) -> void { C.reset(); }))));

  // commitSent -> blockSent
  auto commitSentToBlockSent = new Transition<McDBFT>(blockSent);
  commitSent->addTransition(std::shared_ptr<Transition<McDBFT>>(
      commitSentToBlockSent
          ->add(Condition<McDBFT>(
              "EnoughCommits",
              [](const Timer& C, TMcDBFT d, MachineId me) -> bool {
                vector<std::shared_ptr<Event>> events = d->getEvents(me.id);
                cout << "waiting for 2f+1 Commits" << endl;
                // waiting for 2f+1 Commit(v,H)
                vector<string> evArgs = {
                    std::to_string(d->getParams(me.id)->v),
                    std::to_string(d->getParams(me.id)->H)};
                int count = 0;
                for (int id = 0; id < d->getParams(me.id)->R; id++) {
                  for (unsigned e = 0; e < events.size(); e++)
                    if (events[e]->getFrom().id == id)
                      if (events[e]->isActivated("Commit", evArgs)) count++;
                }
                cout << "count Commit = " << count << " / "
                     << (d->getParams(me.id)->M() - 1) << endl;
                // count >= 2f+1 (or M)
                return count >= (d->getParams(me.id)->M() - 1);
              }))
          ->add(Action<McDBFT>("send: BlockRelay(v, H)",
                               [](Timer& C, TMcDBFT d, MachineId me) -> void {
                                 cout << "sending BlockRelay for H="
                                      << d->getParams(me.id)->H << endl;
                                 // TODO: attach  block hash as well?
                                 vector<string> evArgs = {
                                     std::to_string(d->getParams(me.id)->v),
                                     std::to_string(d->getParams(me.id)->H)};
                                 d->broadcast("BlockRelay", me, evArgs);
                               }))
          ->add(Action<McDBFT>(
              "C := 0",
              [](Timer& C, TMcDBFT d, MachineId me) -> void { C.reset(); }))));

  // viewChanging -> started
  auto viewChToStarted = new Transition<McDBFT>(started);
  viewChanging->addTransition(std::shared_ptr<Transition<McDBFT>>(
      viewChToStarted
          ->add(Condition<McDBFT>(
              "EnoughViewChanges",
              [](const Timer& C, TMcDBFT d, MachineId me) -> bool {
                vector<std::shared_ptr<Event>> events = d->getEvents(me.id);
                cout << "waiting for 2f+1 View Changes" << endl;
                // waiting for 2f+1 ChangeView(v+1,H)
                vector<string> evArgs = {
                    std::to_string(d->getParams(me.id)->v + 1),
                    std::to_string(d->getParams(me.id)->H)};
                int count = 0;
                for (int id = 0; id < d->getParams(me.id)->R; id++) {
                  for (auto& event : events) {
                    if (event->getFrom().id == id)
                      if (event->isActivated("ChangeView", evArgs)) count++;
                  }
                }
                cout << "count ChangeView = " << count << " / "
                     << d->getParams(me.id)->M() << endl;
                // count >= 2f+1 (or M)
                return count >= d->getParams(me.id)->M();
              }))
          ->add(Action<McDBFT>("v := v + 1",
                               [](Timer& C, TMcDBFT d, MachineId me) -> void {
                                 d->getParams(me.id)->v =
                                     d->getParams(me.id)->v + 1;
                                 cout << " ***** MOVING TO NEXT VIEW: "
                                      << d->getParams(me.id)->v << endl;
                               }))
          ->add(Action<McDBFT>(
              "C := 0",
              [](Timer& C, TMcDBFT d, MachineId me) -> void { C.reset(); }))));

  // cout << "Machine => " << machine->toString() << endl;
  string graphviz = machine->toStringFormat(StringFormat::Graphviz);

  FILE* fgraph = fopen("fgraph.dot", "w");    // NOLINT
  fprintf(fgraph, "%s\n", graphviz.c_str());  // NOLINT
  fclose(fgraph);                             // NOLINT
  cout << "Generating image 'fgraph.png'" << endl;
  if (!system("dot -Tpng fgraph.dot -o fgraph.png")) {
    cout << "Problem generating graph" << endl;
  }

  return machine;
}

int main() {
  // v = 0, H = 1500, T = 3 (secs), R = 1 (one node network), f=0 // 1500 ->
  // primary (R=1)
  auto machine0 =
      std::shared_ptr<SingleTimerStateMachine<McDBFT>>(commit_phase_dbft2(0));

  auto ctxData0 =
      std::shared_ptr<dBFT2Context>(new dBFT2Context(0, 1500, 3, 1, 0));
  auto ctx0 = std::shared_ptr<McDBFT>();
  ctx0->vm.emplace_back(ctxData0, machine0);

  // run for 5.0 seconds max
  // machine0->setWatchdog(5.0);

  ReplicatedSTSM<dBFT2Context> machine;
  machine.registerMachine(machine0);

  machine.scheduleEvent(1.0,           // 1 second to expire: after initialize()
                        MachineId(0),  // machine 0
                        "OnStart",
                        vector<string>(0));  // no parameters

  auto minitial = std::shared_ptr<MultiState<dBFT2Context>>(
      new MultiState<dBFT2Context>(1, nullptr));
  (*minitial)[0] = machine0->getStateByName("PreInitial");

  // run for 5.0 seconds max (watchdog limit)
  machine.setWatchdog(5.0);
  //
  machine.run(minitial, ctx0);
  // machine0->run(machine0->states[0], &ctx0); // explicitly passing first
  // state as default

  return 0;
}

/*
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

   //std::thread t([](bool b){return true;});
   //t.join();

   cout << "finished successfully!" << endl;

   return 0;
}
*/
