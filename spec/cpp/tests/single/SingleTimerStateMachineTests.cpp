#include <memory>
#include <gtest/gtest.h>

#include "single/SingleTimerStateMachine.hpp"

using namespace std;
using namespace libbft;

TEST(SingleSingleTimerStateMachine, ToString) {
   auto machine = std::shared_ptr<SingleTimerStateMachine<int>>(new SingleTimerStateMachine<int>());

   EXPECT_EQ("STSM {#id = 0;Timer='Timer {name=''}';States=[]}", machine->toString());
}
