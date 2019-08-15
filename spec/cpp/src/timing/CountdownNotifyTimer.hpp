#pragma once
#ifndef LIBBFT_SRC_CPP_COUNTDOWN_NOTIFY_TIMER_HPP
#define LIBBFT_SRC_CPP_COUNTDOWN_NOTIFY_TIMER_HPP

// system includes
#include <sstream>

// libbft includes
#include "Clock.hpp"
#include "CountdownTimer.hpp"

// standard Timer for a TSM

using namespace std; // TODO: remove

namespace libbft {

// this CountdownTimer should "notify" its "condition_variable" when expired
// this can be used for more ellaborate interconnection between running processes, events and timers

class CountdownNotifyTimer : public CountdownTimer
{
public:
   CountdownNotifyTimer(double _countdown, string _name = "", Clock* _clock = nullptr)
     : CountdownTimer(_countdown, _name, _clock)
   {
      init(_countdown);
   }

   virtual ~CountdownNotifyTimer()
   {
   }

public:
   virtual void init(double _countdown) override
   {
      this->countdown = _countdown;
      // TODO: should launch an actual system timer here (thread-based)
   }

   void reset() override
   {
      // TODO: should stop current running CountdownTimer, and relaunch it
   }

   bool expired() const override
   {
      // TODO: should evaluate if current system timer has expired.
      // when expired, it should automatically trigger "condition_variable",
      // and "notify_all" dependent threads
      return false;
   }

   string toString() const override
   {
      stringstream ss;
      ss << "CountdownNotifyTimer {name='" << name << "'}";
      return ss.str();
   }
};

} // libbft

#endif // LIBBFT_SRC_CPP_COUNTDOWN_NOTIFY_TIMER_HPP