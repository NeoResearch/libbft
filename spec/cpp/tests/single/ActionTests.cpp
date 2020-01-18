#include <memory>
#include <gtest/gtest.h>

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
   Timer timer("T", std::shared_ptr<Clock>());

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

struct ActionReference {
   int age;
};

TEST(SingleAction, UseTimedActionReference) {
   Action<ActionReference>::TimedActionType f = [](const Timer &t, ActionReference *p, const MachineId &id) -> void {
      p->age = id.id;
   };
   unique_ptr<Action<ActionReference>> action(new Action<ActionReference>("T", f));
   Timer timer("T", std::shared_ptr<Clock>());

   ActionReference p{-1};
   const MachineId &id = MachineId(1);
   f(timer, &p, id);
   EXPECT_EQ(1, p.age);
   p.age = -1;

   action->timedAction(timer, &p, id);
   EXPECT_EQ(1, p.age);
}
