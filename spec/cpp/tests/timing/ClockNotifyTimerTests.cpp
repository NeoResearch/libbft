#include <thread>
#include <gtest/gtest.h>

#include "timing/CountdownNotifyTimer.hpp"

using namespace std;
using namespace libbft;

TEST(TimingClockNotifyTimer, ToString) {
   CountdownNotifyTimer clockNotifyTimer(1);
   EXPECT_EQ("CountdownNotifyTimer {name=''}", clockNotifyTimer.toString());
}
