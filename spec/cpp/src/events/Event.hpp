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
protected:
   // event type (used to matching)
   std::string type;
   // event name (optional)
   std::string name;
   // event called from machine 'from'. If -1, it came from a broadcast (or machine itself)
   int from;

public:
   Event(string _type, string _name = "", int _from = -1)
     : type(_type)
     , name(_name)
     , from(_from)
   {
   }

   virtual ~Event()
   {
   }

   virtual bool isActivated(string _type) const
   {
      return (type == _type);
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
      ss << "}";
      return ss.str();
   }
};

template<class Param = nullptr_t>
class TimedEvent : public Event<Param>
{
protected:
   // Timer sent in countdown mode
   Timer* timer;

public:
   TimedEvent(double countdown, string _type, string _name = "", int _from = -1)
     : Event<Param>(_type, _name, _from)
   {
      timer = (new Timer())->init(countdown);
   }

   virtual ~TimedEvent()
   {
      delete timer;
   }

   virtual bool isActivated(string _type) const override
   {
      return (this->type == _type) && (timer->expired());
   }

   virtual string toString() const override
   {
      stringstream ss;
      ss << "TimedEvent {name=" << this->name;
      ss << "; type=" << this->type;
      ss << "; timer={countdown:" << timer->getCountdown() << "}";
      ss << "}";
      return ss.str();
   }
};

} // libbft

#endif // LIBBFT_SRC_CPP_EVENT_HPP