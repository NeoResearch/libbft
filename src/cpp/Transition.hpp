#ifndef LIBBFT_SRC_CPP_TRANSITION_HPP
#define LIBBFT_SRC_CPP_TRANSITION_HPP

// system includes
#include <sstream>

// standard Transition

// Every transition may or may not be a timed transition

using namespace std; // TODO: remove

namespace libbft {

// forward declaration
class State;

class Transition
{
public:
   State* to;
   string name;

   // TODO: add a "when" function here? (make this class "final"?)
   Transition(State* _to, string _name)
     : to(_to)
     , name(_name)
   {
   }

   string toString()
   {
      stringstream ss;
      ss << "t() => {name = '" << name << "', to=ptr'" << to << "'}";
      return ss.str();
   }

   virtual bool isValid()
   {
      // always valid
      // TODO: replace by lambda?
      return true;
   }
};

} // libbft

#endif // LIBBFT_SRC_CPP_TRANSITION_HPP