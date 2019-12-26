#include <memory>
#include <gtest/gtest.h>

#include "timing/Clock.hpp"

using namespace libbft;

TEST(TimingClock, GetTime) {
   unique_ptr<Clock> clock(new Clock("T"));
   EXPECT_LT(0, clock->getTime());
}

TEST(TimingClock, ToString) {
   unique_ptr<Clock> clock(new Clock("T"));
   EXPECT_EQ("Clock {name='T'}", clock->toString());
}
