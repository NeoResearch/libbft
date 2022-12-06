// SPDX-License-Identifier:  MIT
// Copyright (C) 2019-2022 - LibBFT developers
// https://github.com/neoresearch/libbft

#ifndef INCLUDE_LIBBFT_UTILS_SHORTCUTS_HPP_
#define INCLUDE_LIBBFT_UTILS_SHORTCUTS_HPP_

#include <memory>
#include <optional>

namespace libbft {
//
template <typename T>
using sptr = std::shared_ptr<T>;
//
template <typename T>
using uptr = std::unique_ptr<T>;
//
template <typename T>
using op = std::optional<T>;
//
}  // namespace libbft

#endif  // INCLUDE_LIBBFT_UTILS_SHORTCUTS_HPP_
