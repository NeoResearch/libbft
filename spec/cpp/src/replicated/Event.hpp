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
//#include "State.h"

using namespace std; // TODO: remove

namespace libbft {

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

public:
   Event(string _type, string _name = "", int _from = -1)
     : type(_type)
     , name(_name)
     , from(_from)
   {
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
      ss << "Event {name=" << name << "; type=" << type << "}";
      return ss.str();
   }
};

} // libbft

#endif // LIBBFT_SRC_CPP_EVENT_HPP