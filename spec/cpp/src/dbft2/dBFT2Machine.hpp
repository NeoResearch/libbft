#pragma once
#ifndef LIBBFT_SRC_CPP_DBFT2_DBFT2MACHINE_HPP
#define LIBBFT_SRC_CPP_DBFT2_DBFT2MACHINE_HPP

// system includes
#include <iostream> // TODO: remove
#include <sstream>
#include <vector>
// simulate non-deterministic nature
#include <algorithm>
#include <random>

// standard Transition
#include "../replicated/ReplicatedSTSM.hpp"
#include "../single/Transition.hpp"
#include "../timing/Timer.hpp"

#include "dBFT2Context.hpp"

using namespace std; // TODO: remove

namespace libbft {

/*class dBFT2Machine : public ReplicatedSTSM<>
{
   // view number
   int v;
   // blockchain height
   int H;
   // block time (in seconds)
   int T;
   // number of nodes (TODO: better solution for this?)
   int R;

   // cached events

   // add ConsensusContext information here

   dBFT2Context(int _v, int _H, int _T, int _R)
     : v(_v)
     , H(_H)
     , T(_T)
     , R(_R)
   {
   }
};
*/

SingleTimerStateMachine<MultiContext<dBFT2Context>>*
create_dBFTMachine(int id)
{
   // ---------------------
   // declaring dBFT states
   // ---------------------

   auto initial = new State<MultiContext<dBFT2Context>>(false, "Initial");
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
   initial->addTransition(
     (new Transition<MultiContext<dBFT2Context>>(backup))->add(Condition<MultiContext<dBFT2Context>>("not (H+v) mod R = i", [](const Timer& t, MultiContext<dBFT2Context>* d, int me) -> bool {
        cout << "lambda1" << endl;
        return !((d->vm[me].params->H + d->vm[me].params->v) % d->vm[me].params->R == me);
     })));

   // initial -> primary
   initial->addTransition(
     (new Transition<MultiContext<dBFT2Context>>(primary))->add(Condition<MultiContext<dBFT2Context>>("(H+v) mod R = i", [](const Timer& t, MultiContext<dBFT2Context>* d, int me) -> bool {
        cout << "lambda2 H=" << d->vm[me].params->H << " v=" << d->vm[me].params->v << " me=" << me << endl;
        return (d->vm[me].params->H + d->vm[me].params->v) % d->vm[me].params->R == me;
     })));

   // backup -> reqSentOrRecv
   auto toReqSentOrRecv1 = new Transition<MultiContext<dBFT2Context>>(reqSentOrRecv);
   backup->addTransition(
     toReqSentOrRecv1->add(Condition<MultiContext<dBFT2Context>>("OnPrepareRequest", [](const Timer& t, MultiContext<dBFT2Context>* d, int me) -> bool {
        cout << "waiting for event OnPrepareRequest at " << me << endl;
        return d->hasEvent("OnPrepareRequest", me);
     })));

   // reqSentOrRecv -> commitSent
   reqSentOrRecv->addTransition(
     (new Transition<MultiContext<dBFT2Context>>(commitSent))->add(Condition<MultiContext<dBFT2Context>>("(H+v) mod R = i", [](const Timer& t, MultiContext<dBFT2Context>* d, int me) -> bool {
        cout << "nothing to do... assuming all preparations were received!" << endl;
        return true;
     })));

   // commitSent -> blockSent
   commitSent->addTransition(
     (new Transition<MultiContext<dBFT2Context>>(blockSent))->add(Condition<MultiContext<dBFT2Context>>("(H+v) mod R = i", [](const Timer& t, MultiContext<dBFT2Context>* d, int me) -> bool {
        cout << "nothing to do... assuming all commits were received!" << endl;
        return true;
     })));

   machine->registerState(initial);
   machine->registerState(blockSent);

   return machine;
}

} // libbft

#endif // LIBBFT_SRC_CPP_DBFT2_DBFT2MACHINE_HPP