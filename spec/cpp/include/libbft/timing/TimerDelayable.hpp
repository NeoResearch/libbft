// SPDX-License-Identifier:  MIT
// Copyright (C) 2019-2022 - LibBFT developers
// https://github.com/neoresearch/libbft

#ifndef INCLUDE_LIBBFT_TIMING_TIMERDELAYABLE_HPP_
#define INCLUDE_LIBBFT_TIMING_TIMERDELAYABLE_HPP_

// standard Timer for a TSM
#include <libbft/timing/Timer.hpp>

// This Timer allows for non-negative delays

namespace libbft {

class TimerDelayable : public Timer {};

}  // namespace libbft

#endif  // INCLUDE_LIBBFT_TIMING_TIMERDELAYABLE_HPP_
