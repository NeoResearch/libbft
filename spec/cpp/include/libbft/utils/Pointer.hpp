// SPDX-License-Identifier:  MIT
// Copyright (C) 2019-2022 - LibBFT developers
// https://github.com/neoresearch/libbft

#ifndef INCLUDE_LIBBFT_UTILS_POINTER_HPP_
#define INCLUDE_LIBBFT_UTILS_POINTER_HPP_

#include <memory>

namespace libbft {

template <class T>
std::unique_ptr<T> clonePtr(const T& obj) {
  return std::unique_ptr<T>(new T(obj));
}

}  // namespace libbft

#endif  // INCLUDE_LIBBFT_UTILS_POINTER_HPP_
