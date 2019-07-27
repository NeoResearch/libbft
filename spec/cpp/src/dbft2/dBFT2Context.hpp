#pragma once
#ifndef LIBBFT_SRC_CPP_DBFT2_DBFT2CONTEXT_HPP
#define LIBBFT_SRC_CPP_DBFT2_DBFT2CONTEXT_HPP

// system includes
// ...

using namespace std; // TODO: remove

namespace libbft {

struct dBFT2Context
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

} // libbft

#endif // LIBBFT_SRC_CPP_DBFT2_DBFT2CONTEXT_HPP