#include "Transition.h"

using namespace libbft;

string
Transition::toString()
{
   stringstream ss;
   ss << "t() => {name = '" << name << "', to='" << to->toString(false) << "'}";
   return ss.str();
}