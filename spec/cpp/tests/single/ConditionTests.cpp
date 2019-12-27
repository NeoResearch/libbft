#include <chrono>
#include <memory>
#include <gtest/gtest.h>
#include <thread>

#include "single/Transition.hpp"

using namespace std;
using namespace libbft;

TEST(SingleCondition, ToString) {
//   unique_ptr<Condition> condition(new Condition("T"));
//   EXPECT_EQ("Clock {name='T'}", clock->toString());
}

TEST(SingleCondition, GetTime) {
   unique_ptr<Clock> clock(new Clock("T"));
//
//   auto actualTime = clock->getTime();
//   EXPECT_LT(0, actualTime);
//
//   this_thread::sleep_for(chrono::milliseconds(200));
//   EXPECT_LT(0.19, clock->getTime() - actualTime);
//   EXPECT_GT(0.8, clock->getTime() - actualTime);
}
