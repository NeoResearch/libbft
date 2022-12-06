// SPDX-License-Identifier:  MIT
// Copyright (C) 2019-2022 - LibBFT developers
// https://github.com/neoresearch/libbft

#include <span>  // c++20
#include <string>
//
#include <bftevents-grpc/BFTEventsServer.hpp>

using namespace std;     // NOLINT
using namespace libbft;  // NOLINT

int main(int argc, char** argv) {
  // error: do not use pointer arithmetic
  // [cppcoreguidelines-pro-bounds-pointer-arithmetic,-warnings-as-errors]
  // https://stackoverflow.com/questions/45718389/how-to-avoid-pointer-arithmetic-when-using-char-argv
  auto args = std::span(argv, static_cast<size_t>(argc));
  //
  int me = 0;
  if (argc >= 2) {
    // std::string s(argv[1]);  // get value
    std::string s(args[1]);  // get value
    me = stoi(s);
  }

  std::cout << "I am # " << me << std::endl;

  BFTEventsServer<> server(me);
  server.RunForever();

  return 0;
}
