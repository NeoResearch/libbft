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
#include "Timer.hpp"
#include "State.h"

// standard State

// every state is a Timed state (states that allow timed transitions)

using namespace std; // TODO: remove

namespace libbft {

template<class Param = nullptr_t>
class Event
{
private:
   std::string type;
   std::string name;

public:
   Event(string _type, string _name = "")
     : type(_type)
     , name(_name)
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

   virtual string toString() const
   {
      stringstream ss;
      ss << "Event {name=" << name << "; type=" << type << "}";
      return ss.str();
   }
};

/*
template<class Param = nullptr_t>
class EventHandler
{
private:
   std::string type;
   std::string name;

public:
   EventHandler(State<Param>* from, string _name, string _type)
     : type(_type)
     , name(_name)
   {
   }

   // process and removes event
   bool process(vector<Event*>& events)
   {
      for (unsigned i = 0; i < events.size(); i++)
         if (events[i]->getType() == type) {
            // remove event i
            events.erase(events.begin() + i);
            return true;
         }
      return false;
   }

   virtual string toString() const
   {
      stringstream ss;
      ss << "EventHandler {name=" << name << "; type=" << type << "}";
      return ss.str();
   }
};
*/

} // libbft

#endif // LIBBFT_SRC_CPP_EVENT_HPP