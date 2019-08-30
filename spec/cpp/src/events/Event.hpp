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
#include "../machine/MachineId.hpp"
#include "../timing/Timer.hpp"
//#include "State.h"

using namespace std; // TODO: remove

namespace libbft {

/*
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

   EventParameter(string _type = "", int _content = 0)
     : type(_type)
   {
      stringstream ss;
      ss << _content;
      content = ss.str();
   }

   string getType() const
   {
      return type;
   }

   // default implementation of equals uses toString() visualization
   virtual bool equals(const EventParameter& other)
   {
      cout << "equals?? content = " << content << " -> " << other.toString() << endl;
      return (type == other.getType()) && (this->toString() == other.toString());
   }

   virtual string toString() const
   {
      stringstream ss;
      ss << "EventParameter:{type=" << type << ";content='" << content << "'}";
      return ss.str();
   }
};
*/

// this Event class is mostly used for simulation
// it indicates a type for event (for matching), a name for printing, a source 'from', and possibly a countdown Timer

class Event
{
protected:
   // event name (used to matching)
   std::string name;
   // event called from machine 'from'. If -1, it came from a broadcast (or machine itself)
   MachineId from;
   // extra parameter to compare
   vector<string> parameters;

public:
   Event(string _name, MachineId _from = MachineId(-1), vector<string> _parameters = vector<string>(0))
     : name(_name)
     , from(_from)
     , parameters(_parameters)
   {
   }

   virtual ~Event()
   {
   }

   // TODO: receive a lambda for special validation and filtering here? perhaps... (bool matching?)
   virtual bool isActivated(string _name, vector<string> pattern) const
   {
      //return (name == _name) && checkEventArgs(parameters, pattern, matching);
      return (name == _name) && (parameters == pattern);
   }

   virtual MachineId getFrom() const
   {
      return from;
   }

   /*
   static bool checkEventArgs(vector<string> myArgs, vector<string> pattern, bool matching)
   {
      // if not matching, a raw comparison is made (no wildcards)
      if (!matching)
         return myArgs == pattern;
      else {
         // perform matching. wilcard * is supported here.. suppressing event
         if (myArgs.size() != pattern.size())
            return false;
         for (unsigned i = 0; myArgs.size(); i++) {
            if (pattern[i] == "*")
               continue; // always accepts
            else if (pattern[i] != myArgs[i])
               return false;
         }
         return true;
      }
   }
   */

   virtual string toString() const
   {
      stringstream ss;
      ss << "Event [args=" << parameters.size() << "] " << name << "(";
      for (int i = 0; i < ((int)this->parameters.size()); i++)
         ss << this->parameters[i] << ((i != ((int)parameters.size()) - 1) ? "," : "");
      ss << ")";
      return ss.str();
   }
};

class TimedEvent : public Event
{
protected:
   // Timer sent in countdown mode
   Timer* timer;

public:
   TimedEvent(double countdown, string _name, MachineId _from = MachineId(-1), vector<string> _parameters = vector<string>(0))
     : Event(_name, _from, _parameters)
   {
      timer = (new Timer())->init(countdown);
   }

   virtual ~TimedEvent()
   {
      delete timer;
   }

   virtual bool isActivated(string _name, vector<string> pattern) const override
   {
      return (this->name == _name) && (timer->expired()) && (this->parameters == pattern);
   }

   virtual string toString() const override
   {
      stringstream ss;
      ss << "TimedEvent " << this->name << "(";
      for (int i = 0; i < ((int)this->parameters.size()); i++)
         ss << this->parameters[i] << ((i != ((int)parameters.size()) - 1) ? "," : "");
      ss << ") " << (timer->expired() ? "expired" : "notexpired"); // default suffix '()' (empty parameters)
      return ss.str();
   }
};

} // libbft

#endif // LIBBFT_SRC_CPP_EVENT_HPP