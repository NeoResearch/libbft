#include "timing/Clock.hpp"

#include <gtest/gtest.h>

using namespace libbft;

TEST(TimingClock, ToString) {
    auto clock = new Clock("T");
    EXPECT_EQ("Clock {name='T'}", clock->toString());
}

TEST(TimingClock, GetTime) {
    auto clock = new Clock("T");
    EXPECT_LT(0, clock->getTime());
}
