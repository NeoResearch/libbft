#pragma once
#ifndef LIBBFT_SRC_CPP_TIMER_HPP
#define LIBBFT_SRC_CPP_TIMER_HPP

// system includes
#include <memory>
#include <sstream>

// libbft includes
#include "Clock.hpp"

// standard Timer for a TSM

namespace libbft {

class Timer
{
private:
   /** object name */
   std::string name;
   /** beware if clock precision is terrible */
   std::shared_ptr<Clock> clock;
   /** nice precision timer */
   double mytime;
   /** countdown timer (if value is positive) - in seconds */
   double countdown{ -1.0 };

public:
   explicit Timer(std::string _name = "", std::shared_ptr<Clock> _clock = nullptr)
     : name(std::move(_name))
     , clock(_clock)
   {
      init();
   }

   /**
    * countdown in seconds
    * @param _countdown
    * @return
    */
   Timer* init(double _countdown = -1.0)
   {
      // update countdown
      countdown = _countdown;
      if (!clock) {
         // beware if it's a terrible clock
         clock = std::shared_ptr<Clock>(new Clock());
      }
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

   /**
    * time in seconds (for counting-up)
    * @return
    */
   double elapsedTime() const
   {
      // this should be a precision time
      double newtime = clock->getTime();
      return newtime - mytime;
   }

   /**
    * time in seconds (for counting-down)
    * @return
    */
   double remainingTime() const
   {
      if (countdown >= 0.0)
         return std::max(0.0, countdown - elapsedTime());
      else
         return 1000000000.0; // INF
   }

   /**
    * When returning 0.0, time is over
    * @return
    */
   bool expired() const
   {
      return remainingTime() == 0.0;
   }

   std::string toString() const
   {
      std::stringstream ss;
      ss << "Timer {name='" << name << "'}";
      return ss.str();
   }
};

} // libbft

#endif // LIBBFT_SRC_CPP_TIMER_HPP
