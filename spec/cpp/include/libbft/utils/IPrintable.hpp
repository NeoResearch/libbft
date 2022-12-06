// SPDX-License-Identifier:  MIT
// Copyright (C) 2019-2022 - LibBFT developers
// https://github.com/neoresearch/libbft

#ifndef INCLUDE_LIBBFT_UTILS_IPRINTABLE_HPP_
#define INCLUDE_LIBBFT_UTILS_IPRINTABLE_HPP_

#include <string>

namespace libbft {

enum class StringFormat { Default, Graphviz };

class IFPrintable {
 public:
  virtual std::string toString() {
    return toStringFormat(StringFormat::Default);
  }
  virtual std::string toStringFormat(StringFormat format) = 0;
  virtual ~IFPrintable() = default;
};

}  // namespace libbft

#endif  // INCLUDE_LIBBFT_UTILS_IPRINTABLE_HPP_
