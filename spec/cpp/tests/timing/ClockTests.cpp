#include <thread>
#include <gtest/gtest.h>

#include "timing/Clock.hpp"

using namespace std;
using namespace libbft;

TEST(TimingClock, ToString) {
   unique_ptr<Clock> clock(new Clock("T"));
   EXPECT_EQ("Clock {name='T'}", clock->toString());
}

TEST(TimingClock, GetTime) {
   unique_ptr<Clock> clock(new Clock("T"));

   auto actualTime = clock->getTime();
   EXPECT_LT(0, actualTime);

   this_thread::sleep_for(chrono::milliseconds(200));
   EXPECT_LT(0.19, clock->getTime() - actualTime);
   EXPECT_GT(0.8, clock->getTime() - actualTime);
}
