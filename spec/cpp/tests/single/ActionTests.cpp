#include <chrono>
#include <memory>
#include <gtest/gtest.h>
#include <thread>

#include "single/Transition.hpp"
#include "timing/Clock.hpp"
#include "timing/Timer.hpp"

using namespace std;
using namespace libbft;

TEST(SingleAction, ToString) {
   Action<int>::TimedActionType f = [](const Timer &t, int *p, const MachineId &) -> void {
   };
   unique_ptr<Action<int>> action(new Action<int>("T", f));

   EXPECT_EQ("T", action->toString());
}

TEST(SingleAction, UseTimedAction) {
   int value = -1;
   Action<int>::TimedActionType f = [&value](const Timer &t, int *p, const MachineId &) -> void {
      value = *p;
   };
   unique_ptr<Action<int>> action(new Action<int>("T", f));
   Clock clock;
   Timer timer("T", &clock);

   int p = 1;
   const MachineId &id = MachineId(-1);
   f(timer, &p, id);
   EXPECT_EQ(1, value);
   value = -1;

   action->timedAction(timer, &p, id);
   EXPECT_EQ(1, value);
   value = -1;

   for (int i = 0; i < 10; ++i) {
      f(timer, &i, id);
      EXPECT_EQ(i, value);
      value = -1;

      action->timedAction(timer, &i, id);
      EXPECT_EQ(i, value);
      value = -1;
   }
}
