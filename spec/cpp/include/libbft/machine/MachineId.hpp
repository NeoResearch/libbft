#pragma once
#ifndef LIBBFT_SRC_CPP_MACHINEID_HPP
#define LIBBFT_SRC_CPP_MACHINEID_HPP

// default clock
//#include "timing/Clock.hpp"

#include <string>

namespace libbft {

struct MachineId
{
   explicit MachineId(int _id = 0, std::string _address = "")
     : id(_id)
     , address(std::move(_address))
   {
   }

   int id;
   std::string address;

   // what else we need here?
};

} // libbft

#endif // LIBBFT_SRC_CPP_MACHINEID_HPP
