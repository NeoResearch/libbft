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

template<class Param = nullptr_t>
class EventParameter
{
protected:
   // type of EventParameter class (usually, empty... is string)
   string type;
   // content stored on event parameter (as string). other implementations can ignore this field.
   string content;

public:
   EventParameter(string _type = "", string _content = "")
     : type(_type)
     , content(_content)
   {
   }

   string getType() const
   {
      return type;
   }

   // default implementation of equals uses toString() visualization
   virtual bool equals(const EventParameter& other)
   {
      return (type == other.getType()) && (this->toString() == other->toString());
   }

   virtual string toString()
   {
      stringstream ss;
      ss << "EventParameter:{type=" << type << ";content='" << content << "'}";
      return ss.str();
   }
};

// this Event class is mostly used for simulation
// it indicates a type for event (for matching), a name for printing, a source 'from', and possibly a countdown Timer

template<class Param = nullptr_t>
class Event
{
protected:
   // event name (used to matching)
   std::string name;
   // event called from machine 'from'. If -1, it came from a broadcast (or machine itself)
   int from;

public:
   Event(string _name, int _from = -1)
     : name(_name)
     , from(_from)
   {
   }

   virtual ~Event()
   {
   }

   virtual bool isActivated(string _name) const
   {
      return (name == _name);
   }

   virtual int getFrom() const
   {
      return from;
   }

   virtual string toString() const
   {
      stringstream ss;
      ss << "Event {name=" << name << "()"; // default '()', empty parameters
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
   TimedEvent(double countdown, string _name, int _from = -1)
     : Event<Param>(_name, _from)
   {
      timer = (new Timer())->init(countdown);
   }

   virtual ~TimedEvent()
   {
      delete timer;
   }

   virtual bool isActivated(string _name) const override
   {
      return (this->name == _name) && (timer->expired());
   }

   virtual string toString() const override
   {
      stringstream ss;
      ss << "TimedEvent {name=" << this->name << "()"; // default suffix '()' (empty parameters)
      ss << "; timer={countdown:" << timer->getCountdown() << "}";
      ss << "}";
      return ss.str();
   }
};

} // libbft

#endif // LIBBFT_SRC_CPP_EVENT_HPP