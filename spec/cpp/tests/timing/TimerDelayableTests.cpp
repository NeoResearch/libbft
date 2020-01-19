#include <thread>
#include <gtest/gtest.h>

#include "timing/TimerDelayable.hpp"

using namespace std;
using namespace libbft;

TEST(TimingTimerDelayable, ToString) {
   TimerDelayable timerDelayable;
   EXPECT_EQ("Timer {name=''}", timerDelayable.toString());
}
