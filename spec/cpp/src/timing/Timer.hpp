#pragma once
#ifndef LIBBFT_SRC_CPP_TIMER_HPP
#define LIBBFT_SRC_CPP_TIMER_HPP

// system includes
#include <sstream>

// libbft includes
#include "Clock.hpp"

// standard Timer for a TSM

using namespace std; // TODO: remove

namespace libbft {

class Timer
{
private:
   // object name
   string name;
   // beware if clock precision is terrible
   Clock* clock;
   // nice precision timer
   double mytime;
   // countdown timer (if value is positive) - in seconds
   double countdown{ -1.0 };

public:
   Timer(string _name = "", Clock* _clock = nullptr)
     : name(_name)
     , clock(_clock)
   {
      init();
   }

   // countdown in seconds
   Timer* init(double _countdown = -1.0)
   {
      // update countdown
      countdown = _countdown;
      if (!clock)
         clock = new Clock(); // beware if it's a terrible clock
      // this should be a precision time
      mytime = clock->getTime();
      return this; // allow chaining effect
   }

   double getCountdown() const
   {
      return countdown;
   }

public:
   void reset()
   {
      // this should be a precision time
      mytime = clock->getTime();
   }

   // time in seconds (for counting-up)
   double elapsedTime() const
   {
      // this should be a precision time
      double newtime = clock->getTime();
      return newtime - mytime;
   }

   // time in seconds (for counting-down)
   double remainingTime() const
   {
      if (countdown >= 0.0)
         return max(0.0, countdown - elapsedTime());
      else
         return 1000000000.0; // INF
   }

   // when returning 0.0, time is over
   bool expired() const
   {
      return remainingTime() == 0.0;
   }

   string toString() const
   {
      stringstream ss;
      ss << "Timer {name='" << name << "'}";
      return ss.str();
   }
};

} // libbft

#endif // LIBBFT_SRC_CPP_TIMER_HPP