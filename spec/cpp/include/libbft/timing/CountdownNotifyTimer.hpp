// SPDX-License-Identifier:  MIT
// Copyright (C) 2019-2022 - LibBFT developers
// https://github.com/neoresearch/libbft

#ifndef INCLUDE_LIBBFT_TIMING_COUNTDOWNNOTIFYTIMER_HPP_
#define INCLUDE_LIBBFT_TIMING_COUNTDOWNNOTIFYTIMER_HPP_

// system includes
#include <memory>
#include <sstream>
#include <string>
#include <utility>

// libbft includes
#include <libbft/timing/Clock.hpp>
#include <libbft/timing/CountdownTimer.hpp>

// standard Timer for a TSM

namespace libbft {

/**
 * this CountdownTimer should "notify" its "condition_variable" when expired
 * this can be used for more ellaborate interconnection between running
 * processes, events and timers
 */
class CountdownNotifyTimer : public CountdownTimer {
 public:
  explicit CountdownNotifyTimer(double _countdown, std::string _name = "",
                                TClock _clock = nullptr)
      : CountdownTimer(_countdown, std::move(_name), std::move(_clock)) {
    init(_countdown);
  }

  ~CountdownNotifyTimer() override = default;

 public:
  void init(double _countdown) override {
    this->countdown = _countdown;
    // TODO: should launch an actual system timer here (thread-based)
  }

  void reset() override {
    // TODO: should stop current running CountdownTimer, and relaunch it
  }

  bool expired() const override {
    // TODO: should evaluate if current system timer has expired.
    // when expired, it should automatically trigger "condition_variable",
    // and "notify_all" dependent threads
    return false;
  }

  std::string toString() const override {
    std::stringstream ss;
    ss << "CountdownNotifyTimer {name='" << name << "'}";
    return ss.str();
  }
};

}  // namespace libbft

#endif  // INCLUDE_LIBBFT_TIMING_COUNTDOWNNOTIFYTIMER_HPP_
