#pragma once
#ifndef LIBBFT_SRC_CPP_EVENT_HPP
#define LIBBFT_SRC_CPP_EVENT_HPP

// system includes
#include <iostream> // TODO: remove
#include <sstream>
#include <vector>
// simulate non-deterministic nature
#include <algorithm>
#include <random>

// standard Transition
#include "../timing/Timer.hpp"
//#include "State.h"

using namespace std; // TODO: remove

namespace libbft {

// this Event class is mostly used for simulation
// it indicates a type for event (for matching), a name for printing, a source 'from', and possibly a countdown Timer

template<class Param = nullptr_t>
class Event
{
private:
   // event type (used to matching)
   std::string type;
   // event name (optional)
   std::string name;
   // event called from machine 'from'. If -1, it came from a broadcast (or machine itself)
   int from;
   // Timer sent in countdown mode
   Timer* timer;

public:
   Event(string _type, string _name = "", int _from = -1, Timer* _timer = nullptr)
     : type(_type)
     , name(_name)
     , from(_from)
     , timer(_timer)
   {
   }

   virtual bool isActivated(string _type) const
   {
      return (type == _type) && ((timer == nullptr) || (timer->expired()));
   }

   virtual string getType() const
   {
      return type;
   }

   virtual string getName() const
   {
      return name;
   }

   virtual int getFrom() const
   {
      return from;
   }

   virtual string toString() const
   {
      stringstream ss;
      ss << "Event {name=" << name;
      ss << "; type=" << type;
      if (timer)
         ss << "; timer={countdown:" << timer->getCountdown() << "}";
      ss << "}";
      return ss.str();
   }
};

} // libbft

#endif // LIBBFT_SRC_CPP_EVENT_HPP