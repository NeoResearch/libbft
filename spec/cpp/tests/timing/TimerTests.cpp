#include <chrono>
#include <memory>
#include <gtest/gtest.h>
#include <thread>

#include "timing/Timer.hpp"

using namespace std;
using namespace libbft;

TEST(TimingTimer, ToString) {
   unique_ptr<Timer> timer(new Timer("T"));
   EXPECT_EQ("Timer {name='T'}", timer->toString());
}

TEST(TimingTimer, GetCountdown) {
   unique_ptr<Timer> timer(new Timer("T"));
   timer->init(2);
   EXPECT_EQ(2, timer->getCountdown());
}

TEST(TimingTimer, ElapsedTime) {
   unique_ptr<Timer> timer(new Timer("T"));

   this_thread::sleep_for(chrono::milliseconds(200));
   EXPECT_LT(.15, timer->elapsedTime());
}

TEST(TimingTimer, Expired) {
   unique_ptr<Timer> timer(new Timer("T"));
   timer->init(.3);

   this_thread::sleep_for(chrono::milliseconds(100));
   EXPECT_FALSE(timer->expired());
   this_thread::sleep_for(chrono::milliseconds(250));
   EXPECT_TRUE(timer->expired());
}

TEST(TimingTimer, RemainingTime) {
   unique_ptr<Timer> timer(new Timer("T"));
   timer->init(1);

   this_thread::sleep_for(chrono::milliseconds(100));
   ASSERT_GE(1.0, timer->remainingTime());
   ASSERT_LE(.5, timer->remainingTime());

   this_thread::sleep_for(chrono::milliseconds(100));
   ASSERT_GE(.85, timer->remainingTime());
   ASSERT_LE(.4, timer->remainingTime());

   this_thread::sleep_for(chrono::milliseconds(800));
   ASSERT_TRUE(timer->expired());
   ASSERT_EQ(0, timer->remainingTime());
}
