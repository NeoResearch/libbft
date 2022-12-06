// SPDX-License-Identifier:  MIT
// Copyright (C) 2019-2022 - LibBFT developers
// https://github.com/neoresearch/libbft

#ifndef INCLUDE_LIBBFT_EVENTS_EVENT_HPP_
#define INCLUDE_LIBBFT_EVENTS_EVENT_HPP_

// system includes
#include <algorithm>
#include <memory>
#include <random>
#include <sstream>
#include <string>
#include <utility>
#include <vector>

// standard Transition
#include <libbft/machine/MachineId.hpp>
#include <libbft/timing/Timer.hpp>
// #include "State.h"

namespace libbft {

/*
class EventParameter
{
protected:
   // type of EventParameter class (usually, empty... is string)
   string type;
   // content stored on event parameter (as string). other implementations can
ignore this field. string content;

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
      cout << "equals?? content = " << content << " -> " << other.toString() <<
endl; return (type == other.getType()) && (this->toString() ==
other.toString());
   }

   virtual string toString() const
   {
      stringstream ss;
      ss << "EventParameter:{type=" << type << ";content='" << content << "'}";
      return ss.str();
   }
};
*/

/**
 * this Event class is mostly used for simulation
 * it indicates a type for event (for matching), a name for printing, a source
 * 'from', and possibly a countdown Timer
 */
class Event {
 protected:
  /** event name (used to matching) */
  std::string name;
  /** event called from machine 'from'. If -1, it came from a broadcast (or
   * machine itself) */
  MachineId from;
  /** extra parameter to compare */
  std::vector<std::string> parameters;

 public:
  explicit Event(
      std::string _name, MachineId _from = MachineId(-1),
      std::vector<std::string> _parameters = std::vector<std::string>(0))
      : name(std::move(_name)),
        from(std::move(_from)),
        parameters(std::move(_parameters)) {}

  virtual ~Event() = default;

  /**
   * TODO: receive a lambda for special validation and filtering here?
   * perhaps... (bool matching?)
   * @param _name
   * @param pattern
   * @return
   */
  virtual bool isActivated(std::string _name,
                           std::vector<std::string> pattern) const {
    // return (name == _name) && checkEventArgs(parameters, pattern, matching);
    return (name == _name) && (parameters == pattern);
  }

  virtual MachineId getFrom() const { return from; }

  /*
  static bool checkEventArgs(vector<string> myArgs, vector<string> pattern, bool
  matching)
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

  virtual std::string toString() const {
    std::stringstream ss;
    ss << "Event [args=" << parameters.size() << "] " << name << "(";
    auto comma = "";
    for (auto& parameter : this->parameters) {
      ss << comma << parameter;
      comma = ",";
    }
    ss << ")";
    return ss.str();
  }
};

using TEvent = std::shared_ptr<Event>;
using Events = std::vector<TEvent>;

class TimedEvent : public Event {
 protected:
  /** Timer sent in countdown mode */
  TTimer timer;

 public:
  TimedEvent(double countdown, std::string _name,
             MachineId _from = MachineId(-1),
             std::vector<std::string> _parameters = std::vector<std::string>(0))
      : Event(std::move(_name), std::move(_from), std::move(_parameters)) {
    timer = std::unique_ptr<Timer>((new Timer())->init(countdown));
  }

  ~TimedEvent() override = default;

  bool isActivated(std::string _name,
                   std::vector<std::string> pattern) const override {
    return (this->name == _name) && (timer->expired()) &&
           (this->parameters == pattern);
  }

  std::string toString() const override {
    std::stringstream ss;
    ss << "TimedEvent " << this->name << "(";
    auto comma = "";
    for (auto& parameter : this->parameters) {
      ss << comma << parameter;
      comma = ",";
    }
    // default suffix '()' (empty parameters)
    ss << ") " << (timer->expired() ? "expired" : "notexpired") << " "
       << timer->remainingTime();
    return ss.str();
  }
};

}  // namespace libbft

#endif  // INCLUDE_LIBBFT_EVENTS_EVENT_HPP_
