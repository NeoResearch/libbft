#ifndef LIBBFT_SRC_CPP_TIMER_HPP
#define LIBBFT_SRC_CPP_TIMER_HPP

// standard Timer for a TSM

namespace libbft {

class Timer
{
public:
private:
   // terrible timer (TODO: improve!)
   long mytime;

public:
   Timer()
   {
      start();
   }

   void start()
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

   // time in seconds
   double elapsedTime()
   {
      // terrible, only seconds! should be micro and asynchronous...
      long newtime = time(NULL);
      return newtime - mytime;
   }
};

} // libbft

#endif // LIBBFT_SRC_CPP_TIMER_HPP