#pragma once
#ifndef LIBBFT_SRC_CPP_DBFT2_DBFT2CONTEXT_HPP
#define LIBBFT_SRC_CPP_DBFT2_DBFT2CONTEXT_HPP

// system includes
// ...

namespace libbft {

struct dBFT2Context
{
   // view number
   int v;
   // blockchain height
   int H;
   // block time (in seconds)
   int T;
   // number of nodes/replicas
   int R;
   // number of allowed faulty nodes
   int f;
   // M limit: 2f+1
   int M() const
   {
      return 2*f + 1;
   }

   // add extra ConsensusContext information here

   dBFT2Context(int _v, int _H, int _T, int _R, int _f)
     : v(_v)
     , H(_H)
     , T(_T)
     , R(_R)
     , f(_f)
   {
   }
};

} // libbft

#endif // LIBBFT_SRC_CPP_DBFT2_DBFT2CONTEXT_HPP
