#pragma once
#ifndef LIBBFT_SRC_CPP_TIMERDELAYABLE_HPP
#define LIBBFT_SRC_CPP_TIMERDELAYABLE_HPP

// standard Timer for a TSM
#include "Timer.hpp"

// This Timer allows for non-negative delays

namespace libbft {

class TimerDelayable : public Timer
{
};

} // libbft

#endif // LIBBFT_SRC_CPP_TIMERDELAYABLE_HPP