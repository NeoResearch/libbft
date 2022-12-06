// SPDX-License-Identifier:  MIT
// Copyright (C) 2019-2022 - LibBFT developers
// https://github.com/neoresearch/libbft

#ifndef INCLUDE_LIBBFT_DBFT2_DBFT2CONTEXT_HPP_
#define INCLUDE_LIBBFT_DBFT2_DBFT2CONTEXT_HPP_

// system includes
// ...

namespace libbft {

struct dBFT2Context {
  /** view number */
  int v;
  /** blockchain height */
  int H;
  /** block time (in seconds) */
  int T;
  /** number of nodes/replicas */
  int R;
  /** number of allowed faulty nodes */
  int f;
  /** M limit: 2f+1 */
  int M() const { return 2 * f + 1; }

  // add extra ConsensusContext information here

  dBFT2Context(int _v, int _h, int _t, int _r, int _f)
      : v(_v), H(_h), T(_t), R(_r), f(_f) {}
};

}  // namespace libbft

#endif  // INCLUDE_LIBBFT_DBFT2_DBFT2CONTEXT_HPP_
