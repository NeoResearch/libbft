#include <thread>
#include <gtest/gtest.h>

#include "single/SingleTimerStateMachine.hpp"
#include "replicated/MultiContext.hpp"
#include "replicated/MachineContext.hpp"

using namespace std;
using namespace libbft;

TEST(ReplicatedMachineContext, ToString) {
   MachineContext<int> machineContext(
      std::shared_ptr<int>(new int{1}),
      std::shared_ptr<SingleTimerStateMachine<MultiContext<int>>>(new SingleTimerStateMachine<MultiContext<int>>()));

   EXPECT_EQ("STSM {#id = 0;Timer='Timer {name=''}';States=[]}", machineContext.machine->toString());
}
