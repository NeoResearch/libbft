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
   // get time in seconds (milliseconds precision here.. could be more, perhaps)
   virtual double getTime()
   {
      // using chrono
      system_clock::time_point now = system_clock::now();
      auto now_ms = std::chrono::time_point_cast<std::chrono::milliseconds>(now);
      auto epoch = now_ms.time_since_epoch(); // which epoch? unix?
      auto value = std::chrono::duration_cast<std::chrono::milliseconds>(epoch);
      long duration = value.count();
      return static_cast<double>(duration) / 1000.0;
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
