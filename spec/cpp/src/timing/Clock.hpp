#pragma once
#ifndef LIBBFT_SRC_CPP_CLOCK_HPP
#define LIBBFT_SRC_CPP_CLOCK_HPP

// system includes
#include <chrono>
#include <sstream>

// standard Timer for a TSM

using namespace std;         // TODO: remove
using namespace std::chrono; // TODO: remove

namespace libbft {

// this clock is supposed to be a very precise one
// it can also be used on tests as a drifted clock (using inheritance)

class Clock
{
private:
   string name;

public:
   Clock(string _name = "")
     : name(_name)
   {
   }

public:
   // get time in seconds
   virtual double getTime()
   {
      // using chrono
      system_clock::time_point now = system_clock::now();
      system_clock::duration tp = now.time_since_epoch(); // which epoch.. unix? // TODO: check

      tp -= duration_cast<seconds>(tp);

      auto f_secs = std::chrono::duration_cast<std::chrono::duration<float>>(tp);
      // todo: fix this clock to return seconds in double/float format
      return f_secs.count();
   }

   string toString() const
   {
      stringstream ss;
      ss << "Clock {name='" << name << "'}";
      return ss.str();
   }
};

} // libbft

#endif // LIBBFT_SRC_CPP_CLOCK_HPP