#ifndef LIBBFT_SRC_CPP_TIMER_HPP
#define LIBBFT_SRC_CPP_TIMER_HPP

// system includes
#include <sstream>

// standard Timer for a TSM

using namespace std; // TODO: remove

namespace libbft {

class Timer
{
public:
private:
   // terrible timer (TODO: improve!)
   long mytime;
   string name;

   double countdown{ -1.0 };

public:
   Timer(double _countdown = -1.0, string _name = "")
     : name(_name)
     , countdown(_countdown)
   {
      initialize();
   }

   void initialize()
   {
      // terrible, only seconds! should be micro and asynchronous...
      mytime = time(NULL);
   }

public:
   void reset()
   {
      // terrible, only seconds! should be micro and asynchronous...
      mytime = time(NULL);
   }

   // time in seconds (for counting-up)
   double elapsedTime() const
   {
      // terrible, only seconds! should be micro and asynchronous...
      long newtime = time(NULL);
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