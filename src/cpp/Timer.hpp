#ifndef LIBBFT_SRC_CPP_TIMER_HPP
#define LIBBFT_SRC_CPP_TIMER_HPP

// system includes 
#include<sstream>

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

public:
   Timer(string _name = "")
     : name(_name)
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

   // time in seconds
   double elapsedTime() const
   {
      // terrible, only seconds! should be micro and asynchronous...
      long newtime = time(NULL);
      return newtime - mytime;
   }

   string toString() const
   {
      stringstream ss;
      ss << "Timer {name='" << name <<"'}";
      return ss.str();
   }
};

} // libbft

#endif // LIBBFT_SRC_CPP_TIMER_HPP