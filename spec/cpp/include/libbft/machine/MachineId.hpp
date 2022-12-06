// SPDX-License-Identifier:  MIT
// Copyright (C) 2019-2022 - LibBFT developers
// https://github.com/neoresearch/libbft

#ifndef INCLUDE_LIBBFT_MACHINE_MACHINEID_HPP_
#define INCLUDE_LIBBFT_MACHINE_MACHINEID_HPP_

// default clock
// #include "timing/Clock.hpp"

#include <string>
#include <utility>

namespace libbft {

struct MachineId {
  explicit MachineId(int _id = 0, std::string _address = "")
      : id(_id), address(std::move(_address)) {}

  int id;
  std::string address;

  // what else we need here?
};

}  // namespace libbft

#endif  // INCLUDE_LIBBFT_MACHINE_MACHINEID_HPP_
