// SPDX-License-Identifier:  MIT
// Copyright (C) 2019-2022 - LibBFT developers
// https://github.com/neoresearch/libbft

#include <span>  // C++20
#include <sstream>
#include <string>

//
#include <bftevents-grpc/BFTEventsClient.hpp>

void RunTest(int me, int to) {
  std::stringstream ss;
  ss << "0.0.0.0:500" << to;  // 0 -> 5000
  std::string toAddress(ss.str());

  BFTEventsClient client(me, toAddress);

  int from = me;
  std::string event = "MustStart()";

  std::vector<std::string> eventArgs;
  eventArgs.push_back("a1");
  eventArgs.push_back("a2");

  int response = client.informEvent(from, event, eventArgs);
  std::cout << "Answer received: " << from << " ; " << event << " => "
            << response << std::endl;
}

int main(int argc, char* argv[]) {
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
  int to = me;

  if (argc >= 3) {
    // std::string s(argv[2]);  // get value
    std::string s(args[2]);  // get value
    to = stoi(s);
  }

  std::cout << "I am # " << me << std::endl;
  std::cout << "will send to # " << to << std::endl;

  RunTest(me, to);

  return 0;
}
