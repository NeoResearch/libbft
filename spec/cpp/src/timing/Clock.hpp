#pragma once
#ifndef LIBBFT_SRC_CPP_CLOCK_HPP
#define LIBBFT_SRC_CPP_CLOCK_HPP

// system includes
#include <sstream>

// standard Timer for a TSM

using namespace std; // TODO: remove

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
      // terrible, only seconds! should be micro at least...
      return time(NULL);
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