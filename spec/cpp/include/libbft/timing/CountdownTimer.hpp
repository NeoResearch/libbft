// SPDX-License-Identifier:  MIT
// Copyright (C) 2019-2022 - LibBFT developers
// https://github.com/neoresearch/libbft

#ifndef INCLUDE_LIBBFT_TIMING_COUNTDOWNTIMER_HPP_
#define INCLUDE_LIBBFT_TIMING_COUNTDOWNTIMER_HPP_

// system includes
#include <algorithm>
#include <memory>
#include <sstream>
#include <string>
#include <utility>

// libbft includes
#include <libbft/timing/Clock.hpp>

// standard Timer for a TSM

namespace libbft {

class CountdownTimer {
 protected:
  /** beware if clock precision is terrible */
  TClock clock;
  /** nice precision timer */
  double mytime;
  /** object name */
  std::string name;
  /** countdown timer (if value is positive) */
  double countdown;

 public:
  explicit CountdownTimer(double _countdown, std::string _name = "",
                          TClock _clock = nullptr)
      : clock{std::move(_clock)},
        mytime{0.0},
        name{std::move(_name)},
        countdown{0.0} {
    init(_countdown);
  }

  virtual ~CountdownTimer() = default;

 public:
  virtual void init(double _countdown) {
    // update countdown
    countdown = _countdown;
    if (!clock) {
      // beware if it's a terrible clock
      clock = std::unique_ptr<Clock>(new Clock());
    }
    // this should be a precision time
    mytime = clock->getTime();
  }

  double getCountdown() const { return countdown; }

  virtual void reset() {
    // this should be a precision time
    mytime = clock->getTime();
  }

  /**
   * when returning 0.0, time is over
   * @return
   */
  virtual bool expired() const {
    // elapsed time
    double newtime = clock->getTime();
    double elapsed = newtime - mytime;
    double remaining = 1000000000.0;  // INF
    if (countdown >= 0.0) {
      remaining = std::max(0.0, countdown - elapsed);
    }

    return remaining == 0.0;
  }

  virtual std::string toString() const {
    std::stringstream ss;
    ss << "CountdownTimer {name='" << name << "'}";
    return ss.str();
  }
};

}  // namespace libbft

#endif  // INCLUDE_LIBBFT_TIMING_COUNTDOWNTIMER_HPP_
