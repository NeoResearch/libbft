// SPDX-License-Identifier:  MIT
// Copyright (C) 2019-2022 - LibBFT developers
// https://github.com/neoresearch/libbft

#ifndef INCLUDE_LIBBFT_TIMING_CLOCK_HPP_  // NOLINT
#define INCLUDE_LIBBFT_TIMING_CLOCK_HPP_

// system includes
#include <chrono>  // NOLINT
#include <memory>
#include <sstream>
#include <string>
#include <utility>

//
#include <libbft/utils/Shortcuts.hpp>

// standard Timer for a TSM

namespace libbft {

/**
 * this clock is supposed to be a very precise one
 * it can also be used on tests as a drifted clock (using inheritance)
 */
class Clock {
 private:
  std::string name;

 public:
  explicit Clock(std::string _name = "") : name{std::move(_name)} {}
  // keep this as non-default (for clarity)
  Clock(const Clock& other) : name{other.name} {}

  // MANDATORY PACK
  virtual ~Clock() = default;

  // Mandatory?
  Clock(Clock&& corpse) noexcept = default;
  Clock& operator=(const Clock& other) = default;
  Clock& operator=(Clock&& corpse) = default;

 public:
  /**
   * get time in seconds (milliseconds precision here.. could be more, perhaps)
   * @return
   */
  virtual double getTime() {
    // using chrono
    std::chrono::system_clock::time_point now =
        std::chrono::system_clock::now();
    auto now_ms = std::chrono::time_point_cast<std::chrono::milliseconds>(now);
    auto epoch = now_ms.time_since_epoch();  // which epoch? unix?
    auto value = std::chrono::duration_cast<std::chrono::milliseconds>(epoch);
    int64_t duration = value.count();
    return static_cast<double>(duration) / 1000.0;
  }

  std::string toString() const {
    std::stringstream ss;
    ss << "Clock {name='" << name << "'}";
    return ss.str();
  }
};

using TClock = uptr<Clock>;

}  // namespace libbft

#endif  // INCLUDE_LIBBFT_TIMING_CLOCK_HPP_ // NOLINT
