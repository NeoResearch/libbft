#include <thread>
#include <gtest/gtest.h>

#include "timing/CountdownTimer.hpp"

using namespace std;
using namespace libbft;

TEST(TimingCountdownTimer, ToString) {
   CountdownTimer countdownTimer(1);
   EXPECT_EQ("CountdownTimer {name=''}", countdownTimer.toString());
}
